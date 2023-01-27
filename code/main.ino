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

    uint32_t currentTime = strtoul(now.toString(formatTime), NULL, 10);
    digitalWrite(STCP, LOW);
    shiftOut(DATA, SHCP, MSBFIRST, currentTime);
    shiftOut(DATA, SHCP, MSBFIRST, (currentTime >> 8));
    shiftOut(DATA, SHCP, MSBFIRST, (currentTime >> 16));
    digitalWrite(STCP, HIGH);
}
