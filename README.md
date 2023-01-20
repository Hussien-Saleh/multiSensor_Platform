## Modular energy-efficient multi-sensor platform for autonomous ocean drifter

## Introduction

The world has recently been under pressure due to the climate change and the accelerated widespread pollution. The aquatic ecosystem is one of the leading contributors to the environmental damage. As a consequence, there has been intensive research towards water surface monitoring approaches to leverage solutions for a sustainable future with environmental protection. Utilizing mobile drifter buoys has been sought out by data scientists to investigate the water currents and enhance the monitoring capabilities. However, this intriguing approach is accompanied with major challenges; existing drifters in the market lack the ability to integrate additional sensor technologies to their main system, and log accurate sensing data, which in return causes overhead costs for the unceasing firmware updates required to meet each scientist expectations. 
Multi-sensor system management has been a time-consuming and error-prone task over the past few years due to their necessitating frequent update cycles in addition to the overhead costs for integrating further sensors to the system. Continuous adaptation to the limitless development requirements for massive scale measurement networks with efficient data collection has become a major challenge.

Therefore, a framework with a significant modularity has been developed using a generic defined protocol and physical interface to flexibly integrate further external sensors to the system and allow transmission of precise data content, and timestamp of each sensory measurement from external microcontroller units to the main unit. The implemented platform is based on the OSI model to accomplish software reusability with a structured layered architecture using the combination of event-driven programming paradigm, and finite state machine design. The system is composed of a scheduling system to manage concurrent sensory tasks, and a parameterizable buffer to log generic measurement data into an integrated microSD card to be directly accessed by the end-user.

The main board is particularly integrated with fundamental sensor modules (GPS, IMU, and temperature sensors) to measure the drifter localization, wave height and temperature. The aforementioned components have been further investigated for energy efficiency using introduced low-power and hibernate modes. The platform validation has been eventually carried out using a continuous testing environment against possible external and internal disturbances following the V-model methodical approach to ensure the firmware robustness and reliability.

<p align="center">
<img width="521" alt="buoy" src="https://user-images.githubusercontent.com/60155345/213570985-6c6f60b4-42da-45b6-a320-17f9be36e843.png">
</p>
<div align="center">
  Fig.1 AMuSeD Multi-sensor Ocean Drifter (tuhh-imek)
</div>

----

## Concept and System Design

### *HW Components*

The low-cost ocean-based Autonomous Multi Sensor Drifter (AMuSeD) project has been launched with a main objective of environmental data acquisition in multi sensor applications for maritime research. A printed circuit board and electronic components have been designed, developed, and integrated by IMEK-TUHH as shown in Figure 2. The integrated hardware is composed of the following components:

• **ATSAMG55 MCU** : main unit for the platform which controls the program flow with integrated sensor modules and external connected devices.

• **GPS Erinome_II** and **IMU ICM20948** sensor modules to acquire the drifter localisation position and wave height respectively along with **MCP9808 digital temperature sensor**.

• **FatFs-based MicroSDCard** to store the sensing data content and timestamp.

• USART serial debugger interface used for printing debug messages.

<p align="center">
<img width="514" alt="hardware" src="https://user-images.githubusercontent.com/60155345/213571982-62095beb-c021-4090-b5af-b86c1b4e2dac.png"></p>
<div align="center">
  Fig. 2: AMuSeD PCB Hardware
</div>

### *Research Concept*

The platform modularity is achieved by defining an extension interface with the given AMuSeD main unit hardware for customized external sensor modules as highlighted in Figure 3. In addition, the communication protocol between the AMuSeD master unit and an external slave unit with generic sensor modules is illustrated.

<p align="center">
<img width="676" alt="modular" src="https://user-images.githubusercontent.com/60155345/213572792-aa249be4-7e40-43ea-88e1-b416f264a96b.png">

<div align="center">
Fig. 3: External Device Interfacing
</div>

<br />

To achieve the external communication with generic devices, the master unit enables the physical interface and interconnects with the activated slave unit through request and response messages in a predefined standardized protocol. Moreover, the protocol allows the selection of specified external sensors and the exchange of sensory data from the slave unit to the master unit in a predefined format without any wiring connections between the master unit and the available sensor modules connected with the slave unit. In the end of the process, the measurement data is retrieved by the master main unit where it is converted into human-readable information before being saved on the integrated microSD card.
The master unit can be used as a key component for further research studies with only adapting the firmware of slave sensor unit according to the customer needs. For effective project outcome, the development is therefore structured into five main domains as shown in Figure 4.

