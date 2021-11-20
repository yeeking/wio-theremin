#include "Ultrasonic.h" //include ultrasonic distance sensor library 
#include "TFT_eSPI.h" //include TFT LCD library 

TFT_eSPI tft; //initialize TFT LCD 

TFT_eSprite spr = TFT_eSprite(&tft); //initialize buffer
 
Ultrasonic ultrasonic(D0); //set ultrasonic distance sensor pin as D0

int frequency = 4000; //Buzzer frequency
int cycle = 1000000/frequency; //one time period in microseconds (1s=1000000us)

int step = 0;
float pfreq;


void setup()
{
  tft.begin(); //start TFT LCD
  tft.setRotation(3); //set screen rotation
  spr.createSprite(TFT_HEIGHT,TFT_WIDTH); //create buffer
   pinMode(WIO_BUZZER,OUTPUT); //set buzzer pin as OUTPUT
 
}
void loop()
{
  digitalWrite(WIO_BUZZER,HIGH); //set buzzer pin to HIGH
  delayMicroseconds(cycle/2); //first half of the time period
  digitalWrite(WIO_BUZZER,LOW); //set buzzer pin to LOW
  delayMicroseconds(cycle/2); //second half of the time period
  step ++;
 //   frequency = ultrasonic.MeasureInCentimeters() * 300;
 //  if (frequency < 8000) cycle = 1000000/frequency; //one time period in microseconds (1s=1000000us)

  if (step % 100 == 0)
  {
    frequency = ultrasonic.MeasureInCentimeters() * 100;
    if (abs(pfreq-frequency) > 2000) return;
    if (pfreq == frequency) return;
    pfreq = frequency;
  //  delay(25); // don't read it again for a while!
    delayMicroseconds(100);
    if (frequency < 5000) cycle = 1000000/frequency; //one time period in microseconds (1s=1000000us)

    spr.fillSprite(TFT_BLACK); //fill background 
    spr.setTextSize(5); //set text size 
    spr.setTextColor(TFT_YELLOW); //set text color 
 
    spr.drawString("Freq: ", 0, 10);
  
    spr.drawNumber(frequency, 200,10); //display distance in centimeters
    //spr.drawNumber(ultrasonic.MeasureInInches(), 130,80); //display distance in inches 
    drawSine(frequency, spr);
    spr.pushSprite(0,0); //push to LCD 
  } 
}


void drawSine(float freq, TFT_eSprite spr)
{
  float phase = 0;
  float dphase = 3.1415928 * 2 / TFT_WIDTH * (freq / 500); 
  float y, px, py;
  px = 0;
  py = 0;
  for (float x = 0; x < TFT_WIDTH + 75; x++)
  {
    y = sin(phase);
    y = (y * 0.5 + 0.5) * TFT_HEIGHT * 0.70;
    //spr.drawPixel(x, y, TFT_GREEN);
    spr.drawLine(px,py,x,y,TFT_GREEN); //draw line 
    px = x;
    py = y;
    phase += dphase;
  }
}