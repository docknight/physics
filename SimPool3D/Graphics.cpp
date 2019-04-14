/* ******************************************************************************** */
/* Graphics.cpp                                                                     */
/*                                                                                  */
/* Direct3D initialization and rendering routines                                   */
/*                                                                                  */
/* ******************************************************************************** */
#include "stdafx.h"
#include "Graphics.h"
#include "dxutil.h"
#include "d3denumeration.h"

#define MOUSESENSITIVITY	300		// resistance to rotation and cue movement

//-----------------------------------------------------------------------------
// Global external variables
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D           = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pD3DDevice     = NULL; // Our rendering device

D3DXVECTOR3				g_vEyePt;	 // view transformation vectors
D3DXVECTOR3				g_vLookatPt; // view transformation vectors
D3DXVECTOR3				g_vUpVec;    // view transformation vectors
float					g_fCameraRadius;// distance between the eye point and look-at point

//-----------------------------------------------------------------------------
// Local variables
//-----------------------------------------------------------------------------

GrObjectListPtr pGrObjectList;

CD3DEnumeration*	d3denumeration;



//-----------------------------------------------------------------------------
// Name: DisplayGrErrorMsg()
// Desc: Displays error messages in a message box
//-----------------------------------------------------------------------------
void DisplayGrErrorMsg( HRESULT hr)
{
    TCHAR strMsg[512];
    switch( hr )
    {
        case D3DAPPERR_NODIRECT3D:
            _tcscpy( strMsg, _T("Could not initialize Direct3D. You may\n")
                             _T("want to check that the latest version of\n")
                             _T("DirectX is correctly installed on your\n")
                             _T("system.") );
            break;

        case D3DAPPERR_NOCOMPATIBLEDEVICES:
            _tcscpy( strMsg, _T("Could not find any compatible Direct3D\n")
                             _T("devices.") );
            break;

        case D3DAPPERR_NOWINDOWABLEDEVICES:
            _tcscpy( strMsg, _T("This application cannot run in a desktop\n")
                             _T("window with the current display settings.\n")
                             _T("Please change your desktop settings to a\n")
                             _T("16- or 32-bit display mode and re-run the\n")
                             _T("application.") );
            break;

        case D3DAPPERR_NOHARDWAREDEVICE:
            _tcscpy( strMsg, _T("No hardware-accelerated Direct3D devices\n")
                             _T("were found.") );
            break;

        case D3DAPPERR_HALNOTCOMPATIBLE:
            _tcscpy( strMsg, _T("The application requires functionality that is\n")
                             _T("not available on your Direct3D hardware\n")
                             _T("accelerator.") );
            break;

        case D3DAPPERR_NOWINDOWEDHAL:
            _tcscpy( strMsg, _T("Your Direct3D hardware accelerator cannot\n")
                             _T("render into a window.") );
            break;

        case D3DAPPERR_NODESKTOPHAL:
            _tcscpy( strMsg, _T("Your Direct3D hardware accelerator cannot\n")
                             _T("render into a window with the current\n")
                             _T("desktop display settings.") );
            break;

        case D3DAPPERR_NOHALTHISMODE:
            _tcscpy( strMsg, _T("This application requires functionality that is\n")
                             _T("not available on your Direct3D hardware\n")
                             _T("accelerator with the current desktop display\n")
                             _T("settings.") );
            break;

        case D3DAPPERR_MEDIANOTFOUND:
        case HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND ):
            _tcscpy( strMsg, _T("Could not load required media." ) );
            break;

        case D3DAPPERR_RESETFAILED:
            _tcscpy( strMsg, _T("Could not reset the Direct3D device." ) );
            break;

        case D3DAPPERR_NONZEROREFCOUNT:
            _tcscpy( strMsg, _T("A D3D object has a non-zero reference\n")
                             _T("count (meaning things were not properly\n")
                             _T("cleaned up).") );
            break;

        case D3DAPPERR_NULLREFDEVICE:
            _tcscpy( strMsg, _T("Warning: Nothing will be rendered.\n")
                             _T("The reference rendering device was selected, but your\n")
                             _T("computer only has a reduced-functionality reference device\n")
                             _T("installed.  Install the DirectX SDK to get the full\n")
                             _T("reference device.\n") );
            break;

        case E_OUTOFMEMORY:
            _tcscpy( strMsg, _T("Not enough memory.") );
            break;

        case D3DERR_OUTOFVIDEOMEMORY:
            _tcscpy( strMsg, _T("Not enough video memory.") );
            break;

        case D3DERR_DRIVERINTERNALERROR:
            _tcscpy( strMsg, _T("A serious problem occured inside the display driver.") );
            break;

        default:
            _tcscpy( strMsg, _T("Generic application error. Enable\n")
                             _T("debug output for detailed information.") );
    }

    MessageBox( NULL, strMsg, "DirectX initialization error", MB_ICONERROR|MB_OK );
}


