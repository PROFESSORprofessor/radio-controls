#include "SPI.h"
#include <nRF24L01.h>
#include<RF24.h>

RF24 radio(9, 10);
const uint64_t pipe = 0xF0F1F2F3F4LL;
struct Signals
{
  byte y_2;
  byte x_2;
  byte y_1;
  byte x_1;
};
Signals data;

void setup() {
Serial.begin(9600);
radio.begin();
radio.setChannel(0);
radio.setDataRate(RF24_250KBPS);
radio.setPALevel(RF24_PA_HIGH);
radio.openWritingPipe(pipe);
radio.stopListening();
}

void loop() {

//  Serial.print(analogRead(A0));
//  Serial.print("  ");
//  Serial.print(analogRead(A1));
//  Serial.print("  ");
//  Serial.print(analogRead(A2));
//  Serial.print("  ");
//  Serial.println(analogRead(A3));


  Serial.print(data.y_1);
  Serial.print("  ");
  Serial.print(data.x_1);
  Serial.print("  ");
  Serial.print(data.y_2);
  Serial.print("  ");
  Serial.println(data.x_2);
  
  data.y_2 = Border_f(analogRead(A0), 0, 512, 1023);
  data.x_2 = Border_f(analogRead(A1), 0, 516, 1023);
  data.y_1 = Border_f(analogRead(A2), 0, 517, 1023);
  data.x_1 = Border_f(analogRead(A3), 0, 496, 1023);

  radio.write(&data, sizeof(Signals));

}

int Border_f(int val, int lower, int middle, int upper)
{
  val = constrain(val, lower, upper);
  if(val > middle)
  {
    val = map(val, middle, upper, 128, 255);
  }
  else
  {
    val = map(val, lower, middle, 0, 128);  
  }
  return 255 - val;
}
