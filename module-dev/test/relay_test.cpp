extern "C" {
  #include <relay.h>
}

#include <gtest/gtest.h>
#include <memory>
#include <mock_api.hpp>
#include <gmock_gpio.hpp>
#include <gmock_interrupt.hpp>

using namespace ::testing;

struct RelayTest : public Test
{
  std::unique_ptr<GMockGpio> gpio_mock_;

  void SetUp() override
  {
    gpio_mock_ = std::make_unique<NiceMock<GMockGpio>>();

    //Setup GMockGpio
    ON_CALL(*gpio_mock_, gpiod_get_value_impl(An<gpio_desc*>()))
      .WillByDefault(Return(0));

    ON_CALL(*gpio_mock_, desc_to_gpio_impl(An<gpio_desc*>()))
      .WillByDefault(Return(0));

    ON_CALL(*gpio_mock_, gpio_to_irq_impl(An<int>()))
      .WillByDefault(Return(0));

    ON_CALL(*gpio_mock_, gpiod_direction_output_impl(An<gpio_desc*>(), An<int>()))
      .WillByDefault(Return(0));

    MockApi<GMockGpio>::set_mock(gpio_mock_.get());
  }

  void TearDown() override
  {
    MockApi<GMockGpio>::unset_mock();

    gpio_mock_.reset();
  }
};

TEST_F(RelayTest, WhenSensorIsInitializedThenAppropirateGpioShouldBeConfiguredAndIrqRequested)
{
  //Arrange
  auto sut = relay{};
  auto gpio_descriptor = gpio_desc{};
  EXPECT_CALL(*gpio_mock_, desc_to_gpio_impl(An<gpio_desc*>()));
  EXPECT_CALL(*gpio_mock_, gpiod_direction_output_impl(An<gpio_desc*>(), An<int>()));
  EXPECT_CALL(*gpio_mock_, gpiod_export_impl(An<gpio_desc*>(), An<int>()));

  //Act
  const auto ret = relay_init(&sut, &gpio_descriptor);

  //Assert
  ASSERT_EQ(ret, 0);
}