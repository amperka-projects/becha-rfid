/*
We use Arduino Leonardo
*/

#include <SoftwareSerial.h>

#define MY_SSID            "Amperka.ru"
#define MY_PASSWORD        "password"
#define MY_WIFI_CHANNEL    "6"

#define MY_SECURITY_WPA_TKIP    "2"
#define MY_SECURITY_WPA_MIXED   "3"
#define MY_SECURITY_WPA2_AES    "4"

#define JOIN_MODE_MANUAL   "0"
#define JOIN_MODE_AUTO     "1"

#define UDP_HOST_IP        "192.168.10.159"
#define UDP_REMOTE_PORT    "2014"
#define UDP_LOCAL_PORT     "2014"

#define LED_PIN 13

SoftwareSerial mySerial(10, 11);

const char getReadID[] =  { 0xAA , 0x00, 0x03, 0x25, 0x26, 0x00, 0x00, 0xBB };
const char nothing[] =    { 0xAA , 0x00, 0x02, 0x01, 0x83, 0x80, 0xBB }; //

#define GET_READ_ID_LEN   (sizeof(getReadID) / sizeof(getReadID[0]))
#define NOTHING_LEN       (sizeof(nothing) / sizeof(nothing[0]))
#define RFID_DATA_LEN     13

#define THROTTLE_DELAY    300
#define REPEAT_DELAY      5000


char incomingData[RFID_DATA_LEN];
char sendData[RFID_DATA_LEN];

unsigned long timePoint = 0;
unsigned long timeClearPoint = 0;

void setup() 
{
  Serial1.begin(9600);
  mySerial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
}


void loop() 
{
  while (Serial1.available())
    Serial1.read(); //clear buffer

  // send an UDP packet periodically
  if (millis() - timePoint > THROTTLE_DELAY) {

    readRfidData();

    bool dataDiffers = memcmp(sendData, incomingData, RFID_DATA_LEN);
    bool hasData = memcmp(incomingData, nothing, NOTHING_LEN);

    if (dataDiffers && hasData) {
      memcpy(sendData, incomingData, RFID_DATA_LEN);

      for (int i = 0; i < RFID_DATA_LEN; ++i)
        Serial1.print(sendData[i], HEX);

      Serial1.print("\r\n");
    }

    timePoint = millis();

    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }

  if (millis() - timeClearPoint > REPEAT_DELAY) { 
    // clear stored data after to repeat send
    memset(sendData, 0, RFID_DATA_LEN);
    timeClearPoint = millis();
  }
}


void readRfidData()
{
  for (int counter = 0; counter < GET_READ_ID_LEN; ++counter)
    mySerial.write(getReadID[counter]);

  while (!mySerial.available())
    ;

  delay(10); // wait for RX buffer fill up

  int i = 0;
  while (mySerial.available() && i < RFID_DATA_LEN)
      incomingData[i++] = mySerial.read();

  while (mySerial.available())
    mySerial.read(); //clear buffer
}


void setupWiFly()
{
  delay(1000);

  readWiFlyMessages();

// command mode
  sendWiFlyPreference("$$$");

// wi-fi settings
  sendWiFlyPreference("set wlan join "    JOIN_MODE_AUTO       "\n");
  sendWiFlyPreference("set wlan ssid "    MY_SSID              "\n");
  sendWiFlyPreference("set wlan auth "    MY_SECURITY_WPA_TKIP "\n");
  sendWiFlyPreference("set wlan phrase "  MY_PASSWORD          "\n");
  sendWiFlyPreference("set wlan channel " MY_WIFI_CHANNEL      "\n");

// network settings
  sendWiFlyPreference("set ip proto 1");                      // enable UDP as the protocol
  sendWiFlyPreference("set ip host "   UDP_HOST_IP     "\n"); // set the IP address of remote host
  sendWiFlyPreference("set ip remote " UDP_REMOTE_PORT "\n"); // set the remote port number on which the host is listening
  sendWiFlyPreference("set ip local "  UDP_LOCAL_PORT  "\n"); // set the port number on which the WiFly module will listen

// save new settings
  sendWiFlyPreference("save\n");                                // saves the settings in config file
  sendWiFlyPreference("reboot\n");                              // reboots the module so that the above settings take effect

  delay(1000);

  readWiFlyMessages();
}

void sendWiFlyPreference(const char *preferenceText)
{
  Serial1.print(preferenceText);

  if (Serial) {
    Serial.println("");
    Serial.print("> ");
    Serial.println(preferenceText);
  }

  while (!Serial1.available())
    ;

  delay(10);

  readWiFlyMessages();
}

void readWiFlyMessages()
{
  while (Serial1.available()) {
    if (Serial)
      Serial.write(Serial1.read());
    else
      Serial1.read();
  }
}

