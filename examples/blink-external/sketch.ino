#define LED_1 D6
#define LED_2 D7

void NodeMCU_setup() {
    pinMode(LED_1, OUTPUT);
    pinMode(LED_2, OUTPUT);
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
}

void NodeMCU_loop() {
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, LOW);
    delay(500);
    digitalWrite(LED_2, HIGH);
    digitalWrite(LED_1, LOW);
    delay(500);
}
