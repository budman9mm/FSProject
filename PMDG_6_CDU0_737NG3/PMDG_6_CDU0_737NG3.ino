//This is UDP Socket version of PMDG_6_CDU made to interface with a native c++ program
//instead of using FSUIPC and LUA scripts.  This also replaces the CDU update calls with
//UDP socket data instead of relying on the intermittantly and inexplicably slow updates
//from URLBrowse calls with the EtherCard lib.
//Change log
//6-20-2018 Changed Ethernet hardware to the Wiznet5500(W5500) adapter which is supposed
//          to be a better chip than the ENC28J60.  The library is changed to Ethernet2.h
//          which is easier to work with due to the better chip.  We will see!
//          Check.  It's way better.
//5-11-2019 Updating comments to reflect the fact that we are using TCP instead of UDP for
//          the CDU to use the built in error correction of the protocol.  This was done
//          back in the previous update, but I neglected to note it here.
//          Clarified some variables and removed some redundant and unneeded ones.
//          Changed row and column offsets to better center all the text on the display as
//          the physical panel is designed.
//          Fixed the DHCP/Static setup so static actually works.
//1-4-2020  Update for the new 737 NGu(NG3).  There are no changes to this code and is only
//          for populating the new Arduino 737 NG3 folder with all sketches.
//7-3-2024  Using for the PMDG MSFS 777 now.  Nothing needs changed here.  It should be noted that
//          I discovered that this is only a one way TCP connection - specifically receive.  See
//          the MCP TCP version for examples.
//3-15-2025 Some troubleshooting notes.  There seems to be an issue where this device often won't
//          connect the Ethernet (times out and defaults to static & still doesn't work).  Looks
//          to be something with the USB connection.  It will work on some hubs or directly into the
//          PC USB port.  Some of these hubs are pretty cheap, so maybe power related.
#include <Ethernet2.h>
#include <Streaming.h>  //A Serial print based on cout
#include <UTFT.h>
#include <stdlib.h>

// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x72, 0x2A, 0x2A, 0x2D, 0x30, 0xF6 };
IPAddress myip(192, 168, 1, 150);
IPAddress gwip(192, 168, 1, 1);
IPAddress dnsip(192, 168, 1, 1);
IPAddress mask(255, 255, 255, 0);
IPAddress host(192, 168, 1, 5);
IPAddress host9(192, 168, 1, 134);

static unsigned int portClient = 27015; //TCP port
static unsigned int PMDG_9_Client = 27017; //TCP port to Arduino

#define BUFFER_LEN 1011

char recvBuffer[BUFFER_LEN];
char txBuffer[10];

EthernetClient client;
EthernetClient client9; //PMDG 9

//TFT Settings and vars
// Set the pins to the correct ones for your development shield
// ------------------------------------------------------------
// Standard Arduino Mega/Due shield            : <display model>,38,39,40,41
UTFT LCD(SSD1963_480272, 38, 39, 40, 41); //(byte model, int RS, int WR, int CS, int RST, int SER)

extern uint8_t arial_small[]; //Normal size?
extern uint8_t arial_small_rev[]; //Reverse text

int y = 19; //Row offset - For pixel position of the Y position on the LCD
int x = 17; //Column offset - X position
int xshift = 32; //Shift the X position this amount due to char width and spacing
long scrSaveTimer;//If not connected to the sim for x minutes we will shut off the display
bool simDisconnect = false;

//The entire display symbols, colors, and flags get sent from the server together
char symbol[24][14][3]; //14*(24 + 2 for \n file) 14*(24+1 for \n via TCP)
char p_symbol[24][14][3]; //Previous symbol for comparison - If there's no change, we don't print it again
char color[25][14];
char p_color[25][14];
char flags[25][14];
char p_flags[25][14];

//Misc vars
boolean console = true;
boolean staticIP = false; //Cant get to work with static unless using PROGMEM
String msg = ""; //Messages via sendMsg
bool quit = false; //quit and close sock
unsigned int briteness = 127; //Briteness level of the CDU display received from sim

