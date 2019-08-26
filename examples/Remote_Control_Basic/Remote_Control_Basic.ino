/**
 * Upload and send commands like LED = 0, floatvar = 5.6,
 * mynameis(Bob), and floatvar over serial.
 * 
 **/

#include <RemoteControl.h>    // https://github.com/idotuchman/RemoteControl

RemoteControl control;
int intvar = 7;
float floatvar = 3.14;
char charvar = 'h';
String name = "?";
bool boolvar = true;

/**
 * Functions controlled by RemoteControl must return 
 * a String and receive only a character string argument.
 * Here, the function argument is also handled by
 * RemoteControl to set name.
 * 
 * In this example, sending "hello(name=Bob)" both
 * calls the hello function and sets name equal to Bob.
 **/
String hello(char *arg) {
  control.handle(arg);
  return "Hello " + name + "!\n";
}

void setup() {
  Serial.begin(115200);         // Start serial communication to send/receive messages
  delay(10);
  Serial.println('\n');

  control.pin("LED", LED_BUILTIN, OUTPUT);
  control.variable("intvar", &intvar);
  control.variable("floatvar", &floatvar);
  control.variable("charvar", &charvar);
  control.variable("name", &name);
  control.variable("boolvar", &boolvar);
  control.function("hello", &hello);
}

void loop() {
  String response;
  char command[30] = "";
  int i = 0;

  // reply only when you receive data:
  if (Serial.available() > 0) {
    while(Serial.available()) {
      // read the incoming byte:
      command[i] = Serial.read();
      if (i<30) {
        i++;
      }
      delay(3);                                 // wait for serial buffer to fill
    }
    command[i] = '\0';
    response = control.handle(command);         // parse command
    if(response != "") {
      Serial.printf("%s", response.c_str());    // print any response from parsing
    }    
  }
  delay(0);
}