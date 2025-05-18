# Voice-Controlled Smart Lighting System for Smart Home

## 1. Technical Factors
### 1.1. Technical Requirements
- The system must have a response time of less than 10 seconds.
- Detect human presence within a radius of 6 meters (automatic mode).
- The system must recognize the user's voice from a minimum distance of 10 meters.
- The timing deviation in timer mode must be less than 3 seconds.

### 1.2. System Block Diagram
- The user issues commands to control the lights via a laptop connected to WiFi.
- The laptop’s microphone captures the commands as audio, which are then converted
to text using a Speech-to-Text algorithm.
- Once converted into text, the commands are sent to the MQTT server. From the
MQTT server, the commands are transmitted to the ESP32 via WiFi.
- The ESP32 receives the text commands sent from MQTT and forwards them to the
Arduino Nano via UART communication for processing.
- Upon receiving commands from the ESP32, the Arduino Nano analyzes the
instructions to control the light on/off functions according to user requests and activates the
corresponding status indicator lights. The command analysis algorithm of the Arduino
Nano will be detailed in the following section.13
- The operation of the smart voice-controlled timer light system is illustrated in the
block diagram below.
<p align="center">
  <img src="https://github.com/bangnguyen1122/Voice-Controlled-Smart-Lighting-System-for-Smart-Home/blob/master/images/Block-Diagram-of-Voice-Controlled-Smart-Lighting-System-for-Smart-Home.png">
  <br>
  <em>Figure 1: Block Diagram of Voice-Controlled Smart Lighting System for Smart Home.</em>
</p>

### 1.3. Speech Recognition Process
To perform voice recognition and convert speech to text before sending it to MQTT,
we use Python along with the SpeechRecognition, paho.mqtt, and PyAudio libraries.
- First, the program establishes a connection with the MQTT broker to send data.
- Next, the program waits to capture voice input through the microphone (Vietnamese
speech), then converts it into Vietnamese text without diacritics. If no command is detected,
the program...

### 1.4. Algorithm Flowchart
When the system is powered on, it will enter the default mode, which is the automatic
mode (indicated by the red LED). Then, the Arduino initializes two communication
protocols: UART and I2C. UART is used for communication between the Arduino Nano
and the ESP32, while I2C is used for communication between the RTC module and the
Arduino.
After initialization, the Arduino enters a loop waiting to receive commands from the
ESP32. Upon receiving a command from the ESP32, the Arduino checks whether it is a
mode setting command or a control command.14
- If it is a mode setting command, the Arduino sets the light mode, turns on the
corresponding mode indicator LED, and returns to the waiting loop.
- If it is a control command, the Arduino controls the light on/off according to the
command. However, if the light is currently in automatic mode, timer control commands
will not take effect. Therefore, the system must check whether it is in automatic mode
before executing the control commands to ensure proper operation.
Lighting Control Algorithm for Specific Modes:
Automatic mode:
- Arduino will read the value from the sensor, then determine whether a person is
present or not. If a person is detected, the light will be turned on. If no person is detected,
the system will wait for about 10 seconds to verify if someone is actually present. If no
presence is detected after that, the light will be turned off.
Timer Mode:
- Arduino will analyze the command request. If the command is to turn off the light,
it will proceed to turn off the light and return to the loop to wait for the next command.
- If the command is to turn on the light, Arduino will turn on the light, then analyze
the time specified in the command and calculate and set the timer for the RTC module.
Once the RTC module is configured and started, Arduino will continuously read the current
time from the RTC every 0.2 seconds and compare it with the user-defined target time. If
the required time has not yet been reached, the system will continue reading in the next
cycle. When the specified time is reached, the system will turn off the light and return to
the loop to wait for the next command.
In summary, the algorithm of the voice-controlled smart lighting timer system can be
simply illustrated by the following flowchart.
<p align="center">
  <img src="https://github.com/bangnguyen1122/Voice-Controlled-Smart-Lighting-System-for-Smart-Home/blob/master/images/Flowchart-of-Light-Control-Algorithm-for-Automatic-and-Timer-Modes.png">
  <br>
  <em>Figure 1: Flowchart of Light Control Algorithm for Automatic and Timer Modes.</em>
</p>

### 1.5. Circuit Diagram


## 2. Results and Evaluation
### 2.1. Results
For the project “Voice-Controlled Smart Lighting System for Smart Home”, the
team successfully met the set requirements, specifically:
- The light operates according to the designed functions.
- The system can recognize voice commands and control the lights as requested by
the user via WiFi.
- The system responds to commands with low latency (under 5 seconds).

### 2.2. Evaluation
Through this project, the team acquired new knowledge, specifically:
- Successfully implemented data transmission via WiFi using the ESP32 module.
- Designed and built a simple IoT system.
- Learned how to develop a voice recognition program using Python and how to
program the Arduino.
- Gained understanding of data transmission between MQTT and ESP32.

## 3. References
[1] Speech Recognition with Python: https://reintech.io/blog/how-to-create-a-speechrecognition-system-with-python <br>
[2] Using MQTT in Python with Paho Client: https://www.emqx.com/en/blog/how-to-usemqtt-in-python
