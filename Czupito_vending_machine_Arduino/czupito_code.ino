//czupito v1.0 - robocza nazwa urządzenia
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Stepper.h>

LiquidCrystal_I2C lcd(0x27,16,2);


  const int ledpin=13;
  const int pomp_1=2;
  const int pomp_2=3;
  const int pomp_3=4;
  const int pomp_4=5;

  int state=0;
  const int krokinaobrot = 200;

  int odczyt_fotorez = 0;
  
Stepper silnik(krokinaobrot, 8, 9, 10, 11);

void napoj_1()
{
    
    lcd.setCursor(0,1);
    lcd.print("    Napoj 1    ");
    Serial.println("Wybrano Napoj 1");
    delay(2000);
    lcd.setCursor(0,1);
    lcd.print(" Przygotowywanie");
    pobierz_kubek();
    delay(1000);
        
    lcd.setCursor(0,1);
    lcd.print(" Przygotowywanie");
    Serial.println("Przygotowanie");
        
    digitalWrite(pomp_1,LOW);
    delay(2500);
    digitalWrite(pomp_1,HIGH);
    delay(2000);
       
    lcd.setCursor(0,1);
    lcd.print("     Gotowe     ");
    Serial.println(" Odbierz napój ");
}

void napoj_2()
{
    lcd.setCursor(0,1);
    lcd.print("    Napoj 2    ");
    Serial.println("wybrano Napoj 2");
    delay(2000);
    lcd.setCursor(0,1);
    lcd.print(" Przygotowywanie");
    pobierz_kubek();
    delay(1000);
        
    lcd.setCursor(0,1);
    lcd.print(" Przygotowywanie");
    Serial.println("Przygotowanie");
    
    digitalWrite(pomp_2,LOW);
    delay(2500);
    digitalWrite(pomp_2,HIGH);
    delay(2000);

    lcd.setCursor(0,1);
    lcd.print("     Gotowe     ");
    Serial.println(" Odbierz napój ");
}

void mix_1()
{
    lcd.setCursor(0,1);
    lcd.print("      Mix 1    ");
    Serial.println("Wybrano Mix 1");
    delay(2000);
    lcd.setCursor(0,1);
    lcd.print(" Przygotowywanie");
    pobierz_kubek();
    delay(1000);
        
    lcd.setCursor(0,1);
    lcd.print(" Przygotowywanie");
    Serial.println("Przygotowanie");
        
    digitalWrite(pomp_1,LOW);
    delay(1500);
    digitalWrite(pomp_1,HIGH);
    delay(500);
    digitalWrite(pomp_3,LOW);
    delay(600);
    digitalWrite(pomp_3,HIGH);
    delay(500);
    digitalWrite(pomp_4,LOW);
    delay(400);
    digitalWrite(pomp_4,HIGH);
    delay(2000);
       
    lcd.setCursor(0,1);
    lcd.print("     Gotowe     ");
    Serial.println("Odbierz napój");
}

void mix_2()
{
    lcd.setCursor(0,1);
    lcd.print("      Mix 2    ");
    Serial.println("Wybrano Mix 2");
    delay(2000);
    lcd.setCursor(0,1);
    lcd.print(" Przygotowywanie");
    pobierz_kubek();
    delay(1000);
    
    lcd.setCursor(0,1);
    lcd.print(" Przygotowywanie");
    Serial.println("Przygotowanie");
    
    digitalWrite(pomp_2,LOW);
    delay(1500);
    digitalWrite(pomp_2,HIGH);
    delay(500);
    digitalWrite(pomp_3,LOW);
    delay(1000);
    digitalWrite(pomp_3,HIGH);
    delay(2000);


    lcd.setCursor(0,1);
    lcd.print("     Gotowe     ");
    Serial.println("Odbierz napój");
}

void pobierz_kubek()
{
  test_poprz_kubek();
      lcd.setCursor(0,1);
    lcd.print(" Przygotowywanie");
    Serial.println("Przygotowanie");
 silnik.setSpeed(100);
  for (int i = 0; i <= 2100; i++) 
  {
    silnik.step(krokinaobrot / 200);
  }
  delay(500);
}

void test_poprz_kubek()
{
  odczyt_fotorez = analogRead(A0);
  if (odczyt_fotorez<100)
  {
   digitalWrite(ledpin,HIGH);
   Serial.println(" Odbierz poprzedni napój! ");
   lcd.setCursor(0,1);
   lcd.print("Przyg.wstrzymane");
   do
   {
      analogRead(A0);
      digitalWrite(ledpin,HIGH);
      delay(500);
      digitalWrite(ledpin,LOW);
      delay(500);
    }while(analogRead(A0)<100);
  }
  else {
     digitalWrite(ledpin,LOW);    

    }
}


void setup() {
  pinMode(ledpin, OUTPUT);
  pinMode(pomp_1, OUTPUT);
  pinMode(pomp_2, OUTPUT);
  pinMode(pomp_3, OUTPUT);
  pinMode(pomp_4, OUTPUT);

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Czupito v1.0");

        digitalWrite(ledpin,LOW);
        digitalWrite(pomp_1,HIGH);
        digitalWrite(pomp_2,HIGH);
        digitalWrite(pomp_3,HIGH);
        digitalWrite(pomp_4,HIGH);

  
}

void loop() 
{
  if (Serial.available()>0)
    {
    int state= Serial.read();
    if (state=='1')
      {
        napoj_1();
      }
    if (state=='2')
      {
        napoj_2();
      }
       if (state=='3')
      {
        mix_1();
      }
    if (state=='4')
      {
        mix_2();
      }
  }
  delay(1000);
}