void setup () {
  Serial.begin(115200);
  Serial.println(F("\n[PMDG 6 CDU0]"));
  // The following two lines are needed for the  display
  // module to enable the backlight. If you are using any other
  // display module these lines should be commented out.
  // -------------------------------------------------------------
  pinMode(8, OUTPUT);  //backlight
  digitalWrite(8, HIGH);//on
  // -------------------------------------------------------------
  // Initial setup
  LCD.InitLCD();
  LCD.clrScr();
  LCD.setBackColor(VGA_BLUE);
  LCD.setBrightness(5);
  LCD.setFont(arial_small);
  LCD.setColor(0xCC6600); //0x1F5AE2
  LCD.print("737 NG3/777X CDU", x + (xshift * 3), y);
  LCD.setFont(arial_small_rev);
  LCD.setColor(VGA_BLACK);
  LCD.print("Dustin Moss(2019-25)", x + (xshift * 2), y + 32);
  LCD.setBackColor(VGA_BLACK);
  //The CS pin has to be set in the Ethernet2.h file for the MEGA to 53 from 10 for uno
  if (!staticIP) {
    Serial << "Trying DHCP..." << endl;
    if (Ethernet.begin(mymac) == 0) {
      Serial.println("Failed to configure Ethernet using DHCP...Trying static settings.");
      Ethernet.begin(mymac, myip, gwip, mask);
      Ethernet.begin(mymac, myip);
      LCD.setBackColor(VGA_GREEN);
      LCD.print("DHCP FAILED - Check Power ", x * 2, y * 9);
      LCD.print("STATIC ", x * 3, y * 10);
      LCD.print("192.168.1.", x * 10, y * 10);
      LCD.printNumI(myip[3], x * 19, y * 10);
    } else {
      Serial << "DHCP Successful!" << endl;
      myip = Ethernet.localIP();
      for (byte thisByte = 0; thisByte < 4; thisByte++) {
        // print the value of each byte of the IP address:
        Serial.print(myip[thisByte], DEC);
        Serial.print(".");
      }
      LCD.setBackColor(VGA_GREEN);
      LCD.print("DHCP ", x * 5, y * 10);
      LCD.print("192.168.1.", x * 10, y * 10);
      LCD.printNumI(myip[3], x * 19, y * 10);
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
    LCD.setBackColor(VGA_GREEN);
    LCD.print("STATIC ", x * 3, y * 10);
    LCD.print("192.168.1.", x * 10, y * 10);
    LCD.printNumI(myip[3], x * 19, y * 10);
    Serial.println();
  }
  Serial << "Attempting to connect to " <<  host[0] << "." << host[1] << "." << host[2] << "." << host[3];
  Serial << " TCP port " << portClient << "..." << endl;
}//END SETUP

void initLCD() { //reset display to our default values and clear the screen
  LCD.InitLCD();
  LCD.clrScr();
  LCD.setBackColor(VGA_BLACK);
  LCD.setBrightness(5);
  LCD.setFont(arial_small);
  LCD.setColor(0xCC6600); //0x1F5AE2
}
void checkKeyInput() {
  Serial << "CheckKeyInput" << endl;
  if (msg == "quit") {
    quit = true;
    Serial << "quit detected...closing socket" << endl;

  }
  if (msg == "restart") {
    quit = false;
    if (!client.available()) {
      scrSaveTimer = millis();//Start the timer for screen saver
      //Serial << "Screen Save Timer set: " << scrSaveTimer << endl;
      simDisconnect = true;
      LCD.setColor(VGA_WHITE);
      LCD.setBackColor(VGA_RED);
      LCD.print("SIM DISCONNECT", x * 7, y * 12);
      Serial << "Attempting to reconnect to CDU Control Server..." << endl;
      Serial << "Screen blank in 60 minutes" << endl;
    }
    while (!client.available()) { //Loop until reconnected
      client.stop();
      //Serial << "P3D disconnected: " << (millis() - scrSaveTimer) / 1000 << " seconds" << endl;
      if (simDisconnect && (millis() - scrSaveTimer > 3600000)) {
        LCD.clrScr();
        Serial << "Screen blanked after sim disconnect timeout - Will continue to try connecting..." << endl;
      }
      if (client.connect(host, portClient)) {
        Serial << "Connected to CDU Control Server!" << endl;
        msg = "";
        quit = false;
        LCD.InitLCD();
        LCD.clrScr();
        simDisconnect = false;
        return;
      }
      delay (1000);
    }//end while
  }
  /*if (msg == "restart9") {
    quit = false;
    while (!client9.available()) {
      client9.stop();
      LCD.setFont(arial_small);
      LCD.setColor(VGA_WHITE);
      LCD.setBackColor(VGA_RED);
      LCD.print("Arduino DISCONNECT", x * 3, y * 12);
      Serial << "Attempting to reconnect to PMDG_9 Arduino...It won't display without this!" << endl;
      if (client9.connect(host9, PMDG_9_Client)) {
        Serial << "Connected to PMDG_9!" << endl;
        msg = "";
        quit = false;
        initLCD();
        return;
      }
      delay (1000);
    }
  }*/
}//END checkKeyInput

void loop() {

  if (client.available()) {
    int sz = client.read(recvBuffer, BUFFER_LEN);
    //Serial << "recvBuffer[" << sz << "]" << endl;
    //for (int i = 0; i < sz; i++) {
    //Serial << recvBuffer[i];
    //}
    //Serial << endl;
    
    int szm = sizeof(recvBuffer);

    if (recvBuffer[0] == '#') {
      POUND(szm, recvBuffer);
      parseDisplay();
      client.println("ACK"); //Basically a keepalive
      client.flush();
    }
    if (recvBuffer[0] == '!') {
      Serial << "Received a \"!\"" << endl;
      EXCLAMATION(szm, recvBuffer);
    }
  }
  if (client9.available()) {
    int sz = client9.read(recvBuffer, 4);
    Serial << "recvBuffer[" << sz << "]" << endl;
    //for (int i = 0; i < sz; i++) {
    //Serial << recvBuffer[i];
    //}
    Serial << endl;
    int szm = sizeof(recvBuffer);
    if (recvBuffer[0] == '$') {
      Serial << "Received a \"$\"" << endl;
      DOLLAR(szm, recvBuffer);
    }
  }
  if (!client.connected() && !quit) {
    Serial << "Socket disconnected on client" << endl;
    msg = "restart";
    checkKeyInput();
  }
  /*
  //This is really not necessary unless we use the BRT knob
  if (!client9.connected() && !quit) {
    Serial << "Socket disconnected on client9" << endl;
    msg = "restart9"; //Uncomment to reenable the BRT functionality
    checkKeyInput();
  }
  */
  while (Serial.available() > 0) {
    msg = Serial.readStringUntil('\r');
    Serial << "[" << msg.length() << "]" << msg << endl;
    checkKeyInput();
  }

  if (client.connected() && msg != "") {
    client.print(msg);
  }

  if (client.connected() && quit == true) {
    Serial << "Disconnecting client.  Quit = " << quit << endl;
    client.flush();
    client.stop();
    msg = "restart";
  }

  if (client9.connected() && quit == true) {
    Serial << "Disconnecting client9.  Quit = " << quit << endl;
    client9.flush();
    client9.stop();
    msg = "restart9";
  }
}//END MAIN

void POUND(int sz, char *msg) {
  //Serial << "in the POUND" << endl;
  char pbuf[sz];
  memcpy(pbuf, msg, sz);
  //Serial << "pbuf[" << sizeof(pbuf) << "]:" << pbuf << endl;
  int lp_count = 2; //Start at the third character of rx_1
  switch (char(pbuf[1])) {
    case 'X': //Rx Symbols
      //Serial << "Got an X!" << endl;
      for (int h = 0 ; h < 3; h++) {
        for (int i = 0; i < 14; ++i) {
          for (int j = 0; j < 24; ++j) {
            symbol[j][i][h] = pbuf[lp_count];
            lp_count++;
          }
        }
      }
      //Debugging Section
      /*
        //Display the chars
        for (int i = 0; i < 14; ++i) {
        for (int j = 0; j < 24; ++j) {
          //Serial.write(symbol[j][i][0]); //Show Character
          Serial.print(symbol[j][i][0]); //Show DEC
        }
        Serial.println();
        }
        //Debug display decimal vals
        for (int i = 0; i < 14; ++i) {
          for (int j = 0; j < 24; ++j) {
          Serial.write(symbol[j][i][1]);
        }
        Serial.println();
        }
        Serial.println();
        for (int i = 0; i < 14; ++i) {
          for (int j = 0; j < 24; ++j) {
          Serial.write(symbol[j][i][2]);
        }
        Serial.println();
        }
      */
      break;
    case 'R': //Forced screen refresh request
      //Serial << "Got an R!" << endl;
      for (int h = 0 ; h < 3; h++) {
        for (int i = 0; i < 14; ++i) {
          for (int j = 0; j < 24; ++j) {
            symbol[j][i][h] = pbuf[lp_count];
            lp_count++;
          }
        }
      }
    break;
  }//close switch
}

void EXCLAMATION(int sz, char *msg) {
  switch (msg[1]) {
    case 'B':  //Change the briteness of the display - This really makes no sense so disabling for now
      briteness = ( ((msg[2] - '0') * 100) + ((msg[3] - '0') * 10) + (msg[4] - '0') );
      //LCD.setBackColor(briteness, briteness, briteness);
      //LCD.clrScr();
      //lcd_display_all();
      if (console) Serial << "Briteness received: " << briteness << endl;
      break;
  }
}//END void EXCLAMATION

void DOLLAR(int sz, char *msg) { //From PMDG_9 Arduino
  switch (msg[1]) {
    case 'B': //Briteness Level - Funcationality TBD
      Serial << "Received $B" << endl;
      break;
  }
}

void parseDisplay() {
  //PMDG_NGX_CDU_COLOR_WHITE    0
  //PMDG_NGX_CDU_COLOR_CYAN     1
  //PMDG_NGX_CDU_COLOR_GREEN    2
  //PMDG_NGX_CDU_COLOR_MAGENTA  3
  //PMDG_NGX_CDU_COLOR_AMBER    4
  //PMDG_NGX_CDU_COLOR_RED      5
  for (int row = 0; row < 14; row ++) {
    for (int col = 0; col < 24; col ++) {
      switch (symbol[col][row][1] - 48) { //Decimal value of the char
        case 0:
          LCD.setColor(VGA_WHITE);
          break;
        case 1:
          LCD.setColor(VGA_AQUA);
          break;
        case 2:
          LCD.setColor(VGA_GREEN);
          break;
        case 3:
          LCD.setColor(VGA_FUCHSIA);
          break;
        case 4:
          LCD.setColor(VGA_YELLOW);
          break;
        case 5:
          LCD.setColor(VGA_RED);
          break;
      }//end switch color
      switch (symbol[col][row][2] - 48) { //Decimal value of the char
        //Setting the color within these cases can make it easier to read, but lose some of the highlighed areas like overweight in payload should be yellow
        //Obviously this could be fixed with code.
        case 0: //SMALL FONT
          LCD.setFont(arial_small);
          LCD.setBackColor(VGA_BLACK);
          break;
        case 1: //"UNUSED" as PMDG terms it - Dimmed font, we will just change the color to gray
          LCD.setFont(arial_small);
          LCD.setBackColor(VGA_BLACK);
          LCD.setColor(VGA_SILVER);
          break;
        case 2: //REVERSE
          LCD.setFont(arial_small_rev);
          //Background and color are reversed since our font is reversed
          LCD.setBackColor(VGA_BLACK);
          LCD.setColor(0xFFBF00); //Amber
          break;
        case 4: //An action that occurs, such as door CLOSING or OPENING
          LCD.setFont(arial_small);
          LCD.setBackColor(VGA_BLACK);
          LCD.setColor(0x505657);//Davys Gray
          break;
      }//end switch flags
      outputLCD(col, row);
    }
  }
}//END VOID
void outputLCD(int col, int row) {
  //Check to see if anything has actually changed, in which case, print it on the LCD - Otherwise don't waste time printing everything
  if (p_symbol[col][row][0] != symbol[col][row][0] || p_symbol[col][row][1] != symbol[col][row][1] || p_symbol[col][row][2] != symbol[col][row][2]) {
    //Serial.println(symbol[1][2], DEC);
    if (symbol[col][row][0] == -95) symbol[col][row][0] = 97;  //Substitute for the special arrows, sub for 'a'
    if (symbol[col][row][0] == -94) symbol[col][row][0] = 98;  //Substitute for the special arrows, sub for 'b'
    if (symbol[col][row][0] == -80) symbol[col][row][0] = 99; //Substitute for the degree symbol, sub for 'c'
    if (symbol[col][row][0] == -22) symbol[col][row][0] = 100; //Substitute for the box symbol, sub for 'd'
    LCD.printChar(symbol[col][row][0], (x * col) + xshift, (y * row));
    p_symbol[col][row][0] = symbol[col][row][0];
    p_symbol[col][row][1] = symbol[col][row][1];
    p_symbol[col][row][2] = symbol[col][row][2];
  }
}
/*void fullOutputLCD(int col, int row) {  //Used for a forced full refresh of the LCD requested from the main program
  if (symbol[col][row][0] == -95) symbol[col][row][0] = 97;  //Substitute for the special arrows, sub for 'a'
    if (symbol[col][row][0] == -94) symbol[col][row][0] = 98;  //Substitute for the special arrows, sub for 'b'
    if (symbol[col][row][0] == -80) symbol[col][row][0] = 99; //Substitute for the degree symbol, sub for 'c'
    if (symbol[col][row][0] == -22) symbol[col][row][0] = 100; //Substitute for the box symbol, sub for 'd'
    LCD.printChar(symbol[col][row][0], (x * col) + xshift, (y * row));
    p_symbol[col][row][0] = symbol[col][row][0];
    p_symbol[col][row][1] = symbol[col][row][1];
    p_symbol[col][row][2] = symbol[col][row][2];
}*/
