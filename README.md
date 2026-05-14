# Stormwater Drone

### Overview
Project repository for the Stormwater Drone group of Senior Projects in Elon Engineering Design (SPEED) 2025-2026.  
Group members: Will Borges, Tyberious Brooks, Amalie Keefe, Colby Robbins, Wren Sobolewski.  
Primarily contains code and documentation for the controller implementation, split into two ESP-IDF project 
folders - one for each the drone and the controller.

## Warning to All that Follow:
I recommend against using this code base. Strongly. If you need a list of reasons why, 
then I beg you to ask someone, anyone, for help. Instead, if you inherit this project 
and are pointed to this repository, start your own. Use Arduino. You don't need ESP-IDF. 
ESP-IDF is amazing for a lot of things. You can access all necessary components from it 
in Arduino (IDE or CLI). Heed these warnings. 

### Basic Coding Principles (where I messed up):
- pass arguments, don't globally include a header file (stormwater_config.h)
- not everything needs it's own file. Organize and subdivide, but not *that* much (and not that poorly)
- use logging and error catching, not print statements

### MCU specific advice:
- make use of freertos on esp32. and read the [documentation](https://www.freertos.org/Documentation), at least the Beginner's guide.
- make most things separate tasks so that the cpu shares time appropriately between them
- if nothing else, make logging a separate task - it takes up significant time if left in the main loop.
  - (ref: [freeRTOS](https://www.freertos.org): TaskCreate/Delete, QueueSend/Receive, TaskDelay)
- also read up on interrupts - you can use esp-idf or arduino function calls for this, but they are far 
more efficient than 100% cpu active time and continuous polling.

### Recommended Libraries (beyond arduino core functions):
- [RadioLib](https://github.com/jgromes/RadioLib) for LoRa (or many other RF devices)
- [LiquidCrystal](https://docs.arduino.cc/libraries/liquidcrystal/) or [LiquidCrystal I2C](https://docs.arduino.cc/libraries/liquidcrystal-i2c) for LCD driver

### Also worth noting (but enter at your own risk):
- ExpressLRS (RF protocol common in FPV drones, built on LoRa)
- ESP-NOW (direct ESP32 communication - integrated and low latency, but possibly lower range.)
