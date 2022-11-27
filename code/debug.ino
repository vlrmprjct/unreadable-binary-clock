void serialMonitor(int hour, int minute, int second, uint32_t time) {
    Serial.print(hour, DEC);
    Serial.print(":");
    Serial.print(minute, DEC);
    Serial.print(":");
    Serial.print(second, DEC);
    Serial.println();
    Serial.print(time);
    Serial.println();
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
