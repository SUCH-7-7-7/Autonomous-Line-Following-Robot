# Autonomous-Line-Following-Robot
A multifunctional autonomous robot using an ATmega2560 microcontroller programmed in Arduino IDE. The system integrates line-following and obstacle-avoidance capabilities, with multiple operating modes selectable via a rotary switch.
In this project, using the Elisa-3 robot with the ATMega2560 microcontroller, we solved challenges we were faced with using sensor integration and logic-based solutions. Each challenge provides instructions along with a template, involving line following, line avoidance and obstacle avoidance. Each program aims to find the most efficient and reliable way of carrying out each challenge, needing to be flexible in order to function consistently on each run. We made the entire program interface-friendly by implementing a switch statement that changes based on the Rotary Switch on the bottom of the robot.
# Challenges

Challenge 1 – Line Following: Follow a black oval track using ground sensors and motor steering corrections.

Challenge 2 – Line Following on an Irregular Track: Navigate tighter corners using adjusted motor speeds and sensor thresholds.

Challenge 3 – Boundary Avoidance: Remain inside a black-bordered area by detecting edges, reversing, and randomly turning away.

Challenge 4 – Line and Corridor Following: Navigate obstacle-filled junctions using proximity sensor readings before transitioning to corridor navigation.

# Key Features

Ground and infrared proximity sensor integration.

PWM-based motor control and directional steering.

Threshold-based sensor detection and conditional decision-making.

Randomised turning behaviour for boundary avoidance.

Rotary switch interface for selecting between all four challenges.

Modular code structure with separate functions for each challenge and movement behaviour.

# Technologies

Microcontroller: ATmega2560

Robot platform: Elisa-3

Programming language: C/C++ (Arduino environment)

Concepts: Embedded C, analogue sensor interfacing, digital I/O, PWM motor control, and autonomous robotics.

# Results

Challenges 1–3 were completed successfully following testing and calibration. Challenge 4 demonstrated successful navigation during development but may observe inconsistencies due to proximity sensor sensitivity, ambient lighting, and junction detection reliability.

# Learning Outcomes

This project developed practical experience in embedded programming, sensor integration, hardware debugging, motor control, and iterative testing of autonomous robotic systems.
