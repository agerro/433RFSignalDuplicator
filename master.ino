#include <RCSwitch.h>

const int buttonPin = 4; // the number of the button pin
const int ledPin =  13; // the number of the on board LED

RCSwitch mySwitch = RCSwitch();

boolean toggle = true; // used for toggling sending signals for on or off
boolean first = true; // used for toggling reading signals for on or off

long off = 0; // long value for decimal off signal
long on = 0; // long value for decimal on signal

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  
  Serial.begin(9600);
  
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(10);
  // Receiver on interrupt 0 => that is pin #2
  mySwitch.enableReceive(0);  
}

void loop() {
  // when button is push, send stored signals
  if(digitalRead(buttonPin) == HIGH){
    // send on, then the other
    if(toggle){
      // send signal on
      mySwitch.send(on, 24);
      toggle=!toggle;
      // logging
      Serial.print("Send first: ");
      Serial.println(on);
    }else{
      // send signal off
      mySwitch.send(off, 24);
      toggle=!toggle;
      // logging
      Serial.print("Send second: ");
      Serial.println(off);
    }
  }else{
    // when nothing is pushed, the code listens and stores read signals
    if (mySwitch.available()){
      int value = mySwitch.getReceivedValue();
      if(value != 0){
        // store in the first, then the second
        if(first){
          on = value;
          first = false;
          // blink to indicate read
          blinkOnce();
          // logging
          Serial.print("Read first: ");
          Serial.println(on);
        }else{
          off = value;
          first = true;
          // blink to indicate read
          blinkTwice();
          // logging
          Serial.print("Read second: ");
          Serial.println(off);
        }
      }
    }
    // reset the receiver so it dosent continue to read
    mySwitch.resetAvailable();
  }
}

// indication
void blinkOnce(){
  digitalWrite(ledPin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);
}

// indication
void blinkTwice(){
  digitalWrite(ledPin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);
  delay(100);
  digitalWrite(ledPin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);
}
