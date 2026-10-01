
An ultrasonic radar built for 2303ENG Embedded Systems on the TM4C1294NCPDT
LaunchPad, written in register-level C. An SG90 servo sweeps an SRF05 ultrasonic sensor, 
and a 2.8" ILI9341 LCD shows a live sonar map with a moving sweep line, detected objects 
and a range gauge. It has three modes: IDLE, AUTO (sweeps between adjustable min/max angles) 
and MANUAL (aims with a potentiometer or joystick). It's controlled from a UART terminal or a 
Funduino joystick shield. Every peripheral is interrupt-driven (UART, GPIO buttons, ADC, timer 
input capture, SysTick), and the code is split into separate modules for each part of the system.

 **Hardware used**
 - **TM4C1294NCPDT LaunchPad (EK-TM4C1294XL):** the microcontroller that runs everything, at 16 MHz.
 - **SG90 servo:** rotates the sensor from 0° to 180°, driven by a 50 Hz PWM signal.
 - **SRF05 ultrasonic sensor:** measures distance to objects by timing the echo pulse.
 - **2.8" ILI9341 LCD (240×320):** shows the mode, live readings, errors, the sonar map and the range gauge, over SPI.
 - **Potentiometer:** sets the servo angle in MANUAL mode.
 - **Funduino Joystick Shield V1.A:** the X axis aims the servo in MANUAL mode, and the buttons trigger every command (modes, angle limits, echo reading, input toggle).
 - **Piezo buzzer:** beeps for mode changes, the end of a sweep, echo readings and errors.
 - **100 Ω resistor:** in series with the buzzer to limit the current drawn from the GPIO pin.
 - **1 kΩ and 2 kΩ resistors:** a voltage divider on the SRF05's echo output, to drop its 5 V signal to about 3.3 V so it's safe for the Tiva's input pin.

**images**
<img width="2160" height="2576" alt="topDown" src="https://github.com/user-attachments/assets/b54888ad-2757-4a8f-8f38-feb1da9e602e" />

<img width="2160" height="2880" alt="tiva_breadboard" src="https://github.com/user-attachments/assets/f250361f-c1fb-4978-b792-cc6095379019" />

<img width="2160" height="2307" alt="auto" src="https://github.com/user-attachments/assets/ced740ee-6276-42b2-b6ab-9ecd91a270ed" />

<img width="2730" height="2160" alt="manual" src="https://github.com/user-attachments/assets/54f61845-da8f-4fb7-befb-1372ed0f98cc" />

<img width="2296" height="2160" alt="idle" src="https://github.com/user-attachments/assets/5e779f63-b01c-44cc-9856-43de5f3ac972" />

<img width="2315" height="2160" alt="error" src="https://github.com/user-attachments/assets/9f6d84fb-4076-4a38-aaf1-846510b6a01e" />

