/* ******************************************************************************** */
/* DrawingHelper.h                                                                  */
/*                                                                                  */
/* Helper functions for creating DirectX objects from geometry parameters.          */
/*                                                                                  */
/* ******************************************************************************** */
#include "..\SimPool3D\MathObjects.h"

// A structure for our custom vertex type
struct CUSTOMVERTEX
{
    FLOAT x, y, z;		// The untransformed position for the vertex
    DWORD color;        // The vertex color
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

BYTE* CreateRectangle(CMathObjectRect* rect, int *numvertex, int *vertexsize, DWORD *fvf, D3DPRIMITIVETYPE* primitivetype, int *numprimitives, System::Drawing::Color color);
BYTE* CreateCircle(CMathObjectCircle* rect, int *numvertex, int *vertexsize, DWORD *fvf, D3DPRIMITIVETYPE* primitivetype, int *numprimitives);
BYTE* CreateLine(CMathObjectLine* line, int *numvertex, int *vertexsize, DWORD *fvf, D3DPRIMITIVETYPE* primitivetype, int *numprimitives);
