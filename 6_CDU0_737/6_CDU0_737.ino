// PMDG_6_CDU0_737NG3 for FSProject
// Interfaces with CDU0Manager via TCP (192.168.1.5:27015)
// Receives 1008-byte #X packets (336 Symbols, 3 bytes each)
// Displays 24x14 CDU on UTFT SSD1963 480x272
// No keypad support (handled by Arduino #9)
// Changelog:
// 6-20-2018: Switched to W5500, Ethernet2.h
// 5-11-2019: TCP instead of UDP, fixed offsets, static IP
// 1-4-2020: Updated for 737 NG3
// 7-3-2024: Used for MSFS 777, one-way TCP receive
// 3-15-2025: Noted Ethernet connection issues (USB/power)
// 5-15-2025: Updated for FSProject #X 1008-byte Symbol-only packets, removed keypad, simplified arrays, Ethernet fixes

#include <Ethernet2.h>
#include <Streaming.h>
#include <UTFT.h>

// Ethernet settings
static byte mymac[] = {0x72, 0x2A, 0x2A, 0x2D, 0x30, 0xF6};
IPAddress myip(192, 168, 1, 150);
IPAddress gwip(192, 168, 1, 1);
IPAddress dnsip(192, 168, 1, 1);
IPAddress mask(255, 255, 255, 0);
IPAddress server(192, 168, 1, 5); // CDU0Manager server
static unsigned int portServer = 27015;
#define BUFFER_LEN 337 // #, X, 335 Symbols (process in chunks)
char recvBuffer[BUFFER_LEN];
EthernetClient client;

// TFT settings
UTFT LCD(SSD1963_480272, 38, 39, 40, 41); // RS, WR, CS, RST
extern uint8_t arial_small[];
extern uint8_t arial_small_rev[];

int y = 19; // Row offset
int x = 17; // Column offset
int xshift = 32; // Char spacing
long scrSaveTimer;
bool simDisconnect = false;

// CDU data
//The entire display symbols, colors, and flags get sent from the server together
char symbol[24][14][3]; //14*(24 + 2 for \n file) 14*(24+1 for \n via TCP)
char p_symbol[24][14][3]; //Previous symbol for comparison - If there's no change, we don't print it again
//char color[25][14];
//char p_color[25][14];
//char flags[25][14];
//char p_flags[25][14];

//Misc vars
boolean console = true;
bool quit = false;
String msg = "";

void setup() {
    Serial.begin(115200);
    Serial.println(F("\n[6_CDU0_737]"));

    // Backlight
    pinMode(8, OUTPUT);
    digitalWrite(8, HIGH);

    // Initialize LCD
    LCD.InitLCD();
    LCD.clrScr();
    LCD.setBackColor(VGA_BLUE);
    LCD.setBrightness(5);
    LCD.setFont(arial_small);
    LCD.setColor(0xCC6600);
    LCD.print("737 NG3/777X CDU*", x + (xshift * 3), y);
    LCD.setFont(arial_small_rev);
    LCD.setColor(VGA_BLACK);
    LCD.print("Dustin Moss(2019-25)", x + (xshift * 2), y + 32);
    LCD.setBackColor(VGA_BLACK);

    // Initialize Ethernet
    if (Ethernet.begin(mymac) == 0) {
        Serial.println("DHCP failed, using static...");
        Ethernet.begin(mymac, myip, gwip, mask);
        LCD.setBackColor(VGA_GREEN);
        LCD.print("DHCP FAILED - Check Power ", x * 2, y * 9);
        LCD.print("STATIC 192.168.1.", x * 3, y * 10);
        LCD.printNumI(myip[3], x * 19, y * 10);
    } else {
        Serial << "DHCP Successful!" << endl;
        myip = Ethernet.localIP();
        Serial << "IP Address:  ";
        char ip[4] = { '1', '1', '1', '1' };
        sprintf(ip, "%d.%d.%d.%d", myip[0], myip[1], myip[2], myip[3]);
        Serial.println(ip);
        Serial << endl << "Mac Address:  ";
        char mac[6] = { 'FF', 'FF', 'FF', 'FF', 'FF', 'FF' };
        sprintf(mac, "%01x:%01x:%01x:%01x:%01x:%01x", mymac[0], mymac[1], mymac[2], mymac[3], mymac[4], mymac[5]);
        Serial.println(mac);
        Serial.println();
        LCD.setBackColor(VGA_GREEN);
        LCD.print("DHCP 192.168.1.", x * 5, y * 10);
        LCD.printNumI(myip[3], x * 19, y * 10);
    }
    
    // Test Ethernet connection
    pinMode(LED_BUILTIN, OUTPUT);
    connectToServer();
}