<p align="center">
<img width="531" alt="concept" src="https://user-images.githubusercontent.com/60155345/213573418-8df0f619-80be-4d12-9522-097613f65c09.png">

<div align="center">
Fig. 4: Domains of AMuSeD Modular platform
</div>

<br />

To fulfill the requirements of this project, a proper software has been implemented and validated to include modular functionality for drifter applications. The platform focuses on the communication with slave microcontroller units with no prior knowledge of the external integrated sensors. Additionally, the platform has a selection feature of individual sensors at predefined time horizons ensuring a reduction in the energy consumption. The domains are described as follows:

- **Data Acquisition** includes the drifter localization and sensory measurementtasks.
+ **Data Processing** enables the sensor selection process and collection of measurement data at predefined time intervals.
- **Data Transmission** describes the storage of sensory data along with measurement timestamp to be retrieved world wide.
+ **Energy Efficiency** ensures a reduction in energy consumption while maintaining high measurement accuracy.
- **Fault Tolerance** improves the system robustness to acquire accurate sensory data in events of partial failures.

### *Work Scope*

The work scope includes the development of generic communication protocol and physical interface which allows the transmission of generic type and size of sensory data. Additionally, a bare-metal embedded firmware is developed on the main unit for interfacing with sensors integrated on external micro- controller units; the firmware is a combination of a finite-state machine design and event-driven programming paradigm. 

A parameterizable logic buffer for logging sensory data sets in a human-readable format is as well implemented along with a scheduling system to manage the concurrent sensory tasks and power modes on the platform. besides, the initial firmware for the integrated sensor modules is reviewed and optimized for better performance and energy efficiency. 

A reduction in the power consumption of the system is evaluated through the introduction of low-power modes for the integrated sensor modules in addition to a criteria-based analysis of the power saving methodology. Ultimately, the software validation is achieved using a continuous testing environment to ensure the software robustness and reliability.

### *Methodical Approach*

The V-model SDLC -illustrated in Figure 5- has been applied for the modular platform design. The approach is divided into the following phases:

1. Product requirements analysis for generic interfacing between microcontrollers and external sensors selection process.
2. High-level software design for the platform architecture which is composed of a list of modules along with their brief functionality and interfacing dependencies.
3. Low-level hardware-related software design. In this phase,the system is divided into modules and the detailed design for each module is specified.
4. Hardware development of electronic circuit interface between microcontrollers and sensor modules.
5. Software implementation phase where the code goes through several reviews for optimized performance.
6. Unit testing approach to ensure the functionality of modules and eliminate bugs at code level.
7. Integration testing approach to ensure functional compatibility between the integrated modules.
8. System testing methodology to ensure that application expectations are met.

<p align="center">
<img width="547" alt="vcycle" src="https://user-images.githubusercontent.com/60155345/213573700-aa1684ae-7578-4830-9fa8-058c797402e5.png">

<div align="center">
Fig.5: V-model SDLC
</div>
<br />

### *Software Architecture Design*

<p align="center">
<img width="653" alt="layers" src="https://user-images.githubusercontent.com/60155345/213573812-4a724ace-5ee0-4c70-94d0-1336678b7b8c.png">

<div align="center">
Fig. 6: AMuSeD Layered Software Architecture
</div>
<br />

The software framework is a layered architecture -illustrated in Figure 6- with endeavour to achieve abstraction from the hardware components in addition to software modularity and reusability. 

**The software architecture consists of five layers:** 

- Physical Layer where the master and slave MCUs, sensor modules, and other hardware components are integrated.
+ Microcontroller Abstraction Layer -MCAL- which utilizes drivers to directly access the MCU peripheral modules and external devices.
- Electronic Abstraction Layer -ECAL- used to provide API to interface the external device drivers with the MCAL peripherals with no dependencies on the MCU.
+ Services Layer which provides functional services for the main application; for instance scheduling and handling of sensory measurement tasks, power management, fault tolerance and data management.
- Application Layer which is the main controller for the end-user application and functional requirements specification.

### *Generic Platform Protocol*

This section defines a physical interface and generic communication protocol between the AMuSeD main board and external sensor units. The main objective of the protocol is to provide a standardized interface which enables the connection of further sensor technologies to the platform without the need for firmware updates on the main unit. The protocol has no dependencies on hardware and is based on the OSI model which defines the entire communication aspects covered in this project.
The interface framework defines the communication between external sensor units and the AMuSeD main platform. It has one operation mode where the generic external unit provides the measurement data upon request to the main unit as shown in Figure 7.

<p align="center">
<img width="489" alt="usecase" src="https://user-images.githubusercontent.com/60155345/213574070-55970c22-0623-4291-bb80-37d03e94fcc0.png">

