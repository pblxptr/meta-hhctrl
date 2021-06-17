extern "C" {
  #include <linux/interrupt.h>
}

#include <mock_api.hpp>
#include <gmock_interrupt.hpp>

int request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags,
	    const char *name, void *dev)
{
  if (!MockApi<GMockInterrupt>::has_mock()) {
    return 0;
  }

  return MockApi<GMockInterrupt>::get_mock()->request_irq_impl(irq, handler, flags, name, dev);
}