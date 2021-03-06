/*
 * linux/arch/arm/mach-adifcc/irq.c
 * 
 * Author:  Deepak Saxena
 * Copyright:   (C) 2001 MontaVista Software Inc.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/config.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/interrupt.h>

#include <asm/mach/irq.h>
#include <asm/irq.h>
#include <asm/hardware.h>

#include <asm/mach-types.h>

void xs80200_irq_mask (unsigned int irq)
{
	long INTCTL;
	asm ("mrc p13, 0, %0, c0, c0, 0" : "=r" (INTCTL));
	switch (irq) {
	    case IRQ_XS80200_BCU:     INTCTL &= ~(1<<3); break;
	    case IRQ_XS80200_PMU:     INTCTL &= ~(1<<2); break;
	    case IRQ_XS80200_EXTIRQ:  INTCTL &= ~(1<<1); break;
	    case IRQ_XS80200_EXTFIQ:  INTCTL &= ~(1<<0); break;
	}
	asm ("mcr p13, 0, %0, c0, c0, 0" : : "r" (INTCTL));
}

void xs80200_irq_unmask (unsigned int irq)
{
	long INTCTL;
	asm ("mrc p13, 0, %0, c0, c0, 0" : "=r" (INTCTL));
	switch (irq) {
	    case IRQ_XS80200_BCU:	INTCTL |= (1<<3); break;
	    case IRQ_XS80200_PMU:	INTCTL |= (1<<2); break;
	    case IRQ_XS80200_EXTIRQ:	INTCTL |= (1<<1); break;
	    case IRQ_XS80200_EXTFIQ:	INTCTL |= (1<<0); break;
	}
	asm ("mcr p13, 0, %0, c0, c0, 0" : : "r" (INTCTL));
}

void __init xs80200_init_irq(void)
{
	int i;
	unsigned long int_steer = 0x0;

	/*
	 * Steer interrupts to IRQ instead of FIQ
	 */
	asm ("mcr p13, 0, %0, c8, c0, 0" : : "r" (int_steer));

	for (i = 0; i < NR_XS80200_IRQS; i++) {
		irq_desc[i].valid	= 1;
		irq_desc[i].probe_ok	= 0;
		irq_desc[i].mask_ack	= xs80200_irq_mask;
		irq_desc[i].mask	= xs80200_irq_mask;
		irq_desc[i].unmask	= xs80200_irq_unmask;
	}
}

