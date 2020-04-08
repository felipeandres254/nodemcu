load('api_timer.js');
load('api_gpio.js');

let ONBOARD_LED = 2;

GPIO.setup_output(ONBOARD_LED, 1);
Timer.set(500, Timer.REPEAT, function() {
    GPIO.toggle(ONBOARD_LED);
}, null);
