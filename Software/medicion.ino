#include <LiquidCrystal.h>
const float r1=978; //en kohm
const float e_r1 = r1*0.005+0.02; //en kohm
const float r2=97.4; //en kohm
const float e_r2 = r1*0.005+0.02; //en kohm
const float e_v = 5/1024; //en V
const int N = 100; // numero de mediciones
const float e_vn = e_v/sqrt(N);
float r;
float e_r;
LiquidCrystal lcd(12, 11, 10, 9, 8, 7); //rs,e,d4,d5,d6,d7
byte pmChar[8] = {
  B00100,
  B00100,
  B11111,
  B00100,
  B00100,
  B00000,
  B11111,
  B00000,
};

byte ohmChar[8] = {
  B00000,
  B01110,
  B10001,
  B10001,
  B10001,
  B01010,
  B11011,
  B00000,
};
void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.createChar(0, pmChar);
  lcd.createChar(1, ohmChar);
}

void loop() {
  lcd.clear();
  float v0;
  float v1;
  float vR;
  char mode[] = "none";
  v0=voltageRead(A0,N);
  v1=voltageRead(A2,N);//A1 does not work
  vR=voltageRead(A3,1);
  if (vR<1){
    r=r1;
    e_r=e_r1;
    strcpy(mode,"1 M");
  }

  if (vR>4){
    r=r2;
    e_r=e_r2;
    strcpy(mode,"100 k");
  }

  float Z = v0/v1*r;
  float e_z = sqrt(pow(r/v1*e_vn,2)+pow(v0*r*e_vn/pow(v1,2),2)+pow(v0/v1*e_r,2));
  
  //display
  if (Z>=1000){lcd.print(Z/1000,2); lcd.print(" M");lcd.write((byte)1);}
  else {lcd.print(Z,2); lcd.print(" K");lcd.write((byte)1);}
  lcd.setCursor(0,1);
  lcd.print("Escala:");lcd.print(mode);lcd.print((byte)1);
  delay(2000);
}

float voltageRead(int A,int nreads){
  int i;
  unsigned int val = 0;
  for (i=0;i<nreads;i++){
    val=val+analogRead(A);
  }

  float voltage = val/nreads * 5.0/1024;
  return voltage;
}
