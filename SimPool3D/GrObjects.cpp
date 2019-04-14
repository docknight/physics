/* ******************************************************************************** */
/* GrObjects.cpp                                                                    */
/*                                                                                  */
/* Direct3D encapsulation of objects in game                                        */
/*                                                                                  */
/* ******************************************************************************** */
#include "stdafx.h"
#include "GrObjects.h"
#include "Graphics.h"


//-----------------------------------------------------------------------------
// Name: CGrObject()
// Desc: class constructor; load object vertices and textures;
//-----------------------------------------------------------------------------
CGrObject::CGrObject(LPSTR pFilename)
{
    LPD3DXBUFFER pD3DXMtrlBuffer;
	pVertexBuffer = NULL;
	eFillMode = D3DFILL_SOLID;
	eCull = D3DCULL_CCW; // TODO: get the default cull direction from graphics initialization

    if( FAILED( D3DXLoadMeshFromX( pFilename, /*D3DXMESH_SYSTEMMEM*/NULL, 
                                   g_pD3DDevice, NULL, 
                                   &pD3DXMtrlBuffer, NULL, &dwNumMaterials, 
                                   &pMesh ) ) )
    {
        MessageBox(NULL, "Can not find object", "Simulation Pool", MB_OK);
        bInitialized = false;
		return;
    }
    // We need to extract the material properties and texture names from the 
    // pD3DXMtrlBuffer
    D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    pMeshMaterials = new D3DMATERIAL9[dwNumMaterials];
    pMeshTextures  = new LPDIRECT3DTEXTURE9[dwNumMaterials];

    for( DWORD i=0; i<dwNumMaterials; i++ )
    {
        // Copy the material
        pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

        // Set the ambient color for the material (D3DX does not do this)
        pMeshMaterials[i].Ambient = pMeshMaterials[i].Diffuse;

        pMeshTextures[i] = NULL;
        if( d3dxMaterials[i].pTextureFilename != NULL && 
            lstrlen(d3dxMaterials[i].pTextureFilename) > 0 )
        {
            // Create the texture
            if( FAILED( D3DXCreateTextureFromFile( g_pD3DDevice, 
                                                d3dxMaterials[i].pTextureFilename, 
                                                &pMeshTextures[i] ) ) )
            {
                // If texture is not in current folder, try Graphics folder
                const TCHAR* strPrefix = TEXT("Graphics/");
                const int lenPrefix = lstrlen( strPrefix );
                TCHAR strTexture[MAX_PATH];
                lstrcpyn( strTexture, strPrefix, MAX_PATH );
                lstrcpyn( strTexture + lenPrefix, d3dxMaterials[i].pTextureFilename, MAX_PATH - lenPrefix );
                if( FAILED( D3DXCreateTextureFromFile( g_pD3DDevice, 
                                                    strTexture, 
                                                    &pMeshTextures[i] ) ) )
                {
                    MessageBox(NULL, "Could not find texture map", "Simulation Pool", MB_OK);
                }
            }
        }
    }

    // Done with the material buffer
    pD3DXMtrlBuffer->Release();

	bInitialized = true;
}

//-----------------------------------------------------------------------------
// Name: CGrObject(pmesh, texturefname)
// Desc: class constructor; use pre-initialized mesh data and load a texture
//       if texturefname == NULL, no texture is attached
//-----------------------------------------------------------------------------
CGrObject::CGrObject(LPD3DXMESH pmesh, char* texturefname)
{
	pVertexBuffer = NULL;
	pMesh = pmesh;
	dwNumMaterials = 1;
	pMeshMaterials = new D3DMATERIAL9[1];
	pMeshMaterials[0].Specular.r = 0.88f;
	pMeshMaterials[0].Specular.g = 0.88f;
	pMeshMaterials[0].Specular.b = 0.88f;
	pMeshMaterials[0].Specular.a = 1.0f;
	pMeshMaterials[0].Power = 11.0f;
	pMeshMaterials[0].Diffuse.r = 0.7f;
	pMeshMaterials[0].Diffuse.g = 0.2f;
	pMeshMaterials[0].Diffuse.b = 0.7f;
	pMeshMaterials[0].Diffuse.a = 1.0f;
	pMeshMaterials[0].Emissive.r = 0.06f;
	pMeshMaterials[0].Emissive.g = 0.06f;
	pMeshMaterials[0].Emissive.b = 0.06f;
	pMeshMaterials[0].Emissive.a = 0.06f;
	pMeshMaterials[0].Ambient = pMeshMaterials[0].Diffuse;
	pMeshTextures = NULL;
	eFillMode = D3DFILL_SOLID;
	eCull = D3DCULL_CCW; // TODO: get the default cull direction from graphics initialization
	if (texturefname != NULL)
	{
		pMeshTextures = new LPDIRECT3DTEXTURE9[1];
		if( FAILED( D3DXCreateTextureFromFile( g_pD3DDevice, 
											   texturefname, 
											   &pMeshTextures[0] ) ) )
		{
			// If texture is not in current folder, try Graphics folder
			const TCHAR* strPrefix = TEXT("Graphics/");
			const int lenPrefix = lstrlen( strPrefix );
			TCHAR strTexture[MAX_PATH];
			lstrcpyn( strTexture, strPrefix, MAX_PATH );
			lstrcpyn( strTexture + lenPrefix, texturefname, MAX_PATH - lenPrefix );
			if( FAILED( D3DXCreateTextureFromFile( g_pD3DDevice, 
												   strTexture, 
												   &pMeshTextures[0] ) ) )
			{
				MessageBox(NULL, "Could not find texture map", "Simulation Pool", MB_OK);
				free(pMeshTextures);
				pMeshTextures = NULL;
			}
		}
	}

	bInitialized = true;
}

