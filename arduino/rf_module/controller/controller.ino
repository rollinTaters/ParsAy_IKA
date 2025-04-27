#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN pinleri (nano icin D9 ve D10)

const byte address[6] = "00001";

struct Data {
  int xAxis;
  int yAxis;
};

Data data;

void setup() {
  Serial.begin(9600); //haberleşme hızını 9600 bit olarak ayarladım
  radio.begin(); //NRF24L01 modülü başlatılıyor
  radio.openWritingPipe(address); //5 bytleık kanal adresine ayarlanıyo
  radio.setPALevel(RF24_PA_MIN); //modülün göndeim gücü ayarlanir
  radio.stopListening(); //kumandadaki verileri gönderir
}

void loop() {
  data.xAxis = analogRead(A0); // joystick X ekseni icin A0 pininde
  data.yAxis = analogRead(A1); // Joystick Y ekseni için A1 pininde
  
  radio.write(&data, sizeof(Data));
  
  Serial.print("X: ");
  Serial.print(data.xAxis);
  Serial.print(" | Y: ");
  Serial.println(data.yAxis);
  
  delay(42); 
}
