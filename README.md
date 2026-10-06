**********EMBEDDED RADAR PROJECT**********


An ultrasonic radar built for 2303ENG Embedded Systems on the TM4C1294NCPDT
LaunchPad, written in Embedded C. An SG90 servo sweeps an SRF05 ultrasonic sensor, 
and a 2.8" ILI9341 LCD shows a live sonar map with a moving sweep line, detected objects 
and a range gauge. It has three modes: IDLE, AUTO (sweeps between adjustable min/max angles) 
and MANUAL (aims with a potentiometer or joystick). It's controlled from a UART terminal or a 
Funduino joystick shield. Every peripheral is interrupt-driven (UART, GPIO buttons, ADC, timer 
input capture, SysTick), and the code is split into separate modules for each part of the system.

 ***Hardware used***
 - **TM4C1294NCPDT LaunchPad (EK-TM4C1294XL):** the microcontroller that runs everything, at 16 MHz.
 - **SG90 servo:** rotates the sensor from 0° to 180°, driven by a 50 Hz PWM signal.
 - **SRF05 ultrasonic sensor:** measures distance to objects by timing the echo pulse.
 - **2.8" ILI9341 LCD (240×320):** shows the mode, live readings, errors, the sonar map and the range gauge, over SPI.
 - **Potentiometer:** sets the servo angle in MANUAL mode.
 - **Funduino Joystick Shield V1.A:** the X axis aims the servo in MANUAL mode, and the buttons trigger every command (modes, angle limits, echo reading, input toggle).
 - **Piezo buzzer:** beeps for mode changes, the end of a sweep, echo readings and errors.
 - **100 Ω resistor:** in series with the buzzer to limit the current drawn from the GPIO pin.
 - **1 kΩ and 2 kΩ resistors:** a voltage divider on the SRF05's echo output, to drop its 5 V signal to about 3.3 V so it's safe for the Tiva's input pin.


***BOARD***

<img width="949" height="673" alt="image" src="https://github.com/user-attachments/assets/ae44d61d-28d9-43b9-9cea-33b9b97d4f27" />



***DISTANCE VERIFICATION***

<img width="766" height="807" alt="Screenshot 2026-10-06 153118" src="https://github.com/user-attachments/assets/8ce884f4-c18f-44ad-ac09-5287e1133cde" />
<img width="760" height="1136" alt="Screenshot 2026-10-06 153104" src="https://github.com/user-attachments/assets/d82df6f2-ad89-4ffe-9c48-90898317b6a8" />
<img width="768" height="908" alt="Screenshot 2026-10-06 153050" src="https://github.com/user-attachments/assets/da924b77-9468-4274-8e06-d04fffa9e2c9" />



***ERRORS***

<img width="769" height="486" alt="Screenshot 2026-10-06 153435" src="https://github.com/user-attachments/assets/3b30dec2-164a-40aa-959e-8b614ed6ad41" />
<img width="742" height="469" alt="Screenshot 2026-10-06 153420" src="https://github.com/user-attachments/assets/f4408786-4e6b-463d-b9c5-a8e00d02f5d5" />
<img width="780" height="496" alt="Screenshot 2026-10-06 153405" src="https://github.com/user-attachments/assets/bf4f02b2-f103-47a8-95c2-c50dbb6f03fb" />




***EXTRA IMAGES***

<img width="590" height="1278" alt="IMG_5103" src="https://github.com/user-attachments/assets/450479bb-3c09-4426-8a2a-d6c007126e5f" />
<img width="590" height="1278" alt="IMG_5106" src="https://github.com/user-attachments/assets/9c4175cc-d992-4432-b309-73ba9eda06ff" />
<img width="590" height="1278" alt="IMG_5111" src="https://github.com/user-attachments/assets/002510ca-d506-4317-947b-0b2472345e15" />
<img width="590" height="1278" alt="IMG_5108" src="https://github.com/user-attachments/assets/a0c94597-ac06-4c52-b289-3e857a75906e" />
<img width="590" height="1278" alt="IMG_5105" src="https://github.com/user-attachments/assets/a5cd8196-b67d-4af8-9d23-9bd2a69be7b5" />
<img width="590" height="1278" alt="IMG_5102" src="https://github.com/user-attachments/assets/e2932b2c-d427-4708-a482-283d289c8e79" />

