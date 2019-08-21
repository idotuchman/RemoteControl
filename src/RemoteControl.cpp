#include "RemoteControl.h"

RemoteControl::RemoteControl() {}

/**
 * I/O PIN
 **/
void RemoteControl::pin(char* name, int pinNumber, bool output) {
  if (_pinListIndex < MAX_PINS_CONTROLLED) {
    // TODO check if name already used
    strcpy(_pinList[_pinListIndex].name, trim(name));
    _pinList[_pinListIndex].pinNumber = pinNumber;
    _pinList[_pinListIndex].output = output;
    output ? pinMode(pinNumber, OUTPUT) : pinMode(pinNumber, INPUT);
    Serial.printf("%s pin added to RemoteControl.\n", _pinList[_pinListIndex].name, _pinList[_pinListIndex].pinNumber, _pinList[_pinListIndex].output ? "true" : "false");
    _pinListIndex++;
  } else {
    Serial.printf("Pin %s not controlled. Too many pins controlled.", name);
  }
}

/**
 * INT VARIABLE
 **/
void RemoteControl::variable(char *name, int *variable) {
  if (_variableListIndex < MAX_VARIABLES_CONTROLLED) {
    // TODO check if name already used
    strcpy(_variableList[_variableListIndex].name, trim(name));
    _variableList[_variableListIndex].variable = variable;
    _variableList[_variableListIndex].type = INT;
    Serial.printf("Int %s variable added to RemoteControl.\n", trim(name));
    // Serial.printf("%i: %s = %i\n", _index, _variableNames[_index].c_str(), *_variablePointers[_index]);
    _variableListIndex++;
  } else {
    Serial.printf("Variable %s not controlled. Too many variables controlled.\n", name);
  }
}

/**
 * FLOAT VARIABLE
 **/
void RemoteControl::variable(char *name, float *variable) {
  if (_variableListIndex < MAX_VARIABLES_CONTROLLED) {
    // TODO check if name already used
    strcpy(_variableList[_variableListIndex].name, trim(name));
    _variableList[_variableListIndex].variable = variable;
    _variableList[_variableListIndex].type = FLOAT;
    Serial.printf("Float %s variable added to RemoteControl.\n", trim(name));
    // Serial.printf("%i: %s = %i\n", _index, _variableNames[_index].c_str(), *_variablePointers[_index]);
    _variableListIndex++;
  } else {
    Serial.printf("Variable %s not controlled. Too many variables controlled.\n", name);
  }
}

/**
 * CHAR VARIABLE
 **/
void RemoteControl::variable(char *name, char *variable) {
  if (_variableListIndex < MAX_VARIABLES_CONTROLLED) {
    // TODO check if name already used
    strcpy(_variableList[_variableListIndex].name, trim(name));
    _variableList[_variableListIndex].variable = variable;
    _variableList[_variableListIndex].type = CHAR;
    Serial.printf("Char %s variable added to RemoteControl.\n", trim(name));
    // Serial.printf("%i: %s = %i\n", _index, _variableNames[_index].c_str(), *_variablePointers[_index]);
    _variableListIndex++;
  } else {
    Serial.printf("Variable %s not controlled. Too many variables controlled.\n", name);
  }
}

/**
 * STRING VARIABLE
 **/
void RemoteControl::variable(char *name, String *variable) {
  if (_variableListIndex < MAX_VARIABLES_CONTROLLED) {
    // TODO check if name already used
    strcpy(_variableList[_variableListIndex].name, trim(name));
    _variableList[_variableListIndex].variable = variable;
    _variableList[_variableListIndex].type = STRING;
    Serial.printf("String %s variable added to RemoteControl.\n", trim(name));
    // Serial.printf("%i: %s = %i\n", _index, _variableNames[_index].c_str(), *_variablePointers[_index]);
    _variableListIndex++;
  } else {
    Serial.printf("Variable %s not controlled. Too many variables controlled.\n", name);
  }
}

/**
 * BOOL VARIABLE
 **/
