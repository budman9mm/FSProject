//PMDG_7_CDU0_NG3 - Keypad functions
//Dustin Moss 1-4-2020
//The NG3 update removes toggle tracking necessity that was in the old NGX. Other than that
//it's all the same code.

#include <stdlib.h>
#include <Streaming.h>
#include <Ethernet2.h> //For the Wiz5500
#include <EthernetUdp2.h>
#include <Keypad.h>
#include <Rotary.h>

//Ethernet
static byte mymac[] = {0x72, 0x2A, 0x2A, 0x2D, 0x30, 0x28};
IPAddress myip = (192, 168, 1, 134);
IPAddress gwip = (192, 168, 1, 1);
IPAddress dnsip = (192, 168, 1, 1);
IPAddress mask = (255, 255, 255, 0);
IPAddress host(192, 168, 1, 5);
IPAddress PMDG_6(192, 168, 1, 150); //Arduino with the LCD display

static unsigned int portTx = 47035;
static unsigned int portListen = 47034;

static unsigned int TCPPort = 27017; //TCP port

EthernetServer TCPServer(27017);
EthernetClient client = TCPServer.available();
EthernetUDP udp;

char recvBuffer[UDP_TX_PACKET_MAX_SIZE]; //24
char txBuffer[16];
int clientPacketSz;

//Keypad p1
unsigned long keypadTime1;
const byte ROWS1 = 6;
const byte COLS1 = 6;
char k1[ROWS1][COLS1] = {
  //p1
  {'!', '@', '#', '$', '%', '4'}, //LSK1, RSK1, INIT REF, LEGS, FIX, #4
  {'^', '&', '*', '(', ')', '5'}, //LSK2, RSK2, RTE, DEP ARR, NEXT PAGE, #5
  {'-', '+', '_', '=', '{', '6'}, //LSK3, RSK3, CLB, HOLD, PREV PAGE, #6
  {'}', '[', ']', ':', '1', '7'}, //LSK4, RSK4, CRZ, PROG, #1, #7
  {'~', '\'', ';', '"', '2', '8'}, //LSK5, RSK5, DES, EXEC, #2, #8
  {'|', '\\', '<', '>', '3', '9'}, //LSK6, RSK6, MENU, N1 LIMIT, #3, #9
};
byte rowPins_1[ROWS1] = {2, 3, 4, 5, 6, 7};
byte colPins_1[COLS1] = {8, 9, 10, 11, 12, 14};
Keypad p1 = Keypad( makeKeymap(k1), rowPins_1, colPins_1, ROWS1, COLS1 );

//Keypad P2
unsigned long keypadTime2;
const byte ROWS2 = 6;
const byte COLS2 = 6;
char k2[6][6] = {
  //P2
  {'.', 'D', 'J', 'P', 'V', '?'}, //.(dot), D, J, P, V, DEL
  {'0', 'E', 'K', 'Q', 'W', '/'}, //0, E, K, Q, W, /
  {'a', 'F', 'L', 'R', 'X', '='}, //+/-, F, L, R, X, CLR
  {'A', 'G', 'M', 'S', 'Y', 'b'}, //A, G, M, S, Y, NIL
  {'B', 'H', 'N', 'T', 'Z', 'c'}, //B, H, N, T, Z, NIL !!!!!!!!!!Had to use integers here due to some compiler error, most likely.
  {'C', 'I', 'O', 'U', '|', 'd'} //C, I, O, U, SP, NIL
};
byte rowPins_2[ROWS2] = {22, 23, 24, 25, 26, 27};
byte colPins_2[COLS2] = {28, 29, 30, 31, 32, 33};
Keypad p2 = Keypad( makeKeymap(k2), rowPins_2, colPins_2, ROWS2, COLS2 );

