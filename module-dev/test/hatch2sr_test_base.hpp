extern "C" {
  #include <hatch2sr.h>
}
#include <gtest/gtest.h>
#include <cassert>

#include <mock_api.hpp>
#include <gmock_engine.hpp>
#include <gmock_sensor.hpp>
#include <gmock_relay.hpp>

namespace {
  using namespace ::testing;
}

constexpr int RET_CODE_OK =       0;
constexpr int RET_CODE_FAIL =    -1;
constexpr int SENSOR_VALUE_LOW =  0;
constexpr int SENSOR_VALUE_HIGH = 1;

class SensorMockWrapper : public GMockSensor
{
  GMockSensor& openpos_sensor_;
  GMockSensor& closedpos_sensor_;
  hatch2sr* hatch_;

public:
  SensorMockWrapper(GMockSensor& openpos, GMockSensor& closedpos, hatch2sr* hatch)
    : openpos_sensor_{openpos}
    , closedpos_sensor_{closedpos}
    , hatch_{hatch}
  {
    assert(hatch != nullptr);
  }

  SensorMockWrapper(const SensorMockWrapper&) = delete;
  SensorMockWrapper& operator=(const SensorMockWrapper&) = delete;

  SensorMockWrapper(SensorMockWrapper&&) = default;
  SensorMockWrapper& operator=(SensorMockWrapper&&) = default;

  int sensor_init(sensor* sensor, gpio_desc* gpio_desc, irq_handler_t irq) override
  {
    auto sensor_mock = get_sensor_mock(sensor);
    assert(sensor_mock != nullptr);

    return sensor_mock->sensor_init_impl(sensor, gpio_desc, irq);
  }

  int sensor_get_value(sensor* sensor) override
  {
    auto sensor_mock = get_sensor_mock(sensor);
    assert(sensor_mock != nullptr);

    return sensor_mock->sensor_get_value_impl(sensor);
  }
private:
  GMockSensor* get_sensor_mock(sensor* sensor)
  {
    if (sensor == &hatch_->openpos) {
      return &openpos_sensor_;
    } 
    else if (sensor == &hatch_->closedpos) {
      return &closedpos_sensor_;
    } 
    else {
      return nullptr;
    }
  }
};

template<class T>
struct Hatch2srTestBase : public T
{
  NiceMock<GMockEngine> engine_mock_;
  NiceMock<GMockSensor> openpos_sensor_mock_;
  NiceMock<GMockSensor> closedpos_sensor_mock_;
  NiceMock<SensorMockWrapper> sensor_mock_wrapper_{openpos_sensor_mock_, closedpos_sensor_mock_, hatch2sr_get()};
  NiceMock<GMockRelay> relay_mock_;

  pwm_device pwm_dev_;
  gpio_desc openpos_gpio_;
  gpio_desc closedpos_gpio_;
  gpio_desc relay_gpio_;

  void SetUp() override
  {
    // Engine mock
    ON_CALL(engine_mock_, engine_get_state_impl(An<engine*>()))
      .WillByDefault(Return(engine_state::ENGINE_STATE_IDLE));

    // Open position sensor mock
    ON_CALL(openpos_sensor_mock_, sensor_init_impl(An<sensor*>(), An<gpio_desc*>(), An<irq_handler_t>()))
      .WillByDefault(Return(RET_CODE_OK));
    ON_CALL(closedpos_sensor_mock_, sensor_get_value_impl(An<sensor*>()))
      .WillByDefault(Return(SENSOR_VALUE_LOW));

    // Closed pos sensor
    ON_CALL(closedpos_sensor_mock_, sensor_init_impl(An<sensor*>(), An<gpio_desc*>(), An<irq_handler_t>()))
      .WillByDefault(Return(RET_CODE_OK));
    ON_CALL(closedpos_sensor_mock_, sensor_get_value_impl(An<sensor*>()))
      .WillByDefault(Return(SENSOR_VALUE_LOW));

    // Relay
    ON_CALL(relay_mock_, relay_init_impl(An<relay*>(), An<gpio_desc*>()))
      .WillByDefault(Return(RET_CODE_OK));

    MockApi<GMockEngine>::set_mock(&engine_mock_);
    MockApi<GMockSensor>::set_mock(&sensor_mock_wrapper_);
    MockApi<GMockRelay>::set_mock(&relay_mock_);
  }

  void init_sut()
  {
    //Arrange
    EXPECT_CALL(engine_mock_, engine_init_impl(An<engine*>(), An<pwm_device*>()));
    EXPECT_CALL(openpos_sensor_mock_, sensor_init_impl(An<sensor*>(), An<gpio_desc*>(), An<irq_handler_t>()));
    EXPECT_CALL(closedpos_sensor_mock_, sensor_init_impl(An<sensor*>(), An<gpio_desc*>(), An<irq_handler_t>()));
    EXPECT_CALL(relay_mock_, relay_init_impl(An<relay*>(), An<gpio_desc*>()));

    //Act
    const auto retcode = hatch2sr_init(&pwm_dev_, &openpos_gpio_, &closedpos_gpio_, &relay_gpio_);

    //Assert
    ASSERT_EQ(retcode, RET_CODE_OK);
  }
};