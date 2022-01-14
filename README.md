# Project : Autonomous hive monitor
Monitoring system for beehive using Ubidots STEM, Microsoft Azure and Arduino microcontroller.
All the codes and components explanations are on the Hackster.io project page, the link is at the bottom of the page.

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

## Main steps of the project

- Research and discussion between members on how to create this monitoring system

- Familiarization with the chosen sensors and how to code using them

- Creation of a prototype

- Creation of a PCB Circuit to implement the prototype on it

- Optimization 

- Deployment of the system on an apiary


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

**High level of requirements :**

*- Inside temperature, with an accuracy of 0.1°C*

*- Outside temperature, with an accuracy of 0.1°C*

*- Hive's weight, with an accuracy of 100g*

*- The battery state, with an accuracy of 1%*


**Moderate level of requirements :**

*- Have several temperature sensors to be more precise*

*- Humidity sensors inside and outside the beehive*


**Low level of requirements :**

*- Environnement luminosity*

*- Daylight time in a day*


# Getting Started

Clone the repo on your machine  
`$ git clone https://github.com/noah-maupu/projet_hive.git`

In the Arduino IDE, go to ***File -> Open***, open ***"OpenRucheSoftware.ino"***

**WARNING !**  
- Check that the COM port selected is the port where your Arduino is detected  
To do so, go to ***Tools -> Port***, select the port with the name of your microcontroller.

- Check that you are using the good setting for your microcontroller  
To do so, go to ***Tools -> Card Type*** and select the ***"Arduino Nano BLE Sense 33"***

Upload your code in your Arduino Nano BLE Sense 33 microcontroller, by clicking on ***Upload***, the arrow icon next to the checkmark on the top-left of the screen.


How to download the Arduino IDE ?  
https://www.arduino.cc/en/software 
 
Download the package for your version of your OS (Windows, MacOS or Linux and 32-bits or 64-bits), a tutorial is available on their site.  



# External links

[Link to the Hackster.io page of the project] (https://www.hackster.io/noahmaupu/hive-monitoring-system-05767d)


