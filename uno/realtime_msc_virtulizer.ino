
// Libraries to include
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <LCDGraph.h>
#include <AS3935MI.h>
#include <AceWire.h>

// Constructors
#define LCD_ADDRESS 0x27
#define LCD_ROWS 2
#define LCD_COLS 16
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);
LCDGraph<float, LiquidCrystal_I2C> graph(8, 0); // We want to store floats in this case

#define SPEAKER_PIN 9 // Define the pin for the speaker

// Define a simple melody
const int melody[] = {
  262, 294, 330, 349, 392, 440, 494, 523
};

const int noteDurations[] = {
  4, 4, 4, 4, 4, 4, 4, 4
};

// Initialize the AS3935MI lightning sensor
AceWireI2C aceWire;
AS3935MI lightningSensor(&aceWire);

void setup() {
    // Set up the lcd
    lcd.init();
    lcd.backlight();
    graph.begin(&lcd);

    // Welcome messages
    Serial.begin(38400);
    Serial.println(F("LCD Graph I2C demonstration"));
    lcd.print(F("I2C Sine Wave"));
    delay(2000); // Display the welcome message for 2 seconds
    lcd.clear();

    // Set up the graph:
    graph.yMin = -800;
    graph.yMax = 800;
    graph.filled = false;
    graph.setRegisters();
    graph.display(0, 1);

    // Set up the speaker pin
    pinMode(SPEAKER_PIN, OUTPUT);

    // Initialize the lightning sensor
    lightningSensor.begin();
    // Configure the lightning sensor as needed
    lightningSensor.setIndoors();
    lightningSensor.setNoiseLevel(2);
    lightningSensor.setWatchdogThreshold(2);
    lightningSensor.setSpikeRejection(2);
}

void loop() {
    static float counter = 0;
    float result = 800 * sin(counter);

    // Plot the sine wave on the LCD
    lcd.clear();
    for (int i = 0; i < LCD_COLS; i++) {
        float value = 800 * sin(counter + (i * 0.2));
        int yPos = map(value, -800, 800, 0, LCD_ROWS - 1);
        lcd.setCursor(i, yPos);
        lcd.write(byte(255)); // Draw a block at the calculated position
    }

    // Check for lightning detection
    int lightning = lightningSensor.readInterruptReg();
    if (lightning != 0) {
        Serial.print(F("Lightning detected: "));
        Serial.println(lightning);
        lcd.setCursor(0, 0);
        lcd.print(F("Lightning Det!"));
    }

    // Play the melody
    for (int thisNote = 0; thisNote < 8; thisNote++) {
        int noteDuration = 1000 / noteDurations[thisNote];
        tone(SPEAKER_PIN, melody[thisNote], noteDuration);
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        noTone(SPEAKER_PIN);
    }

    // Display the base frequency on the LCD
    lcd.setCursor(0, 1);
    lcd.print(F("Playing Melody"));

    counter += 0.2;
    delay(200); // Adjust the delay to control the update speed
}
