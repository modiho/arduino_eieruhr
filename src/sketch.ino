#include <LiquidCrystal.h>

const int startPin = 2;
const int addPin = 3;
const int buzzPin = 10;

int minutes = 1;
int timerDuration = minutes;
long startTime = 0;
boolean timerStarted = false;

boolean startPressedBefore = false;
boolean addPressedBefore = false;

LiquidCrystal lcd(12, 11, 4, 5, 6, 7);


void setup()
{
    Serial.begin(9600);
    pinMode(buzzPin, OUTPUT);
    lcd.begin(16, 2);
    setDisplay(String(minutes));
}

long lastTime = millis();
long milliseconds = 0;

void loop()
{
    boolean startPressed = btnPressed(startPin, startPressedBefore);
    boolean addPressed = btnPressed(addPin, addPressedBefore);

    if (addPressed && ! addPressedBefore && ! timerStarted) {
        minutes += 1;
        setDisplay(String(minutes));
    }

    addPressedBefore = addPressed;

    if (startPressed && ! startPressedBefore) {
        if (timerStarted) {
            resetTimer();
        } else {
            startTimer();
            lastTime = millis();
        }
    }
    startPressedBefore = startPressed;

    long currentTime = millis();
    if ((currentTime - lastTime) > 1000 && timerStarted) {
        timerLoop(addPressed, startPressed);
        lastTime = currentTime;
    }
}

void setDisplay(String text) {
    lcd.home();
    lcd.clear();
    lcd.print(text);
}

void timerLoop(boolean addPressed, boolean startPressed) {
    Serial.println(String(millis()));
    minutes--;

    setDisplay(String(minutes));

    if (minutes == 0) {
        buzz();
        resetTimer();
        setDisplay(String(minutes));
    }
}

boolean timeGone(int time) {
    return timer() % time == 0;
}

boolean oneSecondGone() {
    return timeGone(1000);
}

void startTimer() {
    timerDuration = minutes;
    timerStarted = true;
    startTime = millis();
}

void resetTimer() {
    timerStarted = false;
    minutes = timerDuration;
}

int timer() {
    return millis() - startTime;
}

void buzz() {
    analogWrite(buzzPin, 50);
    delay(1000);
    digitalWrite(buzzPin, LOW);
}

boolean btnPressed(int pin, boolean pressedBefore) {
    boolean pressed = digitalRead(pin);

    if (pressed && (! pressedBefore)) {
        delay(10);
        pressed = digitalRead(pin);
    }

    return pressed;
}
