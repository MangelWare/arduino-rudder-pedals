
int NUM_SENSORS = 3;
int sensorPins[] = {A0,A1,A2};    // select the input pin for the potentiometer
int curr = 0;
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
int buttonPins[] = {2,3,4};
int button_c = 0;
int resetButton = 5;

const int A_PIN_C = A0;
const int A_PIN_L = A1;
const int A_PIN_R = A2;

// -------------------------------
// Calibration
//
// Center (rotation) calibration
const long C_MAX_V = 32767l;
const long C_MIN_V = -32768l;
long calib_c_neutral;
long calib_c_left;
long calib_c_right;
int get_calibrated_c(int sensor_v) {
  long l_sv = (long) sensor_v;
  
  // Adapt calibration
  if(l_sv < calib_c_left) {
    calib_c_left = l_sv;
  } else if (l_sv > calib_c_right) {
    calib_c_right = l_sv;
  }
  
  // Map to appropriate value
  if(l_sv > calib_c_neutral) {
    return (int) (((l_sv - calib_c_neutral) * C_MAX_V) / (calib_c_right-calib_c_neutral));
  } else if(l_sv < calib_c_neutral) {
    return (int) (((calib_c_neutral - l_sv) * C_MIN_V) / (calib_c_neutral-calib_c_left));
  } else {
    return 0;
  }
}
//
// Left pedal calibration
const long L_MIN = -32768l;
const long L_MAX = 32767l;
long calib_l_min;
long calib_l_max;
long get_calibrated_l(int sensor_v) {
  long l_sv = (long) sensor_v;

  // Adapt calibration
  if(l_sv < calib_l_min) {
    calib_l_min = l_sv;
  } else if(l_sv > calib_l_max) {
    calib_l_max = l_sv;
  }

  // Map to appropriate value
  return (int) (((l_sv - calib_l_min) * (L_MAX - L_MIN) / (calib_l_max-calib_l_min)) + L_MIN);
}
// 
// Right pedal calibration
const long R_MIN = -32768l;
const long R_MAX = 32767l;
long calib_r_min;
long calib_r_max;
long get_calibrated_r(int sensor_v) {
  long l_sv = (long) sensor_v;

  // Adapt calibration
  if(l_sv < calib_r_min) {
    calib_r_min = l_sv;
  } else if(l_sv > calib_r_max) {
    calib_r_max = l_sv;
  }

  // Map to appropriate value
  return (int) (((l_sv - calib_r_min) * (R_MAX - R_MIN) / (calib_r_max-calib_r_min)) + R_MIN);
}
//
// Reset calibration
void reset_calib() {
  long l_c = (long) analogRead(A_PIN_C);
  calib_c_neutral = l_c-1;
  calib_c_left = l_c;
  calib_c_right = l_c+1;
  
  long l_v = (long) analogRead(A_PIN_L);
  calib_l_min = l_v;
  calib_l_max = l_v+1;
  
  long r_v = (long) analogRead(A_PIN_R);
  calib_r_min = r_v;
  calib_r_max = r_v+1;
}
// -------------------------------


void setup() {
  // Start serial interface
  Serial.begin(9600);
  
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);

  // declare the buttonPins as INPUT:
  for(int i = 0; i < NUM_SENSORS; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  // declare reset button as input:
  pinMode(resetButton, INPUT_PULLUP);

  //init/reset calibration:
  reset_calib();
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPins[curr]);

  int calibratedValue = 0;
  switch(curr) {
    case 0:
      calibratedValue = get_calibrated_c(sensorValue);
      break;
     case 1:
      calibratedValue = get_calibrated_l(sensorValue);
      break;
     case 2:
      calibratedValue = get_calibrated_r(sensorValue);
      break;
  }
  Serial.println(calibratedValue);

  /*for(int i = 0; i < NUM_SENSORS; i++) {
    Serial.print(i);
    Serial.print(":");
    Serial.print(digitalRead(buttonPins[i]));
    Serial.print(" ");
  }
  Serial.println();*/

  button_c++;
  if(button_c >= 10) {
    button_c = 0;

    if(digitalRead(resetButton == 0)) {
      reset_calib();
    }
    
    for(int i = 0; i<NUM_SENSORS; i++) {
      if(digitalRead(buttonPins[i]) == 0) {
        curr = i;
        break;
      }
    }
  }
  delay(10);
}
