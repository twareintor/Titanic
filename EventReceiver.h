#pragma once

#include <irrlicht.h>

using namespace irr;

// EventReceiverXwator.h

// 
class MyEventReceiver : public IEventReceiver
{
private:
	// We use this array to store the current state of each key
	bool	KeyIsDown[KEY_KEY_CODES_COUNT];
	SEvent::SJoystickEvent	JoystickState;

public:
	// We'll create a struct to record info on the mouse state
	struct SMouseState
	{
		core::position2di Position;
		bool LeftButtonDown;
		bool MiddleButtonDown;
		bool RightButtonDown;
		SMouseState(): LeftButtonDown(false), MiddleButtonDown(false), RightButtonDown(false){}

	} MouseState;
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
		// Remember whether each key is down or up
		if(event.EventType == irr::EET_KEY_INPUT_EVENT)
		{
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
		}
		// Remember the mouse state
		if(event.EventType == irr::EET_MOUSE_INPUT_EVENT)
		{
			switch(event.MouseInput.Event)
			{
			case EMIE_LMOUSE_PRESSED_DOWN:
			{
				MouseState.LeftButtonDown = true;
				break;
			}
			case EMIE_MMOUSE_PRESSED_DOWN:
			{
				MouseState.MiddleButtonDown = true;
				break;
			}
			case EMIE_RMOUSE_PRESSED_DOWN:
			{
				MouseState.RightButtonDown = true;
				break;
			}
			case EMIE_LMOUSE_LEFT_UP:
			{
				MouseState.LeftButtonDown = false;
				break;
			}
			case EMIE_MMOUSE_LEFT_UP:
			{
				MouseState.MiddleButtonDown = false;
				break;
			}
			case EMIE_RMOUSE_LEFT_UP:
			{
				MouseState.RightButtonDown = false;
				break;
			}
			case EMIE_MOUSE_MOVED:
			{
				MouseState.Position.X = event.MouseInput.X;
				MouseState.Position.Y = event.MouseInput.Y;
				break;
			}
			default:
			{
				// We won't use the wheel
				break;
			}

			}
		}
		if (event.EventType == irr::EET_JOYSTICK_INPUT_EVENT
			&& event.JoystickEvent.Joystick == 0)
		{
			JoystickState = event.JoystickEvent;
		}

		return false;
	}

	const SEvent::SJoystickEvent & GetJoystickState(void) const
	{
		return JoystickState;
	}

	// This is used to check whether a key is being held down
	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}

	MyEventReceiver()
	{
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}
	const SMouseState & GetMouseState(void) const
	{
		return MouseState;
	}
	// custom function
	const f32 getRamp(scene::ISceneNode* nodX) const
	{
		f32 y = 0.f;
		static core::vector3df posX0 = nodX->getPosition();
		core::vector3df posX1 = nodX->getPosition();
		y = posX1.Y - posX0.Y;		
		if(fabs(y)<0.01f) y = 0.f;
		posX0 = posX1;
		return y;
	}
};

