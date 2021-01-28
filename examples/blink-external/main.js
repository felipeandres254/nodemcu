// MAIN PROGRAM
load('api_timer.js');
load('api_gpio.js');

let EXTERNAL_LED = 14;

GPIO.setup_output(EXTERNAL_LED, 1);
Timer.set(500, Timer.REPEAT, function() {
    GPIO.toggle(EXTERNAL_LED);
}, null);
