#include <math.h>

const int stepping[8][4] = {{0, 1, 1, 1}, {0, 0, 1, 1}, 
                            {1, 0, 1, 1}, {1, 0, 0, 1}, 
                            {1, 1, 0, 1}, {1, 1, 0, 0}, 
                            {1, 1, 1, 0}, {0, 1, 1, 0}};

class Motor {
  int a, b, c, d;
  int stepIndex = 0;

public:
  Motor(int a, int b, int c, int d) : a(a), b(b), c(c), d(d) {
    pinMode(a, OUTPUT);
    pinMode(b, OUTPUT);
    pinMode(c, OUTPUT);
    pinMode(d, OUTPUT);  
  }
  
  void eightStep(bool reverse) {
    digitalWrite(a, stepping[stepIndex][0]);      
    digitalWrite(b, stepping[stepIndex][1]);
    digitalWrite(c, stepping[stepIndex][2]);
    digitalWrite(d, stepping[stepIndex][3]);
    stepIndex = !reverse ? (stepIndex == 0 ? 7 : stepIndex - 1) : (stepIndex + 1) % 8;
  }

  void fullStep(int d, bool reverse) {
    for (int i = 0; i < 8; i++) {
      eightStep(reverse);
      delay(d);
    }
  }

  void rotate(int deg) {
    bool reverse = false;
    if (deg > 180) {
      reverse = true;
      deg = 360 - deg;
    }
    if (deg < 0) {
      reverse = !reverse;
      deg = -deg;
    }
    int steps = int(deg * 512.0 / 360.0);
    for (int i = 0; i < steps; i++) fullStep(1, reverse);
  }
  
};


//int a = 9, b = 8, c = 11, d = 10;
//int a = 5, b= 4, c = 7, d = 6;

Motor mot1(9, 8, 11, 10);
Motor mot2(5, 4, 7, 6);
 
void setup() {
  // initialize the serial port:
  Serial.begin(9600);
  //mot1 = Motor::Motor(9, 8, 11, 10);
  //mot2 = Motor::Motor(5, 4, 7, 6);
  delay(1);
}
 
//void rotate(float degress){
// 
//}
 
void loop() {
  if (Serial.available() > 0) {
    int command = Serial.parseInt();
    if (command == 0) return;
    
    Motor* m = command > 0 ? &mot1 : &mot2;
    command = abs(command);
   
    m->rotate(command);
    Serial.print("OK"); 
  }  
}
