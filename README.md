[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0)   [![BuildCI](https://github.com/Pera-Swarm/robot-IR/actions/workflows/build.yml/badge.svg)](https://github.com/Pera-Swarm/robot-IR/actions/workflows/build.yml)

# Robot IR Communication for Pera-Swarm

### Setup Guide - Software

- Download VS Code (https://code.visualstudio.com/)
- Install PlatformIO Extension (https://platformio.org/install/ide?install=vscode)
- Clone this repository and open it using VSCode
- Try to build the project, using PlatformIO Build command 
  - During the first time of build, the necessary softwares and tools will be downloaded automatically 
  

A detailed guide can be found in [here](https://randomnerdtutorials.com/vs-code-platformio-ide-esp32-esp8266-arduino/)

### Setup Guide - Hardware

BOM:
- 1 x Breadboard 
- 1 x ESP32 Dev Board
- 1 x 74LS04 Hex Inverter IC
- 1 x LED (Blue) 
- 1 x IR LED 
- 1 x IR Receiver (138B)
- 2 x 470 Ohm resistors 

##### Please setup the circuit as below:
![image](https://user-images.githubusercontent.com/11540782/236611785-6ddd4041-0eef-49c8-83e6-0a45fb7efba7.png)

##### This is the Schematic Diagram of the circuit:
![image](https://user-images.githubusercontent.com/11540782/236611808-5e39dcd1-902c-4c63-bb75-302126cf9970.png)
