#pragma once

#include <gmock/gmock.h>

extern "C" {
  #include <linux/interrupt.h>
}

struct GMockInterrupt 
{
  MOCK_METHOD(int, request_irq_impl, (unsigned int, irq_handler_t, unsigned long, const char *, void*));
  MOCK_METHOD(int, free_irq_impl, (unsigned int, void*));
};