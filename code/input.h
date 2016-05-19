#pragma once

// Author(s): Tom, Simon

struct Input
{
	//NOTE(Simon): SDL already provides this array. Index it with the scancodes. Not safe for non QWERTY
	//TODO(Simon): Fix for non qwerty?
	uint8* keys;
	int32 mouseRelativeX;
	int32 mouseRelativeY;
	int32 mouseX;
	int32 mouseY;
	int32 mouseScroll;
};

//NOTE(Simon): Only this is required to handle all keyevents ever. The *keys* array is managed by SDL and is guaranteed to have a lifetime equal to the app.
//			   No subsequent calls need to be made, but can be if it makes you feel better.
void InputInitKeyStates(Input* input, uint8* keys)
{
	//NOTE(simon): Link the array provided by SDL to our struct
	input->keys = keys;
}

//NOTE(Simon): this does need to be called for every mouse motion event
void InputSetRelativeMouseMotion(Input* input, int x, int y)
{
	input->mouseRelativeX = x;
	input->mouseRelativeY = y;
}

void InputSetMousePosition(Input* input, int x, int y)
{
	input->mouseX = x;
	input->mouseY = y;
}

//NOTE(Simon): Mousewheel is a strange beast. Data is only provided through an event, which can obviously happen multiple times per frame.
//			   There is no access to the absolute wheel position, so to account for all of this:
//					Call InputResetMouseScroll at the beginning of the frame.
//					Then for each SDL_MouseWheelEvent call InputAddMouseScroll. This will accumulate all mousewheel events into one relative mousescroll

void InputAddMouseScroll(Input* input, SDL_MouseWheelEvent* wheelEvent)
{
	input->mouseScroll += wheelEvent->y;
}

void InputResetMouseScroll(Input* input)
{
	input->mouseScroll = 0;
}
