void setup() {
  // put your setup code here, to run once:
  //pinMode(LED_BUILTIN, OUTPUT);

  pinMode(D0, OUTPUT);//UART0_Rx
  pinMode(D1, OUTPUT);//UART0_Tx
  pinMode(D2, OUTPUT);//
  //pinMode(D3, OUTPUT);//3.6V or 1V
  //pinMode(D4, OUTPUT);//not sure
  //pinMode(D5, OUTPUT);//not sure
  //pinMode(D6, OUTPUT);//Vss
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  pinMode(D9, OUTPUT);
  pinMode(D10, OUTPUT);//Uart1_Tx
  //pinMode(D11, OUTPUT);//I2C_SDA, pull up
  pinMode(D12, OUTPUT);
  pinMode(D13, OUTPUT);//Uart1_Rx
  //pinMode(D14, OUTPUT);//?
  //pinMode(D15, OUTPUT);//?
  //pinMode(D16, OUTPUT);//?
  //pinMode(D17, OUTPUT);//?
  //pinMode(D18, OUTPUT);//?
  //pinMode(D19, OUTPUT);//?
  

  Serial.begin(9600);//VCOM
}

int kk=0;
void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(D0, HIGH);
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D7, HIGH);
  digitalWrite(D8, HIGH);
  digitalWrite(D9, HIGH);
  digitalWrite(D10, HIGH);
  digitalWrite(D12, HIGH);
  digitalWrite(D13, HIGH);
    
  delay(1000);                      // wait for a second
  //digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  digitalWrite(D0, LOW);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D7, LOW);
  digitalWrite(D8, LOW);
  digitalWrite(D9, LOW);
  digitalWrite(D10, LOW);
  digitalWrite(D12, LOW);
  digitalWrite(D13, LOW);
  
  delay(1000);                      // wait for a second

  if(kk<100)kk++;
  else kk=0;
  Serial.print("**Serial: VCOM test loop:");
  Serial.println(kk, DEC);
}
