#pragma once

typedef int irqreturn_t;
typedef irqreturn_t (*irq_handler_t)(int, void *);

#define IRQ_HANDLED 0
#define IRQF_TRIGGER_FALLING 1

int request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags,
	    const char *name, void* dev);
void free_irq(unsigned int irq, void* dev);

