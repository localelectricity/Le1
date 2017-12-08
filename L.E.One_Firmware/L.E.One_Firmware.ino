// firmware for the MVP

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define SSD1306_LCDHEIGHT 64
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

  const int voltagePin = 0;
  const int currentPin = 1;
  float Vin=0;                     // input voltage
  float Iin=0;                     // input current
  float Iin_raw=0;                 // untuned current input
  float Pin=0;                     // input power in
  
  const int numReadings = 30;      // number of reading taken for smoothing
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
  int Ioffset = 0;               // initial current offset for ACS712ELC-5A
  int IinOld = 0;                  // current value from previous iteration (used for calibration)
  
  int Preadings[numReadings];      // the readings from the analog input
  int PreadIndex = 0;              // the index of the current reading
  int Ptotal = 0;                  // the running total

  float x1, y1, x2, y2, x3, y3, x4, y4, x5, y5;
  int mainGaugesize = 18;
  int mainHandsize = mainGaugesize-5;
  int mainTicksize = 3;
  int otherGaugesize = 14;
  int otherHandsize = otherGaugesize-5;
  int otherTicksize = 3;
  
  int VgaugeCenterX = 20;
  int VgaugeCenterY = 25;
  int IgaugeCenterX = 108;
  int IgaugeCenterY = 25;
  int PgaugeCenterX = 64;
  int PgaugeCenterY = 20;

void setup() {
  // initialize digital pins as outputs.
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, INPUT);
  
  Serial.begin(9600); // begin reading serial

    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.setTextSize(0);
  display.setTextColor(WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();  

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

//// calbration button, allowing the calbration of the current sensor
//  rstButton = digitalRead (13);
//  if (rstButton == HIGH){
//    if ( Iin < 0.53 || Iin > 0.55){
//       if (Iin < 0.53){
//         Ioffset = Ioffset-1;
//         Serial.print("BOON");
//       }
//       else{
//        Ioffset = Ioffset+1;
//       }
//    digitalWrite(12, HIGH); // turn on LED to let user know the calbration is in progress
//    }
//    else{
//    Ioffset = Ioffset;
//    digitalWrite(12, LOW);
//    }
//  }
//  else{
//    Ioffset = Ioffset; 
//  }
//  IinOld = Iin;

// voltage reading smoothing

   // subtract the last reading:
  Vtotal = Vtotal - Vreadings[VreadIndex];
  // read from the sensor:
  Vreadings[VreadIndex] = analogRead(voltagePin);
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
  Ireadings[IreadIndex] = analogRead(currentPin);
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

//  Vin = Vaverage*0.104;
  //Iin_raw = (Iaverage-Ioffset)*0.02642*0.25;
  //Iin = (2.5247*Iin_raw*Iin_raw)+ (0.9098*Iin_raw) + 0.012;

  //Pin = Vin*Iin;
//
  Vin = analogRead(7)*0.0528;
  Iin = (analogRead(6)-468)*0.01;
  Pin = Vin*Iin;

  x1=otherHandsize*cos(((Vin*9)+135)*0.0175);
  y1=otherHandsize*sin(((Vin*9)+135)*0.0175);
  x2=otherHandsize*cos(((Iin*270)+135)*0.0175);
  y2=otherHandsize*sin(((Iin*270)+135)*0.0175);
  x3=mainHandsize*cos(((Pin*54)+135)*0.0175);
  y3=mainHandsize*sin(((Pin*54)+135)*0.0175);
  
  display.drawLine(VgaugeCenterX, VgaugeCenterY, x1+VgaugeCenterX, y1+VgaugeCenterY, WHITE);
  display.drawCircle(VgaugeCenterX, VgaugeCenterY, otherGaugesize, WHITE);
  for(int angle=135; angle<=405; angle+=45){
     x4=otherGaugesize*cos(angle*0.0175);
     y4=otherGaugesize*sin(angle*0.0175);
     x5=(otherGaugesize-otherTicksize)*cos(angle*0.0175);
     y5=(otherGaugesize-otherTicksize)*sin(angle*0.0175);
     display.drawLine(x4+VgaugeCenterX, y4+VgaugeCenterY, x5+VgaugeCenterX, y5+VgaugeCenterY, WHITE);
  }  
  
  display.drawLine(IgaugeCenterX, IgaugeCenterY, x2+IgaugeCenterX, y2+IgaugeCenterY, WHITE);
  display.drawCircle(IgaugeCenterX, IgaugeCenterY, otherGaugesize, WHITE);
  for(int angle=135; angle<=405; angle+=45){
     x4=otherGaugesize*cos(angle*0.0175);
     y4=otherGaugesize*sin(angle*0.0175);
     x5=(otherGaugesize-otherTicksize)*cos(angle*0.0175);
     y5=(otherGaugesize-otherTicksize)*sin(angle*0.0175);
     display.drawLine(x4+IgaugeCenterX, y4+IgaugeCenterY, x5+IgaugeCenterX, y5+IgaugeCenterY, WHITE);
  }
  
  display.drawLine(PgaugeCenterX, PgaugeCenterY, x3+PgaugeCenterX, y3+PgaugeCenterY, WHITE);
  display.drawCircle(PgaugeCenterX, PgaugeCenterY, mainGaugesize, WHITE);
   for(int angle=135; angle<=405; angle+=45){
     x4=mainGaugesize*cos(angle*0.0175);
     y4=mainGaugesize*sin(angle*0.0175);
     x5=(mainGaugesize-mainTicksize)*cos(angle*0.0175);
     y5=(mainGaugesize-mainTicksize)*sin(angle*0.0175);
     display.drawLine(x4+PgaugeCenterX, y4+PgaugeCenterY, x5+PgaugeCenterX, y5+PgaugeCenterY, WHITE);
  }

  display.setCursor(VgaugeCenterX-13,45);
  display.print("Voltg");
  display.setCursor(VgaugeCenterX-13,55);
  display.print(Vin, 1);
  display.print("V");
  display.setCursor(IgaugeCenterX-13,45);
  display.print("Crrnt");
  display.setCursor(IgaugeCenterX-13,55);
  display.print(Iin);
  display.print("A");
  display.setCursor(PgaugeCenterX-13,45);
  display.print("Power");
  display.setCursor(PgaugeCenterX-13,55);
  display.print(Pin, 1);
  display.print("W");
  
  display.display();
  delay(2);
  display.clearDisplay();
  
  Serial.print("Vin:");
  Serial.print(Vin, 1);
  Serial.print(" Iin:");
  Serial.print(Iaverage);
  Serial.print(" Pin:");
  Serial.print(Pin,1); 
  Serial.print(" Ioffset");
  Serial.print(Ioffset);
  Serial.print("  | ");
  Serial.print(analogRead(voltagePin));
  Serial.print(",");
  Serial.print(analogRead(currentPin));
  Serial.println(pinval);  
}
