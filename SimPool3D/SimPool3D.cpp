
#include "stdafx.h"
#include "SimPool3D.h"
#include "Graphics.h"
#include "GameEngine.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
HWND hWnd;										// main window handle
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

LPDIRECTINPUT8       g_pDI    = NULL;			// direct input object
LPDIRECTINPUTDEVICE8 g_pMouse = NULL;			// direct input device object

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

// instance of the game engine
CGameEngine			cGameEngine;

bool				bStrokeKeyPressed;


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SIMPOOL3D, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	// initialize graphics module
	InitGraphics();

	// Initialize Direct3D
    if( SUCCEEDED( InitD3D( hWnd, false ) ) )
    { 
	}
	else
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_SIMPOOL3D);

	// load scene objects
	if (!LoadSceneObjects())
	{
		DestroyWindow(hWnd);
		return -1;
	}

	// initialize DirectInput
	if (!DirectInputSetup())
	{
		MessageBox(hWnd, "DirectInput initialization failed.", "Simulation Pool", MB_OK);
		DestroyWindow(hWnd);
		return -1;
	}

	// setup table
	cGameEngine.SetupTable();

	// Main message loop:
    ZeroMemory( &msg, sizeof(msg) );
    while( msg.message!=WM_QUIT )
    {
        if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
            Render();
    }

	UnregisterClass( szWindowClass, hInstance );

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_SIMPOOL3D);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_BACKGROUND+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_SIMPOOL3D;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      0, 0, 800, 600, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   bStrokeKeyPressed = false;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	//PAINTSTRUCT ps;
	//HDC hdc;

	switch (message) 
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
/*	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;*/
    case WM_TIMER:
            // Update the input device every timer message
        {
			DIMOUSESTATE2 ms = ReadMouseImmediateData();
			cGameEngine.RotateView(ms.lX, ms.lY, ms.lZ, ms.rgbButtons);
			cGameEngine.StepSimulation(1/12.0f);
		}
		break;
	case WM_KEYDOWN:
		if ( (int)wParam == 83 && !bStrokeKeyPressed )  // "S" - stroke mode
		{
			bStrokeKeyPressed = true;
			cGameEngine.SwitchStrokeMode();
		}
		break;
	case WM_KEYUP:
		if ( (int)wParam == 83 && bStrokeKeyPressed )  // "S" - back to aim mode
		{
			bStrokeKeyPressed = false;
			cGameEngine.SwitchStrokeMode();
		}
		break;

	case WM_DESTROY:
		CleanupGraphics();
		cGameEngine.Cleanup();
		FreeDirectInput();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}


//-----------------------------------------------------------------------------
// Name: LoadSceneObjects()
// Desc: Load .x objects to be used by graphics module
//-----------------------------------------------------------------------------
bool LoadSceneObjects()
{
	bool bSuccess = true;
	CGrObject* pGrObject;
	// load graphics for POOLTABLE
	pGrObject = LoadObject("Graphics\\pooltable.x");
	// link to math equivalent of the object
	pGrObject->pEntity = cGameEngine.pPoolTableCenter;
	bSuccess &= pGrObject->bInitialized;
	// load graphics for CUE BALL
	pGrObject = LoadObject("Graphics\\nball_00.x");
	// link to math equivalent of the object
	pGrObject->pEntity = cGameEngine.pPoolBall[0];
	bSuccess &= pGrObject->bInitialized;
	// load graphics for OBJECT BALLS
	pGrObject = LoadObject("Graphics\\nball_01.x");
	pGrObject->pEntity = cGameEngine.pPoolBall[1];
	bSuccess &= pGrObject->bInitialized;
	pGrObject = LoadObject("Graphics\\nball_02.x");
	pGrObject->pEntity = cGameEngine.pPoolBall[2];
	bSuccess &= pGrObject->bInitialized;
	pGrObject = LoadObject("Graphics\\nball_03.x");
	pGrObject->pEntity = cGameEngine.pPoolBall[3];
	bSuccess &= pGrObject->bInitialized;
	pGrObject = LoadObject("Graphics\\nball_04.x");
	pGrObject->pEntity = cGameEngine.pPoolBall[4];
	bSuccess &= pGrObject->bInitialized;
	pGrObject = LoadObject("Graphics\\nball_05.x");
	pGrObject->pEntity = cGameEngine.pPoolBall[5];
	bSuccess &= pGrObject->bInitialized;
	pGrObject = LoadObject("Graphics\\nball_06.x");
	pGrObject->pEntity = cGameEngine.pPoolBall[6];
	bSuccess &= pGrObject->bInitialized;
	pGrObject = LoadObject("Graphics\\nball_07.x");
	pGrObject->pEntity = cGameEngine.pPoolBall[7];
	bSuccess &= pGrObject->bInitialized;
	pGrObject = LoadObject("Graphics\\nball_08.x");
	pGrObject->pEntity = cGameEngine.pPoolBall[8];
	bSuccess &= pGrObject->bInitialized;
	pGrObject = LoadObject("Graphics\\nball_09.x");
	pGrObject->pEntity = cGameEngine.pPoolBall[9];
	bSuccess &= pGrObject->bInitialized;
	pGrObject = LoadObject("Graphics\\nball_10.x");
	pGrObject->pEntity = cGameEngine.pPoolBall[10];
	bSuccess &= pGrObject->bInitialized;
	pGrObject = LoadObject("Graphics\\nball_11.x");
	pGrObject->pEntity = cGameEngine.pPoolBall[11];
	bSuccess &= pGrObject->bInitialized;
	pGrObject = LoadObject("Graphics\\nball_12.x");
	pGrObject->pEntity = cGameEngine.pPoolBall[12];
	bSuccess &= pGrObject->bInitialized;
	pGrObject = LoadObject("Graphics\\nball_13.x");
	pGrObject->pEntity = cGameEngine.pPoolBall[13];
	bSuccess &= pGrObject->bInitialized;
	pGrObject = LoadObject("Graphics\\nball_14.x");
	pGrObject->pEntity = cGameEngine.pPoolBall[14];
	bSuccess &= pGrObject->bInitialized;
	pGrObject = LoadObject("Graphics\\nball_15.x");
	pGrObject->pEntity = cGameEngine.pPoolBall[15];
	bSuccess &= pGrObject->bInitialized;
	// load graphics for CUE STICK
	pGrObject = LoadObject("Graphics\\cuestick.x");
	// link to math equivalent of the object
	pGrObject->pEntity = cGameEngine.pCueStick;
	bSuccess &= pGrObject->bInitialized;
	return bSuccess;
}