//-----------------------------------------------------------------------------
// Name: CGrObject()
// Desc: class constructor; use pre-initialized vertex buffer instead of a mesh
//       data;
// Par.: buffer... vertex buffer
//       numvertex... number of vertices in the buffer
//       vertexsize... size of a custom vertex in bytes
//       fvf... custom vertex format
//		 primitivetype... type of vertex data (triangle list, line list, point list, etc.)
//       numprimitives... number of primitives in vertex buffer
//-----------------------------------------------------------------------------
CGrObject::CGrObject(BYTE* buffer, int numvertices, int vertexsize, DWORD fvf, D3DPRIMITIVETYPE primitivetype, int numprimitives)
{
	pVertexBuffer = NULL;
	eFillMode = D3DFILL_SOLID;
	eCull = D3DCULL_CCW; // TODO: get the default cull direction from graphics initialization
	iNumVertices = numvertices;
	iVertexSize = vertexsize;
	ePrimitiveType = primitivetype;
	fvfFormat = fvf;
	iNumPrimitives = numprimitives;
	if( FAILED( g_pD3DDevice->CreateVertexBuffer( numvertices*vertexsize,
         0, fvf, D3DPOOL_DEFAULT, &pVertexBuffer, NULL ) ) )
	{
		bInitialized = false;
	}
	else
	{
		VOID* pVertices;
		if( FAILED( pVertexBuffer->Lock( 0, numvertices*vertexsize, (void**)&pVertices, 0 ) ) )
			bInitialized = false;
		else
		{
			memcpy( pVertices, buffer, numvertices*vertexsize );
			pVertexBuffer->Unlock();
			bInitialized = true;
		}
	}

	pMesh = NULL;
	pMeshMaterials = NULL;
	pMeshTextures = NULL;
}


