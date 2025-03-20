/*
  GPIO Interrupt mode test code. 
  Use Numaker-UNO-M467 board. 
  Connect D2 and D10.
*/


int trigger_pin = D10;
int interrupt_pin = D2;
// These constants won't change. They're used to give names to the pins used:

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);

  pinMode(trigger_pin, OUTPUT);
  pinMode(interrupt_pin, INPUT_PULLUP);
  
  digitalWrite(trigger_pin, LOW);
  attachInterrupt(interrupt_pin, cb_test, RISING);
  
}

void loop() {

  // Generate pulse on D10 pin
  digitalWrite(trigger_pin, LOW);
  Serial.println("Set trigger_pin LOW");
  delay(500);
  // wait 0.5 seconds before the next loop for the analog-to-digital
  
  digitalWrite(trigger_pin, HIGH);
  Serial.println("Set trigger_pin HIGH");
  delay(500);
  // wait 0.5 seconds before the next loop for the analog-to-digital
}


void cb_test() {
  Serial.println("cb_test go");
}