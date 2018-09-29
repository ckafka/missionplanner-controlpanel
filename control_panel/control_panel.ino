/* control_panel.ino 
 *
 * Connor Kafka
 * September 29th, 2018
 * 
 * Mission Planner Control Panel Rev2. 
 * 
 * This Arduino sketch turns on LEDs and sends serial messages when a button or switch is pressed on the 
 * hardware control panel. 
 * 
 * The first byte sent is equal to decimal 8 or 9 for ARMED or DISARMED, respectively 
 * The second byte sent is equal to decimal 0-8, which corresponds to the buttons (0-7) or return to launch (RTL) switch (8)
 */

//Macros
#define ARM_SWITCH    Switches[0]
#define RTL_SWITCH    Switches[1]

//Message IDs - buttons are IDs 0-7
#define RTL       8              
#define ARMED     9
#define DISARMED  10

int activeAction = -1; 
int prevAction = -1; 

typedef struct {
  int inputPin;
  int powerPin;
} Button;

typedef struct {
  int inputPin;
  int powerPin;
  boolean state;
} Switch;

Button Buttons[8];
Switch Switches[2];


void setup() {
  //initialize all of the flight mode buttons
  for(int i = 0; i < 8; i++){
    Buttons[i].inputPin = (8 - i);                            //Button input pins range from 8-1
    pinMode((Buttons[i].inputPin), INPUT_PULLUP);
    Buttons[i].powerPin = (i + 16);                           //Button power pins range from 16-23
    pinMode((Buttons[i].powerPin), OUTPUT);
  }

  //initialize the two switches
  for(int i = 0; i < 2; i++){
    Switches[i].inputPin = (10 - i);                          //Switch input pins range from 10-9
    pinMode((Switches[i].inputPin), INPUT_PULLUP);
    Switches[i].powerPin = (i + 14);                          //Switch power pins range from 14-15
    pinMode((Switches[i].powerPin), OUTPUT);
    Switches[i].state = false;                                //Switches are initially off  
    digitalWrite(Switches[i].powerPin, HIGH);                 //Provide power to switch LED, activates mechanically when switched
  }
  delay(1000);
  Serial.begin(9600);
}

//Main function, runs continously 
void loop() {

  //Handle input from the buttons
  for(int i = 0; i < 8; i++){
    if(digitalRead(Buttons[i].inputPin)){
      activeAction = i; 
      lightsOff(i);                                         //turn the pressed button on and the rest off
    }
  }
  
  //Check the RTL switch
  if(digitalRead(RTL_SWITCH.inputPin)){
    if(!RTL_SWITCH.state){
      prevAction = activeAction;                   
      pulse();                                           
    }
    activeAction = RTL;
    RTL_SWITCH.state = true; 
  } else if (RTL_SWITCH.state){                             //if RTL cancelled, revert to the old flight mode
    activeAction = prevAction; 
    lightsOff(activeAction); 
    RTL_SWITCH.state = false; 
  }

  //Send serial messages
  digitalRead(ARM_SWITCH.inputPin) ? Serial.write(ARMED) : Serial.write(DISARMED);
  Serial.write(activeAction); 

  delay(1000);
}


//This function turns on a selected light and the rest off
void lightsOff(int on){
  digitalWrite(Buttons[on].powerPin, HIGH);
  for(int i = 0; i < 9; i++){
    if(i != on){
      digitalWrite(Buttons[i].powerPin, LOW);
    } 
  }
}

//This function flashes the button lights for 200 ms
void pulse(){
  for(int i = 0; i < 8; i++){
    digitalWrite(Buttons[i].powerPin, HIGH);
  }
  delay(200);
  for(int i = 8; i >= 0; i--){
    digitalWrite(Buttons[i].powerPin, LOW);
  }
}

