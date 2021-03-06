#include "Controller.h"
#include "ButtonCodes.h"
#include "Matrix.h"
void Controller::Poll()
{
  XINPUT_STATE state = {};

  // Simply get the state of the controller from XInput.
  DWORD dwResult = XInputGetState((DWORD)ID, &state);

  last_controller = controller;
  if(dwResult == ERROR_SUCCESS)
  {
    controller = state.Gamepad;
  }
  else
  {
    controller = {};
  }
}

bool Controller::ButtonIsPressed(size_t code)
{
  // Last state returns false, this state returns true
  bool last_state;
  // Special case for triggers as buttons
  if(code == (size_t)ButtonCode::GAMEPAD_LEFT_TRIGGER)
  {
    last_state = (controller.bLeftTrigger > trigger_button_threshold);
  }
  else if(code == (size_t)ButtonCode::GAMEPAD_RIGHT_TRIGGER)
  {
    last_state = (controller.bRightTrigger > trigger_button_threshold);
  }
  last_state = ((code >> 8) & controller.wButtons);
  if(last_state == true)
  {
    return false;
  }
  // Special case for triggers as buttons
  if(code == (size_t)ButtonCode::GAMEPAD_LEFT_TRIGGER)
  {
    return (controller.bLeftTrigger > trigger_button_threshold) == true;
  }
  else if(code == (size_t)ButtonCode::GAMEPAD_RIGHT_TRIGGER)
  {
    return (controller.bRightTrigger > trigger_button_threshold) == true;
  }
  return ((code >> 8) & controller.wButtons) == true;
}

bool Controller::ButtonIsReleased(size_t code)
{
  // Last state returns true, this state returns false
  bool last_state;
  // Special case for triggers as buttons
  if(code == (size_t)ButtonCode::GAMEPAD_LEFT_TRIGGER)
  {
    last_state = (controller.bLeftTrigger > trigger_button_threshold);
  }
  else if(code == (size_t)ButtonCode::GAMEPAD_RIGHT_TRIGGER)
  {
    last_state = (controller.bRightTrigger > trigger_button_threshold);
  }
  last_state = ((code >> 8) & controller.wButtons);
  if(last_state == false)
  {
    return false;
  }
  // Special case for triggers as buttons
  if(code == (size_t)ButtonCode::GAMEPAD_LEFT_TRIGGER)
  {
    return (controller.bLeftTrigger > trigger_button_threshold) == false;
  }
  else if(code == (size_t)ButtonCode::GAMEPAD_RIGHT_TRIGGER)
  {
    return (controller.bRightTrigger > trigger_button_threshold) == false;
  }
  return ((code >> 8) & controller.wButtons) == false;
}

bool Controller::ButtonIsDown(size_t code)
{
  // Special case for triggers as buttons
  if(code == (size_t)ButtonCode::GAMEPAD_LEFT_TRIGGER)
  {
    return controller.bLeftTrigger > trigger_button_threshold;
  }
  else if(code == (size_t)ButtonCode::GAMEPAD_RIGHT_TRIGGER)
  {
    return controller.bRightTrigger > trigger_button_threshold;
  }
  return (code >> 8) & controller.wButtons;
}

Vec2 Controller::LeftStick()
{
  return Vec2(controller.sThumbLX, controller.sThumbLY) / 32768.0f;
}

Vec2 Controller::RightStick()
{
  return Vec2(controller.sThumbRX, controller.sThumbRY) / 32768.0f;
}

Vec2 Controller::Trigger()
{
  return Vec2(controller.bLeftTrigger, controller.bRightTrigger) / 255.f;
}