//-----------------------------------------------------------------------------
// Name: InitGraphics()
// Desc: Initialize graphics module.
//-----------------------------------------------------------------------------
void InitGraphics()
{
	// initialize object list
	pGrObjectList = (GrObjectListPtr)malloc(sizeof(GrObjectList));
	pGrObjectList->pGrObject = NULL;
	pGrObjectList->next = NULL;
	g_fCameraRadius = DEF_CAMERARADIUS;
	d3denumeration = new CD3DEnumeration();
}

//-----------------------------------------------------------------------------
// Name: CleanupGraphics()
// Desc: Release all objects in the graphical objects list and release the list itself.
//       Also, release D3D objects       
//-----------------------------------------------------------------------------
void CleanupGraphics()
{
	GrObjectListPtr objlist = pGrObjectList;
	GrObjectListPtr objlistnext;
	while (objlist != NULL)
	{
		objlistnext = objlist->next;
		// release object data
		objlist->pGrObject->Release();
		// release list element
		free(objlist);
		objlist = objlistnext;
	}

	delete d3denumeration;

	if( g_pD3DDevice != NULL )
        g_pD3DDevice->Release();

    if( g_pD3D != NULL )
        g_pD3D->Release();
}

//-----------------------------------------------------------------------------
// Name: BuildPresentParamsFromSettings()
// Desc:
//-----------------------------------------------------------------------------
D3DPRESENT_PARAMETERS BuildPresentParamsFromSettings(HWND hWnd)
{
	D3DPRESENT_PARAMETERS d3dpp;
    d3dpp.Windowed               = d3denumeration->d3dSettings->IsWindowed;
    d3dpp.BackBufferCount        = 1;
    d3dpp.MultiSampleType        = d3denumeration->d3dSettings->MultisampleType();
    d3dpp.MultiSampleQuality     = d3denumeration->d3dSettings->MultisampleQuality();
    d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    d3dpp.EnableAutoDepthStencil = d3denumeration->AppUsesDepthBuffer;
    d3dpp.hDeviceWindow          = hWnd;
    if( d3denumeration->AppUsesDepthBuffer )
    {
        d3dpp.Flags              = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
        d3dpp.AutoDepthStencilFormat = d3denumeration->d3dSettings->DepthStencilBufferFormat();
    }
    else
    {
        d3dpp.Flags             = 0;
    }

    if( d3dpp.Windowed )
    {
		RECT rcWindowClient;
		GetClientRect( hWnd, &rcWindowClient );
        d3dpp.BackBufferWidth  = rcWindowClient.right - rcWindowClient.left;
        d3dpp.BackBufferHeight = rcWindowClient.bottom - rcWindowClient.top;
        d3dpp.BackBufferFormat = d3denumeration->d3dSettings->PDeviceCombo()->BackBufferFormat;
        d3dpp.FullScreen_RefreshRateInHz = 0;
        d3dpp.PresentationInterval = d3denumeration->d3dSettings->PresentInterval();
    }
    else
    {
        d3dpp.BackBufferWidth  = d3denumeration->d3dSettings->DisplayMode().Width;
        d3dpp.BackBufferHeight = d3denumeration->d3dSettings->DisplayMode().Height;
        d3dpp.BackBufferFormat = d3denumeration->d3dSettings->PDeviceCombo()->BackBufferFormat;
        d3dpp.FullScreen_RefreshRateInHz = d3denumeration->d3dSettings->Fullscreen_DisplayMode.RefreshRate;
        d3dpp.PresentationInterval = d3denumeration->d3dSettings->PresentInterval();
    }
	return d3dpp;
}

