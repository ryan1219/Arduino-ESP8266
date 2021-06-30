const long SLEEP = 12; // sleep time, unit: hour
const long WORK = 30;  // work time, unit: second

const unsigned long SECOND = 1000;
const unsigned long MINUTE = 60 * SECOND;
const unsigned long HOUR = 3600 * SECOND;

void setup()
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop()
{
  digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
  delay(WORK * SECOND);
  digitalWrite(LED_BUILTIN, LOW); // turn the LED off by making the voltage LOW
  delay(SLEEP * HOUR);
}