//-----------------------------------------------------------------------------
// Name: Render()
// Desc: render object onto backbuffer
//-----------------------------------------------------------------------------
bool	CGrObject::Render()
{
    // first, set the coordinates transformation
    D3DXMATRIXA16 matWorld, matRot;
	D3DXVECTOR3 vPos = pEntity->GetPosition();
	D3DXMatrixTranslation( &matWorld, vPos.x, vPos.y, vPos.z);
	D3DXQUATERNION q = pEntity->GetRotation();
	q.z = -q.z; // must negate for the D3DX quaternion methods to work properly in a left-handed system
	D3DXMatrixRotationQuaternion( &matRot, &q);
	matWorld = matRot * matWorld; // rotation matrix multipled by translation matrix
    g_pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );
	// set proper fill mode
	DWORD dwValue;
	g_pD3DDevice->GetRenderState(D3DRS_FILLMODE, &dwValue);
	if (dwValue != this->GetFillMode())
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, this->GetFillMode());
	g_pD3DDevice->GetRenderState(D3DRS_CULLMODE, &dwValue);
	if (dwValue != this->GetCull())
		g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, this->GetCull());

	// meshes are divided into subsets, one for each material; render them in a loop
    if (pMesh != NULL)
	{
		g_pD3DDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
		for( DWORD i=0; i<dwNumMaterials; i++ )
		{
			// Set the material and texture for this subset
			if (pMeshMaterials != NULL)
				g_pD3DDevice->SetMaterial( &pMeshMaterials[i] );
			if (pMeshTextures != NULL)
			{
				g_pD3DDevice->SetTexture( 0, pMeshTextures[i] );
			}
	    
			// Draw the mesh subset
			HRESULT hr = pMesh->DrawSubset( i );
			if (hr != S_OK)
			{
				MessageBox(NULL, "Rendering error", "Error", MB_OK | MB_ICONEXCLAMATION);
			}
		}
	}
	else
	{
		// draw object using vertex buffer
		//g_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
		HRESULT hr = g_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
		hr = g_pD3DDevice->SetStreamSource( 0, pVertexBuffer, 0, iVertexSize );
		if (hr != D3D_OK)
		{
			MessageBox(NULL, "Rendering error", "Error", MB_OK | MB_ICONEXCLAMATION);
		}
        DWORD fvf;
		g_pD3DDevice->GetFVF(&fvf);
		hr = g_pD3DDevice->SetFVF( fvfFormat );
		g_pD3DDevice->SetTexture( 0, NULL );
        hr = g_pD3DDevice->DrawPrimitive( ePrimitiveType, 0, iNumPrimitives );
		if (hr != D3D_OK)
		{
			MessageBox(NULL, "Rendering error", "Error", MB_OK | MB_ICONEXCLAMATION);
		}
		g_pD3DDevice->SetFVF( fvf ); // restore old vertex format
	}
	return true;
}

//-----------------------------------------------------------------------------
// Name: GetBoundingBox(width, height)
// Desc: Return bounding box dimensions, either from mesh or from private vertex buffer.
//       Return false, if an error occured.
//-----------------------------------------------------------------------------
bool	CGrObject::GetBoundingBox(D3DXVECTOR3 *lowerleft, D3DXVECTOR3 *upperright)
{
	BYTE* pVertices=NULL;
	HRESULT hr;
	if (pMesh != NULL)
	{
		hr = pMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pVertices);
		if (FAILED(hr))
		{
			pMesh->UnlockVertexBuffer();
			return false;
		}
		D3DXComputeBoundingBox((D3DXVECTOR3*)pVertices, pMesh->GetNumVertices(), D3DXGetFVFVertexSize(pMesh->GetFVF()), lowerleft, upperright);
		pMesh->UnlockVertexBuffer();
	}
	else
	{
		hr = pVertexBuffer->Lock(0,0, (LPVOID*)&pVertices, D3DLOCK_READONLY);
		D3DXComputeBoundingBox((D3DXVECTOR3*)pVertices, iNumVertices, iVertexSize, lowerleft, upperright);
		if (FAILED(hr))
		{
			pVertexBuffer->Unlock();
			return false;
		}
		pVertexBuffer->Unlock();
	}

	return true;
}

//-----------------------------------------------------------------------------
// Name: SetFillMode(fillmode)
// Desc: Set fill mode render state for the object.
//-----------------------------------------------------------------------------
void CGrObject::SetFillMode(D3DFILLMODE fillmode)
{
	eFillMode = fillmode;
}

//-----------------------------------------------------------------------------
// Name: GetFillMode()
// Desc: Get fill mode render state for the object.
//-----------------------------------------------------------------------------
D3DFILLMODE	CGrObject::GetFillMode()
{
	return eFillMode;
}

//-----------------------------------------------------------------------------
// Name: SetCull(cull)
// Desc: Set cull render state for the object.
//-----------------------------------------------------------------------------
void CGrObject::SetCull(D3DCULL cull)
{
	eCull = cull;
}

//-----------------------------------------------------------------------------
// Name: GetCull()
// Desc: Get cull render state for the object.
//-----------------------------------------------------------------------------
D3DCULL	CGrObject::GetCull()
{
	return eCull;
}


//-----------------------------------------------------------------------------
// Name: Release()
// Desc: Release object data from memory.
//-----------------------------------------------------------------------------
void	CGrObject::Release()
{
    if( pMeshMaterials != NULL ) 
        delete[] pMeshMaterials;

    if( pMeshTextures )
    {
        for( DWORD i = 0; i < dwNumMaterials; i++ )
        {
            if( pMeshTextures[i] )
                pMeshTextures[i]->Release();
        }
        delete[] pMeshTextures;
    }
    if( pMesh != NULL )
        pMesh->Release();
    if( pVertexBuffer != NULL )
        pVertexBuffer->Release();
}