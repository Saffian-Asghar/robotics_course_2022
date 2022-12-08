# Circuit

## Version 2.0

### Schematics

![alt_text](lib\images\CircuitDiagramV2.png "Circuit Diagram V2.0")
![alt_text](lib\images\zumoRobot.jpg "Zumo Robot V2.0")

### Parts

```
- Arduino Leonardo.
- Zumo Robot v1.3.
- 1 Ultrasonic sensor US- 015.
- 4 jumping wires.
- 2 4x 1 male pins.
```

## Version 1.0

### Schematics

![alt_text](lib\images\CircuitDiagram.png "Circuit Diagram V1.0")

### Parts

```
- Arduino Leonardo.
- H-bridge.
- 2 DC motors.
- 1 BreadBoard.
- 1 Ultrasonic sensor US- 015.
- AA2400mAh.
- 16 Jumping wires.
```

### Overview

Let's take a brief overview of how everything works:

### Pulse Width Modulation

![alt_text](lib\images\pmw.jpg "PMW")

```
A pulse width modulated output signal is one that has a period and an output “on time”, or
duty cycle, within that period.

The Duty Cycle denotes the percentage of time when the signal is on in 1 period. This
effectively controls or "modulates" the speed of our motors.

In Arduino, PWM is controlled by specified pins marked as ~ on the board. You can use
PWM through analogWrite(PIN, DutyCycle). The Duty Cycle ranges from 0 to 255 since it
is an 8 bit register.

For example, if you want 50 % Duty Cycle you can write analogWrite(PIN, 127) or for 20%
Duty Cycle it would be analogWrite(PIN, 51).
```

### L298N H Bridge

![alt_text](lib\images\H-BridgeL298N.jpg "H-Bridge L298N")

```
Any Arduino you will have, will not be able to provide that much voltage and current for DC
motors (let alone 2 of them) to work properly. You need a driver circuit that can do that job
for you. L298N is great one and easy to find. How it does what it does is too complex for
now, bunch of capacitors and transistors, maybe a study for later. Here's what it looks like:

The left and right blue dual slot headers are for your motors. The blue three slot one is where
you give power to this H Bridge. 12V from the battery pack, 5V from Arduino. The ground
must be common from battery back and your Arduino ENA and ENB are speed controls for
motor 1 and motor 2. You will most probably see a jumper jacket on top of these, these are
just shorting the pins to run at max speed. Don't forget to take them off so that your PWM
works properly! IN1, IN2 and IN3, IN4 control the direction of your motor spin.
```

## Reference
```
https://www.arduino.cc/
```