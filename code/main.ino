#include "pattern.h";
#include <RTClib.h>;

#define OE    7 // OE
#define DATA  8 // DS
#define STCP  9 // LATCH
#define SHCP 10 // SHIFT

#define SW    1 // ENCODER SWITCH
#define CLK   2 // ENCODER CLOCK
#define DT    3 // ENCODER DATA

RTC_DS1307 RTC;

int pattern = 0;
int encPos  = 0;
int encCurrentState;
int encLastState;

void setup() {

    pinMode(STCP, OUTPUT);
    pinMode(SHCP, OUTPUT);
    pinMode(DATA, OUTPUT);
    pinMode(OE, OUTPUT);

    // ENABLE IF THERE IS NO EXTERNAL RESISTOR
    // pinMode(CLK, INPUT_PULLUP);
    // pinMode(DT, INPUT_PULLUP);

    // ENCODER INTERRUPT LISTENER
    encLastState = digitalRead(CLK);
    // PCB NOT SUPPORTED IN THIS WAY ON A ATTINY
    // attachInterrupt(0, updateEncoder, CHANGE);
    // attachInterrupt(1, updateEncoder, CHANGE);

    if (!RTC.begin()) {
        while (1)
            delay(10);
    }

    if (!RTC.isrunning()) {
        // INITIAL RTC DEVICE
        RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

}

void loop() {

    DateTime now = RTC.now();

    char formatTime[6] = "hhmmss";
    char formatDate[4] = "DDMM";

    updateEncoder();

    switch (encPos) {
    case 1:
        if (delayTime(250)) {
            uint32_t randomBits = getRandomBits(24);
            digitalWrite(STCP, LOW);
            shiftOut(DATA, SHCP, MSBFIRST, randomBits);
            shiftOut(DATA, SHCP, MSBFIRST, (randomBits >> 8));
            shiftOut(DATA, SHCP, MSBFIRST, (randomBits >> 16));
            digitalWrite(STCP, HIGH);
        }

        break;
    case 2:
        digitalWrite(STCP, LOW);
        shiftOut(DATA, SHCP, MSBFIRST, knight[pattern][2]);
        shiftOut(DATA, SHCP, MSBFIRST, knight[pattern][1]);
        shiftOut(DATA, SHCP, MSBFIRST, knight[pattern][0]);
        digitalWrite(STCP, HIGH);

        if (delayTime(50)) pattern = (pattern + 1) % 37;

        break;
    case 3:
        digitalWrite(STCP, LOW);
        shiftOut(DATA, SHCP, MSBFIRST, police[pattern][2]);
        shiftOut(DATA, SHCP, MSBFIRST, police[pattern][1]);
        shiftOut(DATA, SHCP, MSBFIRST, police[pattern][0]);
        digitalWrite(STCP, HIGH);

        if (delayTime(100)) pattern = (pattern + 1) % 8;

        break;
    default:
        uint32_t currentTime = strtoul(now.toString(formatTime), NULL, 10);
        digitalWrite(STCP, LOW);
        shiftOut(DATA, SHCP, MSBFIRST, currentTime);
        shiftOut(DATA, SHCP, MSBFIRST, (currentTime >> 8));
        shiftOut(DATA, SHCP, MSBFIRST, (currentTime >> 16));
        digitalWrite(STCP, HIGH);

        break;
    }
}
