/*
We use Arduino Leonardo
*/

#define WIFI_DEBUG

#include <SoftwareSerial.h>
#include <WiFly.h>

// Wi-Fi preference

#define SSID      "MySSID" //your SSID
#define KEY       "MyWifiPassword"   //your wi-fi password
// WIFLY_AUTH_OPEN / WIFLY_AUTH_WPA1 / WIFLY_AUTH_WPA1_2 / WIFLY_AUTH_WPA2_PSK
#define AUTH      WIFLY_AUTH_WPA1_2

#define UDP_HOST_IP        "192.168.10.159"
#define UDP_REMOTE_PORT    2014
#define UDP_LOCAL_PORT     2014

WiFly wifly(Serial1);

SoftwareSerial rfidReader(10, 11);

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

#define LED_PIN 13

void setup()
{
  Serial1.begin(9600);

  rfidReader.begin(9600);

  Serial.begin(9600);

#ifdef WIFI_DEBUG
  while (!Serial)
    ;
#endif
  setupWiFly();

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
    rfidReader.write(getReadID[counter]);

  while (!rfidReader.available())
    ;

  delay(10); // wait for RX buffer fill up

  int i = 0;
  while (rfidReader.available() && i < RFID_DATA_LEN)
    incomingData[i++] = rfidReader.read();

  while (rfidReader.available())
    rfidReader.read(); //clear buffer
}


void setupWiFly()
{
  Serial.println("--------- WIFLY UDP --------");

  wifly.reset();

  while (1) {
    Serial.println("Try to join " SSID );
    if (wifly.join(SSID, KEY, AUTH)) {
      Serial.println("Succeed to join " SSID);
      wifly.clear();
      break;
    } else {
      Serial.println("Failed to join " SSID);
      Serial.println("Wait 1 second and try again...");
      delay(1000);
    }
  }

  setupUDP(UDP_HOST_IP, UDP_REMOTE_PORT, UDP_REMOTE_PORT);

  delay(1000);
  wifly.clear();
}

void setupUDP(const char *host_ip, uint16_t remote_port, uint16_t local_port)
{
  char cmd[32];

  wifly.sendCommand("set w j 1\r", "AOK");   // enable auto join

  wifly.sendCommand("set i p 1\r", "AOK");
  snprintf(cmd, sizeof(cmd), "set i h %s\r", host_ip);
  wifly.sendCommand(cmd, "AOK");
  snprintf(cmd, sizeof(cmd), "set i r %d\r", remote_port);
  wifly.sendCommand(cmd, "AOK");
  snprintf(cmd, sizeof(cmd), "set i l %d\r", local_port);
  wifly.sendCommand(cmd, "AOK");
  wifly.sendCommand("save\r");
  wifly.sendCommand("reboot\r");
}
