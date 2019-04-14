/* ******************************************************************************** */
/* GrObjects.h                                                                      */
/*                                                                                  */
/* Direct3D encapsulation of objects in game                                        */
/*                                                                                  */
/* ******************************************************************************** */
#include "stdafx.h"

#pragma once

#include "MathObjects.h"


// methods and data required to render a graphical object;
// note: object is linked to another object pertaining to its mathematical data
class	CGrObject
{
private:
	LPD3DXMESH				pMesh;			// mesh object in memory
	D3DMATERIAL9*           pMeshMaterials; // materials for the mesh
	LPDIRECT3DTEXTURE9*     pMeshTextures;	// textures for the mesh
	DWORD                   dwNumMaterials; // number of mesh materials
	LPDIRECT3DVERTEXBUFFER9	pVertexBuffer;  // vertex data in memory (applicable is mesh is not initialized)
	int						iVertexSize;
	int						iNumVertices;
	D3DPRIMITIVETYPE		ePrimitiveType;
	DWORD					fvfFormat;
	int						iNumPrimitives;
	D3DFILLMODE				eFillMode;		// fill mode render state for the object
	D3DCULL					eCull;			// back face culling
public:
	bool					bInitialized;	// set to true after data has been loaded
	CMathObject*			pEntity;		// pointer to the matching mathematical object

	CGrObject(LPSTR pFilename);				// class constructor which loads a mesh from a file into memory
	CGrObject(LPD3DXMESH pmesh, char* texturefname);
	CGrObject(BYTE* buffer, int numvertices, int vertexsize, DWORD fvf, D3DPRIMITIVETYPE primitivetype, int numprimitives);
	bool					GetBoundingBox(D3DXVECTOR3 *lowerleft, D3DXVECTOR3 *upperright); // return bounding box dimensions, either from mesh or from private vertex buffer
	bool					Render();
	void					SetFillMode(D3DFILLMODE fillmode);
	D3DFILLMODE				GetFillMode();
	void					SetCull(D3DCULL cull);
	D3DCULL					GetCull();
	void					Release();		// release data from memory

	
};