void connectToServer() {
    client.stop();
    if (!client.connect(server, portServer)) {
        Serial << "Connection failed to " << server[0] << "." << server[1] << "." << server[2] << "." << server[3] << ":" << portServer << endl;
        for (int i = 0; i < 5; i++) {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(200);
            digitalWrite(LED_BUILTIN, LOW);
            delay(200);
        }
    } else {
        Serial << "Connected to " << server[0] << "." << server[1] << "." << server[2] << "." << server[3] << ":" << portServer << endl;
    }
}

void initLCD() {
    LCD.InitLCD();
    LCD.clrScr();
    LCD.setBackColor(VGA_BLACK);
    LCD.setBrightness(5);
    LCD.setFont(arial_small);
    LCD.setColor(0xCC6600);
}

void checkKeyInput() {
    if (msg == "quit") {
        quit = true;
        Serial << "Quit detected, closing socket" << endl;
    }
    if (msg == "restart") {
        quit = false;
        if (!client.available()) {
            scrSaveTimer = millis();
            simDisconnect = true;
            LCD.setColor(VGA_WHITE);
            LCD.setBackColor(VGA_RED);
            LCD.print("SIM DISCONNECT", x * 7, y * 12);
            Serial << "Reconnecting to CDU Control Server..." << endl;
            Serial << "Screen blank in 60 minutes" << endl;
        }
        while (!client.available()) {
            client.stop();
            if (simDisconnect && (millis() - scrSaveTimer > 3600000)) {
                LCD.clrScr();
                Serial << "Screen blanked after timeout" << endl;
            }
            if (client.connect(server, portServer)) {
                Serial << "Connected to CDU Control Server!" << endl;
                msg = "";
                quit = false;
                initLCD();
                return;
            }
            // Blink LED to indicate power check
            digitalWrite(LED_BUILTIN, HIGH);
            delay(200);
            digitalWrite(LED_BUILTIN, LOW);
            delay(800);
        }
    }
}

void loop() {
    // Handle TCP data
    if (client.connected() && client.available()) {
        int sz = 0;
        unsigned long start = millis();
        while (sz < BUFFER_LEN && millis() - start < 1000) {
            int bytesRead = client.read((uint8_t*)recvBuffer + sz, BUFFER_LEN - sz);
            if (bytesRead > 0) {
                sz += bytesRead;
            } else if (bytesRead == 0 || bytesRead == -1) {
                break;
            }
        }
        if (sz == 1010 && recvBuffer[0] == '#' && recvBuffer[1] == 'X') {
            POUND(sz, recvBuffer);
            parseDisplay();
            client.println("ACK");
            client.flush();
            Serial << "Received #X packet, size: " << sz << endl;
        } else {
            Serial << "Invalid packet, size: " << sz << ", header: ";
            if (sz >= 2) {
                Serial.print(recvBuffer[0]);
                Serial.print(recvBuffer[1]);
            }
            Serial.println();
            logInvalidPacket(sz);
        }
    }

    // Reconnect if disconnected
    if (!client.connected() && !quit) {
        Serial << "Socket disconnected" << endl;
        msg = "restart";
        checkKeyInput();
    }

    // Handle serial commands
    while (Serial.available() > 0) {
        msg = Serial.readStringUntil('\r');
        Serial << "[" << msg.length() << "]" << msg << endl;
        checkKeyInput();
    }

    if (client.connected() && msg != "") {
        client.print(msg);
    }

    if (client.connected() && quit) {
        Serial << "Disconnecting, Quit = " << quit << endl;
        client.flush();
        client.stop();
        msg = "restart";
    }
}

