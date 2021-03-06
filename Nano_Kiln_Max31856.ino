#include <Servo.h>
#include <Adafruit_MAX31856.h>
#include <Sleep_n0m1.h>

float SetPoint = 650;
int PWM_off = 90 ;
int PWM_on = 0;
Sleep sleep;
Servo myservo;
int pos = 0;
float temp;


unsigned long sleepTime; //how long you want the arduino stay sleepping
// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(9,10, 11, 12);
// use hardware SPI, just pass in the CS pin
//Adafruit_MAX31856 maxthermo = Adafruit_MAX31856(10);

void setup() {
  Serial.begin(115200);
   sleepTime = 5000; //set sleep time in ms, max sleep time is 49.7 days
  Serial.println("MAX31856 thermocouple test");
  myservo.attach(6);
  maxthermo.begin();

  maxthermo.setThermocoupleType(MAX31856_TCTYPE_S);

  Serial.print("Thermocouple type: ");
  switch (maxthermo.getThermocoupleType() ) {
    case MAX31856_TCTYPE_B: Serial.println("B Type"); break;
    case MAX31856_TCTYPE_E: Serial.println("E Type"); break;
    case MAX31856_TCTYPE_J: Serial.println("J Type"); break;
    case MAX31856_TCTYPE_K: Serial.println("K Type"); break;
    case MAX31856_TCTYPE_N: Serial.println("N Type"); break;
    case MAX31856_TCTYPE_R: Serial.println("R Type"); break;
    case MAX31856_TCTYPE_S: Serial.println("S Type"); break;
    case MAX31856_TCTYPE_T: Serial.println("T Type"); break;
    case MAX31856_VMODE_G8: Serial.println("Voltage x8 Gain mode"); break;
    case MAX31856_VMODE_G32: Serial.println("Voltage x8 Gain mode"); break;
    default: Serial.println("Unknown"); break;

  }
  //test funzionamento
    myservo.write(PWM_off);
    delay(2000);
    myservo.write(PWM_on);
    delay(2000);
    myservo.write(PWM_off);
}

void loop() {
  digitalWrite(13,HIGH);
  Serial.print("Cold Junction Temp: "); 
  Serial.println(maxthermo.readCJTemperature());
temp = maxthermo.readThermocoupleTemperature();
  Serial.print("Thermocouple Temp: "); 
  Serial.println(maxthermo.readThermocoupleTemperature());
  // Check and print any faults
  uint8_t fault = maxthermo.readFault();
  if (fault) {
    if (fault & MAX31856_FAULT_CJRANGE) Serial.println("Cold Junction Range Fault");
    if (fault & MAX31856_FAULT_TCRANGE) Serial.println("Thermocouple Range Fault");
    if (fault & MAX31856_FAULT_CJHIGH)  Serial.println("Cold Junction High Fault");
    if (fault & MAX31856_FAULT_CJLOW)   Serial.println("Cold Junction Low Fault");
    if (fault & MAX31856_FAULT_TCHIGH)  Serial.println("Thermocouple High Fault");
    if (fault & MAX31856_FAULT_TCLOW)   Serial.println("Thermocouple Low Fault");
    if (fault & MAX31856_FAULT_OVUV)    Serial.println("Over/Under Voltage Fault");
    if (fault & MAX31856_FAULT_OPEN)    Serial.println("Thermocouple Open Fault");
  }
   Serial.print("SetPoint      ->  ");Serial.println(SetPoint);
   Serial.print("Temp: ->  "); Serial.println(maxthermo.readThermocoupleTemperature());
 if (maxthermo.readThermocoupleTemperature() < SetPoint)
  {  myservo.write(PWM_on);// tell servo to go to position in variable 'pos'
     Serial.println("Valve-ON");
     delay(15);                       // waits 15ms for the servo to reach the position
 }
else
 {   myservo.write(PWM_off); // tell servo to go to position in variable 'pos'
     Serial.println("Valve-OFF");
     delay(15);                       // waits 15ms for the servo to reach the position
};
 
  delay(1000);
  delay(100); //delay to allow serial to fully print before sleep
  digitalWrite(13,LOW);
  sleep.pwrDownMode(); //set sleep mode
  sleep.sleepDelay(sleepTime); //sleep for: sleepTime
}
