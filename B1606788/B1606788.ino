#include <EEPROMex.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myservo;
//Dung cho dong co buoc
int tocdo = 10;
uint16_t  goc = EEPROM.readInt(5);
uint16_t  goc_hientai = EEPROM.readInt(5);
String leeCadena;
const int b1 = 2;
const int b2 = 3;
const int b3 = 4;
const int in    = 7;
const int out   = 6;

const int lua    = 12;
const int coi    = 13;
const int anhsang = 16;
const int led    = 17;

int count = 0;
int count1 = 0;
int count2 = 0;
int count3 = 0;
int valin = 0;
int valout = 0;
int pos = 0;
int tt_lua = 0;
int tt_anhsang = 0;

void setup() {
  Serial.begin(9600);
  //Chan dong co buoc
  pinMode(19, OUTPUT);
  pinMode(b1, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
  pinMode(b3, INPUT_PULLUP);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  //ir hong ngoai
  pinMode(in, INPUT);
  pinMode(out, INPUT);

  pinMode(lua, INPUT);
  pinMode(coi, OUTPUT);
  pinMode(anhsang, INPUT);
  pinMode(led, OUTPUT);

  //servo
  myservo.attach(5);
  myservo.write(0);
  for (pos = 0; pos <= 90; pos += 1) {
    myservo.write(pos);
    delay(30);
  }
  delay(1000);
  for (pos = 90; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(30);
  }

  //lcd
  lcd.init();
  lcd.backlight();

  //bo nho eeprom
  count1 = EEPROM.read(2);
  count2 = EEPROM.read(3);
  count3 = EEPROM.read(4);
  goc = EEPROM.readInt(5);
  //  chay lai gia tri ban dau
  //  EEPROM.write(5, (goc+1));
  //  EEPROM.write(2, (count1+1));
  //  EEPROM.write(3, (count2+1));
  //  EEPROM.write(4, (count3+1));
}
// hien thi ra man hinh
void lcdPrint() {
  //bao chay
  if (tt_lua == LOW) {
    digitalWrite(coi, HIGH);
    lcd.clear();
    delay(500);
    lcd.setCursor(4, 0);
    lcd.print("Co Chay");
    lcd.setCursor(3, 1);
    lcd.print("Chay Nhanh");
    delay(500);
    lcd.clear();
  }
  //so luong va full
  if (count1 >= 2 && count2 >= 2 && count3 >= 2) {
    lcd.clear();
    lcd.setCursor(1, 0);
    delay(500);
    lcd.print("Nha Xe Da Day");
    delay(500);
  } else {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Parking");
    lcd.setCursor(0, 1);
    lcd.print("S1:");
    lcd.print(count1);
    lcd.setCursor(6, 1);
    lcd.print("S2:");
    lcd.print(count2);
    lcd.setCursor(12, 1);
    lcd.print("S3:");
    lcd.print(count3);
    delay(500);
  }
}

//so luong xe o cac slot khi vao
void countIn() {
  if (goc == 0) {
    count1++;
    EEPROM.write(2, (count1 + 1));
    if (count1 >= 2) {
      count1 = 2;
      EEPROM.write(2, (count1 + 1));
    }
  }
  if (goc == 170) {
    count2++;
    EEPROM.write(3, (count2 + 1));
    if (count2 >= 2) {
      count2 = 2;
      EEPROM.write(3, (count2 + 1));
    }
  }
  if (goc == 341) {
    count3++;
    EEPROM.write(4, (count3 + 1));
    if (count3 >= 2) {
      count3 = 2;
      EEPROM.write(4, (count3 + 1));
    }
  }
}

//so luong xe o các slot khi ra
void countOut() {
  if (goc == 0) {
    count1--;
    EEPROM.write(2, (count1 + 1));
    if (count1 <= 0) {
      count1 = 0;
      EEPROM.write(2, (count1 + 1));
    }
  }
  if (goc == 170) {
    count2--;
    EEPROM.write(3, (count2 + 1));
    if (count2 <= 0) {
      count2 = 0;
      EEPROM.write(3, (count2 + 1));
    }
  }
  if (goc == 341) {
    count3--;
    EEPROM.write(4, (count3 + 1));
    if (count3 <= 0) {
      count3 = 0;
      EEPROM.write(4, (count3 + 1));
    }
  }
}

//chay motor tu dong khi ir vao
void autoMotorIn() {
  if ((count1 < 2) || (count2 < 2 && count2 > 0) || (count3 < 2 && count3 > 0)) {
    goc = 0;
    delay(tocdo);
    goc = (goc * 1.4222222222);
    EEPROM.writeInt(5, (goc + 1));
  }
  if ((count1 == 2 && count2 < 2) || (count3 < 2 && count3 > 0)) {
    goc = 120;
    delay(tocdo);
    goc = (goc * 1.4222222222);
    EEPROM.writeInt(5, (goc + 1));
  }
  if (count1 == 2 && count2 == 2 && count3 < 2) {
    goc = 240;
    delay(tocdo);
    goc = (goc * 1.4222222222);
    EEPROM.writeInt(5, (goc + 1));
  }
  RunMotor();
}

//kiem tra xem dang o tang nao
int checkTang(int goc_hientai) {
  if (goc_hientai == 340 && goc_hientai == 350) {
    goc_hientai == 341;
    return 3;
  } else if (goc_hientai == 170 && goc_hientai == 180) {
    goc_hientai == 170;
    return 2;
  } else {
    goc_hientai == 0;
    return 3;
  }
}

//xem tang co xe khong
int checkTangcoxe() {
  if (count3 != 0) {
    return 341;
  }
  else if (count2 != 0) {
    return 170;
  }
  else if (count1 != 0) {
    return 0;
  }
  else return 0 ;
}

//chay motor tu dong kho ir ra
void autoMotorOut() {
  int tanghientai = checkTang(goc);
  if (tanghientai == 1) {
    goc = checkTangcoxe();
    EEPROM.writeInt(5, (goc + 1));
  } else if (tanghientai == 2) {
    goc = checkTangcoxe();
    EEPROM.writeInt(5, (goc + 1));
  } else {
    goc = checkTangcoxe();
    EEPROM.writeInt(5, (goc + 1));
  }
  RunMotor();
}

//dieu khien motor quay trai hay phai
void RunMotor() {
  while (goc > goc_hientai) {
    right();
    goc_hientai = goc_hientai + 1;
  }
  while (goc < goc_hientai) {
    left();
    goc_hientai = goc_hientai - 1;
  }
  stopMotor();
}

//motor quay sang phai
void right() {
  digitalWrite(11, LOW);
  digitalWrite(10, LOW);
  digitalWrite(9, HIGH);
  digitalWrite(8, HIGH);
  delay(tocdo);

  digitalWrite(11, LOW);
  digitalWrite(10, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(8, LOW);
  delay(tocdo);

  digitalWrite(11, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(9, LOW);
  digitalWrite(8, LOW);
  delay(tocdo);

  digitalWrite(11, HIGH);
  digitalWrite(10, LOW);
  digitalWrite(9, LOW);
  digitalWrite(8, HIGH);
  delay(tocdo);
}

//motor quay sang trai
void left() {
  digitalWrite(11, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(9, LOW);
  digitalWrite(8, LOW);
  delay(tocdo);

  digitalWrite(11, LOW);
  digitalWrite(10, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(8, LOW);
  delay(tocdo);

  digitalWrite(11, LOW);
  digitalWrite(10, LOW);
  digitalWrite(9, HIGH);
  digitalWrite(8, HIGH);
  delay(tocdo);

  digitalWrite(11, HIGH);
  digitalWrite(10, LOW);
  digitalWrite(9, LOW);
  digitalWrite(8, HIGH);
  delay(tocdo);
}

//dung motor
void stopMotor() {
  digitalWrite(11, LOW);
  digitalWrite(10, LOW);
  digitalWrite(9, LOW);
  digitalWrite(8, LOW);
}

//thuc hien khi cam bien ir ra co tin hieu
void irOut() {
  valout = digitalRead(out);
  if (valout == LOW) {
    goc = EEPROM.readInt(5) - 1;
    count1 = EEPROM.read(2) - 1;
    count2 = EEPROM.read(3) - 1;
    count3 = EEPROM.read(4) - 1;
    myservo.attach(5);
    for (pos = 0; pos <= 90; pos += 1) {
      myservo.write(pos);
      delay(30);
    }
    while (valout == LOW) {
      valout = digitalRead(out);
    }
    countOut();
    if (count <= 0) {
      count = 0;
    }
    if (count1 <= 0) {
      count1 = 0;
    }
    delay(1000);
    for (pos = 90; pos >= 0; pos -= 1) {
      myservo.write(pos);
      delay(30);
    }
    autoMotorOut();
    stopMotor();
  }
}

//thuc hien khi cam bien ir vao co tin hieu
void irIn() {
  valin = digitalRead(in);
  goc = EEPROM.readInt(5) - 1;
  count1 = EEPROM.read(2) - 1;
  count2 = EEPROM.read(3) - 1;
  count3 = EEPROM.read(4) - 1;
  if (valin == LOW) {
    for (pos = 0; pos <= 90; pos += 1) {
      myservo.write(pos);
      delay(30);
    }
    while (valin == LOW) {
      valin = digitalRead(in);
    }
    delay(1000);
    for (pos = 90; pos >= 0; pos -= 1) {
      myservo.write(pos);
      delay(30);
    }
    autoMotorIn();
    stopMotor();
    countIn();
  }
}

//dung servo khi full
void full() {
  if (count1 >= 2 && count2 >= 2 && count3 >= 2) {
    myservo.write(0);
    delay(1000);
    myservo.detach();
  }
}

void btn() {
  if (digitalRead(b1) == LOW) {
    goc = 0;
    delay(tocdo);
    goc = (goc * 1.4222222222);
    EEPROM.writeInt(5, (goc + 1));
    RunMotor();
  } else if (digitalRead(b2) == LOW) {
    goc = 120;
    delay(tocdo);
    goc = (goc * 1.4222222222);
    EEPROM.writeInt(5, (goc + 1));
    RunMotor();
  } else if (digitalRead(b3) == LOW) {
    goc = 240;
    delay(tocdo);
    goc = (goc * 1.4222222222);
    EEPROM.writeInt(5, (goc + 1));
    RunMotor();
  }
}

void fire() {
  tt_lua = digitalRead(lua);
  if (tt_lua == LOW) {
    digitalWrite(coi, HIGH);
  } else {
    digitalWrite(coi, LOW);
  }
}

void light() {
  tt_anhsang = digitalRead(anhsang);
  if (tt_anhsang == HIGH) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }
}

void loop() {
  fire();
  light();
  btn();
  full();
  irOut();
  irIn();
  lcdPrint();
}
