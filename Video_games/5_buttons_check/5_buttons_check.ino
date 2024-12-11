#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Define button pins
const int buttonPins[] = {4, 5, 19, 18, 23};
const int numButtons = 5;

// Task handles
TaskHandle_t buttonTaskHandle;

void setup() {
  // Initialize Serial
  Serial.begin(115200);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED allocation failed");
    while (true);
  }

  display.clearDisplay();
  display.display();

  // Set up button pins
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);  // Use INPUT_PULLUP as buttons send 0V when pressed
  }

  // Create the button monitoring task
  xTaskCreate(buttonTask, "Button Task", 2048, NULL, 1, &buttonTaskHandle);
}

void loop() {
  // Main loop does nothing, tasks handle button monitoring
}
void buttonTask(void *parameter) {
  while (true) {
    bool anyButtonPressed = false;
    display.clearDisplay();
    display.setCursor(0, 19);
    display.setTextSize(1);
    display.setTextColor(WHITE);

    // Check each button's state
    for (int i = 0; i < numButtons; i++) {
      if (digitalRead(buttonPins[i]) == LOW) {  // Button pressed
        display.printf("Pin D%d, ", buttonPins[i]);
        anyButtonPressed = true;
      }
    }

    if (anyButtonPressed) {
      display.display();
      delay(300);  // Debounce delay
    } else {
      display.clearDisplay();  // Clear if no buttons pressed
      display.display();
    }

    vTaskDelay(100 / portTICK_PERIOD_MS);  // Small delay to avoid rapid checking
  }
}
