// library
#include <VirtualWire.h>
#include <SoftwareSerial.h>

byte message[VW_MAX_MESSAGE_LEN]; // a buffer to store the incoming messages
byte messageLength = VW_MAX_MESSAGE_LEN; // the size of the message

//serielle Verbindung einrichten
SoftwareSerial rxSerial(7, 8); // RX, TX

void setup()
{
  delay(1000);
  // pinMode(6, OUTPUT);
  rxSerial.begin(4800);
  vw_set_rx_pin(4); //Pin fuer Empfang durch Funkmodul
  vw_setup(2000); // bps
  vw_rx_start();
}

void loop()
{
  if (vw_get_message(message, &messageLength)) // non-blocking
  {
    // digitalWrite(6, HIGH);
    
    for (int i = 0; i < messageLength; i++)
    {
      rxSerial.write(message[i]);
    }
    rxSerial.println();
    // digitalWrite(6, LOW);
  }
}