<div align="center">
Fig.7: Operation Mode
</div>
<br />

From architectural point of view, the roles are defined as follows:

- AMuSeD main unit is the master controller of the platform which incorporates low-level standardized protocols for handling the communication with the inte- grated and external slave devices; it also allows the variable connection to the specified slave devices along with supplying them with the electrical energy to operate. The master unit manages the measurement tasks, processes and stores the acquired sensing data. Furthermore, it tolerates the software in case of partial event failures and introduces the low-power modes for integrated sensor modules.

+ External sensor unit is the slave device of the platform which acts as the counterpart to the AMuSeD board. It handles the communication requests from the master unit and processes the measured sensors data in a specified format as de- fined in the generic protocol. The slave unit is composed of one or more external sensor modules and is programmed to work with the sensor selected by the master unit. In addition, it supports the interfaces with external sensor modules and handles the communication with them using low-level standardized protocols.

### *Use Cases*

A single external unit is the main use case scenario of the platform where it is connected with the AMuSeD main unit providing multiple sensory measurements. However, the modular framework makes it feasible to connect multiple external units to a single port of the main unit with enabling a selection criteria based on corresponding slave addresses using a standardized protocol. The architecture is presented in Figure 8.

<p align="center">
<img width="577" alt="architecture" src="https://user-images.githubusercontent.com/60155345/213574238-c719a3e2-df11-460c-959d-9373269da1fc.png">

<div align="center">
Fig.8: External Interfacing Architecture
</div>
<br />

### *Scheduling System*

Embedded systems are generally characterized by their limited hardware resources. Optimization of these resources is always required for better performance and efficient energy consumption. For instance, using multiple hardware timers in a project is unpromising. Moreover, the execution of a time-consuming task inside interrupt service routines -ISRs- blocks other interrupts from execution which is considered a setback in the system design. Therefore, a non-blocking task scheduler mounted on one hardware timer is implemented to overcome these challenges.
The scheduler enables the execution of multiple concurrent tasks at predefined time periods in a synchronous context. It periodically polls for executable tasks to provide a synchronous execution for the platform. During the polling process, the scheduler executes the ready tasks and deletes the non-periodic ones after their execution is completed. The complete scheduling process is illustrated in Figure 9.

<p align="center">
<img width="533" alt="scheduler" src="https://user-images.githubusercontent.com/60155345/213574369-4756b29d-729e-4006-b37b-3b006b7c41cf.png">

<div align="center">
Fig.9: Scheduling System
</div>
<br />

Likewise, the scheduling system utilizes low-power modes for the integrated GPS, IMU, and temperature sensor modules on the main unit to reduce the overall energy consumption and enhance the system performance. The process is initiated by activating a specific sensor for a defined period of time; once the measurement is completed, the selected sensor is forced into low-power sleep mode for an equivalent time interval.

### *Data Processing*

Data processing is a crucial element in developing embedded systems. The purpose of processing is to manipulate and convert items of collected sensing raw data into meaningful human-readable information to be stored and accessed by the end user as illustrated in Figure 10.

<p align="center">
<img width="565" alt="dataprocessing" src="https://user-images.githubusercontent.com/60155345/213574467-624c411a-3f25-42cf-b3a9-205c8a2e3a8b.png">

<div align="center">
Fig.10: Sensory Data Processing
</div>
<br />

Sensing data is retrieved periodically when the machine is running. To facilitate the data management, a generic data logger -FIFO ring buffer- is developed to interpret the sensing data and process them into a MicroSD card. The logger is used to queue all types of data with different allocated sizes. When the buffer is full, the most outdated data is discarded and overwritten by the latest one. Storing the processed data is executed at a higher rate before the buffer becomes full to avoid permanent data loss.

### *Event-driven State Machine Design*

The concept of events is a critical point for developing embedded systems since they are predominately reactive by nature and generally designed to interact with the environment through external events. The event can be in the form of a pin logic level change, an interrupt or in this project case, a message packet received from another system through a defined I2C interface.
Identifying the proper reaction to be executed in response to an occurred event is a crucial element in developing reactive systems. The reactions are mainly dependent on the executed event and the state context defined inside the process along with the sequence of prior events. In order to eliminate perplexing code with disproportionate number of conditional logic branches (if-else statements), the software utilizes the finite state machine design to effectively handle concurrent and asynchronous events and reduce the CPU cycle consumption and the memory footprint for generated code.

<p align="center">
<img width="475" alt="statexplained" src="https://user-images.githubusercontent.com/60155345/213574615-b456d44b-0e82-4108-82cb-7d2c2f719e85.png">

