#include <RemoteControl.h>    // https://github.com/idotuchman/RemoteControl

RemoteControl control;
int myvar;

void setup() {
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  control.pin("LED", LED_BUILTIN, OUTPUT);
  control.variable("myvar", &myvar);
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
      delay(3);      // wait for serial buffer to fill
    }
    command[i] = '\0';
    response = control.handle(command);
    if(response != "") {
      Serial.printf("Response: %s \n", response.c_str());
    }    
  }
  delay(0);
}