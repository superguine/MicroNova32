/******************************************************************************************************************************************
  Created by Roytech Prototypes (Shawon Roy).
  date: 04/12/24
  Version: v1.6
  GitHub: https://github.com/superguine 
  -----------------------------------------------------------------------------------
  Download notes header file from here: https://github.com/superguine/MicroNova32/tree/main/custom_headers/notes 
  
*//*****************************************************************************************************************************************/

//Reqired libraries
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "notes.h"

// Display Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Speaker config
const int tonePin = 25; // Pin connected to the speaker

// Button Configuration
#define BUTTON_PIN 18 // Middle button

// Game Variables
int ballY = SCREEN_HEIGHT - 3; // Ball's vertical position (on the ground)
int groundLevel = SCREEN_HEIGHT - 3;
int dragonJumpHeight = 40;        // Jump height
bool isJumping = false;           // Is the dragon jumping?
int jumpVelocity = 3;             // Jump speed
int gravity = 1;                  // Gravity effect
int obstacleX = SCREEN_WIDTH;     // Obstacle's horizontal position
int obstacleX2 = 64;
int obstacleWidth = 8;           // Obstacle width
int obstacleWidth2 = 8;
int obstacleHeight = 16;          // Obstacle height
int obstacleHeight2 = 16;
int minObstacleDistance = 30;     // Minimum distance between obstacles
int maxObstacleDistance = 50;     // Maximum distance between obstacles
int gameSpeed;
bool play = false,first = true,gameOver = false;   // Game state
int score = 0;                    // Player's score

void setup() {
// Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.display();

  // Initialize Button
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize random seed
  randomSeed(200); // Used to generate randomness
  //boot Screen
  BootImg();
  // Initial screen
  display.setTextSize(1);
  display.setCursor(20, 30);
  display.setTextColor(SSD1306_WHITE);
  display.println("Bounce the ball");
  display.display();
  delay(1500);
  display.clearDisplay();
  display.setCursor(12, 30);
  display.setTextColor(SSD1306_WHITE);
  display.println("Press'o'to bounce");
  display.display();
  delay(1500);
}

void loop() {
  digitalWrite(LED_BUILTIN, 0);
  if (!gameOver) {
    // Handle jumping
    if (digitalRead(BUTTON_PIN) == LOW && !isJumping) {
      isJumping = true;
      jumpVelocity = -8; // Initial upward velocity
      digitalWrite(LED_BUILTIN, 1);
      tone(tonePin, NOTE_C6,50);
    }

    if (isJumping) {
      ballY += jumpVelocity;
      jumpVelocity += gravity; // Apply gravity

      if (ballY >= groundLevel) {
        ballY = groundLevel;
        isJumping = false;
      }
    }

    // Move obstacle
    obstacleX -= 3;
    if (obstacleX < -obstacleWidth) {
      obstacleX = SCREEN_WIDTH ; 
      score++; // Increment score
      play=true;
      obstacleHeight = random(5,15);
    }
    // Collision detection
    if (obstacleX < 10 && obstacleX + obstacleWidth > 0) {
      if (ballY > SCREEN_HEIGHT - obstacleHeight - 10) {
        gameOver = true;
        GameOverMusic();
      }
    }
    // for 2nd obstacle -
    obstacleX2 -= 3;
    if (obstacleX2 < -obstacleWidth2) {
      obstacleX2 = SCREEN_WIDTH ;
      if (first){obstacleX2=256;first=false;}
      if (score>1){score++;play=true;} // Increment score
      obstacleHeight2 = random(5,15);
    }

    if (score!=0 ){
    // Collision detection
      if (obstacleX2 < 10 && obstacleX2 + obstacleWidth2 > 0) {
        if (ballY > SCREEN_HEIGHT - obstacleHeight2 - 10) {
          gameOver = true;
          GameOverMusic();
        }
      }
    }

    // Render game elements
    display.clearDisplay();

    // Draw ground
    display.drawLine(0, SCREEN_HEIGHT - 1, SCREEN_WIDTH, SCREEN_HEIGHT - 1, SSD1306_WHITE);

    // Draw ball
    display.drawCircle(5, ballY - 5, 3, SSD1306_WHITE); //fillRect(2, ballY - 5, 5, 5, SSD1306_WHITE);

    // Draw obstacle
    display.fillRect(obstacleX, SCREEN_HEIGHT - obstacleHeight - 1, obstacleWidth, obstacleHeight, SSD1306_WHITE);
    
    if (score !=0){
      display.drawRect(obstacleX2, SCREEN_HEIGHT - obstacleHeight2 - 1, obstacleWidth2, obstacleHeight2, SSD1306_WHITE);
    }

    // Draw score
    display.setCursor(2, 2);
    display.print("Score: ");
    display.print(score);
    display.display();
    int spd=20;

    // Control game speed
    switch (score){
      case 25 ... 49: gameSpeed=spd-5;
      break;
      case 50 ... 74: gameSpeed=spd-15;
      break;
      case 75 ... 99: gameSpeed=3;
      break;
      case 100 ... 2000: gameSpeed=1;
      break;
      default: gameSpeed=spd;
    }
    
    delay(gameSpeed);
    switch (score){
      case 25:notout();
      break;
      case 50:notout();
      break;
      case 75:notout();
      break;
      case 100:notout();
      break;
      default: ;
    }
  
  } else {
    // Game Over Screen
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(7, 15);
    display.println("Game Over!");
    display.setTextSize(1);
    display.setCursor(40, 40);
    display.print("Score: ");
    display.print(score);
    display.setCursor(10, 55);
    display.println("Press'o'to Restart");  // 'o' means the middle button 
    display.display();

    // Wait for button press to restart
    if (digitalRead(BUTTON_PIN) == LOW) {
      resetGame();
    }
  }
}

