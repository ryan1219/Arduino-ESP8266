//#include <SoftwareSerial.h>
// uncomment follow line if use software serial port
//SoftwareSerial mySerial(11, 12); // Rx, Tx
void setup()
  // open the serial ports:
{
  Serial.begin(115200);
  Serial1.begin(115200);
  
  pinMode(13, OUTPUT); 
  digitalWrite(13, HIGH); //wifi chip enable
  delay(500);//delay
  Serial1.println("AT+RST");
  delay(500);
}

void loop() // run over and over
{
  if (Serial1.available())
    Serial.write(Serial1.read());
  if (Serial.available())
    Serial1.write(Serial.read());
}

