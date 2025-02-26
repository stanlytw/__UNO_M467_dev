/*
  Analog input, analog output, serial output

  Reads an analog input pin, maps the result to a range from 0 to 255 and uses
  the result to set the pulse width modulation (PWM) of an output pin.
  Also prints the results to the Serial Monitor.

  The circuit:
  - potentiometer connected to analog pin 0.
    Center pin of the potentiometer goes to the analog pin.
    side pins of the potentiometer go to +5V and ground
  - LED connected from digital pin 9 to ground through 220 ohm resistor

  created 29 Dec. 2008
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogInOutSerial
*/
#define TEST_PWM
//#define TEST_ADC

// These constants won't change. They're used to give names to the pins used:
#ifdef TEST_ADC
const int analogInPin = A5;  // Analog input pin that the potentiometer is attached to
int sensorValue = 0;  // value read from the pot
#endif

#ifdef TEST_PWM
const int analogOutPin = D8;  // Analog output pin that the LED is attached to
int dacoutputValue = 1000;  // dacvalue output to the PWM (analog out)
#endif

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}

void loop() {
#ifdef TEST_ADC
  //ADC test
  // read the analog in value:
  sensorValue = analogRead(analogInPin);
  //map it to the range of the analog out:
  //outputValue = map(sensorValue, 0, 1023, 0, 255);
  
  // print the results to the Serial Monitor:
  Serial.print("sensor = ");
  Serial.println(sensorValue);
  //Serial1.print("\t output = ");
  //Serial1.print(outputValue);
#endif

#ifdef TEST_PWM
  //DAC test 
  if(dacoutputValue < 1000)
      dacoutputValue+=100;
  else 
      dacoutputValue = 0;

  // change the analog out value:
  analogWrite(analogOutPin, dacoutputValue);
  Serial.print("dac(pwm) output = ");
  Serial.println(dacoutputValue);
#endif  
  // wait 1 seconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(1000);
}
