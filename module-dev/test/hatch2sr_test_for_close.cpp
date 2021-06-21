#include "hatch2sr_test_base.hpp"

using namespace ::testing;

struct Hatch2srTestForClose : public Hatch2srTestBase<Test>
{
};

TEST_F(Hatch2srTestForClose, WhenHatchIsCalledToCloseThenRelayPositonShouldBeSetAndEngineEnabled)
{
  //Arrange
  EXPECT_CALL(engine_mock_, engine_start_impl(An<engine*>()));
  EXPECT_CALL(relay_mock_, relay_set_to_close_impl(An<relay*>()));
    
  //Act
  init_sut();
  hatch2sr_close();

  //Assert - covered by expectation
}

TEST_F(Hatch2srTestForClose, WhenHatchIsCalledToCloseAndItIsAlreadyOpenThenNothingShouldHappen)
{
  //Arrange
  EXPECT_CALL(closedpos_sensor_mock_, sensor_get_value_impl(An<sensor*>()))
    .WillOnce(Return(SENSOR_VALUE_HIGH));
  EXPECT_CALL(engine_mock_, engine_start_impl(An<engine*>()))
    .Times(0);
  EXPECT_CALL(relay_mock_, relay_set_to_open_impl(An<relay*>()))
    .Times(0);
    
  //Act
  init_sut();
  hatch2sr_close();

  //Assert - covered by expectation
}