<div align="center">
Fig.11: Event-driven FSM
</div>
<br />

The FSM is composed of a finite number of states with entry and exit points along with state transitions based on input events and conditions. Every state machine has a default state which must be initialized before the program execution. The triggered event evaluates the configured conditions. Once the condition rules are realized, the current state in the machine transits to another state depending on the guard conditions. In order to intertwine the state machine design with the event-driven paradigm, an event dispatching mechanism along with an execution context are required to map the captured events to the respective handler which executes the tasks associated with the dispatched event in addition to a queue structure to process the incoming events.
As illustrated in Figure 11, the incoming event is initially stored inside an event queue buffer. Subsequently, a loop mechanism pops the event out of the queue and dispatches it to the state machine to be handled. The current state then transits to the succeeding one based on the dispatched event.
In this project, the event loop processor and queue structure are replaced by the developed scheduling system which is based on a linked-list structure format.

----

## Detail Specification and Implementation

### *Protocol Specification*

This section describes the protocol used for master-slave communication. The protocol is based on the OSI model which consists of seven layers as depicted in the following table. Network and Session layers are not applied in this project scope; detailed descriptions of the implemented layers are presented in the consecutive subsections.

<p align="center">
<img width="664" alt="Screenshot 2023-01-19 at 23 36 54" src="https://user-images.githubusercontent.com/60155345/213578391-0e2106cc-ee04-4492-8bf0-ad74227fd098.png">

<br />

#### Physical Layer
  
The first layer of OSI model is composed of pin assignments for standardized low-level I2C protocol and the relative electric interface parameters.

<p align="center">
<img width="652" alt="Screenshot 2023-01-19 at 23 37 09" src="https://user-images.githubusercontent.com/60155345/213578499-e27f210e-6765-43db-bc88-8ccba5beda5a.png">
  
<p align="center">
<img width="652" alt="Screenshot 2023-01-19 at 23 37 22" src="https://user-images.githubusercontent.com/60155345/213578515-5ca90865-0e69-443e-8c68-0bca4931e73d.png">

#### Data Link Layer

The second layer of OSI model which establishes and terminates the node-to-node communication between master and slave units. I2C protocol is incorporated into this layer.

<p align="center">
<img width="651" alt="Screenshot 2023-01-19 at 23 37 37" src="https://user-images.githubusercontent.com/60155345/213578592-43f4d48c-1885-422e-b777-39b5954778c9.png">

<br />

The communication between master and slave units is executed in consecutive opera- tions of request (I2C write) and response (I2C read). The master initiates the process with a request message to the slave using I2C write operation followed by I2C read operation to retrieve the slave response message. The data transmitted during I2C operations (Figure 12) varies with the request and response commands. Message packets are predefined at the Presentation Layer to facilitate the master-slave communication. The maximum number of transmitted data bytes during the I2C operations is restricted to 255 bytes which is identified by the interconnected units. The restriction can be adjusted according to the data transfer requirements.

<p align="center">
<img width="511" alt="i2coperations" src="https://user-images.githubusercontent.com/60155345/213578645-cabb185d-c50a-4ae0-b715-b61bb5991489.png">

<div align="center">
Fig.12: I2C Write and Read Operations
</div>
<br />

The external slave unit waits for initiated communication commands requested by the AMuSeD main unit. Each request is answered immediately by the slave unit with a response for confirming the command receipt.
Only available sensory data is acquired from the slave unit. In case of data unavailability, the slave unit answers with a response containing an error code. To tolerate this faulty scenario and ensure the data availability, the maximum measuring time for each sensor is queried in order to apply a delay functionality before starting the retrieval process. General overview of the request-response sequence is presented in Figure 13.

<p align="center">
<img width="555" alt="sequence" src="https://user-images.githubusercontent.com/60155345/213577479-6c5a2bfa-d542-42e1-8025-282f54252e05.png">

<div align="center">
Fig.13: Request Response Sequence
</div>
<br />

#### Network Layer

The third layer of OSI model which determines the physical path for data transmission. The Network Layer is not included in the project since the communication between master and slave units happens over the same network. However, the framework can be extended to define a Network Layer using a multiplexer which handles individual point to point connections with each multiplexed slave unit, switches between their physical slave addresses and reports the measurements back to the master unit.

#### Transport Layer

In this layer, communication request (I2C Write) and response (I2C Read) messages are defined in packet frames. The I2C bit fields (ACK/NACK, Start/Repeated Start, and Stop) are not included to simplify the process. A similar frame structure is used for both request and response messages. The frame length and message are particularly defined according to the packet type.

