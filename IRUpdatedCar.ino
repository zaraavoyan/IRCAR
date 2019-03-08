int RECV_PIN = 11;
//motor pins
int M1a = 10;
int M1b = 9;
int M2a = 6;
int M2b = 5;
int orangePin = 3;
int bluePin = 2;
int whitePin = 4;

//Values controlling the movement
//Forward Backwards
float FB = 0;
//Left and Right
float LR = 0;

//Value for the last button pressed
int pressed = 0x00;


#include <IRremote.h>

IRrecv irrecv(RECV_PIN);
decode_results results;

//Here I'm just making an array of buttons so we can just call numbers easily when checking things.
//If you want to add more buttons, jsut add to the initial array value and add the button hex code to the array. Make sure to add the name into the comment next to it so we can refer to it later.
//also don't forget to change irCheck(false) to true. it will let you see the values you need to add the the array.

int IRButtons[7] = {0xFDA05F, 0xFDB04F, 0xFD10EF, 0xFD50AF, 0xFD906F, 0xFD08F7, 0xFD8877};
//UP[0], DOWN[1], LEFT[2], RIGHT[3], CENTER[4]

void setup() {
  Serial.begin(9600);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
  pinMode(whitePin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(orangePin, OUTPUT);

}

void loop() {
  //TEMP value for the Reading
  //irCheck is a function I made that holds the ir reading function and returns an int holding the sent data.
  //the false in the irCheck function enables the function to print to the Serial monitor. This is useful for adding new button controls.
  int currentVal = irCheck(false);
  //0xFFFFFFFF is the int value given when you hold a button down. So you can continuiously hold down a button and it'll read.
  if(currentVal == 0xFFFFFFFF){
    //Serial.println("HELD");
    if(pressed == IRButtons[0]) Up();
    if(pressed == IRButtons[1]) Down();
    if(pressed == IRButtons[2]) Left();
    if(pressed == IRButtons[3]) Right();
    if(pressed == IRButtons[4]) Stop();
    if(pressed == IRButtons[5]) LightsOn();
     if(pressed == IRButtons[6]) LightsOff();
  } else if (currentVal != 0){
    //stores the initial button press.
    pressed = currentVal;
  }
  //If statements and ternary operators that handle the resetting to default values.
  if(FB!=0) FB = FB < 0 ? FB += .7 : FB -= .7;
  if(LR!=0) LR = LR < 0 ? LR += .7 : LR -= .7;

  //custom Function that handles the motor analog write functions
  UpdateMotors();
}

void UpdateMotors(){
  //LEFT MOTOR
  analogWrite(M1a, constrain(FB  - LR,0,255));
  analogWrite(M1b, constrain(-FB + LR,0,255));
  //RIGHT MOTOR
  analogWrite(M2a, constrain(FB  + LR,0,255));
  analogWrite(M2b, constrain(-FB - LR,0,255));
}

//Functions to handle the button presses. 
void Up(){
  Serial.println("Up");
  FB = constrain(FB + 50,0,255);
}

void Down(){
  Serial.println("Down");
  FB = constrain(FB - 50,-255,0);
}

void Left(){
  Serial.println("Left");
  LR = constrain(LR + 50,0,255);
}

void Right(){
  Serial.println("Right");
  LR = constrain(LR - 50,-255,0);
}

void Stop(){
  Serial.println("Stop");
  FB = 0;
  LR = 0;
}

void LightsOn() {
  Serial.println("LightsOn");
  setColor(255, 0, 0);
  
}

void LightsOff() {
  Serial.println("LightsOn");
  setColor(0, 0, 0);
  
}
void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(orangePin, redValue);
  analogWrite(bluePin, greenValue);
  analogWrite(whitePin, blueValue);
}
