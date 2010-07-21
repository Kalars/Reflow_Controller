#ifndef SOFT_TIMERS_H_INCLUDED
#define SOFT_TIMERS_H_INCLUDED

// timertick in mS. Overflow must be a multiple of timertick
#define SW_TIMERTICK_mS 25                                      // Hardware timer interrupt period, in milliSeconds
#define SW_TIMERTICK_uS (long)(SW_TIMERTICK_mS*1000)            // Hardware timer interrupt period, in microSeconds
#define SW_TIMERTICK_S  (SW_TIMERTICK_mS/1000)                  // Hardware timer interrupt period, in Seconds


// F_OSC is defined by the fuses and prescaler if Internal Oscillator is used, else by external oscillator
//#define F_OSC           (8000000)                               // oscillator frequency, uses internal oscillator. Not critical.
//#define F_CPU           (F_OSC/4)                               // CPU frequency, no pipelining in PIC, 1 op uses 4 cycles.
#define F_TMR           (F_CPU/TMR_PRESCALE)                    // Timer frequency

//#define TIMER1_TICK       (1/F_TMR)                           // time it takes for timer to increment 1 step, in Seconds
//#define TIMER1_TICK_mS    (1/(F_TMR/1000))                    // time it takes for timer to increment 1 step, in milliSeconds
#define TIMER1_TICK_uS      (1/(F_TMR/1000000))                 // time it takes for timer to increment 1 step, in microSeconds

//#define TIMER1_TICKS  (SW_TIMERTICK_S/TIMER1_TICK)            // number of ticks to reach the requested timer interrupt interval
//#define TIMER1_TICKS  (SW_TIMERTICK_mS/TIMER1_TICK_mS)        // number of ticks to reach the requested timer interrupt interval
#define TIMER1_TICKS    (SW_TIMERTICK_uS/TIMER1_TICK_uS)        // number of ticks to reach the requested timer interrupt interval

#define TIMER1_MAX          (2^16)                              // Timer1 is 16 bits. Max = 0xFFFF
#define TIMER1_RESTART      (TIMER1_MAX - TIMER1_TICKS)         // Timer 1 needs to restart at this value so that the correct
                                                                // number of steps remain for our requested interupt interval
// Bytes used to reload TIMER1
#define TIMER1_RESTART_L    (TIMER1_RESTART & 0xFF)             // Low byte restart value
#define TIMER1_RESTART_H    ((TIMER1_RESTART & 0xFF00) >> 8)    // High Low byte restart value

// macros for use in defining softtimer expiry times
#define mS_TO_TICK(mS)  mS/SW_TIMERTICK_mS                      // convert milliSeconds to software-Timer-ticks, must be a multiple of "SW_TIMERTICK_mS"
#define S_TO_TICK(S)    (S*1000)/SW_TIMERTICK_mS                // convert seconds to Timer-ticks, must be a multiple of "SW_TIMERTICK_mS"

#endif  // SOFT_TIMERS_H_INCLUDED
