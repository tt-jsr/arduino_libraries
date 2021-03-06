#ifndef MESSAGE_QUEUE_IMPL_H_
#define MESSAGE_QUEUE_IMPL_H_

struct Message
{
    int msg_type;
    int arg1;
    int arg2;
};

struct Timer
{
    unsigned long interval;
    unsigned long nextTrigger;
    bool repeat;  
};

#define PIN_PIN_MASK        0x00ff
#define PIN_DIGITAL_BIT     0x1000  // digital/analog
#define PIN_ENABLED_BIT     0x2000

#define PIN_PIN(p)           (p.flags & PIN_PIN_MASK)
#define PIN_SET_PIN(p, v)    (p.flags |= v )

#define PIN_IS_DIGITAL(p)    ((p.flags & PIN_DIGITAL_BIT) == PIN_DIGITAL_BIT)
#define PIN_SET_DIGITAL(p)   (p.flags |= PIN_DIGITAL_BIT)
#define PIN_UNSET_DIGITAL(p) (p.flags &= ~PIN_DIGITAL_BIT)

#define PIN_IS_ENABLED(p)    ((p.flags & PIN_ENABLED_BIT) == PIN_ENABLED_BIT)
#define PIN_SET_ENABLED(p)   (p.flags |= PIN_ENABLED_BIT)
#define PIN_UNSET_ENABLED(p) (p.flags &= ~PIN_ENABLED_BIT)
struct Pin
{
    int flags;  // contains the pin id, enabled flag, and digital/analog flag
    int debounceValue;
    unsigned long  debounceTime;  
    unsigned long  triggerComplete;    
};

struct Pulse
{
    int pin;  // contains the pulse id, pin id
    unsigned long duration;  
    unsigned long pulseComplete;    
};

struct Value
{
    int id;
    int value;
};

#endif
