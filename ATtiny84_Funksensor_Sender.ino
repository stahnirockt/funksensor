#include <VirtualWire.h>
#include <dht11.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

dht11 DHT11;
#define DHT11PIN 4

int sender_id = 1;

//Pins für die Stromversorgung
int vcc_dht = 5;
int vcc_funk = 6;

void setup() {

  //Ausgänge zur Stromversorgung
  pinMode(vcc_dht, OUTPUT);
  pinMode(vcc_funk, OUTPUT);

  //Pin für die Datenübertragung des Funkmoduls einrichten
  vw_set_tx_pin(3);
  vw_setup(2000);

  //Watchdog-Timer einrichten
  
  MCUSR &= ~(1<<WDRF); //clear reset flag
  
  // To change WDE or the prescaler, we need to set WDCE
  WDTCSR |= (1<<WDCE) | (1<<WDE);

  WDTCSR = 1<<WDP0 | 1<<WDP3; //Watchdog-Timer set for 8.0 seconds
  
  WDTCSR |= _BV(WDIE); // Enable the WD interrupt
  
}

void loop()
{

  //Temperatursensor mit Strom versorgen und auslesen
  digitalWrite(vcc_dht, HIGH);
  delay (500);

  int chk = DHT11.read(DHT11PIN);
 
  float temp_sensor = DHT11.temperature;
  float hum_sensor = DHT11.humidity;
  delay (500); 
  digitalWrite(vcc_dht, LOW);

  //Funksender mit Strom versorgen, Nachricht versenden
  digitalWrite(vcc_funk, HIGH);
  delay (500);
  sendData(sender_id ,temp_sensor, hum_sensor, true); 
  delay (500); 
  digitalWrite(vcc_funk, LOW);
  sleep_fn(1);
}


//Funktion um die Daten des Sensors zu versenden
void sendData(int id, int temp, int hum, bool wait){
 
  //Werte in String umwandeln
  String tempString = String(temp);
  String humString = String(hum);
  String message = String(id);
  
  //Message zusammenstellen
  message += "-" + tempString + "-" + humString;
  
  //Message versenden
  byte messageLength = message.length() + 1; 

  // string in char array umwandeln
  char charBuffer[messageLength]; 
  message.toCharArray(charBuffer, messageLength);

  //Daten versenden
  vw_send((uint8_t *)charBuffer, messageLength); 

  if (wait) vw_wait_tx();  
  }


//Funktion um den ATtiny in den Schlafmodus zu schicken
void sleep_fn(int sleepTimes)
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  for(int i = 1; i <= sleepTimes; i++){
 
    sleep_enable();
    sleep_mode(); /* Now enter sleep mode. */
    
    /* The program will continue from here after the WDT timeout*/
    sleep_disable();
  }
}

ISR(WDT_vect) {
  //Watchdog Interrupt Handler - nothing to do
  }
