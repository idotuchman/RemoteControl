/*
 * Header for RemoteControl
 *
 * MIT License
 *
 * Copyright (c) 2019 Ido Tuchman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *
 */

#ifndef REMOTECONTROL_H
#define REMOTECONTROL_H
#include <String.h>
#include <Arduino.h>

const int MAX_NAME_LENGTH = 20;
const int MAX_PINS_CONTROLLED = 20;
const int MAX_VARIABLES_CONTROLLED = 10;
const int MAX_FUNCTIONS_CONTROLLED = 10;
enum Type {INT, FLOAT, CHAR, CHAR_ARRAY, STRING, BOOL };

class RemoteControl
{
  public: 
    RemoteControl();
    void pin(char* name, int pinNumber, bool ouput);
    void variable(char *name, int *variable);
    void variable(char *name, float *variable);
    void variable(char *name, char *variable);
    // void variable(char *name, char[] *variable);
    void variable(char *name, String *variable);
    void variable(char *name, bool *variable);
    void function(char *name, String (*function)(char* arg));
    String handle(char *message);
    //void getVariables(void);    //TODO returns a structure of variables and values

  private:
    String _processFunction(char *functionName, char *arg);
    String _processCommand(char* command);
    char *trim(char *str);

  // holds all I/O pins controlled remotely
    struct {
      char name[MAX_NAME_LENGTH];
      byte pinNumber;
      bool output;
    } _pinList[MAX_PINS_CONTROLLED];
    int _pinListIndex = 0;

  // holds all int variables controlled remotely
    struct {
      char name[MAX_NAME_LENGTH];
      void *variable;
      Type type;
    } _variableList[MAX_VARIABLES_CONTROLLED];
    int _variableListIndex = 0;

  // holds all fuctions controlled remotely
    struct {
      char name[MAX_NAME_LENGTH];
      String (*function)(char*);
    } _functionList[MAX_FUNCTIONS_CONTROLLED];
    int _functionListIndex = 0;
};

#endif