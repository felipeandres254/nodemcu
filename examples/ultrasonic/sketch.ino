#define TRIG_PIN D1
#define ECHO_PIN D0
#define LED_1    D6
#define LED_2    D7

long MIN_DISTANCE = 5;
long MAX_DISTANCE = 100;
long MIN_TIME     = 50;
long MAX_TIME     = 500;

// Variables
long duration;
long distance;
long led_time;

void NodeMCU_setup() {
    // Setup HC-SR04
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    // Setup LEDs
    pinMode(LED_1, OUTPUT);
    pinMode(LED_2, OUTPUT);
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
}

void NodeMCU_loop() {
    // Clear trigger
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    // Trigger pulse
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Read echo
    duration = pulseIn(ECHO_PIN, HIGH);
    distance = duration*0.034/2;
    if (distance >= MAX_DISTANCE) {
        led_time = MAX_TIME;
    } else if (distance <= MIN_DISTANCE) {
        led_time = MIN_TIME;
    } else {
        float m = (MAX_TIME-MIN_TIME)/(MAX_DISTANCE-MIN_DISTANCE);
        led_time = MIN_TIME + (m*(distance-MIN_DISTANCE));
    }

    // Do LED blink
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, LOW);
    delay(led_time);
    digitalWrite(LED_2, HIGH);
    digitalWrite(LED_1, LOW);
    delay(led_time);
}
