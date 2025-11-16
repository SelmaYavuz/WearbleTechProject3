#include <Adafruit_CircuitPlayground.h>
#include <SimpleKalmanFilter.h> // for filtering
#include "LowPower.h" // for low power (AVR boards)

#define SLEEPTIME 120000 // how long system will sleep for (2 minutes)
#define ACTIVETIME 60000 // how long the system will be active for (1 minute)
#define SAMPLERATE 100 // how often we sample data within the active time period
#define LIGHTDELAY 50 // how long each light waits to pop up

unsigned long last_sleep_time; // to keep track of sleeping

SimpleKalmanFilter simpleKalmanFilter(1.5, 1, 0.1);


float X, Y, Z;
float eX, eY, eZ;

// sequin LEDs

int led1 = 10;
int led2 = 12;
int led3 = 0;
int led4 = 2;

// turn on and off
int on = 0;

void setup() {
  Serial.begin(115200);
  CircuitPlayground.begin();

  // initialize  sequin leds
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  // set up serial plotter
  Serial.println("y:z");
}

void filter(){

  // calculate the estimated value with Kalman Filter
  eX = simpleKalmanFilter.updateEstimate(X);
  eY = simpleKalmanFilter.updateEstimate(Y);
  eZ = simpleKalmanFilter.updateEstimate(Z);

}

void loop() {
  CircuitPlayground.clearPixels();

  // go to sleep for 2 minutes, work for 1 minute
  unsigned long current_time = millis();

  if(current_time - last_sleep_time >= ACTIVETIME)
  {
    // sleep for two minutes, max time you can sleep is 8 seconds, 8*15 = 120 -> 2 minutes.
    for(int i = 0; i < 15; i++)
    {
      LowPower.idle(SLEEP_8S, ADC_OFF, TIMER4_OFF, TIMER3_OFF, TIMER1_OFF, 
  		  TIMER0_OFF, SPI_OFF, USART1_OFF, TWI_OFF, USB_OFF);
    }

    // set the last time slept
    last_sleep_time = millis();
  }

  // check left button press and on state
  if(CircuitPlayground.leftButton() && !on)
  {
      on = 1;
  }
  else if(CircuitPlayground.leftButton() && on)
  {
      on = 0;
  }

  // if the device is on
  if(on)
  {
      X = CircuitPlayground.motionX();
      Y = CircuitPlayground.motionY();
      Z = CircuitPlayground.motionZ();

      filter();

      if(eY < -3)
      {
        Left();
      }
      if(eY > 0)
      {
        Right();
      }
      if(eZ < -1.5)
      {
        Forwards();
      }
      if(eZ > 1.5)
      {
        Backwards();
      }

      // serial monitor debugging

      // Serial.print("x:");
      // Serial.print(eX);
      Serial.print(",");
      Serial.print("y:");
      Serial.println(eY);
      Serial.print(",");
      Serial.print("z:");
      Serial.println(eZ);

      // sample every 25 MS
      delay(SAMPLERATE);
  }


}


