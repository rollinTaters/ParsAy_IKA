// created on 08/03/2025 by F4tihozkra


#include <SoftwareSerial.h>
#include <TinyGPS++.h>

SoftwareSerial gpsSerial(4, 3); // GPS TX -> Arduino D4, GPS RX -> Arduino D3
TinyGPSPlus gps;

void setup() {
    Serial.begin(9600);     // Bilgisayar ile haberleşme
    gpsSerial.begin(9600);  // GPS modülü ile haberleşme
}

void loop() {
    while (gpsSerial.available()) {
        gps.encode(gpsSerial.read()); // GPS verisini işle

        if (gps.location.isUpdated()) { // Yeni veri geldi mi?
            Serial.print("Enlem: ");
            Serial.print(gps.location.lat(), 6);
            Serial.print(" , Boylam: ");
            Serial.println(gps.location.lng(), 6);
        }
    }
}
