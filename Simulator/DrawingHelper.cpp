#include "stdafx.h"
#include "DrawingHelper.h"


//-----------------------------------------------------------------------------
// Name: CreateRectangle(rect, numvertex, vertexsize, fvf, primitivetype)
// Desc: Create a vertex buffer defining a rectangle which matches the defined
//       rectangle in CMathObjectRect instance; new buffer of data is returned.
// Par.: rect... CMathObjectRect instance
//       numvertex... number of vertices in the created buffer
//       vertexsize... size of a custom vertex in bytes
//       fvf... custom vertex format
//		 primitivetype... type of vertex data (triangle list, line list, point list, etc.)
//       numprimitives... number of triangles in the created triangle strip
//-----------------------------------------------------------------------------
BYTE* CreateRectangle(CMathObjectRect* rect, int *numvertex, int *vertexsize, DWORD *fvf, D3DPRIMITIVETYPE* primitivetype, int *numprimitives, System::Drawing::Color color)
{
	CUSTOMVERTEX *buffer = (CUSTOMVERTEX*)malloc(sizeof(CUSTOMVERTEX)*4);
	DWORD r,g,b;
	r = ((byte)color.R) << 16;
	g = ((byte)color.G) << 8;
	b = (byte)color.B;
	int i;
	for (i = 0; i < 4; i++)
		buffer[i].color = 0xff000000 + r + g + b;//0xffff0000;
	// get rectangle plane perpendicular vectors in body coordinates (unrotated)
	D3DXVECTOR3 vLength = rect->GetLengthVector(false);
	D3DXVECTOR3 vWidth = rect->GetWidthVector(false);
	float fLength = rect->GetLength();
	float fWidth = rect->GetWidth();
	// now create the four vertices, forming two triangles of a rectangle centered at
	// world origin;
	D3DXVECTOR3 v;
	v = D3DXVECTOR3(0,0,0) - (vLength*fLength/2) - (vWidth*fWidth/2);
	buffer[0].x = v.x;
	buffer[0].y = v.y;
	buffer[0].z = v.z;
	v = D3DXVECTOR3(0,0,0) - (vLength*fLength/2) + (vWidth*fWidth/2);
	buffer[1].x = v.x;
	buffer[1].y = v.y;
	buffer[1].z = v.z;
	v = D3DXVECTOR3(0,0,0) + (vLength*fLength/2) - (vWidth*fWidth/2);
	buffer[2].x = v.x;
	buffer[2].y = v.y;
	buffer[2].z = v.z;
	v = D3DXVECTOR3(0,0,0) + (vLength*fLength/2) + (vWidth*fWidth/2);
	buffer[3].x = v.x;
	buffer[3].y = v.y;
	buffer[3].z = v.z;

	*primitivetype = D3DPT_TRIANGLESTRIP;
	*numvertex = 4;
	*vertexsize = sizeof(CUSTOMVERTEX);
	*fvf = D3DFVF_CUSTOMVERTEX;
	*numprimitives = 2; // two triangles form a rectangle
	return (BYTE*)buffer;
}

