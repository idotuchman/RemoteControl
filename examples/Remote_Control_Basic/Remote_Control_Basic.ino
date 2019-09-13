/**
 * This is an example of the Remote Control library used with an Arduino
 * programmable device and a serial monitor to control I/O pins, variables 
 * and function calls.
 * 
 * After loading the sketch, open up are arduino serial monitor and send 
 * messages like: "led=1", "led=0", "floatvar" and 
 * "hello(first=Bob, last=Pants);" 
 **/

#include <RemoteControl.h>    // https://github.com/idotuchman/RemoteControl

RemoteControl control;
int intvar = 7;
float floatvar = 3.14;
char charvar = 'h';
String first = "";
String last = "";
bool boolvar = true;

/**
 * Functions controlled by RemoteControl must return 
 * a String and receive only a character string argument.
 * 
 * Function arguments can be extracted by passing the 
 * string argument to the handleArgs() method. Multiple 
 * arguments are separated by commas (,). Make sure the
 * function arguments are registered with RemoteControl.
 * 
 * In this example, the hello() function takes two 
 * arguments: first and last. Thus, sending 
 * "hello(first=SpongeBob, last=SquarePants)" 
 * calls the hello function and sets the first and 
 * last variables to SpongeBob and SquarePants, 
 * respectively.
 **/
String hello(char *args) {
  // set default values
  first = "Foo";
  last = "Bar";

  // read function argument(s)
  control.handleArgs(args);
  return "Hello " + first + " " + last + "!\n";
}

void setup() {
  Serial.begin(9600);         // Start serial communication to send/receive messages
  Serial.println('\n');

  // register I/O pins, variables and functions for RemoteControl to control
  control.pin("led", LED_BUILTIN, OUTPUT);
  control.variable("intvar", &intvar);
  control.variable("floatvar", &floatvar);
  control.variable("charvar", &charvar);
  control.variable("first", &first);
  control.variable("last", &last);
  control.variable("boolvar", &boolvar);
  control.function("hello", &hello);
  Serial.println("Remote Control started. Ready to receive commands.");
}

void loop() {
  String response;
  char command[100] = "";                       // set max command length expected
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