//-----------------------------------------------------------------------------
// Name: DirectInputSetup()
// Desc: Mouse initialization.
//-----------------------------------------------------------------------------
bool DirectInputSetup()
{
    HRESULT hr;
    BOOL    bExclusive = true;
    BOOL    bForeground = true;
    BOOL    bImmediate = true;
    DWORD   dwCoopFlags;

    if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
                                         IID_IDirectInput8, (VOID**)&g_pDI, NULL ) ) )
        return false;
    // Obtain an interface to the system mouse device.
    if( FAILED( hr = g_pDI->CreateDevice( GUID_SysMouse, &g_pMouse, NULL ) ) )
        return false;
    // Set the data format to "mouse format" - a predefined data format 
    //
    // A data format specifies which controls on a device we
    // are interested in, and how they should be reported.
    //
    // This tells DirectInput that we will be passing a
    // DIMOUSESTATE2 structure to IDirectInputDevice::GetDeviceState.
    if( FAILED( hr = g_pMouse->SetDataFormat( &c_dfDIMouse2 ) ) )
        return false;

    // Set the cooperativity level to let DirectInput know how
    // this device should interact with the system and with other
    // DirectInput applications.
    if( bExclusive )
        dwCoopFlags = DISCL_EXCLUSIVE;
    else
        dwCoopFlags = DISCL_NONEXCLUSIVE;

    if( bForeground )
        dwCoopFlags |= DISCL_FOREGROUND;
    else
        dwCoopFlags |= DISCL_BACKGROUND;
    hr = g_pMouse->SetCooperativeLevel( hWnd, dwCoopFlags );
    if( hr == DIERR_UNSUPPORTED && !bForeground && bExclusive )
    {
        MessageBox( hWnd, _T("SetCooperativeLevel() returned DIERR_UNSUPPORTED.\n")
                          _T("For security reasons, background exclusive mouse\n")
                          _T("access is not allowed."), 
                          _T("Mouse"), MB_OK );
        return false;
    }

    if( FAILED(hr) )
        return false;
    if( !bImmediate )
    {
        // IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
        //
        // DirectInput uses unbuffered I/O (buffer size = 0) by default.
        // If you want to read buffered data, you need to set a nonzero
        // buffer size.
        // The buffer size is a DWORD property associated with the device.
        DIPROPDWORD dipdw;
        dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        dipdw.diph.dwObj        = 0;
        dipdw.diph.dwHow        = DIPH_DEVICE;
        dipdw.dwData            = 12; // Arbitary buffer size

        if( FAILED( hr = g_pMouse->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph ) ) )
            return false;
    }

    // Acquire the newly created device
    g_pMouse->Acquire();

    // Set a timer to go off 12 times a second, to read input
    SetTimer( hWnd, 0, 1000 / 12, NULL );

	return true;
}

//-----------------------------------------------------------------------------
// Name: FreeDirectInput()
// Desc: Initialize the DirectInput variables.
//-----------------------------------------------------------------------------
VOID FreeDirectInput()
{
    // Unacquire the device one last time just in case 
    // the app tried to exit while the device is still acquired.
    if( g_pMouse ) 
        g_pMouse->Unacquire();
    
    // Release any DirectInput objects.
    SAFE_RELEASE( g_pMouse );
    SAFE_RELEASE( g_pDI );
}

//-----------------------------------------------------------------------------
// Name: ReadMouseImmediateData()
// Desc: Return current mouse state (position and button state).
//-----------------------------------------------------------------------------
DIMOUSESTATE2 ReadMouseImmediateData()
{
    HRESULT       hr;
	DIMOUSESTATE2 ms;
    ZeroMemory( &ms, sizeof(ms) );
    if( NULL == g_pMouse ) 
        return ms;
    // Get the input's device state, and put the state in ms
    hr = g_pMouse->GetDeviceState( sizeof(DIMOUSESTATE2), &ms );
    if( FAILED(hr) ) 
    {
        // DirectInput may be telling us that the input stream has been
        // interrupted.  We aren't tracking any state between polls, so
        // we don't have any special reset that needs to be done.
        // We just re-acquire and try again.
        
        // If input is lost then acquire and keep trying 
        hr = g_pMouse->Acquire();
        while( hr == DIERR_INPUTLOST ) 
            hr = g_pMouse->Acquire();
		g_pMouse->GetDeviceState( sizeof(DIMOUSESTATE2), &ms );

        return ms; 
    }

	return ms;
}