<p align="center">
<img width="656" alt="Screenshot 2023-01-19 at 23 38 12" src="https://user-images.githubusercontent.com/60155345/213578918-84739152-f5c7-4d44-a9cf-e370ec6a2a47.png">
<br />

The next table describes the defined frame structure for request and response messages. The maximum number of bytes during a single message transmission is restricted to 255.
  
<p align="center">
<img width="656" alt="Screenshot 2023-01-19 at 23 38 26" src="https://user-images.githubusercontent.com/60155345/213578954-3ef6d3fc-4b3c-4ceb-b496-4f1558593f9c.png">
<br />

Communication must start with a request followed by a response. The request is only initiated by master using I2C Write, whereas slave responds back using I2C Read. The payload message frame is specified by individual packet commands in hexadecimal format representation.

 <p align="center">
<img width="656" alt="Screenshot 2023-01-19 at 23 38 43" src="https://user-images.githubusercontent.com/60155345/213579015-6c1e74af-eaef-41ad-92a5-f5f2443667aa.png">
<br />

The generalized structured description of request and response messages are listed below. The request message is typically composed of I2C standard bit fields, the message length, the packet command value, and the payload data content.

<p align="center">
<img width="656" alt="Screenshot 2023-01-19 at 23 38 56" src="https://user-images.githubusercontent.com/60155345/213579111-1df3812a-b534-40ee-83ce-6670352afe68.png">
<br />

The response message has a similar frame format to the request message except it includes a return status code and an additional data content from the slave unit.
  
<p align="center">
<img width="658" alt="Screenshot 2023-01-19 at 23 39 09" src="https://user-images.githubusercontent.com/60155345/213579147-33122631-e00d-42ad-9af9-fae4c96c37bd.png">
<br />

#### Session Layer
  
The Session Layer controls the simultaneous communication dialogue between two nodes. The session is maintained until the data transmission is completed and then promptly terminated. Session layer is not used in the project since the communication is executed as a single transfer of request and response messages.

#### Presentation Layer
  
In this layer, lists of predefined packet commands and return codes are described. The return codes transferred from the external slave unit are defined below.
 
 <p align="center"> 
<img width="658" alt="Screenshot 2023-01-19 at 23 39 32" src="https://user-images.githubusercontent.com/60155345/213579250-04a031a9-256a-42a4-87f3-3f1bd189c478.png">
<br />

Request commands are represented in a predefined format on both master and slave sides in order to facilitate the communication flow. Each time the master sends a request including command ID to the slave, the slave responds back with the corresponding response along with the transmitted message and return status code. The defined minimum and maximum command values are reserved for debugging purposes.
  
 <p align="center"> 
<img width="658" alt="Screenshot 2023-01-19 at 23 40 03" src="https://user-images.githubusercontent.com/60155345/213579317-68061f31-e566-405a-950e-09a02bc6d40f.png">
<br />

Similarly, the response commands are predefined on both master and slave sides with two reserved minimum and maximum values for debugging.

 <p align="center"> 
<img width="658" alt="Screenshot 2023-01-19 at 23 40 32" src="https://user-images.githubusercontent.com/60155345/213579363-484203c0-6734-4081-84d5-b4eb743586bc.png">
<br />

A list of measurement types for multiple sensor candidates is explained as additional data content for the AMuSeD unit to identify the sensor functionality. The list can be extended for further external sensor technologies. Payload messages vary according to the request and response commands. 

<p align="center"> 
<img width="658" alt="Screenshot 2023-01-19 at 23 40 46" src="https://user-images.githubusercontent.com/60155345/213579409-2897e2eb-a886-4e7a-a89c-6df3286c7d41.png">
<br />

#### Application Layer
  
The topmost layer of the OSI model links the user interface to lower-level layers. The layer defines the functionality of the main process and provides services to control the program flow and data management. The flow of messages is rigorously described in order to guarantee the interoperability between the AMuSeD unit and external units.
  
In this layer, an event-driven finite state machine model is developed as a generic event processor to control the communication flow between the master and slave units. The operations regarding the state machine follow a "run-to-completion" semantic and must not be interrupted by another operation on the FSM. This is achieved by running the FSM completely within task context.
  
To store and execute the incoming events which drive the state machine, a linked-list task scheduler is implemented. Additionally, for processing sensory measurement data sets retrieved from the slave unit, a FIFO ring buffer is designed to avoid data loss and log the available data into a MicroSD card to be accessed at a later point in time. General overview of the event-driven state machine framework is depicted in (Figure 14).

