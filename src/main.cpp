#include "terminal.cpp"

// Pin definitions
#define SENSOR_PIN 15       // pulse sensor

// Timer & pulse variables
bool bip = false;
bool bipPrev = false;
bool calibrated = false;
int state = 0;
int delta = 0;
int calibration = 0;
int prevSensorValue = 0;
constexpr float alpha = 0.1; // smoothing factor

float prevTime = 0;
uint64_t nowTime = 0;         // microseconds when pulse started
uint64_t lastPulseTime = 0;   // microseconds of last detected pulse
constexpr uint64_t pulseMinInterval = 1000000; // 1 second in microseconds
uint64_t lapseTime = 0;       // elapsed microseconds

// --- Helper functions ---
bool blink(const float input) {
    return (input > calibration + delta || input < calibration - delta);
}

bool pulse(bool bip, bool bipPrev) {
    if (bip && !bipPrev) {
        uint64_t t = esp_timer_get_time();
        if (t - lastPulseTime >= pulseMinInterval) {
            lastPulseTime = t;
            return true;
        }
    }
    return false;
}

// --- Min-value buffer for spike filtering ---
#define MEDIAN_SIZE 3
int sensorBuffer[MEDIAN_SIZE] = {0};
int bufferIndex = 0;

int minValue(int arr[], int size) {
    int minVal = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] < minVal) minVal = arr[i];
    }
    return minVal;
}

void setup() {
    // Initialize OLED
    display.begin(0x3c, true);
    display.clearDisplay();
    display.setTextSize(4);
    display.setTextColor(SH110X_WHITE);

    pinMode(SENSOR_PIN, INPUT);

    if (analogRead(SENSOR_PIN) < 2200) {
        terminalBootSequence();
    }
}

void loop() {
    // --- Read sensor and update min-value buffer ---
    sensorBuffer[bufferIndex] = analogRead(SENSOR_PIN);
    bufferIndex = (bufferIndex + 1) % MEDIAN_SIZE;
    const int sensorValue = minValue(sensorBuffer, MEDIAN_SIZE);

    delta = abs(sensorValue - prevSensorValue) + 50;

    prevSensorValue = sensorValue;

    bip = blink(sensorValue);

    // --- Calibration with blinking text and alpha filter ---
    static uint64_t calibrationStartTime = 0;
    if (!calibrated) {
        if (calibrationStartTime == 0) calibrationStartTime = esp_timer_get_time();

        // Blinking "Calibrating..."
        static bool blinkState = false;
        if ((esp_timer_get_time() - calibrationStartTime) / 250000 % 2 == 0) blinkState = true;
        else blinkState = false;

        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(10, 30);
        if (blinkState) display.print("Starting timer...");
        display.display();

        // Alpha filter
        calibration = (1 - alpha) * calibration + alpha * sensorValue;

        // Finish calibration after 5 seconds
        if (esp_timer_get_time() - calibrationStartTime > 3000000) {
            calibrated = true;
            display.setTextSize(4);
            //Serial.print("Calibration finished: ");
            //Serial.println(calibration);
            delay(1000);
        }
        return; // skip rest of loop until calibrated
    }

    // --- Update calibration continuously ---
    calibration = (1 - alpha) * calibration + alpha * sensorValue;

    // --- State machine ---
    switch (state) {
        case 0: { // show previous time
            display.clearDisplay();
            display.setCursor(8, 20);
            const double time = prevTime / 1000000.0;
            if (time > 99) {
                display.print(prevTime / 1000000.0, 1);
            } else {
                display.print(prevTime / 1000000.0, 2);
            }
            display.display();

            if (pulse(bip, bipPrev)) {
                nowTime = esp_timer_get_time();
                state = 1;
            }
            break;
        }

        case 1: {
            lapseTime = esp_timer_get_time() - nowTime;

            display.clearDisplay();
            display.setCursor(8, 20);
            display.print(lapseTime / 1000000.0, 2);
            display.display();

            if (pulse(bip, bipPrev)) {
                prevTime = lapseTime;
                state = 0;
            }

            break;
        }
        default:
            display.clearDisplay();
            display.setCursor(8, 20);
            display.print("The Emperor protects");
            display.display();
            break;
    }

    bipPrev = bip;
}
