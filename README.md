# RemoteControl
An Arduino library for basic I/O pin, variable and function call control over communication links (Serial, WebSockets, etc.).

# How to use
First, include the RemoteControl library and create an instance of the RemoteControl class:
```cpp
#include <RemoteControl.h>
RemoteControl control;
```

To control I/O pins, variables, and function calls, use the ```pin```, ```variable```, ```function``` methods respectively in setup(). For example,

```cpp
int myint;
float myfloat = 3.14;
bool mybool;

String mynameis(char *arg) {
  return "Hello " + String(arg) + "!\n";
}

setup() {
   ... 
   control.pin("LED", LED_BUILTIN, OUTPUT);
   control.variable("myint", &myint);
   control.variable("myfloat", &myfloat);
   control.variable("mybool", &mybool);
   control.function("mynameis", &mynameis);
   ...
}
```
Above, we are controlling the LED_BUILTIN pin, the "myint", "myfloat" and "mybool" variables, and the function "mynameis". The LED_BUILTIN pin is assigned the name "LED" and is specified as an output pin. The RemoteControl library will set the pin mode as an output pin.

To process an incoming command, use the ```handle``` method.
```cpp
loop() {
   ...
   control.handle(command);
   ...
}
```

## I/O Pins

The RemoteControl library will set the pin mode as either input or output depending on whether you specify INPUT or OUTPUT in the pin method. To set an I/O pin, send a command with the pin name, an equal sign, either 1 or 0, and an optional semicolon. For example:
```cpp
LED=1
```

To read the I/O pin, send a command with just the pin name, and an optional semicolon. For example:
```cpp
LED
```
The handle method returns a ```String``` with the value of the pin as either 0 or 1, and a semicolon. For example, the return may be ```LED=1;```.

## Variables
To set a variable, send a command with the variable name, an equal sign, a value, and an optional semicolon. For example:
```cpp
myint=99
mybool = false;
```

To read the variable, send a command with just the variable name, and an optional semicolon. For example:
```cpp
myint
mybool
```
The handle method returns a ```String``` with the variable name, an equal sign, the value of the variable, and a semicolon. For example, the return may be ```mybool=0;```.

## Functions
Functions must return a ```String``` and receive only a character string argument (```char *arg```).

To call a function, send the function name with the character string in parenthesis. For example, ```functionName(argument)```.

The handle method returns a ```String``` with the return value of the function.

## Multiple Commands
You can combine multiple commands by separating commands with semicolons (;):
```cpp
LED=1; mybool = true; mynameis(Bob);
```