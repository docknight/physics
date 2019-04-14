#include "stdafx.h"
#include "Form1.h"
//#include <windows.h>


using namespace Simulator;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	System::Threading::Thread::CurrentThread->ApartmentState = System::Threading::ApartmentState::STA;
	Application::Run(new SimulatorWindow());
	return 0;
}
