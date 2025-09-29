#include <LPC21XX.h>
#include "delay.h"

#define EINT0_FUN  FUN2
#define FUN2      1
//#define EINT0_LED 1

extern int Interrupt_Triggered;

void eint0_isr(void) __irq
{
    // toggle the led
    Interrupt_Triggered = 1;

    // clear the interrupt
    EXTINT = 1 << 0;   // EINT0 corresponds to bit 0
    VICVectAddr = 0;
}

void enable_eint0(void)
{
    // Configure P0.16 as EINT0
    PINSEL1 &= ~(3 << 0); // clear P0.16 function bits
    PINSEL1 |= (1 << 0);  // set P0.16 as EINT0 (fun2)

    // select IRQ (not FIQ)
    VICIntSelect = 0;

    // enable EINT0 in VIC
    VICIntEnable = 1 << 14;   // EINT0 is VIC position 14

    // set vector control for slot 0
    VICVectCntl0 = (1 << 5) | 14;  // slot enable + EINT0

    // assign ISR address
    VICVectAddr0 = (unsigned int)eint0_isr;

    // edge triggered
    EXTMODE = 1 << 0;   // EINT0 edge triggered
    EXTPOLAR &= ~(1 << 0); // falling edge
}