void RemoteControl::variable(char *name, bool *variable) {
  if (_variableListIndex < MAX_VARIABLES_CONTROLLED) {
    // TODO check if name already used
    strcpy(_variableList[_variableListIndex].name, trim(name));
    _variableList[_variableListIndex].variable = variable;
    _variableList[_variableListIndex].type = BOOL;
    Serial.printf("Bool %s variable added to RemoteControl.\n", trim(name));
    // Serial.printf("%i: %s = %i\n", _index, _variableNames[_index].c_str(), *_variablePointers[_index]);
    _variableListIndex++;
  } else {
    Serial.printf("Variable %s not controlled. Too many variables controlled.\n", name);
  }
}

/**
 * FUNCTION
 **/
void RemoteControl::function(char *name, String (*function)(char *arg)) {
  if (_functionListIndex < MAX_FUNCTIONS_CONTROLLED) {
    // TODO check if name already used
    strcpy(_functionList[_functionListIndex].name, trim(name));
    _functionList[_functionListIndex].function = function;
    Serial.printf("%s function added to RemoteControl.\n", trim(name));
    // Serial.printf("%i: %s = %i\n", _index, _variableNames[_index].c_str(), *_variablePointers[_index]);
    _functionListIndex++;
  } else {
    Serial.printf("Function %s not controlled. Too many functions controlled.\n", name);
  }
}

String RemoteControl::handle(char *message) {
  char *command;
  String response = "";
  command = strtok(message, ";");       // look for commands delimited with semi-colons
  while( command != NULL) {
    response += _processCommand(command);
    command = strtok(NULL, ";");
  } 
  return response;
}

String RemoteControl::_processCommand(char *command) {
  String response = "";
  char *param, *temp, *value;

  param = strchr(command, '(');    // check if command is a function call by looking for an open parenthesis
  if (param != NULL) {
    *param = '\0';                 // if this is an assignment operation, command holds function name    
    trim(command);
    temp = strchr(++param, ')');  // look for a close parenthesis of the function call
    if (temp != NULL) {
      *temp = '\0';
      return _processFunction(command, param);
    }
    return "";    // if no close parenthesis found, it's an invalid function call
  }

  param = strchr(command, '=');    // check if command is an assignment by looking for an equal sign
  if (param != NULL) {
    *param = '\0';                 // if this is an assignment operation, command holds name of pin/variable
    value = trim(++param);
  }
  trim(command);

  // check if command is in pins array
  for (int i=0; i < _pinListIndex; i++) {
    if (strcmp(command, _pinList[i].name) == 0) {
      // Pin match found
      if(_pinList[i].output && param != NULL) {
        digitalWrite(_pinList[i].pinNumber, atoi(value) == 0? LOW : HIGH);
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
      // Variable match found
      if(param != NULL) {     // check if command is an assignment
        switch(_variableList[i].type) {
          case INT    : *(int *)_variableList[i].variable = atoi(value); break;
          case FLOAT  : *(float *)_variableList[i].variable = atof(value); break;
          case CHAR   : *(char *)_variableList[i].variable = value[0]; break;
          // case CHAR_ARRAY: strcpy((char[] *)_variableList[i].variable, value); break;
          case STRING : *(String *)_variableList[i].variable = String(value); break;
          case BOOL   : *(bool *)_variableList[i].variable = (value[0] == '0' || strcmp(value, "false") == 0) ? false : true; break;
        }        
      } else {
        // variable match found, not an assignment, return variable value
        switch(_variableList[i].type) {
          case INT    : response = String(command) + "=" + *(int *)_variableList[i].variable + ";\n"; break;
          case FLOAT  : response = String(command) + "=" + *(float *)_variableList[i].variable + ";\n"; break;
          case CHAR   : response = String(command) + "=" + *(char *)_variableList[i].variable + ";\n"; break;
          case STRING : response = String(command) + "=" + *(String *)_variableList[i].variable + ";\n"; break;
          case BOOL   : response = String(command) + "=" + *(bool *)_variableList[i].variable + ";\n"; break;
        }
      }
      return response;
    }
  } 
  return "";
}

String RemoteControl::_processFunction(char *functionName, char *arg) {
  // check if function name is in functions array
  for (int i=0; i < _functionListIndex; i++) {
    if (strcmp(functionName, _functionList[i].name) == 0) {
      // function match found, call function and pass char argument
      return _functionList[i].function(arg);
    }
  }
  return "";
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