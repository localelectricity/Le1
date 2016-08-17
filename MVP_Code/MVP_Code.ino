

  float Vin=0;
  float Iin=0;
  float Iin_raw=0;
  float Pin=0;
  const int numReadings = 10;

  int Vreadings[numReadings];      // the readings from the analog input
  int VreadIndex = 0;              // the index of the current reading
  int Vtotal = 0;                  // the running total
  int Vaverage = 0;                // the average

  
  int Ireadings[numReadings];      // the readings from the analog input
  int IreadIndex = 0;              // the index of the current reading
  int Itotal = 0;                  // the running total
  int Iaverage = 0;                // the average

  int Preadings[numReadings];      // the readings from the analog input
  int PreadIndex = 0;              // the index of the current reading
  int Ptotal = 0;                  // the running total
  
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  Serial.begin(9600);
  analogReference(EXTERNAL);
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

// the loop function runs over and over again forever
void loop() {
  if (Pin < 0.5){
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
  }
  else if (Pin < 1){
      digitalWrite(9, HIGH);
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
  }
  else if (Pin < 2){
      digitalWrite(9, HIGH);
      digitalWrite(10, HIGH);
      digitalWrite(11, LOW); 
      digitalWrite(12, LOW);
  }
  else if (Pin < 3){
      digitalWrite(9, HIGH);
      digitalWrite(10, HIGH);
      digitalWrite(11, HIGH);
      digitalWrite(12, LOW);
  }
  else{
      digitalWrite(9, HIGH);
      digitalWrite(10, HIGH);
      digitalWrite(11, HIGH);
      digitalWrite(12, HIGH);
  }
  
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

  Vin = (Vaverage*0.039)+1.3;
  Iin_raw = (Iaverage-511)*0.029326;
  Iin = (-0.1118*Iin_raw*Iin_raw)+(0.725*Iin_raw) + 0.0048;
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
  Serial.println(Pin);
}
