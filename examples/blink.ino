#define LED 2    // Onboard LED of NodeMCU

void NodeMCU_setup() {
    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);
}

void NodeMCU_loop() {
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
}
