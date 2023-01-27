uint32_t getRandomBits(uint8_t n) {
    if (n > 21)
        return random() >> (31 - n);
    static uint32_t buf = 0;
    static uint8_t idx = 0;
    uint32_t t = 0;
    if (n > idx) {
        n -= idx;
        t = buf << n;
        buf = random();
        idx = 31;
    }
    if (t == 0)
        t = buf & ((1UL << n) - 1);
    else
        t |= buf & ((1UL << n) - 1);
    buf >>= n;
    idx -= n;
    return t;
}

void updateEncoder() {
    encCurrentState = digitalRead(CLK);
    if (encCurrentState != encLastState && encCurrentState == 0) {
        if (digitalRead(DT)) {
            encPos = (encPos + 1) % 20;
        } else {
            encPos = (encPos + 19) % 20;
        }
    }
    encLastState = encCurrentState;
    delay(1);
}

boolean delayTime(unsigned long time) {
    static unsigned long previousmillis = 0;
    unsigned long currentmillis = millis();
    if (currentmillis - previousmillis >= time) {
        previousmillis = currentmillis;
        return true;
    }
    return false;
}

void setBrightness(byte brightness) {
    analogWrite(OE, 255 - brightness);
}
