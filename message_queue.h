#ifndef MESSAGE_QUEUE_H_
#define MESSAGE_QUEUE_H_

#include "message_queue_impl.h"

const int IDLE_EVENT = 32001;
const int TIMER_EVENT = 32002;
const int VALUE_EVENT = 32003;
const int USER_EVENT_BASE = 32004;


// Each queue slot takes 6 bytes
//extern const int MAX_MESSAGES;

// Each timer consumes 10 bytes
// Timers id's run from 0 to MAX_TIMERS-1
//extern const int MAX_TIMERS;

// Each pin consumes 16 bytes
// Set to the number of pins you will use for the
// message queue digitalRead() or analogRead() events. Do not set this
// to the number of pins supported by your board.
// If you will be reading pins directly and not using the queue, you 
// do not need to include them here.
//
// digitalRead and analogRead id's run from 0 to MAX_PINS-1
//extern const int MAX_PINS;

// Set this to be the number of state
// variables you will be using.
// create_value id's run from MAX_PINS to MAX_VALUES-1
// The id's from 0 to MAX_PINS-1 are used by digitalRead and analogRead
//extern const int NUMBER_OF_VALUES;

//static const int MAX_VALUES = MAX_PINS + NUMBER_OF_VALUES;

//
// Define the number of pulse outputs
// Each pulse consumes 10 bytes
//extern const int MAX_PULSES;


class MessageQueue
{
public:
    MessageQueue(const int nmsg, const int ntimers, const int npins, const int nvalues, const int npulses);

    // Override if the application would like to be able to generate
    // events. This can be useful for the override to post keypresses
    // or any other events from various sensors into the event queue.
    virtual void OnGenerator();

    /*************************************************************
     Message and queing
    *************************************************************/

    // Post a message to the queue
    void post_message(int msg, int arg1, int arg2);

    // Is the queue empty?
    bool empty();

    // Get and remove a message from the queue. 
    // This will check pins and timers and post any needed events, then
    // will return the next event. If there are no events available in the
    // queue, an IDLE_EVENT will be returned.
    //
    // This call will always return either a queued message, or IDLE_EVENT
    void get_message(int& msg, int& arg1, int& arg2);


    // Get a message from the queue. This does not check timers or pins
    // and does not return any IDLE_EVENT
    bool consume_message(int& msg, int& arg1, int& arg2);

    // Get a message without removing it from the queue.
    // This does not check timers or pins and does not return any IDLE_EVENT
    bool peek_message(int& msg, int& arg, int& arg2);

    /****************************************************************
    Timers
    ****************************************************************/

    // Create a timer. TIMER_EVENT will be posted to the queue
    // with the id as an argument when the timer fires.
    // Returns true if the timer is created.
    //
    // get_message() returns:
    // msg:TIMER_EVENT
    // arg1: id
    // arg0: The number of milliseconds the timer is late from when 
    //       it was supposed to fire
    bool create_timer(int id, unsigned long interval_ms, bool repeat);

    // Reset a timer.
    // This can be used to reset a non repeating timer or
    // modify any existing timer
    void reset_timer(int id, unsigned long interval, bool repeat);

    // Reset a timer
    // On a running timer will reset the next trigger to be the interval from now.
    // On fired non-repeating timer, will restart it.
    // On a paused timer, will restart it.
    void reset_timer(int id);

    // Pause a timer. Use reset() to restart it
    void pause_timer(int id);

    // Cancel a timer given the id
    void cancel_timer(int id);

    /****************************************************************
     Writing pins
    ****************************************************************/
    // Create a pulse on an output pin
    // id is the id of the pulse
    // pin is the digital pin
    // duration_us is the duration of going high for the number of microseconds
    bool digitalPulse(int id, int pin, unsigned long duration_us);

    // Trigger a pulse.
    //
    // There are no EVENTS generated for this call.
    bool pulse(int id);

    // Override the duration specified in the digitalPulse() call.
    bool pulse(int id, unsigned long duration_us);

    /****************************************************************
     Reading pins
    ****************************************************************/

    // Generate VALUE_EVENT messages when this given pin
    // changes state
    // id: The id
    // pin: The digital pin to be read for input.
    // def: Default state is LOW or HIGH. An event will be generated when this changes.
    // debounce: Time in microseconds to debounce the input. 
    //           Zero for no debounce
    //
    // get_message() returns:
    // msg:VALUE_EVENT
    // arg1: id
    // arg2: HIGH or LOW
    bool digitalRead(int id, int pin, int def, unsigned long debounceTimeMicros);

    // Generate VALUE_EVENT messages
    // id: The id.
    // pin: The analog in to be read.
    // timeMicros: set to 0 to post an event on every call to the message_pump(),
    // otherwise, post an event every timeMicros microseconds
    //
    // get_message() returns:
    // msg: VALUE_EVENT
    // arg1: id
    // arg2: value read
    bool analogRead(int id, int pin, unsigned long timeMicros);

    /***************************************************************
     State variables
    ***************************************************************/
    // Create a value that when changed, generates a VALUE_EVENT message
    // id: The id.
    // v: The initial value to store.
    bool create_value(int id, int v);

    // Set a value. If the vaue differs from the current value
    // a VALUE_EVENT message will be posted with the new value
    // id: The id.
    // v: The vaue to set.
    //
    // get_message() returns:
    // msg: VALUE_EVENT
    // arg1: id
    // arg2: new value
    void set_value(int id, int v);

    // Get the current value
    // Returns -1 if the id is not valid.
    int get_value(int id);

    // Toggle a value. Treats the value as a boolean, toggles between
    // 1 and 0. A VALUE_EVENT message will be posted.
    // id: The id.
    //
    // get_message() returns:
    // msg: VALUE_EVENT
    // arg1:id
    // arg2: 1 or 0
    void toggle_value(int id);

    // Enable printing to the monitor for debugging
    void setDebug(bool debug);
private:
    void check_timers();
    void check_pins();
    void digital_check(int id);
    void analog_check(int id);
    unsigned int consumer_;
    unsigned int producer_;
    const int MAX_MESSAGES;
    const int MAX_TIMERS;
    const int MAX_PINS;
    const int MAX_VALUES;
    const int MAX_PULSES;
    Message *msg_queue_;
    Pin *pins_;
    Pulse *pulses_;
    Timer *timers_;
    Value *values_;
    bool debug_;
};


#endif
