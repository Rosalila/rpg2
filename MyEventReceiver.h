#ifndef MYEVENTRECEIVER_H
#define MYEVENTRECEIVER_H

#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class MyEventReceiver : public IEventReceiver
{
public:
    SEvent::SJoystickEvent JoystickState;

	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
		// Remember whether each key is down or up
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
        //Joystick
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

private:
	// We use this array to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};


#endif // MYEVENTRECEIVER_H
