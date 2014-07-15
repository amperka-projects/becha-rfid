/*
We use Arduino Leonardo
*/

#include <SoftwareSerial.h>

#define UDP_HOST_IP        "192.168.10.159"
#define UDP_REMOTE_PORT    2014
#define UDP_LOCAL_PORT     2014

#define MY_SSID "Amperka.ru"
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