void resetGame() {
  // Reset game variables
  ballY = groundLevel;
  obstacleX = SCREEN_WIDTH;
  obstacleX2 = 64;
  score = 0;
  gameOver = false;
  first = true;
}


// game over sound
void GameOverMusic() {
  // Sadder melody for the Game Over theme with lower ending notes
  int melody[] = {
  NOTE_F6, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4,NOTE_G4
  };
  // Durations (4 = quarter note, 8 = eighth note, etc.)
  int noteDurations[] = {
    16,16, 16, 16, 16, 4
  };
  // Play each note in the melody
  for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i++) {
    int note = melody[i];
    int duration = 1000 / noteDurations[i]; // Longer duration for sad effect
    if (note > 0) {
      tone(tonePin, note, duration); // Play the note
    }
  }
}

void notout(){    // plays a tone when score increases by 25 
  if (play){
  tone(tonePin,NOTE_D6,62);
  tone(tonePin,NOTE_G6,125);
  play=false;
  }
}







//---------------  Below section contains only Boot sound and Boot image --------------------------- 

//booot sound 
void BootSound() {
  int melody[] = {  NOTE_D6, NOTE_E6,NOTE_D6, NOTE_E6,NOTE_G6};
  int noteDurations[] = {4, 16, 16, 16,4};
  for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i++) {
    int note = melody[i];
    int duration = 1000 / noteDurations[i]; 
    if (note > 0) {
      tone(tonePin, note, duration);
    }
  }
}

// Boot Image data
const uint8_t rp_logo2[] PROGMEM = {
  // Bitmap data goes here (truncated for brevity)
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfe, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x82, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x01, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3d, 0x80, 0xf0, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7a, 0xc0, 0x88, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79, 0x60, 0x88, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe4, 0xb0, 0xd0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe2, 0x58, 0x20, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xd1, 0x2f, 0x80, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x91, 0x10, 0x40, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x91, 0x08, 0x24, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x91, 0x04, 0x1a, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x71, 0x02, 0x13, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x30, 0x81, 0x13, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x10, 0x40, 0x9e, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x18, 0x20, 0x40, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x14, 0x10, 0x20, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x13, 0xc8, 0x10, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x13, 0x24, 0x08, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x12, 0x23, 0xc4, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x1a, 0x23, 0x23, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0d, 0xc2, 0x23, 0x60, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x86, 0x03, 0x22, 0x21, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x81, 0xe1, 0xc2, 0x21, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x81, 0x20, 0x01, 0xc3, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc1, 0x10, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe1, 0x33, 0x8e, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xe2, 0x49, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x03, 0x8e, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x02, 0x48, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x02, 0x28, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x80, 0x01, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x0f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void BootImg(){  
  display.clearDisplay();
  display.drawBitmap(0, 0, rp_logo2, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
  display.setTextSize(1);           
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(43, 42);         // Position for text
  display.println(F("Roytech"));
  display.setCursor(35, 55);         // Another position for more text
  display.println(F("Prototypes"));
  display.display();
  BootSound(); // boot sound
  delay(1500);  // display time 
  display.clearDisplay();
  display.display();
  delay(500); // blank time 
}