<p align="center">
<img width="535" alt="eventframework" src="https://user-images.githubusercontent.com/60155345/213579537-f17c88a3-8047-45d7-9c75-e4801b656c32.png">

<div align="center">
Fig.14: Event-driven state-machine framework
</div>
<br /> 

#### *Task Scheduler*

The scheduled tasks are organized as a singly linked list and represented by a data structure. The structure parameters must be initialized to define and schedule sensory tasks. A task is scheduled to execute after a predefined time interval described by (time_expire). The milliseconds are counted down until the time expires and the sensory task is executed. Besides, two parameters can be passed to the scheduled sensory task function. 

There are two options for scheduling tasks:

- Single execution (time_after_fire=0). Task is removed after execution. 
+ Periodic execution (time_after_fire>0). Task is repeated periodically.

A predefined task descriptor can be added to the scheduler using scheduler_add_task_( ) API function call. Likewise, added tasks can be removed from the scheduler with scheduler_remove_task_( ). The scheduler initializes the hardware timer using scheduler_init_( ). The timer interrupt registers a callback function for updating the scheduler every 1 ms by decreasing (time_expire) of all scheduled tasks by 1 ms and mark expired tasks for execution. The API function scheduler_run_( ) runs the scheduler in a superloop. It executes the marked executable task and removes non-periodic tasks from the list. The system running the scheduler must allocate memory to store task descriptions since the scheduler does not reserve the memory itself.

#### *Data Logger Ring Buffer*

The data logger ring buffer is defined by a structure. The buffer is represented by a head where new data is written to and a tail which contains the most outdated initial data to be accessed. Both the head and tail are declared as volatile since they are accessed from the application and interrupt contexts. Both are incremented and automatically wrapped around when overflow occurs.

The buffer is a statically allocated array of bytes which is generic for storing any type of data. To accomplish that, the reserved memory size is provided by the end-user defining the number of stored items in addition to the size of each item.
An offset is calculated depending on the items number and the allocated item size to determine the index location where data is accessed and stored. The number of items is restricted to be an even power of two for code optimization purposes when the offset is calculated.

In the scope of this project, the maximum number of data sets to be stored inside the buffer is defined to be 16 sets with an option to dequeue the data from the buffer depending on the percentage of the maximum number of stored items or the sensory data sets in this case. Four selectable options are defined: one percent, twenty five percent, fifty percent, and seventy five percent. The buffer is implemented to store sets of sensory data. The data set stored in the logger is restricted to a total of twenty three bytes organized as follows: the sensor ID is stored in one byte, eight bytes for allocating the timestamp, each of the measurement type and size has one byte storage, whereas a maximum of twelve byte is allocated for the measurement data. 

The data storage can be flexibly adapted in the firmware according to the end-user requirements taking into consideration the maximum transferable bytes in a single transmission of the I2C operations. Two helper functions is_buffer_empty_( ) and is_buffer_full_( ) are used to determine if the buffer is empty or full for further processing operations. The head and tail are never wrapped around at the buffer end; their values are never the same in case of logged data inside the buffer. The buffer is only full when the difference between the head and tail is equal to the total number of allocated items, whereas the buffer becomes empty if the difference between them is zero. enqueue_data_( ) and dequeue_data_( ) APIs are used to push and remove data respectively. buffer_init_( ) is used as a handling constructor to initialize the ring buffer with the number of allocated items along with the item size and the storage array.

A general illustration for processing sensory data and the relation between head and tail of the buffer are presented in Fig. 15. The buffer interprets the raw data into meaningful information to be retrieved by the end-user. The retrieved data is listed as follows:

- Measurement content {type,size,timestamp,data}. 
+ Corresponding sensor ID.

<p align="center">
<img width="474" alt="ringbufferprocess" src="https://user-images.githubusercontent.com/60155345/213579812-1daba815-012b-4e47-a72e-07f1f2e99eff.png">

<div align="center">
Fig.15: Sensing Data Processing
</div>

<br />

#### *Event-driven FSM Framework*

In this section, FSM based framework using event-driven paradigm is proposed. fsm_init_( ) API function is used to setup the state machine with the initial state. Configuration of the scheduled tasks with their defined parameters is executed by config_fsm_scheduled_tasks_( ).
fsm_dispatch_( ) is the dispatcher of events to state machine. A state machine for the active object -slave MCU- is constructed using fsm_mcu_ctor( ) API.The FSM has been implemented using function pointers. activeObjectFsmClass_ structure is defined to include the current state and the object activated by the state machine. A state is described by a pointer to an event handler function with two passed parameters; a pointer to the active FSM, and the executed event.
A TRANSITION macro is defined to flexibly transit between states. Whenever the macro name is encountered by the compiler, it replaces the name with the macro definition.
Possible event signals which drive the state machine and trigger state changes are implemented with a defined entry action to the new state and an exit action from the current state. eventDispatcherClass_ structure is used to identify the event signal type. A UML statechart describing the event-driven FSM functionality to control external slave units is illustrated in Figure 16.

