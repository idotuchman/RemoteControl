# RemoteControl
A library for basic pin and variable control over communication links (Serial, WebSockets, etc.).

# How to use
First, include the RemoteControl library and create an instance of the RemoteControl class:
```cpp
#include <RemoteControl.h>
RemoteControl control;
```

To control I/O pins and integer variables, use the ```control``` method in setup() to register the I/O pins and variables you want to control.

```cpp
int myvar;

setup() {
   ... 
   control.pin("LED", LED_BUILTIN, OUTPUT);
   control.variable("myvar", &myvar);
}
```
Here, we are going to control the LED_BUILTIN pin the the "myvar" variable. The LED_BUILTIN pin is assigned the name "LED" and is specified as an output pin. The RemoteControl library will set the pin mode as an output pin.

To process an incoming , use the ```handle``` method.
```cpp
loop() {
   ...
   control.handle(command);
}
```

## Command format

To set the I/O pin, send a command with the pin name, an equal sign, and either 1 or 0. For example:
```cpp
LED=1
```

To read the I/O pin, send a command with just the pin name. For example:
```cpp
LED
```


To set the variable, send a command with the variable name, an equal sign, and an integer. For example:
```cpp
myVar=99
```

To read the variable, send a command with just the variable name. For example:
```cpp
myVar
```

You can combine multiple commands by separating commands with a semi-colon:
```cpp
LED=1;myVar=11;LED;
```