void logInvalidPacket(int sz) {
    Serial << "Invalid packet hex (first 10 bytes):\n";
    for (int i = 0; i < 10 && i < sz; i++) {
        if ((unsigned char)recvBuffer[i] < 16) Serial << "0";
        Serial.print((unsigned char)recvBuffer[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}

void POUND(int sz, char *msg) {
    if (sz != 1010 || msg[0] != '#' || msg[1] != 'X') {
        Serial << "POUND: Invalid packet size: " << sz << " or header: #" << msg[1] << endl;
        return;
    }
    int lp_count = 2; // Start after #X
    // Debug first row
    Serial << "Raw Symbol values (first row):\n";
    for (int j = 0; j < 24; j++) {
        char c = msg[lp_count + (j * 14 * 3)];
        Serial << "Col " << j << ": " << (int)c << " (0x";
        if ((unsigned char)c < 16) Serial << "0";
        Serial.print((unsigned char)c, HEX);
        Serial << ")\n";
    }
    for (int col = 0; col < 24; col++) {
        for (int row = 0; row < 14; row++) {
            symbol[col][row][0] = msg[lp_count++]; // Symbol
            symbol[col][row][1] = msg[lp_count++]; // Color
            symbol[col][row][2] = msg[lp_count++]; // Flags
        }
    }
}

void parseDisplay() {
    for (int row = 0; row < 14; row++) {
        for (int col = 0; col < 24; col++) {
            switch (symbol[col][row][1] - 48) { // Color (0-5)
                case 0: LCD.setColor(VGA_WHITE); break;
                case 1: LCD.setColor(VGA_AQUA); break;
                case 2: LCD.setColor(VGA_GREEN); break;
                case 3: LCD.setColor(VGA_FUCHSIA); break;
                case 4: LCD.setColor(VGA_YELLOW); break;
                case 5: LCD.setColor(VGA_RED); break;
                default: LCD.setColor(VGA_WHITE); break;
            }
            switch (symbol[col][row][2] - 48) { // Flags (0-4)
                case 0: // Small font
                    LCD.setFont(arial_small);
                    LCD.setBackColor(VGA_BLACK);
                    break;
                case 1: // Dimmed
                    LCD.setFont(arial_small);
                    LCD.setBackColor(VGA_BLACK);
                    LCD.setColor(VGA_SILVER);
                    break;
                case 2: // Reverse
                    LCD.setFont(arial_small_rev);
                    LCD.setBackColor(VGA_BLACK);
                    LCD.setColor(0xFFBF00); // Amber
                    break;
                case 4: // Action
                    LCD.setFont(arial_small);
                    LCD.setBackColor(VGA_BLACK);
                    LCD.setColor(0x505657); // Davys Gray
                    break;
                default: 
                    LCD.setFont(arial_small);
                    LCD.setBackColor(VGA_BLACK);
                    break;
            }
            outputLCD(col, row);
        }
    }
}

void outputLCD(int col, int row) {
    if (p_symbol[col][row][0] != symbol[col][row][0] ||
        p_symbol[col][row][1] != symbol[col][row][1] ||
        p_symbol[col][row][2] != symbol[col][row][2]) {
        char c = symbol[col][row][0];
        if (c == -95) c = 'a'; // Arrow
        if (c == -94) c = 'b'; // Arrow
        if (c == -80) c = 'c'; // Degree
        if (c == -22) c = 'd'; // Box
        LCD.printChar(c, (x * col) + xshift, (y * row));
        for (int h = 0; h < 3; h++) {
            p_symbol[col][row][h] = symbol[col][row][h];
        }
    }
}