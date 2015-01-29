# TD_GUI

A basic graphics library for use with the [2.8" TFT Capacitive Touch Shield for Arduino](https://www.adafruit.com/product/1947).

It builds on top of the Adafruit libraries: [Adafruit_ILI9341](https://github.com/adafruit/Adafruit_ILI9341) (for graphics) and [Adafruit_FT6206](https://github.com/adafruit/Adafruit_FT6206_Library) (for touch input).

# Basic Usage

    //create the two adafruit classes you will need to pass in to the button
    Adafruit_ILI9341 gfx = Adafruit_ILI9341(10, 9);
    Adafruit_FT6206 touch = Adafruit_FT6206();
    
    //create the button, passing in an x and y location, width, height, and amount of padding
    TD_Button button = TD_Button(30, 10, 220, 80, 4);
    
    void setup() {
        Serial.begin(115200);
        
        //start the adafruit classes
        gfx.begin();
        touch.begin(40);
        //rotate screen 90 degrees so it's horizontal
        gfx.setRotation(ROTATE_90);
        gfx.fillScreen(BLACK);
        
        //assign button pressed and released handler methods
        button.onButtonPressed = &buttonPressed;
        button.onButtonReleased = &buttonReleased;
        //setup primary and secondary colors for the button
        button.primaryColor = GREEN;
        button.secondaryColor = DARKGREEN;
        //initalize the button
        button.init(gfx);
    }
    
    void loop(void) {
      //update the button so it can redraw itself and call the pressed/released handlers when it's touched
      button.update(touch, gfx);
    }
    
    void buttonPressed() {
      //do something when button is pressed
    }
    
    void buttonReleased() {
      //do something when button is released
    }
    