//-----------------------------------------------------------------------------
// Name: Initialize3DEnvironment()
// Desc:
//-----------------------------------------------------------------------------
HRESULT Initialize3DEnvironment(HWND hWnd)
{
	HRESULT hr;
	D3DPRESENT_PARAMETERS d3dpp; 
	d3dpp = BuildPresentParamsFromSettings(hWnd);

    if( d3denumeration->d3dSettings->PDeviceInfo()->Caps.PrimitiveMiscCaps & D3DPMISCCAPS_NULLREFERENCE )
    {
        // Warn user about null ref device that can't render anything
        DisplayGrErrorMsg( D3DAPPERR_NULLREFDEVICE );
    }

    DWORD behaviorFlags;
    if (d3denumeration->d3dSettings->GetVertexProcessingType() == SOFTWARE_VP)
        behaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    else if (d3denumeration->d3dSettings->GetVertexProcessingType() == MIXED_VP)
        behaviorFlags = D3DCREATE_MIXED_VERTEXPROCESSING;
    else if (d3denumeration->d3dSettings->GetVertexProcessingType() == HARDWARE_VP)
        behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    else if (d3denumeration->d3dSettings->GetVertexProcessingType() == PURE_HARDWARE_VP)
        behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
    else
        behaviorFlags = 0; // TODO: throw exception

    // Create the device
    hr = g_pD3D->CreateDevice( d3denumeration->d3dSettings->AdapterOrdinal(), d3denumeration->d3dSettings->PDeviceInfo()->DevType,
                               hWnd, behaviorFlags, &d3dpp,
                               &g_pD3DDevice );

    if( SUCCEEDED(hr) )
    {
        // Set up the fullscreen cursor
        if( !d3denumeration->d3dSettings->IsWindowed )
        {
			//TODO: re-enable this
//            HCURSOR hCursor;
//#ifdef _WIN64
//            hCursor = (HCURSOR)GetClassLongPtr( m_hWnd, GCLP_HCURSOR );
//#else
//            hCursor = (HCURSOR)ULongToHandle( GetClassLong( m_hWnd, GCL_HCURSOR ) );
//#endif
//            D3DUtil_SetDeviceCursor( m_pd3dDevice, hCursor, true );
//            m_pd3dDevice->ShowCursor( true );
        }

        // Confine cursor to fullscreen window
        //if( m_bClipCursorWhenFullscreen )
        {
            if (!d3denumeration->d3dSettings->IsWindowed )
            {
                RECT rcWindow;
                GetWindowRect( hWnd, &rcWindow );
                ClipCursor( &rcWindow );
            }
            else
            {
                ClipCursor( NULL );
            }
        }
    }

	else

	{
		// If that failed, fall back to the reference rasterizer
		if( hr != D3DAPPERR_MEDIANOTFOUND && 
			hr != HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND ) && 
			d3denumeration->d3dSettings->PDeviceInfo()->DevType == D3DDEVTYPE_HAL )
		{
			if (d3denumeration->FindBestWindowedMode(false, true, hWnd))
			{
				d3denumeration->d3dSettings->IsWindowed = true;
				RECT rcWindowBounds;
				GetWindowRect( hWnd, &rcWindowBounds );
				SetWindowPos( hWnd, HWND_NOTOPMOST,
							rcWindowBounds.left, rcWindowBounds.top,
							( rcWindowBounds.right - rcWindowBounds.left ),
							( rcWindowBounds.bottom - rcWindowBounds.top ),
							SWP_SHOWWINDOW );

				// Let the user know we are switching from HAL to the reference rasterizer
//				DisplayGrErrorMsg( hr );
				MessageBox(hWnd, "Switched to reference rasterizer.", "Performance warning", MB_OK|MB_ICONEXCLAMATION);
				hr = Initialize3DEnvironment(hWnd);
			}
		}
	}
	return hr;
}

