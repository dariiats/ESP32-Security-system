#include <Arduino.h>
#include <Wire.h>
#include "MFRC522_I2C.h"
#include "BluetoothSerial.h"
#include "string.h"

#define RED_LED 15
#define BLUE_LED 2
#define BUZZER_PIN 4
#define PIR_PIN 16

MFRC522 mfrc522(0x28);
BluetoothSerial SerialBT;

char buffer[20];

String rfid_str = "";
byte pirStat = 0;

const String CMD_ALARM = "alarm";
const String CMD_IGNORE = "ignore";

static int threatLevel = 0;
static bool warningSent = false;

void Signal();

void StopSignal();

void AlarmLoop();

void CheckPIR();

void SendWarning();

void HandleIncomingBT();

String ReadCard();

bool CheckAccess(String uid);

void setup()
{
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(PIR_PIN, INPUT);

  Wire.begin();
  mfrc522.PCD_Init();

  Serial.begin(115200);

  SerialBT.begin("ESP32Security");
}

void loop()
{
  if (!SerialBT.hasClient())
  {
    Serial.println("Waiting for phone connection...");
    StopSignal();
    delay(1000);
    return;
  }

  CheckPIR();

  Serial.print("PIR State: ");
  Serial.println(digitalRead(PIR_PIN));

  if (threatLevel > 3)
  {
    SendWarning();
    HandleIncomingBT();
  }

  delay(1000);
}

void Signal()
{
  digitalWrite(RED_LED, 1);
  digitalWrite(BLUE_LED, 0);
  digitalWrite(BUZZER_PIN, 0);
  delay(500);

  digitalWrite(RED_LED, 0);
  digitalWrite(BLUE_LED, 1);
  digitalWrite(BUZZER_PIN, 1);
  delay(500);
}

void StopSignal()
{
  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(BUZZER_PIN, LOW);
}

String ReadCard()
{
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    delay(50);
    return "";
  }

  if (!mfrc522.PICC_ReadCardSerial())
  {
    delay(50);
    return "";
  }

  rfid_str = "";
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    rfid_str += String(mfrc522.uid.uidByte[i], HEX);
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  return rfid_str;
}

bool CheckAccess(String uid)
{
  const String allowedUID = "6f0169";

  return (uid == allowedUID);
}

void AlarmLoop()
{
  while (true)
  {
    Signal();

    String uid = ReadCard();

    if (uid != "")
    {
      if (CheckAccess(uid))
      {

        StopSignal();

        SerialBT.println("Alarm off!");

        threatLevel = 0;
        warningSent = false;

        break;
      }
    }
  }
}

void CheckPIR()
{
  if (digitalRead(PIR_PIN) == 1)
    threatLevel++;
}

void SendWarning()
{
  if (!warningSent)
  {
    SerialBT.println("WARNING! Threat detected! Send 'alarm' or 'ignore'.");
    warningSent = true;
  }
}

void HandleIncomingBT()
{
  if (SerialBT.available())
  {
    String incoming = SerialBT.readString();
    incoming.trim();

    if (incoming == CMD_ALARM)
    {
      AlarmLoop();
    }
    else if (incoming == CMD_IGNORE)
    {
      threatLevel = 0;
      warningSent = false;
    }
    else
    {
      warningSent = false;
    }
  }
}
