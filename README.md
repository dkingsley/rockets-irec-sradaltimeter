# rockets-irec-sradaltimeter

Code to run on the embedded processor of the SRAD altimeter.

I'm sorry this readme doesn have spellcheck so gg.

### Flight States
The altimeter operates in discrete states that dictate the majority of the actions of the processor. However it is not in statemachine as the entirety of the unit is not determined solely by state and input.

#### States
1. Pre-Launch  
 - 1a. Startup  
 - 1b. Idle
 - 1c. Armed
2. Launch
 - 2a. Liftoff
 - 2b. Coast 
 - 2c. Descent under Drogue 
 - 2d. Descent under Main
3. Landed
 - 3a. Awaiting Recovery
 - 3b. Recovered 

#### Transitions 

This details all of the flight transitions and what the criteria are for passing through. 
 - 1a => 1b: Transistion occurs once startup is comlete and all sensors initialized. Deterministic.
 - 1b => 1c: The altimeter arms when it determines that it is pointing upwards and has been stable for a set ammount of time. Non determinstic, and will likely be determined by set threshholds for both being stable and pointing upwards.
 - 1c => 2a: Transition will occur when the altimeter undergoes an acceleration above a set threshhold for a set ammount of time. Threasholds will be determined from previous flight data.
 - 2a => 2b: Transition occurs when acceleration transitions from positive to negative for a set ammount of time.
 - 2b => 2c: Occures when the altimeter determines that altitude has reached it's maximum and is now starting to decrease. On this transition, an ejection charge is triggered for the drogue chute.
 - 2c => 2d: Occurs when the altimeter has fallen to a set altitude where the main should be deployed (nominally around 700ft). On this transition, an ejection charge is triggered for the main chute.
 - 2d => 3a: Occurs after the altimeter sees a sharp acceleration spike, followed by minimal momement.
 

### Events

The processor manages multiple tasks without the implementation of a full RTOS through the use of events that are triggered by interrupts.

Events are stored in the `uint8_t events` variable. Each bit of the `events` variable can flag an event that signals a task to be processed in the next iteration of the main loop. Each event is enumerated as a single byte with a 1 in a position corrisponding to the flagged bit. For example, the rightmost bit of `events` signals to exectute the main update loop, which is named `EVENT_UPDATE` and enumerated as `0b00000001` After a task is processed, it is unschedualed. 

The main event flags are triggered interupt timers. These timers inturrupt on constant intervals, and call a function that simply edits the inturrupt bit in the `events` variable.

### Code Organization

#### Classes

`Altimeter` - this is the primary class for the board and contains all of the other classes as members. Everything is initialized in this class

`Flight_Data` - this class stores the raw data values that are read from each sensor. It also also contains the methods for logging to SD card and has a member object that is the Kalman Filter object. Supports reading sensors at different frequencies. Does not store history, but probably will be updated in the future so that maybe the past 100 or so values are kept in ram.

`Flight_Sensors` - this class is used for accessing all of the sensors and reading GPIO inputs. Is kept separate from Flight_Data class, as use of this object can be easily swapped out for one for a Hardware-In-The-Loop test object

`Flight_Events` - Object of this class contains the 4 internal interrupt timers, as well as any other pin-driven interrupts, that can be used to time events properly. 

`Altitude_Kalman_Filter` - Custom kalman filter class for determining altitude. Stores all the relevant constants for the filter as well as the previous filter value. Contains all methods needed for filter update. 

#### Members of `Altimeter` class

flight_data: Object of type Flight_Data that stores all of the relevant data values for the flight. 
flight_sensors: Object of type Flight_Sensors that interfaces with all sensors
flight_events: Object of type Flight_Events that contians the 4 interrupt timers

flight_state: Integer that contains the enumerated flight state values
*more to add below* 


#### Non-Class Files

`S-ALT_REV0.h` - header file for the board. Contains all pin definitions and any pcb layout specific values
`Flight_Configurations.h` - header file that defines enumerated list and structures or various things within the project.

