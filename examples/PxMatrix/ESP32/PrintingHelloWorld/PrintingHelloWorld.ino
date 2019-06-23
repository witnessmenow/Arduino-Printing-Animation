/*******************************************************************
    Hello World example using the PrintingAnimation library on
    the ESP32

    This sketch will perform the animation on the entire display
    of a 64 x 32 display.
 *                                                                 *
    Written by Brian Lough
    YouTube: https://www.youtube.com/brianlough
    Tindie: https://www.tindie.com/stores/brianlough/
    Twitter: https://twitter.com/witnessmenow
 *******************************************************************/

// ----------------------------
// Standard Libraries - Already Installed if you have ESP32 set up
// ----------------------------

#include <Ticker.h>

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------


#define double_buffer // this must be enabled to stop flickering
#include <PxMatrix.h>
// The library for controlling the LED Matrix
//
// At time of writing this the double_buffer
// Have been merged into the main PxMatrix library,
// but have not been released, so you will need to install
// from Github
//
// https://github.com/2dom/PxMatrix

// Adafruit GFX library is a dependancy for the PxMatrix Library
// Can be installed from the library manager
// https://github.com/adafruit/Adafruit-GFX-Library

#include <PrintingAnimation.h>
// The library that emulates the style of a printer
// for displaying data on the screen.
// 
// Can be installed from the library manager
// https://github.com/witnessmenow/Arduino-Printing-Animation


// ----------------------------
// Wiring and Display setup
// ----------------------------

#define P_LAT 22
#define P_A 19
#define P_B 23
#define P_C 18
#define P_D 5
#define P_E 15
#define P_OE 2
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// This defines the 'on' time of the display is us. The larger this number,
// the brighter the display. If too large the ESP will crash
uint8_t display_draw_time = 10; //10-50 is usually fine

//PxMATRIX display(matrix_width, matrix_height, P_LAT, P_OE, P_A, P_B, P_C);
//PxMATRIX display(64,32,P_LAT, P_OE,P_A,P_B,P_C,P_D);
PxMATRIX display(64, 32, P_LAT, P_OE, P_A, P_B, P_C, P_D, P_E);

// Some standard colors
uint16_t myRED = display.color565(255, 0, 0);
uint16_t myGREEN = display.color565(0, 255, 0);
uint16_t myBLUE = display.color565(0, 0, 255);
uint16_t myWHITE = display.color565(255, 255, 255);
uint16_t myYELLOW = display.color565(255, 255, 0);
uint16_t myCYAN = display.color565(0, 255, 255);
uint16_t myMAGENTA = display.color565(255, 0, 255);
uint16_t myBLACK = display.color565(0, 0, 0);


PrintingAnimation printer(display);
Ticker animation_ticker;
bool finishedAnimating = false;
unsigned long resetAnimationDue = 0;

void IRAM_ATTR display_updater() {
  display.display(display_draw_time);
}


void display_update_enable(bool is_enable)
{
  if (is_enable)
  {
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &display_updater, true);
    timerAlarmWrite(timer, 2000, true);
    timerAlarmEnable(timer);
  }
  else
  {
    timerDetachInterrupt(timer);
    timerAlarmDisable(timer);
  }
}

// Will center the given text
void displayText(String text, int yPos) {
  int16_t  x1, y1;
  uint16_t w, h;
  display.setTextSize(2);
  char charBuf[text.length() + 1];
  text.toCharArray(charBuf, text.length() + 1);
  display.getTextBounds(charBuf, 0, yPos, &x1, &y1, &w, &h);
  int startingX = 33 - (w / 2);
  if (startingX < 0) {
    display.setTextSize(1);
    display.getTextBounds(charBuf, 0, yPos, &x1, &y1, &w, &h);
    startingX = 33 - (w / 2);
  }
  display.setCursor(startingX, yPos);
  Serial.println(startingX);
  Serial.println(yPos);
  display.print(text);
}

void writeToScreen() {
  display.setTextColor(myRED);
  displayText("Hello", 1);
  display.setTextColor(myRED);
  displayText("World", 17);
}


void animationHandler()
{
  // Not clearing the display and redrawing it when you
  // dont need to improves how the refresh rate appears
    if (!finishedAnimating) {
        
        // Step 1: Clear the display
        display.clearDisplay();

        // Step 2: Draw whatever is required to be displayed
        writeToScreen();

        // Step 3: invoke the printer animation library
        finishedAnimating = printer.animate();
        
        // Optional: add a timer to triger the animation again
        // An external event could replace this either
        if(finishedAnimating){
            unsigned long now = millis();
            resetAnimationDue = now + 5000; // reset the animation in 5 seconds
        }

        // Step 4: Display the buffer
        display.showBuffer();
    }
}



void setup() {
    Serial.begin(115200);
    // Define your display layout here, e.g. 1/8 step
    display.begin(16);

    // Define your scan pattern here {LINE, ZIGZAG, ZAGGIZ, WZAGZIG, VZAG} (default is LINE)
    //display.setScanPattern(LINE);

    // Define multiplex implemention here {BINARY, STRAIGHT} (default is BINARY)
    //display.setMuxPattern(BINARY);

    display.setFastUpdate(true);
    display.clearDisplay();
    display.setTextColor(myCYAN);
    display.setCursor(2, 0);
    display.print("Printing");
    display.setTextColor(myMAGENTA);
    display.setCursor(2, 8);
    display.print("Animation");
    display_update_enable(true);
    display.showBuffer();

    // Sets the bounds of what will be animated
    // the first two params are the X and Y of the 
    // bottom left of your area
    // the next two are the top right of your area.
    printer.setBounds(0, 31, 63, 0);

    delay(3000);

    // Changing the below number will impact the speed
    // the larger the number, the slower it will be
    // 0.02 was the fastest I could go without crashes
    animation_ticker.attach(0.02, animationHandler);

    // You can cheat the speed a little by making it jump
    // multiple blocks at a time. Setting increments to 2
    // will mean it will move the head 2 blocks over.
    // This is not noticeable that it's doing that, but
    // it doubles the speed.
    printer.increments = 1; //1 is the default
}

void loop() {
    if(finishedAnimating){
        unsigned long now = millis();
        if(now > resetAnimationDue){
            printer.resetState();
            finishedAnimating = false;
        }
    }
}