//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D( HWND hWnd, bool bFullscreen )
{
	HRESULT hr;
    // Create the D3D object.
    if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		DisplayGrErrorMsg(D3DAPPERR_NODIRECT3D);
        return E_FAIL;
	}

	d3denumeration->SetD3D( g_pD3D );
    d3denumeration->ConfirmDeviceCallback = NULL;
	d3denumeration->AppRequiresWindowed = !bFullscreen;
	d3denumeration->AppRequiresFullscreen = bFullscreen;
	d3denumeration->AppUsesDepthBuffer = true;
	// enumerate compatible devices
    if( FAILED( hr = d3denumeration->Enumerate() ) )
    {
        SAFE_RELEASE( g_pD3D );
		DisplayGrErrorMsg(hr);
        return E_FAIL;
    }
	bool bFound;
	// search for the best matching device
	if (bFullscreen)
	{
		bFound = d3denumeration->FindBestFullscreenMode(false, false);
		d3denumeration->d3dSettings->IsWindowed = false;
	}
	else
	{
		bFound = d3denumeration->FindBestWindowedMode(false, false, hWnd);
		d3denumeration->d3dSettings->IsWindowed = true;
	}
	// TODO: when full screen mode is selected, might satisfy with windowed mode if full screen initialization fails
	if (!bFound)
	{
		DisplayGrErrorMsg(D3DAPPERR_NOCOMPATIBLEDEVICES);
		return E_FAIL;
	}

    // Initialize the 3D environment for the app
    if( FAILED( hr = Initialize3DEnvironment(hWnd) ) )
    {
        SAFE_RELEASE( g_pD3D );
        DisplayGrErrorMsg( hr );
		return E_FAIL;
    }


    // Turn on the zbuffer
    g_pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

	// setup default transformations
	SetupDefMatrices();

	// setup our lights
	SetupLights();

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: SetupLights()
// Desc: Sets up the lights for the scene.
//-----------------------------------------------------------------------------
VOID SetupLights()
{
    D3DXVECTOR3 vecDir;
    D3DLIGHT9 light;
    ZeroMemory( &light, sizeof(D3DLIGHT9) );
    light.Type       = D3DLIGHT_DIRECTIONAL;
    light.Diffuse.r  = 1.0f;
    light.Diffuse.g  = 1.0f;
    light.Diffuse.b  = 1.0f;
    vecDir = D3DXVECTOR3(0.0f, -1.0f, 1.0f);
    D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecDir );
    light.Range       = 1000.0f;
    g_pD3DDevice->SetLight( 0, &light );
    g_pD3DDevice->LightEnable( 0, TRUE );
    g_pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

    // Finally, turn on some ambient light.
    g_pD3DDevice->SetRenderState( D3DRS_AMBIENT, 0x00202020 );
}


//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Render the scene
//-----------------------------------------------------------------------------
HRESULT Render()
{
    // Clear the backbuffer and the zbuffer
    g_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
                         D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

	// set view matrix according to current camera position
	D3DXMATRIXA16 matView;
    D3DXMatrixLookAtLH( &matView, &g_vEyePt, &g_vLookatPt, &g_vUpVec );
    g_pD3DDevice->SetTransform( D3DTS_VIEW, &matView );
    
    // Begin the scene
    if( SUCCEEDED( g_pD3DDevice->BeginScene() ) )
    {
		// go through the object list and render each object
		GrObjectListPtr objlist;
		objlist = pGrObjectList;
		while (objlist != NULL)
		{
			if (objlist->pGrObject != NULL)
				objlist->pGrObject->Render();
			objlist = objlist->next;
		}
        // End the scene
        g_pD3DDevice->EndScene();
	}
	else
		return E_FAIL;

    // Present the backbuffer contents to the display
    g_pD3DDevice->Present( NULL, NULL, NULL, NULL );

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: SetupDefMatrices()
// Desc: Sets up the default world, view, and projection transform matrices.
//-----------------------------------------------------------------------------
VOID SetupDefMatrices()
{
    // For our world matrix, we will just leave it as the identity
    D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity( &matWorld );
    //D3DXMatrixRotationY( &matWorld, timeGetTime()/1000.0f );
    g_pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );

    // Set up our view matrix. A view matrix can be defined given an eye point,
    // a point to look at, and a direction for which way is up. 
	// note, that distance between the eye point and the look-at point must equal
	// g_fCameraRadius when initialized here, otherwise later calculations will be incorrect;
    g_vEyePt.x = 0.0f;
	g_vEyePt.y = 0.0f;
	g_vEyePt.z = -g_fCameraRadius;
	g_vLookatPt.x = 0.0f;
	g_vLookatPt.y = 0.0f;
	g_vLookatPt.z = 0.0f;
	g_vUpVec.x = g_vUpVec.z = 0.0f;
	g_vUpVec.y = 1.0f;
    D3DXMATRIXA16 matView;
    D3DXMatrixLookAtLH( &matView, &g_vEyePt, &g_vLookatPt, &g_vUpVec );
    g_pD3DDevice->SetTransform( D3DTS_VIEW, &matView );

    // For the projection matrix, we set up a perspective transform (which
    // transforms geometry from 3D view space to 2D viewport space, with
    // a perspective divide making objects smaller in the distance). To build
    // a perpsective transform, we need the field of view (1/4 pi is common),
    // the aspect ratio, and the near and far clipping planes (which define at
    // what distances geometry should be no longer be rendered).
    D3DXMATRIXA16 matProj;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 1.0f, 1.0f, 100.0f );
    g_pD3DDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}


