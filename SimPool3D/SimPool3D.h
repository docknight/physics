#pragma once

#include "resource.h"


//-----------------------------------------------------------------------------
// function declaration
//-----------------------------------------------------------------------------
bool			LoadSceneObjects();
bool			DirectInputSetup(); // mouse initialization
VOID			FreeDirectInput();  // mouse initialization
DIMOUSESTATE2	ReadMouseImmediateData(); // return current mouse state (position and button state)