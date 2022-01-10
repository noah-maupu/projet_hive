# Project : Autonomous hive monitor
Monitoring system for beehive using Ubidots STEM, Microsoft Azure and Arduino microcontroller.

### Features
- Monitoring hive activity
- Getting temperature and humidity inside and outside of the beehive
- Autonomous system thanks to a solar panel recharge



# What is the project ?
## Project presentation
Nowadays, bees are more and more threatened, by predators or even by their environnement (pollution, climate change).
Thus, the use of connected hives is exponentially increasing.
The goal of this project is to assemble a system that monitor beehives activities.
This system has to be autonomous and reliable to allow apiarists to look after their apiary.
It will have a dashboard and able to send alert by mail or SMS, thanks to LPWAN (Low Power Wide Area Network)

## Objectives
This project have some main objectives :

## Constraints
The main constraints of the project are :
### Constraints on the system

- **Being autonomous**

*The system has a battery, that have to recharge thanks to a solar panel.*

- **Low consumption**

*The system has to be energy efficient, to match with the previous point and last the longest time possible before being recharged.*

- **Be able to switch the system on and off**

*It is mandatory to add a button to be able to switch the system on or off, when needed*

- **A Built-In LED has to be lit at the start of the system**

*To show that the system started correctly, the built-in LED has to be lit at the start of the monitoring system*

### Constraints on the data

- **Data must be sent every 10 minutes**

*The data collected must be sent to the Cloud system every 10 minutes*

- **Differents levels of requirements on the data**

*The wanted accuracy differs according to the collected data*

*For example, the level of requirements on the inside temperature is higher than the luminosity one.*

*High level of requirements data must be implemented first, in priority and must be the most watched ones*

*High level of requirements :*

*- Inside temperature, with an accuracy of 0.1°C*

*- Outside temperature, with an accuracy of 0.1°C*

*- Hive's weight, with an accuracy of 100g*

*- The battery state, with an accuracy of 1%*



# Components

## Microcontroller

## Sensors

# Code