// light patterns
void Backwards()
{
  CircuitPlayground.setPixelColor(0, 255,   0,   0);
  CircuitPlayground.setPixelColor(9, 0x0000FF);
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(1, 128, 128,   0);
  CircuitPlayground.setPixelColor(8, 0x008080);
  delay(LIGHTDELAY);

  digitalWrite(led1, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(led4, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(2, 0, 255,   0);
  CircuitPlayground.setPixelColor(7, 0x00FF00);
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(3, 0, 128, 128);
  CircuitPlayground.setPixelColor(6, 0x808000);
  delay(LIGHTDELAY);
  
  digitalWrite(led2, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(led3, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(4, 0,   0, 255);
  CircuitPlayground.setPixelColor(5, 0xFF0000);
  delay(LIGHTDELAY);

  // turn off LEDs

  CircuitPlayground.setPixelColor(4, 0,   0, 0);
  CircuitPlayground.setPixelColor(5, 0, 0, 0);
  delay(LIGHTDELAY);

  digitalWrite(led2, LOW);   // turn the LED off by making the voltage LOW
  digitalWrite(led3, LOW);   // turn the LED off by making the voltage LOW
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(3, 0, 0, 0);
  CircuitPlayground.setPixelColor(6, 0, 0, 0);
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(2, 0, 0, 0);
  CircuitPlayground.setPixelColor(7, 0, 0, 0);
  delay(LIGHTDELAY);

  digitalWrite(led1, LOW);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(led4, LOW);  // turn the LED on (HIGH is the voltage level)
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(1, 0, 0, 0);
  CircuitPlayground.setPixelColor(8, 0, 0, 0);
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(0, 0, 0, 0);
  CircuitPlayground.setPixelColor(9, 0, 0, 0);
  delay(LIGHTDELAY);
}

void Left()
{
  // turn on

  digitalWrite(led1, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(led2, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(7, 255,   0,   0);
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(6, 128, 128,   0);
  CircuitPlayground.setPixelColor(8, 0x008080);
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(5, 0, 255,   0);
  CircuitPlayground.setPixelColor(9, 0x00FF00);
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(4, 0, 128, 128);
  CircuitPlayground.setPixelColor(0, 0x808000);
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(3, 0,   0, 255);
  CircuitPlayground.setPixelColor(1, 0xFF0000);
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(2, 0x0000FF);
  delay(LIGHTDELAY);

  digitalWrite(led3, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(led4, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(LIGHTDELAY);

  // turn off

  digitalWrite(led3, LOW);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(led4, LOW);  // turn the LED on (HIGH is the voltage level)
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(2, 0,   0, 0);
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(3, 0, 0, 0);
  CircuitPlayground.setPixelColor(1, 0, 0, 0);
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(4, 0, 0, 0);
  CircuitPlayground.setPixelColor(0, 0, 0, 0);
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(5, 0, 0, 0);
  CircuitPlayground.setPixelColor(9, 0, 0, 0);
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(6, 0, 0, 0);
  CircuitPlayground.setPixelColor(8, 0, 0, 0);
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(7, 0, 0, 0);
  delay(LIGHTDELAY);

  digitalWrite(led1, LOW);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(led2, LOW);  // turn the LED on (HIGH is the voltage level)
  delay(LIGHTDELAY);
}

void Right()
{
  // turn on

  digitalWrite(led3, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(led4, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(2, 255,   0,   0);
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(1, 128, 128,   0);
  CircuitPlayground.setPixelColor(3, 0x008080);
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(0, 0, 255,   0);
  CircuitPlayground.setPixelColor(4, 0x00FF00);
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(9, 0, 128, 128);
  CircuitPlayground.setPixelColor(5, 0x808000);
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(8, 0,   0, 255);
  CircuitPlayground.setPixelColor(6, 0xFF0000);
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(7, 0x0000FF);
  delay(LIGHTDELAY);

  digitalWrite(led1, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(led2, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(LIGHTDELAY);

  // turn off

  digitalWrite(led1, LOW);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(led2, LOW);  // turn the LED on (HIGH is the voltage level)
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(7, 0,   0, 0);
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(8, 0, 0, 0);
  CircuitPlayground.setPixelColor(6, 0, 0, 0);
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(9, 0, 0, 0);
  CircuitPlayground.setPixelColor(5, 0, 0, 0);
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(4, 0, 0, 0);
  CircuitPlayground.setPixelColor(0, 0, 0, 0);
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(3, 0, 0, 0);
  CircuitPlayground.setPixelColor(1, 0, 0, 0);
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(2, 0, 0, 0);
  delay(LIGHTDELAY);

  digitalWrite(led3, LOW);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(led4, LOW);  // turn the LED on (HIGH is the voltage level)
  delay(LIGHTDELAY);
}

void Forwards()
{
  // turn on

  CircuitPlayground.setPixelColor(4, 255,   0,   0);
  CircuitPlayground.setPixelColor(5, 0x0000FF);
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(3, 128, 128,   0);
  CircuitPlayground.setPixelColor(6, 0x008080);
  delay(LIGHTDELAY);

  digitalWrite(led2, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(led3, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(2, 0, 255,   0);
  CircuitPlayground.setPixelColor(7, 0x00FF00);
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(1, 0, 128, 128);
  CircuitPlayground.setPixelColor(8, 0x808000);
  delay(LIGHTDELAY);

  digitalWrite(led1, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(led4, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(0, 0,   0, 255);
  CircuitPlayground.setPixelColor(9, 0xFF0000);
  delay(LIGHTDELAY);

  // turn off

  CircuitPlayground.setPixelColor(0, 0,   0, 0);
  CircuitPlayground.setPixelColor(9, 0, 0, 0);
  delay(LIGHTDELAY);

  digitalWrite(led1, LOW);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(led4, LOW);  // turn the LED on (HIGH is the voltage level)
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(1, 0, 0, 0);
  CircuitPlayground.setPixelColor(8, 0, 0, 0);
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(2, 0, 0, 0);
  CircuitPlayground.setPixelColor(7, 0, 0, 0);
  delay(LIGHTDELAY);

  digitalWrite(led2, LOW);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(led3, LOW);  // turn the LED on (HIGH is the voltage level)
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(3, 0, 0, 0);
  CircuitPlayground.setPixelColor(6, 0, 0, 0);
  delay(LIGHTDELAY);

  CircuitPlayground.setPixelColor(4, 0, 0, 0);
  CircuitPlayground.setPixelColor(5, 0, 0, 0);
  delay(LIGHTDELAY);
}


