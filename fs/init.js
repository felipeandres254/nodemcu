// Application initialization
load('api_timer.js');
load('api_gpio.js');

// Show state
let ONBOARD_LED = 2;
GPIO.setup_output(ONBOARD_LED, 1);
let _startup = Timer.set(50, Timer.REPEAT, function() {
    GPIO.toggle(ONBOARD_LED);
}, null);

// Application loading
load('main.js');
Timer.set(1000, 0, function() {
    Timer.del(_startup);
    GPIO.write(ONBOARD_LED, 1);
}, null);
