/* ******************************************************************************** */
/* Graphics.h                                                                       */
/*                                                                                  */
/* Direct3D initialization and rendering routines                                   */
/*                                                                                  */
/* ******************************************************************************** */
#include "stdafx.h"
#include "GrObjects.h"

//-----------------------------------------------------------------------------
// Global defines
//-----------------------------------------------------------------------------
#define DEF_CAMERARADIUS	8.0f	// default camera distance from the table
#define FVF_VERTEX			D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1

// error codes
#define D3DAPPERR_NODIRECT3D          0x82000001
#define D3DAPPERR_NOWINDOW            0x82000002
#define D3DAPPERR_NOCOMPATIBLEDEVICES 0x82000003
#define D3DAPPERR_NOWINDOWABLEDEVICES 0x82000004
#define D3DAPPERR_NOHARDWAREDEVICE    0x82000005
#define D3DAPPERR_HALNOTCOMPATIBLE    0x82000006
#define D3DAPPERR_NOWINDOWEDHAL       0x82000007
#define D3DAPPERR_NODESKTOPHAL        0x82000008
#define D3DAPPERR_NOHALTHISMODE       0x82000009
#define D3DAPPERR_NONZEROREFCOUNT     0x8200000a
#define D3DAPPERR_MEDIANOTFOUND       0x8200000b
#define D3DAPPERR_RESETFAILED         0x8200000c
#define D3DAPPERR_NULLREFDEVICE       0x8200000d



//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
extern LPDIRECT3D9             g_pD3D; // Used to create the D3DDevice
extern LPDIRECT3DDEVICE9       g_pD3DDevice; // Our rendering device

// camera vectors; eye point and look-at point together form the so-called camera axis
// camera is rotated by moving the eye point, i.e. rotating it around the look-at point;
// camera is moved by moving both, eye point and the look-at point;
// camera zoom in/out is achieved by closing in/out the eye point relative to look-at point;
extern D3DXVECTOR3				g_vEyePt;		// view transformation vectors
extern D3DXVECTOR3				g_vLookatPt;	// view transformation vectors
extern D3DXVECTOR3				g_vUpVec;		// view transformation vectors
extern float					g_fCameraRadius;// distance between the eye point and look-at point

//-----------------------------------------------------------------------------
// Local variables
//-----------------------------------------------------------------------------

typedef struct GrObjectList* GrObjectListPtr;
// graphical object list
struct GrObjectList
{
	CGrObject		*pGrObject;
	GrObjectListPtr	next;
};

struct _VERTEX
{
    D3DXVECTOR3 pos;     // vertex position
    D3DXVECTOR3 norm;    // vertex normal
    float tu;            // texture coordinates
    float tv;
};



//-----------------------------------------------------------------------------
// function declaration
//-----------------------------------------------------------------------------
void		InitGraphics();					// initialize graphics module
void		CleanupGraphics();				// release all objects in the graphical objects list
HRESULT		InitD3D( HWND hWnd, bool bFullscreen  ); // initialize Direct3D object and device
VOID		SetupLights();					// called in InitD3D
HRESULT		Render( );						// render the scene
VOID		SetupDefMatrices();				// Sets up the default world, view, and projection transform matrices.
CGrObject*	LoadObject(LPSTR pFilename);    // create a new instance of the CGrObject class and add it into object list
void		AddObject(CGrObject* pobject);  // add an initialized object to the object list
void		ReleaseObject(CGrObject* pobject); // release object from memory and remove it from the list
void		RotateCameraY(LONG ofs);		// rotate camera around the world Y axis
void		RotateCameraXZ(LONG ofs);		// rotate camera around the world X axis
void		ZoomCamera(LONG ofs);			// zoom camera by moving in the camera axis direction


