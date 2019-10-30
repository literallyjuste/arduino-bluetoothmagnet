

#include <SoftwareSerial.h>   //Software Serial Port
#include <Keypad.h>

#define RxD         8
#define TxD         6

#define PINLED      9

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
const int buttonPin = 4;     // the number of the pushbutton pin
const int ledPin =  3;      // the number of the LED pin
const int electromagnet = 6; //elektromagnet
const int led2 = 2;
#define DEBUG_ENABLED  1

SoftwareSerial blueToothSerial(RxD,TxD);

int buttonState = 0;         // variable for reading the pushbutton status
int magnetState = 0;

char rx_byte = 0;
String rx_str = "";
String input;
int tueroffen = 0;
char hexaKeys[ROWS][COLS] = {
  {'1','3','2','A'},
  {'4','6','5','B'},
  {'7','9','8','C'},
  {'*','#','0','D'}
};
String keyinput;
char customKey;
byte rowPins[ROWS] = {10, 11, 12, 13}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8, 9}; //connect to the column pinouts of the keypad

unsigned long t=0;
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup()
{
    Serial.begin(9600);
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
  // initialize the LED pin as an output:
    pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
    pinMode(buttonPin, INPUT);
    pinMode(led2, OUTPUT);
    pinMode(electromagnet, OUTPUT);
    setupBlueToothConnection();
}

int count = 0;
void loop()
{
  digitalWrite(electromagnet, HIGH);
  customKey = customKeypad.getKey();
  if (customKey){
        t=millis();
        if (customKey == "#") {
          keyinput = "";
        }
        else {
        keyinput = String(keyinput+customKey);
        }
        if (keyinput=="1234") {
          tueroffen = 1;
          Serial.println("Tür wird geöffnet!");
        }
        
        Serial.println(customKey);
        Serial.println(keyinput);
        customKey = "";

  }
  if (millis()-t>2000) {
          t=millis();
          keyinput="";
        }  
  if (Serial.available() > 0) {    // is a character available?
    //Serial.print("Avaible");
    rx_byte = Serial.read();       // get the character
    //Serial.print(count++);
    //Serial.print(rx_byte);
    
    if (rx_byte != '!') {
      // a character of the string was received
      rx_str += rx_byte;
    } else {
      // end of string
      Serial.print("Welcome ");
      Serial.println(rx_str);
      rx_str = "";                // clear the string for reuse
      Serial.println("");
    }
  }
  
    digitalWrite(led2, HIGH);
    char recvChar;

   
        while(blueToothSerial.available())
        {//check if there's any data sent from the remote bluetooth shield
            recvChar = blueToothSerial.read();
            //Serial.print(recvChar);
            input=String(input+recvChar);
            if (input == "test"){
              tueroffen = 1;
              Serial.println("Tür wird geöffnet!");
            }
                           
        }
    input="";
    if (tueroffen == 1) {
      digitalWrite(electromagnet, LOW);
      digitalWrite(ledPin, HIGH);
      digitalWrite(led2, LOW);
      delay(3000);
      digitalWrite(led2, HIGH);
      digitalWrite(electromagnet, HIGH);
      digitalWrite(ledPin, LOW);
//      digitalWrite(electromagnet, LOW);
//      delay(3000);
//      digitalWrite(electromagnet, HIGH);
      tueroffen = 0;
    } 
}


void setupBlueToothConnection()
{	

	
	
	blueToothSerial.begin(9600);  
	
	blueToothSerial.print("AT");
	delay(400); 

	blueToothSerial.print("AT+DEFAULT");             // Restore all setup value to factory setup
	delay(2000); 
	
	blueToothSerial.print("AT+NAMESeeedBTSlave");    // set the bluetooth name as "SeeedBTSlave" ,the length of bluetooth name must less than 12 characters.
	delay(400);
	
    blueToothSerial.print("AT+PIN0000");             // set the pair code to connect 
	delay(400);
	
	blueToothSerial.print("AT+AUTH1");             //
    delay(400);    

    blueToothSerial.flush();

}
