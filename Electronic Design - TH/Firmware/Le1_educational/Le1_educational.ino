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

  const int voltagePin = 7;        //  
  const int currentPin = 6;        //
  float Vin=0;                     // input voltage
  float Iin=0;                     // input current
  float Iin_raw=0;                 // untuned current input
  float Pin=0;                     // input power in

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
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.display();
  delay(2000);
  display.clearDisplay();  
  display.setCursor(0,0);
}



void loop() {

  Vin = analogRead(voltagePin);
  Iin = (analogRead(currentPin)-462)*0.01; 
  if (Iin<0){
    Iin=0;
    }
  Pin = Vin*Iin;
  
  display.setCursor(0,0);
  display.print("Voltage: ");
  display.print(Vin, 1);
  display.println("V");
  display.print("Current: ");
  display.print(Iin);
  display.println("A");
  display.print("Power: ");
  display.print(Pin, 1);
  display.print("W");
  
  display.display();
  delay(2);
  display.clearDisplay();
  
  Serial.print("Vin:");
  Serial.print(Vin, 1);
  Serial.print(" Iin:");
  Serial.print(Iin);
  Serial.print(" Pin:");
  Serial.println(Pin,1); 
}
