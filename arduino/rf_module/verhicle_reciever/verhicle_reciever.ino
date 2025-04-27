#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE ve CSN pinleri 9 ve 10 a 
const byte address[6] = "00001";

struct Data {
  int xAxis;
  int yAxis;
};

Data data;

// Motor pinleri
const int motorLeftForward = 3; //donebilen tekerlekler icin
const int motorLeftBackward = 4;
const int motorRightForward = 5;
const int motorRightBackward = 6;

void setup() {
  Serial.begin(9600);

 
  pinMode(motorLeftForward, OUTPUT);
  pinMode(motorLeftBackward, OUTPUT);
  pinMode(motorRightForward, OUTPUT);
  pinMode(motorRightBackward, OUTPUT);

  radio.begin(); //nrf24 modulu baslatilir
  radio.openReadingPipe(0, address); //belirtilen adresten veri alma isini yapar
  radio.setPALevel(RF24_PA_MIN); // stabil calismasi icin sinyal gucunu minumuma ayarladik
  radio.startListening(); //modulu dinleme moduna aldik yani veri bekliyor
}

void loop() {
  if (radio.available()) { //veri geldi mi kontrolu
    radio.read(&data, sizeof(Data));

    Serial.print("Received -> X: ");
    Serial.print(data.xAxis); //gelen veriyi data icine okuyoruz
    Serial.print(" | Y: ");
    Serial.println(data.yAxis);

    kontrolMotorlar(); //joystickten gelen verilere gore motoru kontol edecek olan fonksiyon
  }
}

void kontrolMotorlar() {
  int x = data.xAxis;
  int y = data.yAxis;

 //hafif sapmalari dikkate almamak icin toleransimiz
  int tolerans = 45;

  // İleri gider
  if (y > 512 + tolerans) {
    ileri();
  }
  // Geri gider
  else if (y < 512 - tolerans) {
    geri();
  }
  // Sağa dön
  else if (x > 512 + tolerans) {
    sagaDon();
  }
  // Sola dön
  else if (x < 512 - tolerans) {
    solaDon();
  }
  // durdur
  else {
    dur();
  }
}

// Motor fonksiyonları
void ileri() {
  digitalWrite(motorLeftForward, HIGH); //iki tekerleğin ileri gideni calisir
  digitalWrite(motorLeftBackward, LOW);
  digitalWrite(motorRightForward, HIGH);
  digitalWrite(motorRightBackward, LOW);
}

void geri() {
  digitalWrite(motorLeftForward, LOW);
  digitalWrite(motorLeftBackward, HIGH); //geri gidenleri calistirilir
  digitalWrite(motorRightForward, LOW);
  digitalWrite(motorRightBackward, HIGH);
}

void sagaDon() {
  digitalWrite(motorLeftForward, HIGH);
  digitalWrite(motorLeftBackward, LOW);  //aracin tank gibi oldugu yerde donmesini saglar
  digitalWrite(motorRightForward, LOW);
  digitalWrite(motorRightBackward, HIGH);
}

void solaDon() {
  digitalWrite(motorLeftForward, LOW);
  digitalWrite(motorLeftBackward, HIGH);
  digitalWrite(motorRightForward, HIGH);
  digitalWrite(motorRightBackward, LOW);
}

void dur() {
  digitalWrite(motorLeftForward, LOW);
  digitalWrite(motorLeftBackward, LOW);
  digitalWrite(motorRightForward, LOW);
  digitalWrite(motorRightBackward, LOW);
}
