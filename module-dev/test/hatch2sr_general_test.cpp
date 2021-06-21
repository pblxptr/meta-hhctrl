#include "hatch2sr_test_base.hpp"

struct Hatch2srGeneralTest : public Hatch2srTestBase<::testing::Test>
{

};

TEST_F(Hatch2srGeneralTest, WhenAllDependenciesAreNotNullThenDriverShouldBeInitialzied)
{
  init_sut();
}

TEST_F(Hatch2srGeneralTest, WhenAnyOfDependenciesIsNullThenInitializationShouldFail)
{
  //Arrange
  EXPECT_CALL(engine_mock_, engine_init_impl(An<engine*>(), An<pwm_device*>()))
    .Times(0);
  EXPECT_CALL(openpos_sensor_mock_, sensor_init_impl(An<sensor*>(), An<gpio_desc*>(), An<irq_handler_t>()))
    .Times(0);
  EXPECT_CALL(closedpos_sensor_mock_, sensor_init_impl(An<sensor*>(), An<gpio_desc*>(), An<irq_handler_t>()))
    .Times(0);
  EXPECT_CALL(relay_mock_, relay_init_impl(An<relay*>(), An<gpio_desc*>()))
    .Times(0);

  //Act
  const auto retcode = hatch2sr_init(NULL, NULL, NULL, NULL);

  //Assert
  ASSERT_EQ(retcode, RET_CODE_FAIL);
}

TEST_F(Hatch2srGeneralTest, WhenEngineIsRunningAndCallToEitherOpenOrCloseHasBeenPerformedNothingShouldChange)
{
  //Arrange
  EXPECT_CALL(engine_mock_, engine_get_state_impl(An<engine*>()))
    .WillRepeatedly(Return(ENGINE_STATE_RUNNING));

  EXPECT_CALL(engine_mock_, engine_start_impl(An<engine*>()))
    .Times(0);
  EXPECT_CALL(engine_mock_, engine_stop_impl(An<engine*>()))
    .Times(0);
  EXPECT_CALL(relay_mock_, relay_set_to_open_impl(An<relay*>()))
    .Times(0);
  EXPECT_CALL(relay_mock_, relay_set_to_close_impl(An<relay*>()))
    .Times(0);

  //Act
  init_sut();
  
  //Call open && close mutliptle times
  hatch2sr_open();
  hatch2sr_open();
  hatch2sr_close();
  hatch2sr_open();
  hatch2sr_close();

  //Assert - covered by expectations
}

TEST_F(Hatch2srGeneralTest, WhenBothSensorsIndicateActiveStateThenNeitherOpenOrCloseCallShouldMakeEngineRunning)
{
  //Arrange
  EXPECT_CALL(openpos_sensor_mock_, sensor_get_value_impl(An<sensor*>()))
    .WillRepeatedly(Return(SENSOR_VALUE_HIGH));
  EXPECT_CALL(closedpos_sensor_mock_, sensor_get_value_impl(An<sensor*>()))
    .WillRepeatedly(Return(SENSOR_VALUE_HIGH));

  EXPECT_CALL(engine_mock_, engine_start_impl(An<engine*>()))
    .Times(0);
  EXPECT_CALL(engine_mock_, engine_stop_impl(An<engine*>()))
    .Times(0);
  EXPECT_CALL(relay_mock_, relay_set_to_open_impl(An<relay*>()))
    .Times(0);
  EXPECT_CALL(relay_mock_, relay_set_to_close_impl(An<relay*>()))
    .Times(0);

  //Act
  init_sut();
  hatch2sr_open();
  hatch2sr_open();
  hatch2sr_close();
  hatch2sr_close();
  hatch2sr_open();
  hatch2sr_close();

  //Assert - covered by expectations
}