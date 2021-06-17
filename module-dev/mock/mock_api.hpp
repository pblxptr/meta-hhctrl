#pragma once

template<class Mock>
class MockApi
{
  static Mock* mock_;
public:
  static void set_mock(Mock* mock);
  static void unset_mock();
  static bool has_mock();
  static Mock* get_mock();
};

template<class Mock>
Mock* MockApi<Mock>::mock_ = nullptr;

template<class Mock>
void MockApi<Mock>::set_mock(Mock* mock)
{
  mock_ = mock;
}

template<class Mock>
void MockApi<Mock>::unset_mock()
{
  mock_ = nullptr;
}

template<class Mock>
bool MockApi<Mock>::has_mock()
{
  return mock_ != nullptr;
}

template<class Mock>
Mock* MockApi<Mock>::get_mock()
{
  return mock_;
}