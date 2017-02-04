// libraries
#include <VirtualWire.h>
#include <SoftwareSerial.h>

byte message[VW_MAX_MESSAGE_LEN]; // buffer zum Speichern eingehender Nachrichten
byte messageLength = VW_MAX_MESSAGE_LEN; // Groeße der Nachricht

//serielle Verbindung einrichten (--> wird spaeter vom RasPi ausgewertet)
SoftwareSerial rxSerial(7, 8); // RX (i.d.R. gruenes Kabel), TX (i.d.R. weißes Kabel)

void setup()
{
  delay(1000);
  rxSerial.begin(4800);
  vw_set_rx_pin(4); //Pin fuer Empfang durch Funkmodul
  vw_setup(2000); // bps
  vw_rx_start();
}

void loop()
{
  //Empfang durch das Funkmodul und Weitergabe ueber die serielle Verbindung
  if (vw_get_message(message, &messageLength)) // non-blocking
  { 
    for (int i = 0; i < messageLength; i++)
    {
      rxSerial.write(message[i]);
    }
    rxSerial.println();
  }
}
