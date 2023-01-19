#Modular energy-efficient multi-sensor platform for autonomous ocean drifter

##Introduction

The world has recently been under pressure due to the climate change and the acceler- ated widespread pollution. The aquatic ecosystem is one of the leading contributors to the environmental damage. As a consequence, there has been intensive research towards water surface monitoring approaches to leverage solutions for a sustainable future with environmental protection. Utilizing mobile drifter buoys has been sought out by data scientists to investigate the water currents and enhance the monitoring capabilities. However, this intriguing approach is accompanied with major challenges; existing drifters in the market lack the ability to integrate additional sensor technologies to their main system, and log accurate sensing data, which in return causes overhead costs for the unceasing firmware updates required to meet each scientist expectations.
Therefore, a framework with a significant modularity has been developed using a generic defined protocol and physical interface to flexibly integrate further external sensors to the system and allow transmission of precise data content, and timestamp of each sensory measurement from external microcontroller units to the main unit. The imple- mented platform is based on the OSI model to accomplish software reusability with a structured layered architecture using the combination of event-driven programming paradigm, and finite state machine design. The system is composed of a scheduling system to manage concurrent sensory tasks, and a parameterizable buffer to log generic measurement data into an integrated microSD card to be directly accessed by the end-user.
The main board is particularly integrated with fundamental sensor modules (GPS, IMU, and temperature sensors) to measure the drifter localization, wave height and temperature. The aforementioned components have been further investigated for energy efficiency using introduced low-power and hibernate modes. The platform validation has been eventually carried out using a continuous testing environment against possible external and internal disturbances following the V-model methodical approach to ensure the firmware robustness and reliability.


Maritime research relies heavily on precise measurements collected particularly from the ocean’s surface. In recent years, there has been an increasing interest in drifter buoys (Figure 1) to investigate ocean currents and improve monitoring capabilities. Drifter buoys are mobile instruments which ideally move with the water surrounding them and therefore are almost entirely submerged. Part of their structure is necessarily outside the water to facilitate their localization and data transmission [20]. Multi-sensor system management has been a time-consuming and error-prone task over the past few years due to their necessitating frequent update cycles [3] in addition to the overhead costs for integrating further sensors to the system. Continuous adaptation to the limitless development requirements for massive scale measurement networks with efficient data collection has become a major challenge. Hence, a self-sufficient platform with high modularity has been developed and validated to manage selectable sensors and log multiple measurement data.

<img width="521" alt="buoy" src="https://user-images.githubusercontent.com/60155345/213570985-6c6f60b4-42da-45b6-a320-17f9be36e843.png">

##Drifter Project

The low-cost ocean-based Autonomous Multi Sensor Drifter (AMuSeD) [7] project has been launched with a main objective of environmental data acquisition in multi sensor applications for maritime research. A printed circuit board and electronic components have been designed, developed, and integrated by IMEK-TUHH as shown in Figure 2. The integrated hardware is composed of the following components:
• ATSAMG55MCUisthemainunitforAMuSeDplatformwhichcontrolsthepro- gram flow with integrated sensor modules and external connected devices.
• GPS Erinome_II [25] and IMU ICM20948 [13] sensor modules to acquire the drifter localisation position and wave height respectively along with MCP9808 [16] digital temperature sensor.
• FatFs-based MicroSDCard [11] to store the sensing data content and timestamp.
• USART serialdebuggerinterfaceusedforprintingdebugmessages.
The Atmel-ICE tool is then used for programming the chip with debug capabilities [5]. The aforementioned hardware has been given at the time of firmware implementation for this project.

<img width="514" alt="hardware" src="https://user-images.githubusercontent.com/60155345/213571982-62095beb-c021-4090-b5af-b86c1b4e2dac.png">

##Research Concept

This chapter includes descriptive explanation of the research concepts used in this thesis along with the applied methodical approach and software architecture design.