<p align="center">
<img width="590" alt="fsm" src="https://user-images.githubusercontent.com/60155345/213579912-5ff58933-bd2e-4fbc-9a51-6ec61d735002.png">

<div align="center">
Fig.16: UML statechart for event-driven FSM model
</div>

<br />

**The following paragraph describes the framework in natural language:**

The described state machine handles all the states of external slave unit. initially, the state machine is launched at "slave unit startup" state where a timeout delay is defined to wait for the startup completion of slave unit. After the startup is completed and the communication between master and slave is established and activated, the state machine transits to "available sensors check" state where the number of connected sensors on the external slave unit is firstly checked. If the number of available sensors is greater than zero, a structure handler for each available sensor is constructed to manage its predefined parameters and measurement status flags. Once the construction is completed, the available sensors are activated. When the activation is successful, the state machine checks the maximum time each sensor takes to complete one measurement to ensure that the measurement data will be ready at the time of retrieval after triggering the sensor. If an error occurs in this state, the state machine transits into "error" state where a software reset of the slave unit takes place. Once the reset timeout is completed, the state machine restarts from the initial state. If the check for maximum measuring time is finalized with no errors, the state machines enters "slave unit ready to measure" state where the unit is now ready to start the measuring process for selected sensors. Once the slave unit confirms that it is ready to measure, the state machine moves to "slave unit sensors measuring" state where concurrent sensor tasks are executed peri- odically. Each selected sensor goes into its own processing cycle starting from triggering the sensor, followed by a delay determined by the maximum measuring time for the sensor. Afterwards, a check for the measurement status is carried out. Measurement data is then retrieved in case the data is ready. Each sensor has a predefined counter which decides the maximum number of measurements each sensor takes before setting the slave unit into sleep mode. When the measurement tasks for each sensor are finished, the state machine transits into "slave unit sleep" where the slave unit is put into sleep mode for a predefined duration. After the sleep mode is brought to an end, the machine restarts the measuring process and transits one again back to "slave unit sensors measuring state". A state timeout event with a defined time frame is included in every state in case of task unresponsiveness and partial event failures.


#### *Introduction to Low-power Modes*

Low-power modes are utilized for integrated sensor modules (GPS, IMU and temperature sensors) on the main unit to reduce the overall energy consumption and enhance the system performance as presented in Figure 17.

<p align="center">
<img width="566" alt="lowpowermodes" src="https://user-images.githubusercontent.com/60155345/213581002-08e9d70e-2df3-4509-96ff-25ece5b4af1c.png">

<div align="center">
Fig.17: Scheduling Low-power Modes
</div>

<br />

The GPS module consumes much more power compared to other sensor modules. The time needed by the module to accomplish the very first valid location is the most power consuming task. Hence, starting up the module is configured for optimized performance. Moreover, the implemented scheduler exploits the introduced hibernate and low-power modes to power down the modules in case of inactive status and not engaging in the measuring process while retaining their states. Periodic measurement time for each sensor module is predefined before execution time. The process begins by setting up the module into active mode to trigger a measurement. Once the data is retrieved, the module is powered down until the next cycle.


#### *Software Modular Structure*

<p align="center">
<img width="578" alt="modularfolders" src="https://user-images.githubusercontent.com/60155345/213581084-c9af0bac-f82e-4eb9-907f-049213c354b6.png">

<div align="center">
Fig.18: AMuSeD Software Modular Structure
</div>
<br />


The software framework is decomposed into structured modules (Figure 18) with standardized interfaces to facilitate the software reusability and flexible elimination of bug errors. The project structure is specified as follows: main application which initializes the system parameters and sensory tasks, configuration file adjusted to the customer requirements, serial debug folder defines USART standardized APIs for debugging purposes, scheduler for managing sensory tasks and system processes, data processing APIs using parameterizable circular buffer, MCAL folder is composed of SAMG55 hardware abstraction layers, ECAL folder consists of modules for GPS, IMU, SD card and temperature sensor, and external communication folder which includes various defined modules layered on top of each other and are listed as follows: message layer which defines the request and response messages required for master-slave interfac- ing, protocol layer which initiates the low-level protocol standard used for external units interconnection, handler layer for using predefined packet frame messages, and event-driven state machine layer which defines the modular framework.

