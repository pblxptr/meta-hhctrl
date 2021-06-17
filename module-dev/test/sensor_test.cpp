
extern "C" {
  #include <sensor.h>
}

#include <gtest/gtest.h>
#include <memory>
#include <mock_api.hpp>
#include <gmock_gpio.hpp>
#include <gmock_interrupt.hpp>

using namespace ::testing;

struct SensorTest : public Test
{
  std::unique_ptr<GMockGpio> gpio_mock_;
  std::unique_ptr<GMockInterrupt> interrupt_mock_;

  void SetUp() override
  {
    gpio_mock_ = std::make_unique<NiceMock<GMockGpio>>();
    interrupt_mock_ = std::make_unique<NiceMock<GMockInterrupt>>();

    //Setup GMockGpio
    ON_CALL(*gpio_mock_, gpiod_get_value_impl(An<gpio_desc*>()))
      .WillByDefault(Return(0));

    ON_CALL(*gpio_mock_, desc_to_gpio_impl(An<gpio_desc*>()))
      .WillByDefault(Return(0));

    ON_CALL(*gpio_mock_, gpio_to_irq_impl(An<int>()))
      .WillByDefault(Return(0));

    ON_CALL(*gpio_mock_, gpiod_direction_output_impl(An<gpio_desc*>(), An<int>()))
      .WillByDefault(Return(0));

    //Setup GMockInterrupt
    ON_CALL(*interrupt_mock_, request_irq_impl(An<unsigned int>(), An<irq_handler_t>(), 
      An<unsigned long>(), 
      An<const char*>(), 
      An<void*>()))
    .WillByDefault(Return(0));

    MockApi<GMockGpio>::set_mock(gpio_mock_.get());
    MockApi<GMockInterrupt>::set_mock(interrupt_mock_.get());
  }

  void TearDown() override
  {
    MockApi<GMockGpio>::unset_mock();
    MockApi<GMockInterrupt>::unset_mock();

    gpio_mock_.reset();
    interrupt_mock_.reset();
  }
};

irqreturn_t dummy_isr(int, void *)
{
  return IRQ_HANDLED;
}

TEST_F(SensorTest, WhenSensorIsInitializedThenAppropirateGpioShouldBeConfiguredAndIrqRequested)
{
  //Arrange
  auto sut = sensor{};
  auto gpio_descriptor = gpio_desc{};
  EXPECT_CALL(*gpio_mock_, desc_to_gpio_impl(An<gpio_desc*>()));
  EXPECT_CALL(*gpio_mock_, gpio_to_irq_impl(An<int>()));
  EXPECT_CALL(*gpio_mock_, gpiod_direction_input_impl(An<gpio_desc*>()));
  EXPECT_CALL(*gpio_mock_, gpiod_export_impl(An<gpio_desc*>(), An<int>()));
  EXPECT_CALL(*interrupt_mock_, request_irq_impl(An<unsigned int>(), An<irq_handler_t>(),
    An<unsigned long>(), 
    An<const char*>(), 
    An<void*>()));

  //Act
  const auto ret = sensor_init(&sut, &gpio_descriptor, dummy_isr);

  //Assert
  ASSERT_EQ(ret, 0);
}