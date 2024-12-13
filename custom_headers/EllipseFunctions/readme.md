## Library to draw hollow or solid ellipse. 
You can use them via writing 

1.  ``` 
    drawEllipse( x,y, w, h ,uint16_t color);
    
2. ```
    fillEllipse( x, y, w, h, uint16_t color);
   
### Example code:
```
#include <Adafruit_SSD1306.h>
#include <ellipseFunctions.h> // Custom library

// OLED display dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// I2C address for the SSD1306
#define OLED_RESET -1 // Reset pin not used
#define SCREEN_ADDRESS 0x3C // I2C address

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Global variables for eye positions
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN,OUTPUT);
  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
}

void loop() {
  drawEllipse( 0,0, 32, 40,SSD1306_WHITE);
  fillEllipse( 96, 0, 32, 40,SSD1306_WHITE);
}
```