//Rotary Encoder - Brightness knob
Rotary bright_enc = Rotary(39, 40);
const byte brightSw = 41; //Available for future fuction
boolean brightSwState, lastBrightSwState;
long debounce = 0; //Single debounce for brightSw
unsigned int brightPos = 62; //Initially set it to ~half bright. Full is 127.
unsigned int lastBrightPos = 62;

//LEDs
const byte ledPin[] = {34, 35, 36, 37, 38}; //EXEC, CALL, FAIL, MSG, OFST
const short qLedPin = sizeof(ledPin) / sizeof(ledPin[0]);

//MISC
boolean console = true;
String msg = "";
boolean staticIP = false;
String readStr = "";
bool CLRHold = false; //Flag for determining if CLR was in a hold down state so we don't respond to a release

void ethernet() {
  //Ethernet - The CS pin has to be set in the Ethernet2.h file for the MEGA to 53 from 10 for uno
  if (!staticIP) {
    Serial << "Attempting DHCP..." << endl;
    if (Ethernet.begin(mymac) == 0) {
      Serial.println("Failed to configure Ethernet using DHCP...Trying static settings.");
      Ethernet.begin(mymac, myip, gwip, mask);
    } else {
      Serial << "DHCP Successful!" << endl;
      myip = Ethernet.localIP();
      for (byte thisByte = 0; thisByte < 4; thisByte++) {
        // print the value of each byte of the IP address:
        Serial.print(myip[thisByte], DEC);
        Serial.print(".");
      }
      Serial.println();
    }
  } else {
    //Static
    Serial << endl << "Static IP setup..." << endl;
    Ethernet.begin(mymac, myip, gwip, mask);
    Serial << "Static connection created! ";
    myip = Ethernet.localIP();
    for (byte thisByte = 0; thisByte < 4; thisByte++) {
      // print the value of each byte of the IP address:
      Serial.print(myip[thisByte], DEC);
      Serial.print(".");
    }
    Serial.println();
  }
  Serial << "Attempting to connect to " << IPAddress(host);
  Serial << " UDP port " << portListen << "..." << endl;
  udp.begin(portListen);
  Serial << "Attempting to connect to " << Ethernet.localIP();
  Serial << " TCP port " << TCPPort << "..." << endl;
  TCPServer.begin();
}//END ETHERNET

void setup() {
  Serial.begin(115200);
  if (console) Serial << "PMDG_9_CDU0_KP ver. NG3" << endl;
  Serial << LIST_MAX << endl;
  ethernet();
  delay(1000);
  for (int i = 0; i < qLedPin; i++) {
    pinMode(ledPin[i], OUTPUT);
    digitalWrite(ledPin[i], LOW); //Reset LEDs low
  }
  pinMode(brightSw, INPUT_PULLUP);
  if (client) Serial << "TCP client connected!" << endl;
}//END void setup()

void sendMsg(String message) {
  message += '\n';
  message.toCharArray(txBuffer, message.length() + 1);
  udp.beginPacket(IPAddress(192, 168, 1, 5), portTx);
  udp.write(txBuffer, message.length() + 1);
  udp.endPacket();
  Serial << "[" << message.length() + 1 << "]" << message << endl;
}//END sendMsg()

void readUDP() {
  int rxPacketSize = udp.parsePacket();
  if (rxPacketSize) {
    if (console) {
      Serial << "RX" << "[" << rxPacketSize << "]";
    }
    udp.read(recvBuffer, rxPacketSize);
    if (console) {
      Serial.println(recvBuffer);
    }
    udp.flush();
    delay(10);
    if (recvBuffer[0] == '#') {
      pound(rxPacketSize, recvBuffer);
      recvBuffer[0] = '\0'; //Null the buffer
    }
  }//endif rxPac
}//END readUDP()

void readTCP() { //Nothing to read at this time, but it's here
}//END readTCP()

