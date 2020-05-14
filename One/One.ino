#include <Adafruit_NeoPixel.h>
#include <Servo.h>

#define PIN            9
#define NUMPIXELS      1
#define MOSFET_PIN     7

Servo ESC;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
//int Battery;

void setup() {
  //pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(6, INPUT_PULLUP); // 1-> wyłączony          0-> włączony
  pinMode(5, INPUT_PULLUP); // 1-> Klapka otwarta     0-> zamknięta
  pinMode(4, INPUT_PULLUP); // 1-> magazynek wyjęty   0-> włożony
  pinMode(MOSFET_PIN, OUTPUT);
  digitalWrite(MOSFET_PIN, HIGH);
  ESC.attach(3,1000,2000);
  pixels.begin();
}

void CheckBattery()
{
  for (int i = 3; i <= 5; i++)
  {
    if(map(analogRead(i), 0, 1023, 0, 500) < 330)
    {
      //while(true)
      {
        //digitalWrite(MOSFET_PIN, LOW);
        pixels.setPixelColor(0, pixels.Color(150,0,0));
        pixels.show();
        delay(100);
        pixels.setPixelColor(0, pixels.Color(0,0,0));
        pixels.show();
        delay(100);
      }
    }
  }
}

void ShowBattery()
{
  for (int i = 3; i <= 5; i++)
  {
    int Battery = map(analogRead(i), 0, 1023, 0, 255);
    pixels.setPixelColor(0, pixels.Color(255-Battery,Battery,0));
    pixels.show();
    delay(300);
    pixels.setPixelColor(0, pixels.Color(0,0,255/2));
    pixels.show();
    delay(30);
  }
}

// the loop function runs over and over again forever
void loop() {
  CheckBattery();

  if (!digitalRead(6) && !digitalRead(5) && !digitalRead(4))    //Sekwencja ognia
  {
    ESC.write(179);    // Send the signal to the ESC
    pixels.setPixelColor(0, pixels.Color(0,0,255));
    pixels.show();
    delay(100);
  } 
  else if (!digitalRead(6) & digitalRead(5) & digitalRead(4))   // Sekwencja wyłączenia
  {
    while(!digitalRead(6))
      {
        ESC.write(0);
        digitalWrite(MOSFET_PIN, LOW);
        pixels.setPixelColor(0, pixels.Color(255/4,255/2,255));
        pixels.show();
        delay(100);
        pixels.setPixelColor(0, pixels.Color(0,0,0));
        pixels.show();
        delay(100);
      }
  }
  else if (digitalRead(5))
  {
    ESC.write(0);
    ShowBattery();
  } 
  else 
  {
    ESC.write(0);
    pixels.setPixelColor(0, pixels.Color(0,0,0));
    pixels.show();
  }
}
