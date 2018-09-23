#include "./esppl_functions.h" 
#include <Servo.h>

///////////////Static Variables/////////////////
#define RAISED 0 //eyes open
#define DROPPED 140 //closed////////////////////ADJUST TO CREATE DESIRABLE OPEN ANGLE
#define LIST_SIZE 9
#define USER 0
#define SERVO_PIN 1
#define TIME_BUFFER 1200 ///////////////////////Must return "false" XXXX times to make arm drop | used for quality control due to the 'handshake' nature of Mac Addresses
#define SERVO_STARTUP_POS 140 //////////////////The position the servo will start at
////////////////////////////////////////////////

/////////////////Global Variables////////////////
int UsedPins[LIST_SIZE];
int counter[LIST_SIZE];
Servo myservo[LIST_SIZE];
boolean ExisitingUsers[LIST_SIZE];

//////////////MacAddresses_Catelogue/////////////////////
/*{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}Person1
 *{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}Person2
 *{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}Person3
 *{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}Person4
 *{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}Person5
 *{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}Person6
 *{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}Person7
 *{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}Person8
 *{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}Person9
 *{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}Person10
 *{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}Person11
 *{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}Person12
 *{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}Person13
 *{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}Person14
 *{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}Person15
 *{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}Person16
 *{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}Person17
 *{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}Person18
 *{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}Person19
 *{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}Person20
 *{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}Person21
 *{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}Person22
 *{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}Person23
 *{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}Person24
 *{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}Person25
 *
 *Board 1 Pins: 14, 12, 13, 15, 0, 16, 2, 5, 4.
 *Board 2 Pins: 12, 13, 15, 0, 16, 2, 5, 4.
 *Board 3 Pins: 12, 13, 15, 0, 16, 2, 5, 4.
 */

////////////////FRIENDLIST///////////////////////////////
uint8_t friendmac[LIST_SIZE][ESPPL_MAC_LEN] = {
  {0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX} //Person1
 ,{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX} //Person2
 ,{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX} //Person3
 ,{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX} //Person4
 ,{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX} //Person5
 ,{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX} //Person6
 ,{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX} //Person7
 ,{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX} //Person8
 ,{0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX} //Person9
  }; 
  
String FriendList[LIST_SIZE][2] = {
 
  {"Person1's Phone", "14"},
  {"Person2's Phone", "12"},
  {"Person3's Phone", "13"},
  {"Person4's Phone", "15"},
  {"Person5's Phone", "0"},
  {"Person6's Phone", "16"},
  {"Person7's Phone", "2"},
  {"Person8's Phone", "5"},
  {"Person9's Phone", "4"),
    
};
//////////////////////////////////////////////////

///////////////////////these methods are initialise the list variables to default state////////////////////////
void InitialiseExistingUsers(){//default being false
  for(int i = 0; i < LIST_SIZE; i++){
    ExisitingUsers[i] = false;
  }
}
void InitialiseUsedPins(){//default being 0
  for(int i = 0; i < LIST_SIZE; i++){
    UsedPins[i] = 0;
  }
}
void InitialiseServo(){
  for(int i = 0; i < LIST_SIZE; i++){
    myservo[i].attach(FriendList[i][SERVO_PIN].toInt());
    myservo[i].write(SERVO_STARTUP_POS);
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////These methods are related to servo usage////////////////////////////////
void ChangeServoPosition(int row, int pos){//this method changes servo position
  myservo[row].write(pos);
  ChangeCurrentUserStatus(row);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////These methods are incharge of the programs logic//////////////////////////////
bool OverallUser(int pin){ //This method is checking if the user sharing the same pin are all gone or just one device. If all device are gone only then would it return a false. Else it will return true
  int UserCounter = 0;
  for(int i = 0; i < LIST_SIZE; i++){
    if(pin == FriendList[i][SERVO_PIN].toInt()){
      if(ExisitingUsers[i] == true){//if any of the user device is currently online then it will add to the counter
        UserCounter++;
      }
    }
  }
  if(UserCounter == 0){ return false; } else { return true; }//if the value is above 0 then the one or more of the user device is connected
}
void ChangeCurrentUserStatus(int row){//simple method that changes the status of the user. if this method is used it will just change the current status to the opposite... IE: false becomes true vise versa
  if(ExisitingUsers[row] == true){ ExisitingUsers[row] = false;} else
                                  {ExisitingUsers[row] = true;}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool maccmp(uint8_t *mac1, uint8_t *mac2) { //this methods takes two mac address and compares them
 for (int i=0; i < ESPPL_MAC_LEN; i++) { 
   if (mac1[i] != mac2[i]) { 
     return false; 
   } 
 }  
 return true;
} 
void cb(esppl_frame_info *info) { 
 for (int i=0; i<LIST_SIZE; i++) { 
   if (maccmp(info->sourceaddr, friendmac[i]) || maccmp(info->receiveraddr, friendmac[i])) { 
     if(ExisitingUsers[i] == false){
       Serial.printf("\n%s is here! :)", FriendList[i][USER].c_str());
       
      

       ChangeServoPosition(i, RAISED);
     }
     counter[i] = 0;
   } else {
     if(ExisitingUsers[i] == true && counter[i] == TIME_BUFFER){
      Serial.printf("\n%s has left! :)", FriendList[i][USER].c_str());
      ChangeServoPosition(i, DROPPED);
      counter[i] = 0;
     } else if(ExisitingUsers[i] == true && counter[i] < TIME_BUFFER) { 
      counter[i]++;
     }
   }
 }
}
void setup() { 
 InitialiseExistingUsers();
 InitialiseServo();
 delay(500);
 Serial.begin(115200); 
 esppl_init(cb); 
} 
void loop() { 
 esppl_sniffing_start(); //searching for Mac Addresses
 while (true) { 
   for (int i = ESPPL_CHANNEL_MIN; i <= ESPPL_CHANNEL_MAX; i++ ) { 
     esppl_set_channel(i); 
     while (esppl_process_frames()) { 
       // 
     } 
   } 
 } 
 
 }