void checkKeypad_1() {
  if ( (millis() - keypadTime1) > 50 ) {
    keypadTime1 = millis();
  }
  // Returns true if there are ANY active keys.
  if (p1.getKeys())
  {
    for (int i = 0; i < LIST_MAX; i++) // Scan the whole key list.
    {
      if ( p1.key[i].stateChanged )   // Only find keys that have changed state.
      {
        switch (p1.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
          case PRESSED:
            break;
          case HOLD:
            break;
          /*
            {'!', '@', '#', '$', '%', '4'}, //LSK1, RSK1, INIT REF, LEGS, FIX, #4
            {'^', '&', '*', '(', ')', '5'}, //LSK2, RSK2, RTE, DEP ARR, NEXT PAGE, #5
            {'-', '+', '_', '=', '{', '6'}, //LSK3, RSK3, CLB, HOLD, PREV PAGE, #6
            {'}', '[', ']', ':', '1', '7'}, //LSK4, RSK4, CRZ, PROG, #1, #7
            {'~', '\'', ';', '"', '2', '8'}, //LSK5, RSK5, DES, EXEC, #2, #8
            {'|', '\\', '<', '>', '3', '9'} //LSK6, RSK6, MENU, N1 LIMIT, #3, #9S
          */
          case RELEASED:
            switch (p1.key[i].kchar) {
              //P1
              //ROW 1
              case '!': //EVT_CDU_L_L1
                msg = "Z70166:1"; sendMsg(msg);
                if (console) Serial << "LSK1" << endl;
                break;
              case '@': //EVT_CDU_L_R1
                msg = "Z70172:1"; sendMsg(msg);
                if (console) Serial << "RSK1" << endl;
                break;
              case '#': //EVT_CDU_L_INIT_REF
                msg = "Z70178:1"; sendMsg(msg);
                if (console) Serial << "INIT REF" << endl;
                break;
              case '$': //EVT_CDU_L_LEGS
                msg = "Z70184:1"; sendMsg(msg);
                if (console) Serial << "LEGS" << endl;
                break;
              case '%': //EVT_CDU_L_FIX
                msg = "Z70190:1";
                sendMsg(msg);
                if (console) Serial << "FIX" << endl;
                break;
              case '4': //EVT_CDU_L_4
                msg = "Z70196:1"; sendMsg(msg);
                if (console) Serial << "4" << endl;
                break;
              //ROW 2
              case '^': //EVT_CDU_L_L2
                msg = "Z70167:1"; sendMsg(msg);
                if (console) Serial << "LSK2" << endl;
                break;
              case '&': //EVT_CDU_L_R2
                msg = "Z70173:1"; sendMsg(msg);
                if (console) Serial << "RSK2" << endl;
                break;
              case '*': //EVT_CDU_L_RTE
                msg = "Z70179:1"; sendMsg(msg);
                if (console) Serial << "RTE" << endl;
                break;
              case '(': //EVT_CDU_L_DEP_ARR
                msg = "Z70185:1"; sendMsg(msg);
                if (console) Serial << "DEP ARR" << endl;
                break;
              case ')': //EVT_CDU_L_NEXT_PAGE
                msg = "Z70192:1"; sendMsg(msg);
                if (console) Serial << "NEXT PAGE" << endl;
                break;
              case '5': //EVT_CDU_L_5
                msg = "Z70197:1"; sendMsg(msg);
                if (console) Serial << "5" << endl;
                break;
              //ROW 3
              case '-': //EVT_CDU_L_L3
                msg = "Z70168:1"; sendMsg(msg);
                if (console) Serial << "LSK3" << endl;
                break;
              case '+': //EVT_CDU_L_R3
                msg = "Z70174:1"; sendMsg(msg);
                if (console) Serial << "RSK3" << endl;
                break;
              case '_': //EVT_CDU_L_CLB
                msg = "Z70180:1"; sendMsg(msg);
                if (console) Serial << "CLB" << endl;
                break;
              case '=': //EVT_CDU_L_HOLD
                msg = "Z70186:1"; sendMsg(msg);
                if (console) Serial << "HOLD" << endl;
                break;
              case '{': //EVT_CDU_L_PREV_PAGE
                msg = "Z70191:1"; sendMsg(msg);
                if (console) Serial << "PREV PAGE" << endl;
                break;
              case '6': //EVT_CDU_L_6
                msg = "Z70198:1"; sendMsg(msg);
                if (console) Serial << "6" << endl;
                break;
              //ROW 4
              case '}': //EVT_CDU_L_L4
                msg = "Z70169:1"; sendMsg(msg);
                if (console) Serial << "LSK4" << endl;
                break;
              case '[': //EVT_CDU_L_R4
                msg = "Z70175:1"; sendMsg(msg);
                if (console) Serial << "RSK4" << endl;
                break;
              case ']':  //EVT_CDU_L_CRZ
                msg = "Z70181:1"; sendMsg(msg);
                if (console) Serial << "CRZ" << endl;
                break;
              case ':': //EVT_CDU_L_PROG
                msg = "Z70187:1"; sendMsg(msg);
                if (console) Serial << "PROG" << endl;
                break;
              case '1': //EVT_CDU_L_1
                msg = "Z70193:1"; sendMsg(msg);
                if (console) Serial << "1" << endl;
                break;
              case '7': //EVT_CDU_L_7
                msg = "Z70199:1"; sendMsg(msg);
                if (console) Serial << "7" << endl;
                break;
              //ROW 5
              case '~': //EVT_CDU_L_L5
                msg = "Z70170:1"; sendMsg(msg);
                if (console) Serial << "LSK5" << endl;
                break;
              case '\'': //EVT_CDU_L_R5
                msg = "Z70176:1"; sendMsg(msg);
                if (console) Serial << "RSK5" << endl;
                break;
              case ';': //EVT_CDU_L_DES
                msg = "Z70182:1"; sendMsg(msg);
                if (console) Serial << "DES" << endl;
                break;
              case '"': //EVT_CDU_L_EXEC
                msg = "Z70188:1"; sendMsg(msg);
                if (console) Serial << "EXEC" << endl;
                break;
              case '2'://EVT_CDU_L_2
                msg = "Z70194:1"; sendMsg(msg);
                if (console) Serial << "2" << endl;
                break;
              case '8': //EVT_CDU_L_8
                msg = "Z70200:1"; sendMsg(msg);
                if (console) Serial << "8" << endl;
                break;
              //ROW 6
              case '|': //EVT_CDU_L_L6
                msg = "Z70171:1"; sendMsg(msg);
                if (console) Serial << "LSK6" << endl;
                break;
              case '\\': //EVT_CDU_L_R6
                msg = "Z70177:1"; sendMsg(msg);
                if (console) Serial << "RSK6" << endl;
                break;
              case '<': //EVT_CDU_L_MENU
                msg = "Z70183:1"; sendMsg(msg);
                if (console) Serial << "MENU" << endl;
                break;
              case '>': //EVT_CDU_L_N1_LIMIT
                msg = "Z70189:1"; sendMsg(msg);
                if (console) Serial << "N1 LIMIT" << endl;
                break;
              case '3': //EVT_CDU_L_3
                msg = "Z70195:1"; sendMsg(msg);
                if (console) Serial << "3" << endl;
                break;
              case '9': //EVT_CDU_L_9
                msg = "Z70201:1"; sendMsg(msg);
                if (console) Serial << "9" << endl;
                break;
            }//End Switch
            break;
          case IDLE:
            break;
        } //End Switch
        //Serial << "Key " << kpd.key[i].kchar << msg << endl ;
      }
    }
  }
}//END void checkKeypad_1
void checkKeypad_2() {
  if ( (millis() - keypadTime2) > 60 ) {
    keypadTime2 = millis();
  }
  // Returns true if there are ANY active keys.
  if (p2.getKeys())
  {
    int i = 0;
    if ( p2.key[i].stateChanged )   // Only find keys that have changed state.
    {
      switch (p2.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
        //The PRESSED and HOLD cases are causing some kind of issue where the RELEASED case
        //switch is totally missing valid case hits.  At least that seems to have fixed it for now.
        /*case PRESSED:
          if (console) Serial << "Pressed p2: \'" << p2.key[i].kchar << "\'" << endl;
          break;
          case HOLD:
          Serial << "A Key was HELD" << endl;
          switch (p2.key[i].kchar) {
            case '=': //EVT_CDU_L_CLR
                  msg = "Z70234:1"; //0 CLR 1 Char. 1 CLR the line
                  sendMsg(msg);
                  if (console) Serial << "CLR(Hold)" << endl;
                  CLRHold = true;
                  break;
          }//end sw
          break;
           /*
          {'.', 'D', 'J', 'P', 'V', '?'}, //.(dot), D, J, P, V, DEL
          {'0', 'E', 'K', 'Q', 'W', '/'}, //0, E, K, Q, W, /
          {'\a', 'F', 'L', 'R', 'X', '`'}, //+/-, F, L, R, X, CLR
          {'A', 'G', 'M', 'S', 'Y', '\0'}, //A, G, M, S, Y, NIL
          {'B', 'H', 'N', 'T', 'Z', '\0'}, //B, H, N, T, Z, NIL
          {'C', 'I', 'O', 'U', ' ', '\0'} //C, I, O, U, SP, NIL
        */
        case RELEASED:
          //if (console) Serial << "RELEASED case \'" << p2.key[i].kchar << "\'" << endl;
          switch (p2.key[i].kchar) {
            //p2
            //ROW 1
            case '.': //EVT_CDU_L_DOT
              msg = "Z70202:1"; sendMsg(msg);
              if (console) Serial << "." << endl;
              break;
            case 'D': //EVT_CDU_L_D
              msg = "Z70208:1"; sendMsg(msg);
              if (console) Serial << "D" << endl;
              break;
            case 'J': //EVT_CDU_L_J
              msg = "Z70214:1"; sendMsg(msg);
              if (console) Serial << "J" << endl;
              break;
            case 'P': //EVT_CDU_L_P
              msg = "Z70220:1"; sendMsg(msg);
              if (console) Serial << "P" << endl;
              break;
            case 'V': //EVT_CDU_L_V
              msg = "Z70226:1"; sendMsg(msg);
              if (console) Serial << "V" << endl;
              break;
            case '?': //EVT_CDU_L_DEL
              msg = "Z70232:1"; sendMsg(msg);
              if (console) Serial << "DEL" << endl;
              break;
            //ROW 2
            case '0': //EVT_CDU_L_0
              msg = "Z70203:1";
              sendMsg(msg); if (console) Serial << "0 (Zero)" << endl;
              break;
            case 'E': //EVT_CDU_L_E
              msg = "Z70209:1"; sendMsg(msg);
              if (console) Serial << "E" << endl;
              break;
            case 'K': //EVT_CDU_L_K
              msg = "Z70215:1"; sendMsg(msg);
              if (console) Serial << "K" << endl;
              break;
            case 'Q': //EVT_CDU_L_Q
              msg = "Z70221:1"; sendMsg(msg);
              if (console) Serial << "Q" << endl;
              break;
            case 'W':
              msg = "Z70227:1"; sendMsg(msg);
              if (console) Serial << "W" << endl;
              break;
            case '/': //EVT_CDU_L_SLASH
              msg = "Z70233:1"; sendMsg(msg);
              if (console) Serial << "/" << endl;
              break;
            //ROW 3
            case 'a': //EVT_CDU_L_PLUS_MINUS
              msg = "Z70204:1"; sendMsg(msg);
              if (console) Serial << "+/-" << endl;
              break;
            case 'F': //EVT_CDU_L_F
              msg = "Z70210:1"; sendMsg(msg);
              if (console) Serial << "F" << endl;
              break;
            case 'L': //EVT_CDU_L_L
              msg = "Z70216:1"; sendMsg(msg);
              if (console) Serial << "L" << endl;
              break;
            case 'R':  //EVT_CDU_L_R
              msg = "Z70222:1"; sendMsg(msg);
              if (console) Serial << "R" << endl;
              break;
            case 'X':  //EVT_CDU_L_X
              msg = "Z70228:1"; sendMsg(msg);
              if (console) Serial << "X" << endl;
              break;
            case '=': //EVT_CDU_L_CLR - MOUSE SIMULATED EVENT - 1 to press 0 to release - 1 only is a CLR LINE
              if (!CLRHold) {
                msg = "Z70234:1"; sendMsg(msg);
                msg = "Z70234:0"; sendMsg(msg);
                if (console) Serial << "CLR" << endl;
              } else CLRHold = false;
              break;
            //ROW 4
            case 'A': //EVT_CDU_L_A
              msg = "Z70205:1"; sendMsg(msg);
              if (console) Serial << "A" << endl;
              break;
            case 'G': //EVT_CDU_L_G
              msg = "Z70211:1"; sendMsg(msg);
              if (console) Serial << "G" << endl;
              break;
            case 'M': //EVT_CDU_L_M
              msg = "Z70217:1"; sendMsg(msg);
              if (console) Serial << "M" << endl;
              break;
            case 'S': //EVT_CDU_L_S
              msg = "Z70223:1"; sendMsg(msg);
              if (console) Serial << "S" << endl;
              break;
            case 'Y': //EVT_CDU_L_Y
              msg = "Z70229:1"; sendMsg(msg);
              if (console) Serial << "Y" << endl;
              break;
            //ROW 5
            case 'B': //EVT_CDU_L_B
              msg = "Z70206:1"; sendMsg(msg);
              if (console) Serial << "B" << endl;
              break;
            case 'H': //EVT_CDU_L_H
              msg = "Z70212:1"; sendMsg(msg);
              if (console) Serial << "H" << endl;
              break;
            case 'N': //EVT_CDU_L_N
              msg = "Z70218:1"; sendMsg(msg);
              if (console) Serial << "N" << endl;
              break;
            case 'T': //EVT_CDU_L_T
              msg = "Z70224:1"; sendMsg(msg);
              if (console) Serial << "T" << endl;
              break;
            case 'Z': //EVT_CDU_L_Z
              msg = "Z70230:1"; sendMsg(msg);
              if (console) Serial << "Z" << endl;
              break;
            //ROW 6
            case 'C': //EVT_CDU_L_C
              msg = "Z70207:1"; sendMsg(msg);
              if (console) Serial << "C" << endl;
              break;
            case 'I': //EVT_CDU_L_I
              msg = "Z70213:1"; sendMsg(msg);
              if (console) Serial << "I" << endl;
              break;
            case 'O': //EVT_CDU_L_O
              msg = "Z70219:1"; sendMsg(msg);
              if (console) Serial << "O" << endl;
              break;
            case 'U': //EVT_CDU_L_U
              msg = "Z70225:1"; sendMsg(msg);
              if (console) Serial << "U" << endl;
              break;
            case '|': //EVT_CDU_L_SPACE
              msg = "Z70231:1"; sendMsg(msg);
              if (console) Serial << "SPACE" << endl;
              break;

            default:
              Serial << "Can't seem to match a case in p2!" << endl;
              break;
          }//End Switch
          break;

      } //End Switch
    }

  }
}//END void checkKeypad_2
void pound(int sz, char *msg) {
  switch (msg[1]) {
    //Reversed in the c++ code for 1 = off, 0 = on
    case 'a': //EXEC
      digitalWrite(ledPin[0], msg[2] - '0');
      if (console) {
        Serial << "EXEC: " << msg[2] << endl;
      }
      break;
    case 'b': //CALL
      digitalWrite(ledPin[1], msg[2] - '0');
      if (console) {
        Serial << "CALL " << msg[2] << endl;
      }
      break;
    case 'c': //FAIL
      digitalWrite(ledPin[2], msg[2] - '0');
      if (console) {
        Serial << "FAIL: " << msg[2] << endl;
      }
      break;
    case 'd': //MSG
      digitalWrite(ledPin[3], msg[2] - '0');
      if (console) {
        Serial << "MSG: " << msg[2] << endl;
      }
      break;
    case 'e': //OFST
      digitalWrite(ledPin[4], msg[2] - '0');
      if (console) {
        Serial << "OFST: " << msg[2] << endl;;
      }
      break;
      //Briteness doesn't need to be received in this script. Let the controller prog
      //send it directly to the CDU Arduino #6.
      /*case 'f': //Brite Level 1..127
        brightPos = ( ((msg[2] - '0') * 100) + ((msg[3] - '0') * 10) + (msg[4] - '0') );
        if (console) {
          Serial << "Brightness level: " << brightPos << endl;
        }
        break;
      */
  }//end switch
}//END void pound

