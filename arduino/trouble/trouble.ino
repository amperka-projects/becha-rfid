/*
We use Arduino Leonardo
*/

#include <SoftwareSerial.h>

#define UDP_HOST_IP        "192.168.10.159"
#define UDP_REMOTE_PORT    "2014"
#define UDP_LOCAL_PORT     "2014"

#define MY_SSID            "Amperka.ru"
#define MY_PASSWORD        "password"
#define MY_WIFI_CHANNEL    "6"


void setupWiFly()
{
  delay(1000);

  readWiFlyMessages();

  sendWiFlyPreference("$$$");
  sendWiFlyPreference("set ip proto 1");                      // enable UDP as the protocol
  sendWiFlyPreference("set ip host "   UDP_HOST_IP     "\n"); // set the IP address of remote host
  sendWiFlyPreference("set ip remote " UDP_REMOTE_PORT "\n"); // set the remote port number on which the host is listening
  sendWiFlyPreference("set ip local "  UDP_LOCAL_PORT  "\n"); // set the port number on which the WiFly module will listen
  sendWiFlyPreference("save");                                // saves the settings in config file
  sendWiFlyPreference("reboot");                              // reboots the module so that the above settings take effect

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

SoftwareSerial mySerial(10, 11);
//char txrxbuffer[255];

char get_readID[] = { 0xAA , 0x00, 0x03, 0x25, 0x26, 0x00, 0x00, 0xBB };
char nothing[] =    { 0xAA , 0x00, 0x02, 0x01, 0x83, 0x80, 0xBB }; //

//AA0218380BB

char incomingData[13];

char sendData[13];

void setup() {

  Serial1.begin(9600);

  //  setupUDP();

  //  delay(1000);

  mySerial.begin(9600);


  Serial.begin(57600);
  while (!Serial)
  {
    ;
  }
}

unsigned int time_point = 0;

unsigned int timeClearPoint = 0;

unsigned int iterCounter = 0;





void loop() {

  while (Serial1.available()) {
    Serial1.read(); //clear buffer
  }

  // send an UDP packet in every 1 second
  if ((millis() - time_point) > 300) {

    readRfidData();

    if ((memcmp(sendData, incomingData, sizeof(incomingData))) && (memcmp(incomingData, nothing, sizeof(nothing))))
    {
      memcpy(sendData, incomingData, sizeof(incomingData));


      for (int i = 0; i < sizeof(sendData); ++i)
      {
        Serial1.print(sendData[i], HEX);
        Serial.print(sendData[i], HEX);

      }
      Serial1.print("\r\n");

      Serial.print(" - ");
      Serial.println(millis() / 1000);
      Serial.println(++iterCounter);
      /*      Serial.print("Serial1 - ");
            Serial.println(Serial1.available());

            Serial.print("mySerial1 - ");
            Serial.println(mySerial.available());
      */
    }

    time_point = millis();

    digitalWrite(13, !digitalRead(13));
  }

  if (millis() - timeClearPoint > 2000)   // clear stored data after 5 sec
  {
    memset(sendData, 0, sizeof(sendData));
    timeClearPoint = millis();
  }

}


void readRfidData()
{

  for (int counter = 0 ; counter < 8 ; counter++) {
    mySerial.write(get_readID[counter]);
  }

  while (!mySerial.available())
  {
    ;
  }

  for (int i = 0; (i < sizeof(incomingData)) && (mySerial.available()); ++i)
  {
    incomingData[i] = mySerial.read();
  }
  while ((mySerial.available()))
  {
    mySerial.read(); //clear buffer
  }

}

