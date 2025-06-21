
// Gelen komutu okuyup, PWM pinine uygular.
void setup() {
  Serial.begin(9600); // Serial port başlatılıyor
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read(); // Jetson'dan gelen komutu oku

    if (command == 'F') {
      // İleri git komutu
      // motoru çalıştır vb.
      Serial.println("Forward command received");
    }
    else if (command == 'S') {
      // Durdur komutu
      Serial.println("Stop command received");
    }
  }

  delay(10); // Çok hızlı dönmesin
}
