/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://docs.arduino.cc/hardware/

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://docs.arduino.cc/built-in-examples/basics/Blink/
*/


            // !!!!!!ATTENTİON!!!!!!!
//THAT CODE IS JUST FOR A TEST SO IT IS USELESS

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600); // USB üzerinden seri haberleşme başlatılıyor
  while (!Serial) {
    ; // Jetson bağlanana kadar bekle
  }
  Serial.println("Arduino hazır!");
}

void loop() {
  // Jetson'dan veri var mı kontrol et
  if (Serial.available()) {
    String gelen = Serial.readStringUntil('\n'); // Satır sonuna kadar oku
    Serial.print("Geldiiii: ");
    Serial.println(gelen); // Geri cevap olarak gönder

    // Örnek olarak LED yakabiliriz
    if (gelen == "LED ON") {
      // dijital bir pine HIGH ver (örnek: pin 13)
      digitalWrite(13, HIGH);
    } else if (gelen == "LED OFF") {
      digitalWrite(13, LOW);
    }
  }

  delay(100);
}

