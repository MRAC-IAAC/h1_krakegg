/**
   Robot Command State :
   - Inflate Fingers
   - Hold Fingers
   - Deflate Fingers
   - Inflate Knuckles
   - Hold Knuckles
   - Deflate Knuckles

   When finger line is high, inflate for time / until force. Hold when low. Leave open when second is high
   When knuckle line has rising edge, inflate for time
*/

#define PIN_FINGER_CHANGE           4
#define PIN_FINGER_DIRECTION        5
#define PIN_KNUCKLE_CHANGE          6
#define PIN_KNUCKLE_DIRECTION       7
#define PIN_FINGER_INFLATE_INPUT    8
#define PIN_FINGER_DEFLATE_INPUT    9
#define PIN_KNUCKLE_INFLATE_INPUT   10
#define PIN_KNUCKLE_DEFLATE_INPUT   11
#define PIN_LIGHT                   13
#define PIN_FSR_0                   A0
#define PIN_FSR_1                   A1
#define PIN_FSR_2                   A2
#define PIN_FSR_3                   A3

#define FINGER_INFLATE_TIME  200
#define KNUCKLE_INFLATE_TIME 200


#define FSR_CUTOFF           512

boolean fingerInflateSignal = false;
boolean fingerInflateSignalPrevious = false;
boolean fingerDeflateSignal = false;
boolean fingerDeflateSignalPrevious = false;

boolean knuckleInflateSignal = false;
boolean knuckleInflateSignalPrevious = false;
boolean knuckleDeflateSignal = false;
boolean knuckleDeflateSignalPrevious = false;

uint32_t fingerInflateTimer = 0;
uint32_t knuckleInflateTimer = 0;


void setup() {
  Serial.begin(9600);
  pinMode(PIN_FINGER_CHANGE, OUTPUT);
  pinMode(PIN_FINGER_DIRECTION, OUTPUT);
  pinMode(PIN_KNUCKLE_CHANGE, OUTPUT);
  pinMode(PIN_KNUCKLE_DIRECTION, OUTPUT);

  pinMode(PIN_FINGER_INFLATE_INPUT, INPUT);
  pinMode(PIN_FINGER_DEFLATE_INPUT, INPUT);
  pinMode(PIN_KNUCKLE_INFLATE_INPUT, INPUT);
  pinMode(PIN_KNUCKLE_DEFLATE_INPUT, INPUT);

  pinMode(PIN_LIGHT, OUTPUT);

  pinMode(PIN_FSR_0, INPUT);
  pinMode(PIN_FSR_1, INPUT);
  pinMode(PIN_FSR_2, INPUT);
  pinMode(PIN_FSR_3, INPUT);
}


void loop() {
  // Read Input Pins
  fingerInflateSignal = ! digitalRead(PIN_FINGER_INFLATE_INPUT);
  fingerDeflateSignal = ! digitalRead(PIN_FINGER_DEFLATE_INPUT);

  knuckleInflateSignal = ! digitalRead(PIN_KNUCKLE_INFLATE_INPUT);
  knuckleDeflateSignal = ! digitalRead(PIN_KNUCKLE_DEFLATE_INPUT);

  // Set up relay states
  boolean fingerInflateState = fingerInflateSignal;
  boolean fingerDeflateState = fingerDeflateSignal;
  boolean knuckleInflateState = fingerInflateSignal;
  boolean knuckleDeflateState = knuckleDeflateSignal;


//  // Finger Timing and State Control
//  if (fingerInflateSignal == true && fingerInflateSignalPrevious == false) {
//    fingerInflateTimer = millis();
//  }
//  if (fingerInflateSignal && fingerDeflateSignal == false && millis() - fingerInflateTimer < FINGER_INFLATE_TIME) {
//    fingerInflateState = true;
//  }

  float fsrValue0 = analogRead(PIN_FSR_0);
  float fsrValue1 = analogRead(PIN_FSR_1);
  float fsrValue2 = analogRead(PIN_FSR_2);
  float fsrValue3 = analogRead(PIN_FSR_3);
  if (fsrValue0 > FSR_CUTOFF || fsrValue1 > FSR_CUTOFF || fsrValue2 > FSR_CUTOFF || fsrValue3 > FSR_CUTOFF || fingerDeflateState) {
    fingerInflateState = false;
  }

//  // Knuckle Timing and State Control
//  if (knuckleInflateSignal == true && knuckleInflateSignalPrevious == false) {
//    knuckleInflateTimer = millis();
//  }
//  if (knuckleInflateSignal && knuckleDeflateSignal == false && millis() - knuckleInflateTimer < KNUCKLE_INFLATE_TIME) {
//    knuckleInflateState = true;
//  }
  knuckleDeflateState = knuckleDeflateSignal;

  // Write to relay outputs
  digitalWrite(PIN_FINGER_CHANGE, fingerInflateState || fingerDeflateState);
  digitalWrite(PIN_FINGER_DIRECTION, fingerInflateState);
  digitalWrite(PIN_KNUCKLE_CHANGE, knuckleInflateState || knuckleDeflateState);
  digitalWrite(PIN_KNUCKLE_DIRECTION, knuckleInflateState);

  // Update previous state variables
  fingerInflateSignalPrevious = fingerInflateSignal;
  fingerDeflateSignalPrevious = fingerDeflateSignal;
  knuckleInflateSignalPrevious = knuckleInflateSignal;
  knuckleDeflateSignalPrevious = knuckleDeflateSignal;
}
