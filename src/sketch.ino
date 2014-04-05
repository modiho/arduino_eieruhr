const int startPin = 2;
const int addPin = 3;
const int buzzPin = 6;
const int ledPins[5] = { 13, 12, 11, 8, 10 };

int minutes = 1;
int timerDuration = minutes;
long startTime = 0;
boolean timerStarted = false;

boolean startPressedBefore = false;
boolean addPressedBefore = false;
boolean currentMinuteLit = false;

void setup()
{
    Serial.begin(9600);
    pinMode(buzzPin, OUTPUT);
    for (int i = 0; i < 5; i++) {
        pinMode(ledPins[i], OUTPUT);
    }
}

void loop()
{
    boolean startPressed = btnPressed(startPin, startPressedBefore);
    boolean addPressed = btnPressed(addPin, addPressedBefore);

    if (addPressed && ! addPressedBefore && ! timerStarted) {
        minutes = cycleMinutes(minutes);
    }

    if (startPressed && ! startPressedBefore) {
        if (timerStarted) {
            resetTimer();
        } else {
            startTimer();
        }
    }

    if (timerStarted && timer() > timerDuration * 1000) {
        buzz();
        resetTimer();
    }

    if (timerStarted && oneSecondGone()) {
        minutes--;
    }

    lightMinutes(minutes);

    startPressedBefore = startPressed;
    addPressedBefore = addPressed;

    delay(1);
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

int cycleMinutes(int minutes) {
    if (minutes < 5) {
        minutes += 1;
    } else {
        minutes = 1;
    }

    return minutes;
}

void lightMinutes(int minutes) {
    for (int i = 0; i < 5; i++) {
        if (i < minutes - 1) {
            digitalWrite(ledPins[i], HIGH);
        } else if (i == minutes - 1) {
            if (timerStarted) {
                if (timeGone(250)) {
                    digitalWrite(ledPins[i], currentMinuteLit);
                    currentMinuteLit = ! currentMinuteLit;
                }
            } else {
                digitalWrite(ledPins[i], HIGH);
            }
        } else {
            digitalWrite(ledPins[i], LOW);
        }
    }
}

boolean btnPressed(int pin, boolean pressedBefore) {
    boolean pressed = digitalRead(pin);

    if (pressed && (! pressedBefore)) {
        delay(10);
        pressed = digitalRead(pin);
    }

    return pressed;
}