//-----------------------------------------------------------------------------
// Name: CreateCircle(circle, numvertex, vertexsize, fvf, primitivetype)
// Desc: Create a vertex buffer defining a circle which matches the defined
//       circle in CMathObjectCircle instance; new buffer of data is returned.
// Par.: circle... CMathObjectCircle instance
//       numvertex... number of vertices in the created buffer
//       vertexsize... size of a custom vertex in bytes
//       fvf... custom vertex format
//		 primitivetype... type of vertex data (triangle list, line list, point list, etc.)
//       numprimitives... number of triangles in the created triangle strip
//-----------------------------------------------------------------------------
BYTE* CreateCircle(CMathObjectCircle* circle, int *numvertex, int *vertexsize, DWORD *fvf, D3DPRIMITIVETYPE* primitivetype, int *numprimitives)
{
	*numvertex = 361; // number of vertices in a circle 
	CUSTOMVERTEX *buffer = (CUSTOMVERTEX*)malloc(sizeof(CUSTOMVERTEX)*(*numvertex));
	int i;
	for (i = 0; i < *numvertex; i++)
		buffer[i].color = 0xff00ff00;
	// calculate two vectors perpendicular to circle normal vector; note, that there
	// is an infinite amount of these vector pairs
	D3DXVECTOR3 v1,v2;
	D3DXVec3Cross(&v1, &(circle->GetNormal(false)), &(D3DXVECTOR3(1,0,0)));
	if (D3DXVec3Length(&v1) < 0.001f)
	{
		D3DXVec3Cross(&v1, &(circle->GetNormal(false)), &(D3DXVECTOR3(0,1,0)));
		if (D3DXVec3Length(&v1) < 0.001f)
		{
			D3DXVec3Cross(&v1, &(circle->GetNormal(false)), &(D3DXVECTOR3(0,0,1)));
		}
	}
	// found one perpendicular vector, now calculate the other one
	D3DXVec3Cross(&v2, &v1, &(circle->GetNormal(false)));
	D3DXVec3Normalize(&v1, &v1);
	D3DXVec3Normalize(&v2, &v2);

	D3DXVECTOR3 v;
	// 1st vertex is the sphere center
	v = D3DXVECTOR3(0,0,0);
	buffer[0].x = v.x;
	buffer[0].y = v.y;
	buffer[0].z = v.z;
	// remaining vertices are spanned equidistantly around the center
	for (i = 1; i < *numvertex; i++)
	{
		float ratio = ((float)(i-1.0f)/((float)*numvertex-2.0f)) * 2*D3DX_PI; // position of the vertex in spherical parametrized coordinate
		v = v2*cos(ratio) * circle->GetRadius();
		v += v1*sin(ratio) * circle->GetRadius();
		buffer[i].x = v.x;
		buffer[i].y = v.y;
		buffer[i].z = v.z;
	}

	*primitivetype = D3DPT_TRIANGLEFAN;
	*vertexsize = sizeof(CUSTOMVERTEX);
	*fvf = D3DFVF_CUSTOMVERTEX;
	*numprimitives = *numvertex - 2;
	return (BYTE*)buffer;
}


//-----------------------------------------------------------------------------
// Name: CreateLine(line, numvertex, vertexsize, fvf, primitivetype)
// Desc: Create a vertex buffer defining a line which matches the defined
//       line in CMathObjectLine instance; new buffer of data is returned.
// Par.: line... CMathObjectLine instance
//       numvertex... number of vertices in the created buffer
//       vertexsize... size of a custom vertex in bytes
//       fvf... custom vertex format
//		 primitivetype... type of vertex data (triangle list, line list, point list, etc.)
//       numprimitives... number of triangles in the created triangle strip
//-----------------------------------------------------------------------------
BYTE* CreateLine(CMathObjectLine* line, int *numvertex, int *vertexsize, DWORD *fvf, D3DPRIMITIVETYPE* primitivetype, int *numprimitives)
{
	*numvertex = 4;
	CUSTOMVERTEX *buffer = (CUSTOMVERTEX*)malloc(sizeof(CUSTOMVERTEX)*(*numvertex));
	int i;
	for (i = 0; i < *numvertex; i++)
		buffer[i].color = 0xffff0000;
	// now create the two vertices, forming a line starting at
	// world origin;
	buffer[0].x = 0;
	buffer[0].y = 0;
	buffer[0].z = 0;
	D3DXVECTOR3 v;
	v = D3DXVECTOR3(0,0,0) + line->GetDirection()*line->GetMagnitude(); 
	buffer[1].x = v.x;
	buffer[1].y = v.y;
	buffer[1].z = v.z;

	*primitivetype = D3DPT_LINELIST;
	*vertexsize = sizeof(CUSTOMVERTEX);
	*fvf = D3DFVF_CUSTOMVERTEX;
	*numprimitives = 1;

	return (BYTE*)buffer;
}