extern "C" {
  #include <engine.h>
}

#include <gtest/gtest.h>
#include <memory>
#include <mock_api.hpp>
#include <gmock_pwm.hpp>

using namespace ::testing;

struct EngineTest : public Test
{
  std::unique_ptr<GMockPwm> pwm_mock_;

  void SetUp() override
  {
    pwm_mock_ = std::make_unique<NiceMock<GMockPwm>>();
  
    MockApi<GMockPwm>::set_mock(pwm_mock_.get());
  }

  void TearDown() override
  {
    pwm_mock_.reset();

    MockApi<GMockPwm>::unset_mock();
  }
};

TEST_F(EngineTest, WhenEngineIsBeingInitialziedThenPwmShouldBeConfigured)
{
  //Arrange
  auto sut = engine{};
  auto pwm_dev = pwm_device{};
  EXPECT_CALL(*pwm_mock_, pwm_config_impl(An<pwm_device*>(), An<int>(), An<int>()));

  //Act
  engine_init(&sut, &pwm_dev);

  //Assert - covered by expectations
}

TEST_F(EngineTest, WhenEngineHasBeenInitializedThenItShouldBeInIdleState)
{
  //Arrange
  auto sut = engine{};
  auto pwm_dev = pwm_device{};

  //Act
  engine_init(&sut, &pwm_dev);
  const auto state = engine_get_state(&sut);

  //Assert - covered by expectations
  ASSERT_EQ(state, ENGINE_STATE_IDLE);
}