----

## System Validation

In this section, the experimental setup along with software validation findings using a continuous testing environment are proposed.

### *Experimental Setup*

A hardware setup (Figure 19) is constructed to investigate and evaluate the developed system.

<p align="center">
<img width="543" alt="setup" src="https://user-images.githubusercontent.com/60155345/213581346-4ecb28b5-c977-42d6-838c-cd1e1bec7486.png">

<div align="center">
Fig.19: Laboratory Setup
</div>
<br />

*The setup is composed of:*

- AMuSeD main hardware with the integrated MicroSD card, GPS, IMU, and temperature sensor modules along with ATMEL ICE tool and USART serial debugger interface. The main unit chip is characterized by minimization of active power consumption with software-selectable low-power modes.
+ ATSAMD51 Ada fruit board as the external slave unit for the developed modular platform. It is connected with the master through I2C standard interface. The code on the slave unit is implemented in Arduino programming language to respond to the master unit requests.
- MCC DAQ data acquisition device used to analyze the power consumption through recording the analog input to the main board along with HIOKI AC/DC meter [8] for measuring the current and voltage levels.


### *Continuous Testing Environment*

Continuous process of testing is executed as a fragment of the firmware pipeline to eliminate bugs and errors in case of unexpected disturbances and validate the developed code quality. Multiple test cases and scenarios are carried out in compliance with the validation process to confirm the software robustness and reliability ensuring the effectiveness of the implemented error handling techniques.

The software validation is completed in three verification levels described as follows:

1. Unit testing level evaluates the individual software components and modules. The module is generally a section of code which is comprised of a single function or a group of related functions and isolated from the rest of the system to verify its accuracy and performance.
2. Multiple software modules and device peripherals are integrated at Integration testing level where their combined functionality and interactions are tested.
3. System testing validates the complete integrated software product with all its dependencies and evaluates its compliance with the corresponding requirements.

Testing software running on microcontrollers remains a challenge since it differs from PC-based desktop applications. To ensure the software validity for the modular platform, USART standard is used to interface with the system and stimulate its behaviour. This method is carried out by watching live messages printed on serial monitors for both master and slave units. The printed statements are then interpreted with respect to the corresponding executed portions of code. In addition, a run-time methodology using blinking LEDs is implemented to harness the testing environment with further validation for each software module. Subsequently, The software platform is tested and validated against possible external and internal disturbances, for instance, hardware wiring issues between master and slave units. A complete testing environment is illustrated in Figure 20.

<p align="center">
<img width="548" alt="testing" src="https://user-images.githubusercontent.com/60155345/213581457-b8da0e3b-7481-4e0c-ac6e-643c13ff16b4.png">

<div align="center">
Fig.20: Testing Environment
</div>
<br />

To validate the accuracy of data transfer between master and slave units, random values are generated using MATLAB and saved on the slave side as simulated sensory measurement data which are transmitted to the master side upon request and stored on the integrated MicroSD card. Similarities between data on both master and slave units are then investigated through live visualization of serial monitors as illustrated in Figure 21 in addition to further data comparisons between the mounted SD card and the generated MATLAB file. The reliability of data transmission between the interconnected master and slave units has successfully been validated using the aforementioned methodology.

<p align="center">
<img width="585" alt="serialdebugging" src="https://user-images.githubusercontent.com/60155345/213581681-4a4493e9-7711-4326-b3c9-5e88955843ef.png">

<div align="center">
Fig.21: Master-Slave Data Flow Validation
</div>
<br />

### *Low-Power Modes Findings*

The validation has been conducted using a data acquisition device to record the analog input to the main board for a sampling period of 5 minutes with a rate of 10000 samples per second. Mean values of voltage, current and power consumption are subsequently calculated. Comparisons between the amount of consumed current and power required to carry out the same sensory tasks before and after applying the power saving methodology are evaluated.

The integrated GPS module consumed the most energy within the system followed by the IMU and temperature sensor modules. Hence, the initial software has been enhanced in terms of energy efficiency to ensure a notable depletion in the power consumption using the following methodology: the time needed to fix the first valid GPS location is decreased using a more optimized GPS startup configuration. In addition, the switching between active and low power modes for each sensor module has been defined and implemented using the scheduling system.

<p align="center">
<img width="658" alt="Screenshot 2023-01-20 at 04 36 06" src="https://user-images.githubusercontent.com/60155345/213612376-624ffb4e-6d4a-470e-98d5-f638f851f338.png">

<div align="center">
Fig.22: Power Saving Findings
</div>
<br />

