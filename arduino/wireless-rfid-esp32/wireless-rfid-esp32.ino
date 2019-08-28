#include <WiFi.h>
#include <HTTPClient.h>
#include <MFRC522.h>
#include <SPI.h>
#define SS_PIN    21
#define RST_PIN   22
#define buzzerPin 13
#define ON HIGH
#define OFF LOW

MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;
MFRC522 mfrc522(SS_PIN, RST_PIN);

unsigned long cardKey;

char httpLink[250];
char httpAPI[] = "http://192.168.1.2/wireless-rfid-sample-time-in-out/web/api/record.php";

const int type = 1; // IN (Entry)

void setup() {
  Serial.begin(9600);
  
  pinMode(buzzerPin, OUTPUT);
  
  SPI.begin();
  mfrc522.PCD_Init();

  WiFi.begin("SSID", "WIFI_PASSWORD");
  // Change SSID with your WiFi Name and WIFI_PASSWORD with your WiFi Password

  delay(1000);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      beep(1, 50);
      Serial.println(F("."));
  }

  delay(1000);
  beep(5, 50);
  Serial.println(F("READY"));
}

void loop() {
  if((WiFi.status() == WL_CONNECTED)) {
    if(readCard()){
      sendData();
    }
  }else{
    delay(500);
    beep(1, 50);
  }

}

void sendData(){
  HTTPClient http;
  sprintf(httpLink, "%s?uid=%lu&type=%d", httpAPI, cardKey, type);
  Serial.println(httpLink);
  http.begin(httpLink);
  int httpCode = http.GET();
  if(httpCode > 0) {
      if(httpCode == 201) {
          String payload = http.getString();
          Serial.println(payload);
          beep(3, 50);
      }else{
        beep(5, 50);
      }
  } else {
      Serial.printf("ERROR: %s\n", http.errorToString(httpCode).c_str());
      beep(5, 50);
  }

  http.end();
}

bool readCard(){
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return false;
  }

  cardKey =  mfrc522.uid.uidByte[0] << 24;
  cardKey += mfrc522.uid.uidByte[1] << 16;
  cardKey += mfrc522.uid.uidByte[2] <<  8;
  cardKey += mfrc522.uid.uidByte[3];

  Serial.println(cardKey);

  mfrc522.PICC_HaltA(); 
  mfrc522.PCD_StopCrypto1();

  return true;
}

void beep(int t, int d){
  for(int i = 0; i < t; i++){
    digitalWrite(buzzerPin, ON);
    delay(d);
    digitalWrite(buzzerPin, OFF);
    delay(d);
  }
}
