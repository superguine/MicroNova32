### Use it to play different notes.
### Example Code:
```
#include "notes.h" // Include the custom header file

#define BUZZER_PIN 25 // Replace with your buzzer/ speaker pin

void setup() {
    // No need for pinMode(BUZZER_PIN, OUTPUT); 
}

void loop() {
    tone(BUZZER_PIN, NOTE_C4, 500); // Play note C4 for 500 ms
    delay(1000);                    // Wait for 1 second
}

```
