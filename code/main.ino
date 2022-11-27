#include <RTClib.h>;

#define SHCP 8  // SHIFT
#define STCP 9  // LATCH
#define DATA 10 // DS

uint32_t countTime    = 0;
uint32_t countPattern = 0;
bool debug            = 0;

RTC_DS1307 rtc;

void setup() {

    if (debug) Serial.begin(9600);

    pinMode(STCP, OUTPUT);
    pinMode(SHCP, OUTPUT);
    pinMode(DATA, OUTPUT);

    // RTC
    pinMode(A3, OUTPUT);
    digitalWrite(A3, HIGH);
    pinMode(A2, OUTPUT);
    digitalWrite(A2, LOW);

    if (!rtc.begin()) {
        while (1)
            delay(10);
    }

    if (!rtc.isrunning()) {
        // INITIAL RTC DEVICE
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}

void loop() {

    DateTime now = rtc.now();

    char format[6] = "hhmmss";

    countTime = strtoul(now.toString(format), NULL, 10);

    digitalWrite(STCP, LOW);

    shiftOut(DATA, SHCP, MSBFIRST, (countTime >> 16));
    shiftOut(DATA, SHCP, MSBFIRST, (countTime >> 8));
    shiftOut(DATA, SHCP, MSBFIRST, countTime);
    shiftOut(DATA, SHCP, MSBFIRST, 0);

    digitalWrite(STCP, HIGH);

    if (debug) {
        serialMonitor(now.hour(), now.minute(), now.second(), countTime);
        delay(1000);
    }
}