void checkEncoder_BRIGHT() {
  char resultBrightEnc = bright_enc.process();
  if (resultBrightEnc) {
    if (resultBrightEnc == 32) {
      brightPos ++;
      if (brightPos <= 0) {
          brightPos = 0;
        }
        if (brightPos > 127) {
          brightPos = 127;
        }
      if (lastBrightPos != brightPos) {
        msg = "Z70309:"; msg += brightPos;
        sendMsg(msg);
      }
    }
    if (resultBrightEnc == 16) {
      brightPos --;
      if (brightPos <= 0) {
          brightPos = 0;
        }
        if (brightPos > 127) {
          brightPos = 127;
        }
      if (lastBrightPos != brightPos) {
        msg = "Z70309:"; msg += brightPos;
        sendMsg(msg);
      }
    }
    lastBrightPos = brightPos;



    if (console) Serial << "CDU0 Brightness: " << brightPos << endl;
  }
}//END void checkEncoder_BRIGHT()

void checkSwitch() { //Briteness Switch Check
  boolean reading = digitalRead(brightSw);
  if (reading != lastBrightSwState) {
    debounce = millis();
  }
  if (millis() - debounce > 50) {
    if (reading != brightSwState) {
      brightSwState = reading;
      if (console) {
        Serial << "Bright Switch" << endl;
      }
      if (brightSwState == 1) {
        /*Clearly this stuff was not completed.  Shame on me.  Added to the list.*/
        //Send the bright value to PMDG_6_CDU0
        //msg = "$B";
        //TCPServer.println(msg);

        //Also Cycle off the LEDs...Having issues with these being on/off when they should
        //for (int i = 0; i < qLedPin; i++) {
        //pinMode(ledPin[i], OUTPUT);
        //digitalWrite(ledPin[i], LOW); //Reset LEDs low
        //}
        //if (console) {
        //Serial << "Key tracking bits flipped" << endl;
        //}
      }
    }
  }
  lastBrightSwState = reading;
}//end void

