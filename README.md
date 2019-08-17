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
int myvar;

String mynameis(char *arg) {
  return "Hello " + String(arg) + "!\n";
}

setup() {
   ... 
   control.pin("LED", LED_BUILTIN, OUTPUT);
   control.variable("myvar", &myvar);
   control.function("mynameis", &mynameis);
   ...
}
```
Above, we are controlling the LED_BUILTIN pin, the "myvar" variable, and the function call to "mynameis". The LED_BUILTIN pin is assigned the name "LED" and is specified as an output pin. The RemoteControl library will set the pin mode as an output pin.

To process an incoming command, use the ```handle``` method.
```cpp
loop() {
   ...
   control.handle(command);
   ...
}
```

## I/O Pins

The RemoteControl library will set the pin mode as either input or output depending on whether you specify INPUT or OUTPUT in the pin method. To set an I/O pin, send a command with the pin name, an equal sign, and either 1 or 0. For example:
```cpp
LED=1
```

To read the I/O pin, send a command with just the pin name. For example:
```cpp
LED
```

## Variables
Currently, only ```int``` variables are controllable. To set the variable, send a command with the variable name, an equal sign, and an integer. For example:
```cpp
myVar=99
```

To read the variable, send a command with just the variable name. For example:
```cpp
myVar
```

## Functions
Functions must return a ```String``` and receive only a character string argument (```char *arg```).

To call a function, send the function name with the character string in parenthesis. For example, ```functionName(argument)```.

## Multiple Commands
You can combine multiple commands by separating commands with semicolons (;):
```cpp
LED=1;myVar=11;mynameis(Bob);
```