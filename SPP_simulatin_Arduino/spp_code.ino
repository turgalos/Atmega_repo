//SPP 
#include <Servo.h>

int fotorez = A0;  //pin analogowy do zczytywania wartości z dzielnika napięcia fotorezystor - rezystor
int stanfotorez = 0; //wartość zczytana z wejścia analogowego z zakresu 0-1024, określająca czy występuje dym
byte buczek = 5;  //zmienna przypisująca port buczka
Servo klapadymowa; //serwo - model automatyki klap ppoż.
int kontrolajakosci= 13; //pin 13 do kontroli działania ukladu - informuje czy układ jest włączony czy wyłączony
const byte przycisk = 2; //zmienna przypisująca port przycisku
int ostrzeganie = 7; //zmienna przypisująca port diody ostrzegawczej 

volatile bool alarm = true; //zmienna określająca czy jest alarm
volatile byte stan; //zmienna określająca stan pracy ukladu - włączony lub wyłaczony

int stanPrzycisku;      //zmienne wykorzystane do debuncingu     
int poprzedniStanPrzycisku = LOW;

unsigned long poprzedniCzasDebounce = 0;  
unsigned long debounceOpoznienie = 50; 

void setup() {
   //Serial.begin(9600); //przesył danych
   pinMode(buczek, OUTPUT); //deklaracja buczka jako wyjście 
   pinMode(kontrolajakosci, OUTPUT); //deklaracja diody kontoroli jakosci jako wyjście 
   pinMode(ostrzeganie, OUTPUT);  //deklaracja diody ostrzegawczej jakosci jako wyjście 
   pinMode(przycisk, INPUT_PULLUP);  //deklaracja przryciku
   digitalWrite(buczek, LOW);  //początkowy stan buczka - niski
   klapadymowa.attach(9);     //deklaracja serwo - klapy p.poż 
   klapadymowa.write(90);     //początkowy stan serwo
   digitalWrite(kontrolajakosci, LOW); //początkowy stan diody kontoroli jakosci
   digitalWrite(ostrzeganie, LOW); //początkowy stan diody ostrzegawczej
  
   stan = HIGH;
   
}
void loop() {
  stanfotorez = analogRead(fotorez); //zczytanie wartości z fotorezystora
  //Serial.println(stanfotorez);  //wypisanie wartości w monitorze portu - do kalibracji
  
//część programu odpowiedzialna za debouncing - usuwanie zakłóceń wynikających z konstrukcji przyciku 
//oraz działanie przycisku

 int zczytanie = digitalRead(przycisk);
    if (zczytanie != poprzedniStanPrzycisku) 
    {
      
    poprzedniCzasDebounce = millis();
    }
  if ((millis() - poprzedniCzasDebounce ) > debounceOpoznienie) {
   //zadziałanie przycisku
    if (zczytanie != stanPrzycisku) {
      stanPrzycisku = zczytanie;
      if (stanPrzycisku == HIGH) {
         alarm = !alarm;
         stan = !stan;
      }
    }
  }
  
  digitalWrite(kontrolajakosci, state);//przypisanie diodzie kontroli jaosci odpowiedniego stanu 
  poprzedniStanPrzycisku = reading;

//warunki logiczne sprawdzające czy jest pożar

  if (stanfotorez <850 && alarm==true )//stan ostrzegawczy
  {
    digitalWrite(ostrzeganie, HIGH);//dioda ostrzegawcza włączona
    
    if (stanfotorez <750 && alarm==true)  //stan pożaru - DYM
  {
    digitalWrite(ostrzeganie, LOW);//dioda ostrzegawcza wyłączona
    klapadymowa.write(180); //zamknięcie klap p.poż
    digitalWrite(buczek, HIGH); //emisja dźwięku buczka
    delay(50);
    digitalWrite(buczek, LOW);
    delay(25);
    
  } 
  
  }
//normalny stan - brak pożaru
  else
  {
    digitalWrite(ostrzeganie, LOW); //dioda ostrzegawcza wyłączona
    klapadymowa.write(90);   //klapy p.poż otwarte
    digitalWrite(buczek, LOW);  //buczek wyłączony
    digitalWrite(kontrolajakosci, stan); //stan diody zależny od stanu układu (włączony/wyłączony)
  }
}
