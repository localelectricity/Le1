// firmware for the MVP

  float Vin=0; // input voltage
  float Iin=0;                     // input current
  float Iin_raw=0;                 // untuned current input
  float Pin=0;                     // input power in
  
  const int numReadings = 10;      // number of reading taken for smoothing
  int rstButton = 0;               // reset button 
  int pinval=0;                    // pin value

  int Vreadings[numReadings];      // the readings from the analog input
  int VreadIndex = 0;              // the index of the current reading
  int Vtotal = 0;                  // the running total
  int Vaverage = 0;                // the average

  
  int Ireadings[numReadings];      // the readings from the analog input
  int IreadIndex = 0;              // the index of the current reading
  int Itotal = 0;                  // the running total
  int Iaverage = 0;                // the average
  int Ioffset = 510;               // initial current offset for ACS712ELC-5A
  int IinOld = 0;                  // current value from previous iteration (used for calibration)
  
  int Preadings[numReadings];      // the readings from the analog input
  int PreadIndex = 0;              // the index of the current reading
  int Ptotal = 0;                  // the running total
  


void setup() {
  // initialize digital pins as outputs.
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, INPUT);
  
  Serial.begin(9600); // begin reading serial

 // below is the initialisation of the values used for the smoothing of voltage and current readings
 
    // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    Vreadings[thisReading] = 0;
    }
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    Ireadings[thisReading] = 0;
    }
    for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    Preadings[thisReading] = 0;
    }
}

void loop() {

// LED output control for power readings 0-3W

  if (Pin < 0.5){
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
  }
  else if (Pin < 1){
      pinval = (Pin-1)*255; // convert the power reading into a PWM value 
      analogWrite(9, pinval);
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
  }
  else if (Pin < 2){
      pinval = (Pin-2)*255;
      digitalWrite(9, HIGH);
      analogWrite(10, pinval);
      digitalWrite(11, LOW); 
  }
  else if (Pin < 3){
      pinval = (Pin-3)*255;
      digitalWrite(9, HIGH);
      digitalWrite(10, HIGH);
      analogWrite(11, pinval);
  }
  else{
      digitalWrite(9, HIGH);
      digitalWrite(10, HIGH);
      digitalWrite(11, HIGH);
  }

// calbration button, allowing the calbration of the current sensor
  rstButton = digitalRead (13);
  if (rstButton == HIGH){
    if ( Iin < 0.53 || Iin > 0.55){
       if (Iin < 0.53){
         Ioffset = Ioffset-1;
         Serial.print("BOON");
       }
       else{
        Ioffset = Ioffset+1;
       }
    digitalWrite(12, HIGH); // turn on LED to let user know the calbration is in progress
    }
    else{
    Ioffset = Ioffset;
    digitalWrite(12, LOW);
    }
  }
  else{
    Ioffset = Ioffset; 
  }
  IinOld = Iin;

// voltage reading smoothing

   // subtract the last reading:
  Vtotal = Vtotal - Vreadings[VreadIndex];
  // read from the sensor:
  Vreadings[VreadIndex] = analogRead(4);
  // add the reading to the total:
  Vtotal = Vtotal + Vreadings[VreadIndex];
  // advance to the next position in the array:
  VreadIndex = VreadIndex + 1;
  // if we're at the end of the array...
  if (VreadIndex >= numReadings) {
    // ...wrap around to the beginning:
    VreadIndex = 0;
  }
  // calculate the average:
  Vaverage = Vtotal / numReadings;
  delay(1);        // delay in between reads for stability

// current reading smoothing

   // subtract the last reading:
  Itotal = Itotal - Ireadings[IreadIndex];
  // read from the sensor:
  Ireadings[IreadIndex] = analogRead(5);
  // add the reading to the total:
  Itotal = Itotal + Ireadings[IreadIndex];
  // advance to the next position in the array:
  IreadIndex = IreadIndex + 1;
  // if we're at the end of the array...
  if (IreadIndex >= numReadings) {
    // ...wrap around to the beginning:
    IreadIndex = 0;
  }
// calculate the average:
  Iaverage = Itotal / numReadings;
  delay(1);        // delay in between reads for stability

// power reading smoothing

     // subtract the last reading:
  Ptotal = Ptotal - Preadings[IreadIndex];
  // read from the sensor:
  Preadings[PreadIndex] = Pin;
  // add the reading to the total:
  Ptotal = Ptotal + Preadings[PreadIndex];
  // advance to the next position in the array:
  PreadIndex = PreadIndex + 1;
  // if we're at the end of the array...
  if (PreadIndex >= numReadings) {
    // ...wrap around to the beginning:
    PreadIndex = 0;
  }
  // calculate the average:
  Pin = Ptotal / numReadings;
  delay(1);        // delay in between reads for stability

// converting voltage and curret reading to decimal values

  Vin = (Vaverage*0.039);
  Iin_raw = (Iaverage-Ioffset)*0.02642*0.5;
  Iin = Iin_raw; //(-0.1118*Iin_raw*Iin_raw)+(0.725*Iin_raw) + 0.0048;
  Pin = Vin*Iin;
//
//  Vin = analogRead(4);//*0.039;
//  Iin = analogRead(5);//-512)*0.029326;
//  Pin = Vin*Iin;

  Serial.print("Vin:");
  Serial.print(Vin);
  Serial.print(" Iin:");
  Serial.print(Iin);
  Serial.print(" Pin:");
  Serial.print(Pin);  
  Serial.print(" Ioffset");
  Serial.print(Ioffset);
  Serial.print("  |,");
  Serial.print(Vin);
  Serial.print(",");
  Serial.print(Iin);
  Serial.print(",");
  Serial.println(Pin);  
}
