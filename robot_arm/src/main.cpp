#include <Wire.h> // Wire library allows for communication with I2C Devices
#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>

class Stepper {
    public:
    const short step_pin;
    const short dir_pin;
    const short oe_pin;
    const short STEPS_PER_REVOLUTION; 

    Stepper(short t_step_pin, short t_dir_pin, short t_oepin, short t_STEPS_PER_REVOLUTION)
        : step_pin(t_step_pin), dir_pin(t_dir_pin), oe_pin(t_oepin), STEPS_PER_REVOLUTION(t_STEPS_PER_REVOLUTION) {}

    void setup(){
        pinMode(oe_pin,OUTPUT);
        pinMode(dir_pin,OUTPUT);
        pinMode(step_pin,OUTPUT);
    }
    void step_counterclockwise(int step_count,int step_duration){    
        digitalWrite(dir_pin,LOW);
        for (int i =0; i <step_count; i++){
            digitalWrite(step_pin,HIGH);
            delayMicroseconds(step_duration);
            digitalWrite(step_pin,LOW);
            delayMicroseconds(step_duration);
    }}
    void step_clockwise(int step_count,int step_duration){
        digitalWrite(dir_pin,HIGH);
        for (int i =0; i <step_count; i++){
            digitalWrite(step_pin,HIGH);
            delayMicroseconds(step_duration);
            digitalWrite(step_pin,LOW);
            delayMicroseconds(step_duration);
    }
}
    void test_stepper(){
        digitalWrite(dir_pin,HIGH);
        for (int i = 0 ;i<10;i++){
            digitalWrite(step_pin,HIGH);
            delayMicroseconds(5000);
            digitalWrite(step_pin,LOW);
            delayMicroseconds(5000);    
        } 
        delay(10);
        digitalWrite(dir_pin,LOW);
        for (int i = 0 ;i<10;i++){
            digitalWrite(step_pin,HIGH);
            delayMicroseconds(5000);
            digitalWrite(step_pin,LOW);
            delayMicroseconds(5000);    
        } 
}

/*   void change_direction(int direction){
        switch (direction){
        case 0:
            digitalWrite(dir_pin,HIGH);
        
        case 1:
            digitalWrite(dir_pin,LOW);
        
    }

}
*/
    
};

class ServoController {
    public:
    const short NUM_SERVOS;
    const short oe_pin;
    const short SERVO_FREQ;
    const short SERVO_MIN;
    const short SERVO_MAX;
    const long OSCILLATOR_FREQUENCY = 27000000l;

    Adafruit_PWMServoDriver pwm;
    ServoController(short t_NUM_SERVOS,short t_oe_pin,short t_SERVO_FREQ, short t_SERVO_MIN,short t_SERVO_MAX,long t_OSCILLATOR_FREQUENCY,Adafruit_PWMServoDriver t_pwm) : NUM_SERVOS(t_NUM_SERVOS),oe_pin(t_oe_pin),SERVO_FREQ(t_SERVO_FREQ), SERVO_MIN(t_SERVO_MIN),SERVO_MAX(t_SERVO_MAX),OSCILLATOR_FREQUENCY(t_OSCILLATOR_FREQUENCY),pwm(t_pwm) {}

    void setup(){
        pwm.begin();
        pwm.setOscillatorFrequency(OSCILLATOR_FREQUENCY);
        pwm.setPWMFreq(SERVO_FREQ);
        pinMode(oe_pin,OUTPUT);
        digitalWrite(oe_pin,LOW);
    }

    void move_servo(short servo_num,short angle){
        short pulse_ticks = map(angle,0,180,SERVO_MIN,SERVO_MAX);
        Serial.println(pulse_ticks);
        pwm.setPWM(servo_num,0,pulse_ticks);
    }

    void test_servo(short servo_num){
        move_servo(servo_num,0);
        delay(500);

        move_servo(servo_num,180);
        delay(500);
    }

    void test_all(){
        for (int pos = SERVO_MAX; pos > SERVO_MIN;pos-=2)
        {
            for (int i = 0; i < NUM_SERVOS;i++){
                pwm.setPWM(i,0,pos);

            }
        }
        for (int pos = SERVO_MIN; pos <SERVO_MAX;pos+=2)
        {
            for (int i = 0; i < NUM_SERVOS;i++){
                pwm.setPWM(i,0,pos);
            }
        }
        
    }

};

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
Stepper my_stepper = Stepper(8,9,10,200);
ServoController servo_controller = ServoController(6,2,50,100,540,27000000l, pwm);
const int NUM_SERVOS = 6;
void setup() {
    Serial.begin(9600); // Rate of communication between bluetooth module and Serial

    my_stepper.setup();
    servo_controller.setup();

    for (int i = 0; i<NUM_SERVOS;i++){
        servo_controller.move_servo(i,0);
    }
    delay(10); 


}


void loop() {
    // test_stepper();
    // servo_controller.test_all();
    
    if (Serial.available()){
        
        char incomingByte = Serial.read();
        Serial.println(incomingByte);
        if (incomingByte == 'C'){
            my_stepper.step_clockwise(5,1000);

        }
        else if (incomingByte == 'D'){
            my_stepper.step_counterclockwise(5,1000);
        }
// }
}


