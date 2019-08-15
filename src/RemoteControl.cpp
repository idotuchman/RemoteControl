#include "RemoteControl.h"

RemoteControl::RemoteControl() {}

void RemoteControl::pin(char* name, int pinNumber, bool output) {
  if (_pinListIndex < MAX_PINS_CONTROLLED) {
    // TODO check if name already used
    strcpy(_pinList[_pinListIndex].name, trim(name));
    _pinList[_pinListIndex].pinNumber = pinNumber;
    _pinList[_pinListIndex].output = output;
    output ? pinMode(pinNumber, OUTPUT) : pinMode(pinNumber, INPUT);
    Serial.printf("New pin added: Name: %s, Pin: %i, Output: %s\n", _pinList[_pinListIndex].name, _pinList[_pinListIndex].pinNumber, _pinList[_pinListIndex].output ? "true" : "false");
    _pinListIndex++;
  } else {
    Serial.printf("Pin %s not controlled. Too many pins controlled.", name);
  }
}

void RemoteControl::variable(char* name, int *variable) {
  if (_variableListIndex < MAX_VARIABLES_CONTROLLED) {
    // TODO check if name already used
    strcpy(_variableList[_variableListIndex].name, trim(name));
    _variableList[_variableListIndex].variable = variable;
    Serial.printf("New variable added: Name: %s\n", trim(name));
    _variableListIndex++;
  } else {
    Serial.printf("Variable %s not controlled. Too many variables controlled.\n", name);
  }
}

String RemoteControl::handle(char* message) {
  char* command;
  String response = "";
  command = strtok(message, ";");       // look for commands delimited with semi-colons
  while( command != NULL) {
    response += _processCommand(command);
    command = strtok(NULL, ";");
  }
  return response;
}

String RemoteControl::_processCommand(char* command) {
  String response = "";
  char* assign;
  int value = 0;

  assign = strchr(command, '=');    // check if an assignment command
  if (assign != NULL) {
    *assign = '\0';                 // if this is an assignment operation, command holds name
    value = atoi(trim(++assign));
  }
  trim(command);

  // check if command is in pins array
  for (int i=0; i < _pinListIndex; i++) {
    if (strcmp(command, _pinList[i].name) == 0) {
      // Pin match found
      if(_pinList[i].output && assign!=NULL) {
        digitalWrite(_pinList[i].pinNumber, value == 0? LOW : HIGH);
      } else {
        // pin match found, not an assignment, return pin value
        response = String(command) + "=" + digitalRead(_pinList[i].pinNumber) + ";\n";
      }
      return response;
    }
  }

  // check if command is in variables array
  for (int i=0; i < _variableListIndex; i++) {
    if (strcmp(command, _variableList[i].name) == 0) {
      // Pin match found
      if(assign!=NULL) {
        *_variableList[i].variable = value;    // assign value to variable
      } else {
        // variable match found, not an assignment, return variable value
        response = String(command) + "=" + *_variableList[i].variable + ";\n";
      }
      return response;
    }
  }  

  Serial.printf("Command %s not found. Assignment %s. Value = %i.\n", command, assign!=NULL? "true" : "false", value);
  return response;
}

// https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way
char *RemoteControl::trim(char *str)
{
    size_t len = 0;
    char *frontp = str;
    char *endp = NULL;

    if( str == NULL ) { return NULL; }
    if( str[0] == '\0' ) { return str; }

    len = strlen(str);
    endp = str + len;

    /* Move the front and back pointers to address the first non-whitespace
     * characters from each end.
     */
    while( isspace((unsigned char) *frontp) ) { ++frontp; }
    if( endp != frontp )
    {
        while( isspace((unsigned char) *(--endp)) && endp != frontp ) {}
    }

    if( str + len - 1 != endp )
            *(endp + 1) = '\0';
    else if( frontp != str &&  endp == frontp )
            *str = '\0';

    /* Shift the string so that it starts at str so that if it's dynamically
     * allocated, we can still free it on the returned pointer.  Note the reuse
     * of endp to mean the front of the string buffer now.
     */
    endp = str;
    if( frontp != str )
    {
            while( *frontp ) { *endp++ = *frontp++; }
            *endp = '\0';
    }

    return str;
}