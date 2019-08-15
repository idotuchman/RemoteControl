# RemoteControl
A library for basic pin and variable control over communication links (Serial, WebSockets, etc.).

# How to use
First, include the RemoteControl library and create an instance of the RemoteControl class:
```cpp
#include <RemoteControl.h>
RemoteControl control;
```

To control an I/O pin, first use the ```control``` method in setup() to register the I/O pin:
```cpp
control.pin("LED", LED_BUILTIN, OUTPUT);
```
Here, we are going to control the LED_BUILTIN pin by assigning the name "LED" to the pin. We are also specifying the pin is an output pin. The RemoteControl library will set the pin mode as an output pin.

To set the I/O pin, send a command with the pin name, an equal sign, and either 1 or 0. For example:
```cpp
LED=1
```

To read the I/O pin, send a command with just the pin name. For example:
```cpp
LED
```

To control a variable, first declare the variable and then use the ```control``` method in setup() to register the variable:
```cpp
int myVar;
control.variable("myVar", &myVar);
```

To set the variable, send a command with the variable name, an equal sign, and an integer. For example:
```cpp
myVar=99
```

To read the variable, send a command with just the varible name. For example:
```cpp
myVar
```

You can combine multiple commands by separating commands with a semi-colon:
```cpp
LED=1;myVar=11;LED;
```

To process a command, use the ```handle``` method:
```cpp
control.handle(command);
```