//-----------------------------------------------------------------------------
// Name: LoadObject(pFilename)
// Desc: Create a new instance of the CGrObject class and add it into object list.
//       Pointer to the new object is returned
//-----------------------------------------------------------------------------
CGrObject* LoadObject(LPSTR pFilename)
{
	GrObjectListPtr objlist;
	objlist = pGrObjectList;
	// seek to the end of the object list
	while (objlist->next != NULL)
		objlist = objlist->next;
	// we add a new element, unless this is the first object, in which case the element has already been created
	if (objlist->pGrObject != NULL)
	{
		objlist->next = (GrObjectListPtr)malloc(sizeof(GrObjectList));
		objlist = objlist->next;
		objlist->next = NULL;
	}
	// load the object and store its reference into the list
	objlist->pGrObject = new CGrObject(pFilename);

	return objlist->pGrObject;
}

//-----------------------------------------------------------------------------
// Name: AddObject(pobject)
// Desc: Add an initialized object to the object list.
//-----------------------------------------------------------------------------
void AddObject(CGrObject* pobject)
{
	GrObjectListPtr objlist;
	objlist = pGrObjectList;
	// seek to the end of the object list
	while (objlist->next != NULL)
		objlist = objlist->next;
	// we add a new element, unless this is the first object, in which case the element has already been created
	if (objlist->pGrObject != NULL)
	{
		objlist->next = (GrObjectListPtr)malloc(sizeof(GrObjectList));
		objlist = objlist->next;
		objlist->next = NULL;
	}
	// store new object reference into the list
	objlist->pGrObject = pobject;
}

