#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

Adafruit_SH1106G display(128, 64, &Wire, -1);

void typeText(const char* text, int delayMs) {
    while (*text) {
        display.print(*text);
        display.display();
        delay(delayMs);
        text++;
    }
}

void typeLine(const char* text, int y, int delayMs) {
    display.setCursor(0, y);
    typeText(text, delayMs);
}

void terminalBootSequence() {
    display.setTextSize(1);
    display.clearDisplay();

    delay(1000);

    display.setCursor(0, 0);
    display.print("+-------------------+");
    display.setCursor(0, 10);
    display.print("| Imperial          |");
    display.setCursor(0, 20);
    display.print("|      Control      |");
    display.setCursor(0, 30);
    display.print("|          Terminal |");
    display.setCursor(0, 40);
    display.print("+-------------------+");
    display.display();

    delay(1000);

    for (int i = 0; i <= 100; i += 5) { // smaller increments = smoother
        display.clearDisplay();
        display.setCursor(0, 50);
        display.print("> [");

        int steps = 12; // total bar steps
        int filled = (i * steps) / 100; // calculate how many "=" to fill

        for (int j = 0; j < filled; j++) {
            display.print("=");
        }
        for (int j = filled; j < steps; j++) {
            display.print("-");
        }
        display.print("] ");
        display.print(i);
        display.print("%");
        // Display Imperial Control Terminal header
        display.setCursor(0, 0);
        display.print("+-------------------+");
        display.setCursor(0, 10);
        display.print("| Imperial          |");
        display.setCursor(0, 20);
        display.print("|      Control      |");
        display.setCursor(0, 30);
        display.print("|          Terminal |");
        display.setCursor(0, 40);
        display.print("+-------------------+");
        display.display();
        delay(200);  // Delay for visual effect
    }
    delay(1000);

    // Power sequence
    display.clearDisplay();
    typeLine("Machine spirit rising", 10, 25);
    delay(500);
    typeLine("Sacred circuits", 30, 25);
    typeLine("engaged", 40, 25);
    delay(500);

    display.clearDisplay();
    typeLine("Core logic", 10, 25);
    typeLine("sanctified", 20, 25);
    delay(500);
    typeLine("Rites of", 40, 25);
    typeLine("initialization", 50, 25);
    typeLine("complete", 60, 25);
    delay(500);

    display.clearDisplay();
    typeLine("Initializing", 10, 25);
    typeLine("Core...", 20, 25);
    delay(500);

    for (int i = 0; i <= 100; i += 20) {
        display.clearDisplay();
        display.setCursor(0, 40);
        display.print("> [");
        for (int j = 0; j < i / 10; j++) {
            display.print("=");
        }
        for (int j = i / 10; j < 10; j++) {
            display.print("-");
        }
        display.print("] ");
        display.print(i);
        display.print("%");
        display.setCursor(0, 10);
        display.print("Initializing");
        display.setCursor(0, 20);
        display.print("Core...");
        display.display();
        delay(500);  // Delay for visual effect
    }

    // Continue the initialization process
    display.clearDisplay();
    typeLine("Memory check", 10, 25);
    typeLine("complete", 20, 25);
    delay(500);
    typeLine("Sensors", 40, 25);
    typeLine("online", 50, 25);
    delay(500);

    display.clearDisplay();
    typeLine("Calibration", 10, 25);
    typeLine("running", 20, 25);
    delay(500);
    typeLine("Diagnostics", 40, 25);
    typeLine("nominal", 50, 25);
    delay(500);

    display.clearDisplay();
    typeLine("Control", 10, 25);
    typeLine("systems ready", 20, 25);
    delay(500);

    display.clearDisplay();

    display.setCursor(0, 10);
    typeText("Verifying", 25);
    display.setCursor(0, 20);
    typeText("Temporal Integrity...", 25);

    for (int i = 0; i <= 40; i += 5) {
        display.clearDisplay();
        display.setCursor(0, 40);
        display.print("> [");

        for (int j = 0; j < i / 10; j++) {
            display.print("=");
        }
        for (int j = i / 10; j < 10; j++) {
            display.print("-");
        }

        display.print("] ");
        display.print(i);
        display.print("%");

        display.setCursor(0, 10);
        display.print("Verifying");
        display.setCursor(0, 20);
        display.print("Temporal Integrity...");

        display.display();
        delay(250);
    }

    delay(2000);

    // Temporal displacement check
    display.clearDisplay();
    typeLine("*WARNING*", 10, 25);
    typeLine("Error Detected:", 30, 25);
    typeLine("TemporalDisplacement", 50, 25);
    delay(1000);

    display.clearDisplay();
    typeLine("Recalibrating", 10, 10);
    typeLine("Chrono-Synch...", 20, 10);
    delay(500);

    // Additional progress
    for (int i = 10; i <= 100; i += 30) {
        display.clearDisplay();
        display.setCursor(0, 40);
        display.print("> [");

        for (int j = 0; j < i / 10; j++) {
            display.print("=");
        }
        for (int j = i / 10; j < 10; j++) {
            display.print("-");
        }

        display.print("] ");
        display.print(i);
        display.print("%");

        display.setCursor(0, 10);
        display.print("Recalibrating");
        display.setCursor(0, 20);
        display.print("Chrono-Synch...");
        display.display();

        delay(1000);
    }

    // Finalizing initialization
    display.clearDisplay();
    typeLine("Machine spirit:", 10, 25);
    delay(500);
    typeLine("'stable'", 30, 25);
    delay(1000);

    display.clearDisplay();
    typeLine("Diagnostics", 10, 25);
    typeLine("complete", 20, 25);
    delay(500);
    typeLine("All systems nominal", 40, 25);
    delay(500);

    display.clearDisplay();
    typeLine("Sensors awakened", 10, 25);
    typeLine("to the ", 20, 25);
    typeLine("Omnissiahs light", 30, 25);
    delay(500);

    display.clearDisplay();
    typeLine("System Online...", 10, 25);
    typeLine("> Program Initialized", 30, 25);
    typeLine("> Parameters Reset", 40, 25);
    delay(1000);

    // Final status
    display.clearDisplay();
    typeLine("====================", 0, 10);
    typeLine("> SYSTEM STATUS:", 10, 25);
    typeLine("OPERATIONAL", 20, 25);
    typeLine("> STANDBY MODE:", 40, 25);
    typeLine("ENGAGED", 50, 25);
    delay(2000);

    // Awaiting input
    display.clearDisplay();
    typeLine("By steel and code,", 10, 25);
    typeLine("I endure.", 20, 25);
    typeLine("Awaiting input...", 40, 25);

    delay(1500);
}