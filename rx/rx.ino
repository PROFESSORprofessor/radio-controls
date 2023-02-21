#include "SPI.h"
#include "nRF24L01.h"
#include <RF24.h>
#include <Servo.h>

#define M_1 5
#define M_2 6
#define Led 2


Servo ch_1;
Servo ch_2;
Servo ch_3;

unsigned long last_time;
unsigned long new_time;

RF24 radio(9, 10);
const uint64_t pipe = 0xF0F1F2F3F4LL;

struct Signal
{
  byte y_2;
  byte x_2;
  byte y_1;
  byte x_1;

};
Signal data;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setChannel(0);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.openReadingPipe(1, pipe);
  radio.startListening();

  ch_1.attach(3);
  ch_2.attach(4);
  ch_3.attach(7);
  pinMode(M_1, OUTPUT);
  pinMode(M_2, OUTPUT);
  pinMode(Led, OUTPUT);

}

void loop() {
Serial.print(data.y_1);
Serial.print("  ");
Serial.print(data.x_1);
Serial.print("  ");
Serial.print(data.y_2);
Serial.print("  ");
Serial.println(data.x_2);


  ReciverData();
  new_time = millis();
  if (new_time - last_time > 1500)
  {
    data.y_1 = 0;
    data.x_1 = 0;
    data.y_2 = 0;
    data.x_2 = 0;
    digitalWrite(Led, LOW);
  }

  if (data.y_1 >= 130 and data.y_1 <= 255)
  {
    analogWrite(M_1, map(data.y_1, 130, 255, 0, 255));
    analogWrite(M_2, LOW);
  } else if (data.y_1 >= 0 and data.y_1 <= 120)
  {
    analogWrite(M_2 , map(data.y_1, 0, 120, 255, 0));
    analogWrite(M_1, LOW);
  } else
  {
    digitalWrite(M_1, LOW);
    digitalWrite(M_2, LOW);
  }

  ch_1.write(map(data.x_1, 0, 255, 0, 180));
  ch_2.writeMicroseconds(map(data.y_2, 0, 255, 1000, 2000));
  ch_3.write(map(data.x_2, 0, 255, 0, 180));

}

void ReciverData()
{
  while (radio.available())
  {
    radio.read(&data, sizeof(Signal));
    last_time = millis();
    digitalWrite(Led, HIGH);
  }

}