//-----------------------------------------------------------------------------
// Name: ReleaseObject(pobject)
// Desc: Release object from memory and remove it from the list.
//-----------------------------------------------------------------------------
void ReleaseObject(CGrObject* pobject)
{
	if (pobject == NULL)
		return;
	GrObjectListPtr objlist;
	objlist = pGrObjectList;
	// seek to the end of the object list
	while (objlist->next != NULL)
	{
		if (objlist->next->pGrObject == pobject)
			break;
		if (objlist->pGrObject == pobject)
			break;
		objlist = objlist->next;
	}
	if (objlist->next != NULL && objlist->next->pGrObject == pobject)
	{
		// release grobject
		objlist->next->pGrObject->Release();
		GrObjectListPtr oldobj = objlist->next;
		// re-link to next element
		objlist->next = objlist->next->next;
		free(oldobj);
	}
	else if (objlist->pGrObject == pobject) // object to be removed is the first in the list
	{
		objlist->pGrObject->Release();
		pGrObjectList = objlist->next; // new list beginning
		free(objlist);
		if (pGrObjectList == NULL)
		{
			pGrObjectList = (GrObjectListPtr)malloc(sizeof(GrObjectList));
			pGrObjectList->pGrObject = NULL;
			pGrObjectList->next = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// Name: RotateCameraY(ofs)
// Desc: Rotate camera around the world Y axis.
//-----------------------------------------------------------------------------
void RotateCameraY(LONG ofs)
{
	D3DXVECTOR3 v3; // vector from lookat point back to the camera eye point
	D3DXVECTOR3 r3; // rotation axis vector
	D3DXQUATERNION r,rn,rnc, q;
	v3 = g_vEyePt - g_vLookatPt;
	r3.x = 0; 
	r3.y = 1;
	r3.z = 0;
	D3DXQuaternionRotationAxis(&r, &r3, (float)ofs/MOUSESENSITIVITY); // rotational quaternion
	q.x = v3.x;		// quaternion of the vector to rotate
	q.y = v3.y;		// quaternion of the vector to rotate
	q.z = v3.z;		// quaternion of the vector to rotate
	q.w = 0;
	D3DXQuaternionNormalize(&rn, &r);
	D3DXQuaternionConjugate(&rnc, &rn);
	// quaternion rotation: q_new = rn * q * rn'
	D3DXQuaternionMultiply(&rn, &rn, &q);
	D3DXQuaternionMultiply(&q, &rn, &rnc);
	// q now holds the rotated v3 vector; now, apply new camera eye point vector
	v3.x = q.x;
	v3.y = q.y;
	v3.z = q.z;
	g_vEyePt = g_vLookatPt + v3;
}

//-----------------------------------------------------------------------------
// Name: RotateCameraXZ(ofs)
// Desc: Rotate camera around the axis which lies in the world XZ plane.
//-----------------------------------------------------------------------------
void RotateCameraXZ(LONG ofs)
{
	D3DXVECTOR3 v3, v3r; // vector from lookat point back to the camera eye point
	D3DXVECTOR3 r3; // rotation axis vector
	D3DXQUATERNION r,rn,rnc, q;
	v3 = g_vEyePt - g_vLookatPt;
	// rotation axis is perpendicular to the camera axis and camera up vector
	D3DXVec3Cross(&r3, &v3, &g_vUpVec);

	D3DXQuaternionRotationAxis(&r, &r3, (float)ofs/MOUSESENSITIVITY); // rotational quaternion
	q.x = v3.x;		// quaternion of the vector to rotate
	q.y = v3.y;		// quaternion of the vector to rotate
	q.z = v3.z;		// quaternion of the vector to rotate
	q.w = 0;
	D3DXQuaternionNormalize(&rn, &r);
	D3DXQuaternionConjugate(&rnc, &rn);
	// quaternion rotation: q_new = rn * q * rn'
	D3DXQuaternionMultiply(&rn, &rn, &q);
	D3DXQuaternionMultiply(&q, &rn, &rnc);
	// q now holds the rotated v3 vector; now, apply new camera eye point vector
	v3r.x = q.x;
	v3r.y = q.y;
	v3r.z = q.z;
	g_vEyePt = g_vLookatPt + v3r;

	// make sure our view is not flipped over; 
	// this is done by first rotating the XZ coordinate system in the direction perpendicular to
	// rotation axis; if the sign of the previous eye point and the rotated one in this
	// coordinate system is reversed, we have a flip over, which we do not let happen;
	// note(?), that if the camera look-at point had not been at the world center, the coordinate
	// system had to be translated in addition to the rotation, otherwise sign would not be
	// reversed at the rotation peak
	D3DXVECTOR3 vAxis;
	D3DXVec3Cross(&vAxis, &r3, &g_vUpVec);
	if ( ( D3DXVec3Dot(&v3,&vAxis) < 0 && D3DXVec3Dot(&v3r,&vAxis) >= 0 ) ||
		 ( D3DXVec3Dot(&v3,&vAxis) >= 0 && D3DXVec3Dot(&v3r,&vAxis) < 0 ) )
	{
		g_vEyePt = g_vLookatPt + v3; // restore old eye point
		// TODO: instead of restoring old eye point, calculate the eye point exactly at the rotation peak
		// also, apply an additional limit when rotating under the pool table
	}
}


//-----------------------------------------------------------------------------
// Name: ZoomCamera(ofs)
// Desc: Zoom camera by moving the eye point in the camera axis direction.
//-----------------------------------------------------------------------------
void ZoomCamera(LONG ofs)
{
	D3DXVECTOR3 v3,v3n; // vector from lookat point back to the camera eye point (camera axis)
	v3 = g_vEyePt - g_vLookatPt;
	D3DXVec3Normalize(&v3n, &v3);
	// move in/out the camera eye point in the direction of current camera axis
	v3n = ((float)ofs/MOUSESENSITIVITY) * v3n;
	g_vEyePt.x += v3n.x;
	g_vEyePt.y += v3n.y;
	g_vEyePt.z += v3n.z;
}
