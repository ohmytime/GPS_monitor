#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

byte smiley[8] = {
  B00100,
  B01110,
  B11111,
  B00000,
  B00100,
  B01110,
  B11111,
};

byte sat1[8] = {
  B00110,
  B01110,
  B01110,
  B01111,
  B01110,
  B01110,
  B01100,
};

byte sat2[8] = {
  B01110,
  B00100,
  B01110,
  B11111,
  B01110,
  B00100,
  B11111,
};

byte sat3[8] = {
  B01100,
  B01110,
  B01110,
  B11110,
  B01110,
  B01110,
  B00110,
};

TinyGPSPlus gps;
SoftwareSerial gpsSerial(6, 7);
const int serialTime = 200;

void setup() {
  lcd.createChar(1, smiley);
  lcd.createChar(2, sat1);
  lcd.createChar(3, sat2);
  lcd.createChar(4, sat3);
  lcd.begin(16, 2);
  lcd.clear();
  gpsSerial.begin(9600);
  lcd.setCursor(0, 0);
  lcd.print(" CAPTURING ");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("SATELLITES");
  lcd.setCursor(12, 1);
  lcd.write(2);
  lcd.write(3);
  lcd.write(4);
  delay(10000);
  lcd.clear();
}  // end setup

void loop() {
  int start = millis();
  int now = millis();

  do {
    while (gpsSerial.available() > 0) {
      gps.encode(gpsSerial.read());
    }
    now = millis();
  } while ((now - start) < serialTime);

  if (gps.altitude.isUpdated()) {
    int h;
    h = (gps.time.hour()) + 8;

    if (24 <= h) {
      h = h - 24;
    }

    if (h < 10) {
      lcd.setCursor(0, 0);
      lcd.print(" ");
      lcd.setCursor(1, 0);
      lcd.print(h);
    } else {
      lcd.setCursor(0, 0);
      lcd.print(h);
    }

    lcd.setCursor(2, 0);
    lcd.print(":");
    int m;
    m = gps.time.minute();

    if (m < 10) {
      lcd.setCursor(3, 0);
      lcd.print("0");
      lcd.setCursor(4, 0);
      lcd.print(m);
    } else {
      lcd.setCursor(3, 0);
      lcd.print(m);
    }
  }

  if (gps.altitude.isUpdated()) {
    lcd.setCursor(7, 0);
    lcd.print("Alt:");
    int koudo;
    koudo = (int)(gps.altitude.meters());
    if (koudo == 0) {
      lcd.setCursor(11, 0);
      lcd.print("----");
    } else if (koudo < 10) {
      lcd.setCursor(11, 0);
      lcd.print(" ");
      lcd.setCursor(14, 0);
      lcd.print(koudo);
    } else if (koudo < 100) {
      lcd.setCursor(11, 0);
      lcd.print(" ");
      lcd.setCursor(13, 0);
      lcd.print(koudo);
    } else if (koudo < 1000) {
      lcd.setCursor(11, 0);
      lcd.print(" ");
      lcd.setCursor(12, 0);
      lcd.print(koudo);
    } else {
      lcd.setCursor(11, 0);
      lcd.print(koudo);
    }
    lcd.setCursor(15, 0);
    lcd.print("m");
  }

  if (gps.speed.isUpdated()) {
    int s;
    s = (int)(gps.speed.kmph());
    if (s == 0) {
      int satV;
      satV = (int)(gps.satellites.value());
      if (satV < 10) {
        lcd.setCursor(0, 1);
        lcd.print(" ");
        lcd.setCursor(1, 1);
        lcd.print(satV);
      } else {
        lcd.setCursor(0, 1);
        lcd.print(satV);
      }
      lcd.setCursor(2, 1);
      lcd.write(2);
      lcd.write(3);
      lcd.write(4);
      lcd.print("s ");
    } else if (s < 10) {
      lcd.setCursor(0, 1);
      lcd.print(" ");
      lcd.setCursor(2, 1);
      lcd.print(s);
      lcd.print("Km/h");
    } else if (s < 100) {
      lcd.setCursor(0, 1);
      lcd.print(" ");
      lcd.setCursor(1, 1);
      lcd.print(s);
      lcd.print("Km/h");
    } else {
      lcd.setCursor(0, 1);
      lcd.print(s);
      lcd.print("Km/h");
    }
  }

  if (gps.course.isUpdated()) {
    if ((gps.course.deg()) == 0) {
      lcd.setCursor(9, 1);
      lcd.print("WAITING");
    } else if ((gps.course.deg()) < 11.3) {
      lcd.setCursor(9, 1);
      lcd.print("W--N--E");
    } else if ((gps.course.deg()) < 33.8) {
      lcd.setCursor(9, 1);
      lcd.print("--N");
      lcd.setCursor(12, 1);
      lcd.write(1);
      lcd.setCursor(13, 1);
      lcd.print("--E");
    } else if ((gps.course.deg()) < 56.3) {
      lcd.setCursor(9, 1);
      lcd.print("-N-");
      lcd.setCursor(12, 1);
      lcd.write(1);
      lcd.setCursor(13, 1);
      lcd.print("-E-");
    } else if ((gps.course.deg()) < 78.8) {
      lcd.setCursor(9, 1);
      lcd.print("N--");
      lcd.setCursor(12, 1);
      lcd.write(1);
      lcd.setCursor(13, 1);
      lcd.print("E--");
    } else if ((gps.course.deg()) < 101.3) {
      lcd.setCursor(9, 1);
      lcd.print("N--E--S");
    } else if ((gps.course.deg()) < 123.8) {
      lcd.setCursor(9, 1);
      lcd.print("--E");
      lcd.setCursor(12, 1);
      lcd.write(1);
      lcd.setCursor(13, 1);
      lcd.print("--S");
    } else if ((gps.course.deg()) < 146.3) {
      lcd.setCursor(9, 1);
      lcd.print("-E-");
      lcd.setCursor(12, 1);
      lcd.write(1);
      lcd.setCursor(13, 1);
      lcd.print("-S-");
    } else if ((gps.course.deg()) < 168.8) {
      lcd.setCursor(9, 1);
      lcd.print("E--");
      lcd.setCursor(12, 1);
      lcd.write(1);
      lcd.setCursor(13, 1);
      lcd.print("S--");
    } else if ((gps.course.deg()) < 191.3) {
      lcd.setCursor(9, 1);
      lcd.print("E--S--W");
    } else if ((gps.course.deg()) < 213.8) {
      lcd.setCursor(9, 1);
      lcd.print("--S");
      lcd.setCursor(12, 1);
      lcd.write(1);
      lcd.setCursor(13, 1);
      lcd.print("--W");
    } else if ((gps.course.deg()) < 236.3) {
      lcd.setCursor(9, 1);
      lcd.print("-S-");
      lcd.setCursor(12, 1);
      lcd.write(1);
      lcd.setCursor(13, 1);
      lcd.print("-W-");
    } else if ((gps.course.deg()) < 258.8) {
      lcd.setCursor(9, 1);
      lcd.print("S--");
      lcd.setCursor(12, 1);
      lcd.write(1);
      lcd.setCursor(13, 1);
      lcd.print("W--");
    } else if ((gps.course.deg()) < 281.3) {
      lcd.setCursor(9, 1);
      lcd.print("S--W--N");
    } else if ((gps.course.deg()) < 303.8) {
      lcd.setCursor(9, 1);
      lcd.print("--W");
      lcd.setCursor(12, 1);
      lcd.write(1);
      lcd.setCursor(13, 1);
      lcd.print("--N");
    } else if ((gps.course.deg()) < 326.3) {
      lcd.setCursor(9, 1);
      lcd.print("-W-");
      lcd.setCursor(12, 1);
      lcd.write(1);
      lcd.setCursor(13, 1);
      lcd.print("-N-");
    } else if ((gps.course.deg()) < 348.8) {
      lcd.setCursor(9, 1);
      lcd.print("W--");
      lcd.setCursor(12, 1);
      lcd.write(1);
      lcd.setCursor(13, 1);
      lcd.print("N--");
    } else {
      lcd.setCursor(9, 1);
      lcd.print("W--N--E");
    }
  }
}