void checkEthernet() {
  if (TCPServer.available()) {
    if (console) {
      Serial << "TCP Connection to PMDG_6_CDU0 disconnected.  Attempting Reconnect." << endl;
    }
    client.stop();
    TCPServer.begin();
  }
}//END void checkEthernet()

void commandInterpreter() {
  while (Serial.available() > 0) {
    char received = Serial.read();
    if (received == 8) { //Backspace
      readStr.remove(readStr.length() - 1);
    } else readStr += received;
    readStr.toUpperCase();
    if (readStr == "HELP\r" || readStr == "?\r") {
      Serial << endl << "HELP MENU - PMDG ARDUINO 1 - Packs, etc." << endl << endl;
      Serial << "Z[memory offset 5 digits]:[value] - Send offset example Z69702:0" << endl;
      Serial << "X[memory offset 5 digits] - Send Offset with no parameter" << endl;
      Serial << "console - Toggle verbose console - Currently = " << console << endl;
      Serial << "ip - IP Address, ports, etc." << endl;
      Serial << "net - Reset network connection" << endl;
      Serial << "refresh (or RR/rr) - Force Annunciator Re-read" << endl;
      Serial << "brite - Display CDU bright level" << endl;
      Serial << "ipc.read(type) (offset) - Issue and ipc.readXXX to FSUIPC where XXX is one of following:" << endl;
      Serial << "         UB UW UD SB SW SD DD DBL FLT STR" << endl;
      Serial << "         Offset is 4 char Hex memory location --ex. ipc.readUB 6108 or ipc.readDBL 2205" << endl;
      Serial << "annun [on|off|default] - Test Flag - LEDs on, off, or the current default value" << endl;
      Serial << "array - List the p1 & p2 key arrays" << endl;
      Serial <<  endl;
      received = 13;
    }
    if (readStr.startsWith("Z") && readStr.endsWith("\r")) {
      int colonPosition = readStr.indexOf(':');
      if (colonPosition == 6)  {
        send_offset_test(readStr);
      } else {
        Serial << "Please recheck the format including the colon (:), ie. Z70024:1" << endl;
      }
      readStr = "";
    }
    if (readStr.startsWith("X") && readStr.endsWith("\r")) {
      if (readStr.length() == 7) {
        sendMsg(readStr);
        Serial << "Sent: " << readStr << endl;
      } else {
        Serial << "Invalid X Control: " << readStr << " Length: " << readStr.length() << endl;
      }
      readStr = "";
    }
    if (readStr == "NET\r") {
      udp.stop();
      ethernet();
      readStr = "";
    }
    if (readStr == "CONSOLE\r") {
      !console;
      Serial << "Console = " << console << endl;
      readStr = "";
    }
    if (readStr == "ANNUN ON\r") {
      for (int i = 0; i < qLedPin; i++) {
        digitalWrite(ledPin[i], HIGH);
      }
      Serial << "Annunciators set ON" << endl;
      readStr = "";
    }
    if (readStr == "ANNUN OFF\r") {
      for (int i = 0; i < qLedPin; i++) {
        digitalWrite(ledPin[i], LOW);
      }
      Serial << "Annunciators set OFF" << endl;
      readStr = "";
    }
    if (readStr == "RR\r" || readStr == "REFRESH") {
      sendMsg("R");
      Serial << "Refresh request sent" << endl;
    }
    if (readStr == "ARRAY") {
      Serial << "p1" << endl;
      for (int r = 0; r < 6; r++) {
        Serial << "ROW " << r + 1 << ":  ";
        for (int c = 0; c < 6; c++) {
          Serial.print(k1[r][c]);
          Serial.print(" ");
        }
        Serial << endl;
      }
      Serial << endl;
      Serial << "p2" << endl;
      for (int r = 0; r < 6; r++) {
        Serial << "ROW " << r + 1 << ":  ";
        for (int c = 0; c < 6; c++) {
          Serial.print(k2[r][c]);
          Serial.print(" ");
        }
        Serial << endl;
      }
      readStr = "";
    }//endif array
    if (received == 13 && readStr != "") { //no valid command
      Serial << ">" << readStr << endl;
      readStr = "";
    } else break;
  }//end while
}//END void commandInterpreter

void send_offset_test(String instr) { //Send a manual Z instruction to the simulator
  Serial << "Send offset: " << instr << endl << "SENT\r\n" << endl;
  sendMsg(instr);
  readStr = "";
}//END void send_offset_test

void loop() {
  readUDP();
  readTCP();
  checkKeypad_1();
  checkKeypad_2();
  checkEncoder_BRIGHT();
  checkSwitch();
  checkEthernet();
  commandInterpreter();
}
