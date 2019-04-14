/* ******************************************************************************** */
/* GameEngine.cpp                                                                   */
/*                                                                                  */
/* Game is driven using the physics module and applying pool game rules             */
/*                                                                                  */
/* ******************************************************************************** */
#include "stdafx.h"
#include "GameEngine.h"
#include "Graphics.h"

#define SENSITIVITY_STROKE	(0.007f)		// mouse sensitivity in "stroke" mode
#define THRESHOLD_STOPVELOCITY	(0.005f)	// velocity at or below which the end of stroke is detected

//-----------------------------------------------------------------------------
// Name: CGameEngine()
// Desc: Class constructor which initializes all pool entities.
//-----------------------------------------------------------------------------
CGameEngine::CGameEngine()
{
	bActive = true; // note: during the testing period, application starts in active mode
	// initialize pool entities
	InitPoolEntities();
	// initialize pool table geometry (rails, edges)
	InitPoolGeometry();
	// initialize external forces
	InitForces();
}

//-----------------------------------------------------------------------------
// Name: InitPoolEntities()
// Desc: Creation of pool entity objects.
//-----------------------------------------------------------------------------
void CGameEngine::InitPoolEntities()
{
	// TODO: refine pool table into a composite plane
	pPoolTableCenter = new CMathObject();
	pPoolTable = new CMathObjectPlane();
	cPoolPhysics.AddEntity(pPoolTable);
	pPoolBall[0] = new CMathObjectSphere();
	cPoolPhysics.AddEntity(pPoolBall[0]);
	pPoolBall[1] = new CMathObjectSphere();
	cPoolPhysics.AddEntity(pPoolBall[1]);
	pPoolBall[2] = new CMathObjectSphere();
	cPoolPhysics.AddEntity(pPoolBall[2]);
	pPoolBall[3] = new CMathObjectSphere();
	cPoolPhysics.AddEntity(pPoolBall[3]);
	pPoolBall[4] = new CMathObjectSphere();
	cPoolPhysics.AddEntity(pPoolBall[4]);
	pPoolBall[5] = new CMathObjectSphere();
	cPoolPhysics.AddEntity(pPoolBall[5]);
	pPoolBall[6] = new CMathObjectSphere();
	cPoolPhysics.AddEntity(pPoolBall[6]);
	pPoolBall[7] = new CMathObjectSphere();
	cPoolPhysics.AddEntity(pPoolBall[7]);
	pPoolBall[8] = new CMathObjectSphere();
	cPoolPhysics.AddEntity(pPoolBall[8]);
	pPoolBall[9] = new CMathObjectSphere();
	cPoolPhysics.AddEntity(pPoolBall[9]);
	pPoolBall[10] = new CMathObjectSphere();
	cPoolPhysics.AddEntity(pPoolBall[10]);
	pPoolBall[11] = new CMathObjectSphere();
	cPoolPhysics.AddEntity(pPoolBall[11]);
	pPoolBall[12] = new CMathObjectSphere();
	cPoolPhysics.AddEntity(pPoolBall[12]);
	pPoolBall[13] = new CMathObjectSphere();
	cPoolPhysics.AddEntity(pPoolBall[13]);
	pPoolBall[14] = new CMathObjectSphere();
	cPoolPhysics.AddEntity(pPoolBall[14]);
	pPoolBall[15] = new CMathObjectSphere();
	cPoolPhysics.AddEntity(pPoolBall[15]);
	pCueStick = new CMathObjectLine();
	cPoolPhysics.AddEntity(pCueStick);

	iNumOfBalls = 16;
}

//-----------------------------------------------------------------------------
// Name: InitPoolGeometry()
// Desc: Initialize static math objects.
//-----------------------------------------------------------------------------
void CGameEngine::InitPoolGeometry()
{
	float y = 0.16f;
	D3DXVECTOR3 vOrientation;

	pRail[0] = new CMathObjectLine();
	pRail[0]->SetType(OBJTYPE_LINE);
	pRail[0]->SetFixed(true);
	pRail[0]->SetAngVelocity(&(D3DXVECTOR3(0,0,0)));
	pRail[0]->SetKineticFriction(0.15f);
	pRail[0]->SetStaticFriction(0.2f);
	pRail[0]->SetTipKineticFriction(0.15f);
	pRail[0]->SetTipStaticFriction(0.2f);
	pRail[0]->SetRestitution(0.82f);
	pRail[0]->SetVelocity(&D3DXVECTOR3(0,0,0));
	vOrientation = D3DXVECTOR3(-1.985f-(-2.081f), 0, 0.855f-0.965f);
	pRail[0]->SetMagnitude(D3DXVec3Length(&vOrientation));
	pRail[0]->SetPosition(-2.081f, y, 0.965f);
	D3DXVec3Normalize(&vOrientation, &vOrientation);
	pRail[0]->SetOrientation(&vOrientation);
	pRail[0]->SetRotation(&D3DXQUATERNION(0,0,0,1));
	cPoolPhysics.AddEntity(pRail[0]);

	pRail[1] = new CMathObjectLine();
	pRail[1]->SetType(OBJTYPE_LINE);
	pRail[1]->SetFixed(true);
	pRail[1]->SetAngVelocity(&(D3DXVECTOR3(0,0,0)));
	pRail[1]->SetKineticFriction(0.25f);
	pRail[1]->SetStaticFriction(0.3f);
	pRail[1]->SetTipKineticFriction(0.25f);
	pRail[1]->SetTipStaticFriction(0.3f);
	pRail[1]->SetRestitution(0.82f);
	pRail[1]->SetVelocity(&D3DXVECTOR3(0,0,0));
	vOrientation = D3DXVECTOR3(-1.824f-(-1.936f), 0, 1.015f-1.113f);
	pRail[1]->SetMagnitude(D3DXVec3Length(&vOrientation));
	pRail[1]->SetPosition(-1.936f, y, 1.113f);
	D3DXVec3Normalize(&vOrientation, &vOrientation);
	pRail[1]->SetOrientation(&vOrientation);
	pRail[1]->SetRotation(&D3DXQUATERNION(0,0,0,1));
	cPoolPhysics.AddEntity(pRail[1]);

	pRail[2] = new CMathObjectLine();
	pRail[2]->SetType(OBJTYPE_LINE);
	pRail[2]->SetFixed(true);
	pRail[2]->SetAngVelocity(&(D3DXVECTOR3(0,0,0)));
	pRail[2]->SyncAngMomentum();
	pRail[2]->SetKineticFriction(0.25f);
	pRail[2]->SetStaticFriction(0.3f);
	pRail[2]->SetTipKineticFriction(0.25f);
	pRail[2]->SetTipStaticFriction(0.3f);
	pRail[2]->SetRestitution(0.82f);
	pRail[2]->SetVelocity(&D3DXVECTOR3(0,0,0));
	vOrientation = D3DXVECTOR3(-0.113f-(-1.824f), 0, 1.015f-1.015f);
	pRail[2]->SetMagnitude(D3DXVec3Length(&vOrientation));
	pRail[2]->SetPosition(-1.824f, y, 1.015f);
	D3DXVec3Normalize(&vOrientation, &vOrientation);
	pRail[2]->SetOrientation(&vOrientation);
	pRail[2]->SetRotation(&D3DXQUATERNION(0,0,0,1));
	cPoolPhysics.AddEntity(pRail[2]);

	pRail[3] = new CMathObjectLine();
	pRail[3]->SetType(OBJTYPE_LINE);
	pRail[3]->SetFixed(true);
	pRail[3]->SetAngVelocity(&(D3DXVECTOR3(0,0,0)));
	pRail[3]->SyncAngMomentum();
	pRail[3]->SetKineticFriction(0.25f);
	pRail[3]->SetStaticFriction(0.3f);
	pRail[3]->SetTipKineticFriction(0.25f);
	pRail[3]->SetTipStaticFriction(0.3f);
	pRail[3]->SetRestitution(0.82f);
	pRail[3]->SetVelocity(&D3DXVECTOR3(0,0,0));
	vOrientation = D3DXVECTOR3(-0.113f-(-0.092f), 0, 1.015f-1.106f);
	pRail[3]->SetMagnitude(D3DXVec3Length(&vOrientation));
	pRail[3]->SetPosition(-0.092f, y, 1.106f);
	D3DXVec3Normalize(&vOrientation, &vOrientation);
	pRail[3]->SetOrientation(&vOrientation);
	pRail[3]->SetRotation(&D3DXQUATERNION(0,0,0,1));
	cPoolPhysics.AddEntity(pRail[3]);

	pRail[4] = new CMathObjectLine();
	pRail[4]->SetType(OBJTYPE_LINE);
	pRail[4]->SetFixed(true);
	pRail[4]->SetAngVelocity(&(D3DXVECTOR3(0,0,0)));
	pRail[4]->SyncAngMomentum();
	pRail[4]->SetKineticFriction(0.25f);
	pRail[4]->SetStaticFriction(0.3f);
	pRail[4]->SetTipKineticFriction(0.25f);
	pRail[4]->SetTipStaticFriction(0.3f);
	pRail[4]->SetRestitution(0.82f);
	pRail[4]->SetVelocity(&D3DXVECTOR3(0,0,0));
	vOrientation = D3DXVECTOR3(0.125f-(0.104f), 0, 1.015f-1.107f);
	pRail[4]->SetMagnitude(D3DXVec3Length(&vOrientation));
	pRail[4]->SetPosition(0.104f, y, 1.107f);
	D3DXVec3Normalize(&vOrientation, &vOrientation);
	pRail[4]->SetOrientation(&vOrientation);
	pRail[4]->SetRotation(&D3DXQUATERNION(0,0,0,1));
	cPoolPhysics.AddEntity(pRail[4]);

	pRail[5] = new CMathObjectLine();
	pRail[5]->SetType(OBJTYPE_LINE);
	pRail[5]->SetFixed(true);
	pRail[5]->SetAngVelocity(&(D3DXVECTOR3(0,0,0)));
	pRail[5]->SyncAngMomentum();
	pRail[5]->SetKineticFriction(0.25f);
	pRail[5]->SetStaticFriction(0.3f);
	pRail[5]->SetTipKineticFriction(0.25f);
	pRail[5]->SetTipStaticFriction(0.3f);
	pRail[5]->SetRestitution(0.82f);
	pRail[5]->SetVelocity(&D3DXVECTOR3(0,0,0));
	vOrientation = D3DXVECTOR3(1.837f-(0.125f), 0, 1.015f-1.015f);
	pRail[5]->SetMagnitude(D3DXVec3Length(&vOrientation));
	pRail[5]->SetPosition(0.125f, y, 1.015f);
	D3DXVec3Normalize(&vOrientation, &vOrientation);
	pRail[5]->SetOrientation(&vOrientation);
	pRail[5]->SetRotation(&D3DXQUATERNION(0,0,0,1));
	cPoolPhysics.AddEntity(pRail[5]);

	pRail[6] = new CMathObjectLine();
	pRail[6]->SetType(OBJTYPE_LINE);
	pRail[6]->SetFixed(true);
	pRail[6]->SetAngVelocity(&(D3DXVECTOR3(0,0,0)));
	pRail[6]->SyncAngMomentum();
	pRail[6]->SetKineticFriction(0.25f);
	pRail[6]->SetStaticFriction(0.3f);
	pRail[6]->SetTipKineticFriction(0.25f);
	pRail[6]->SetTipStaticFriction(0.3f);
	pRail[6]->SetRestitution(0.82f);
	pRail[6]->SetVelocity(&D3DXVECTOR3(0,0,0));
	vOrientation = D3DXVECTOR3(1.837f-(1.944f), 0, 1.015f-1.108f);
	pRail[6]->SetMagnitude(D3DXVec3Length(&vOrientation));
	pRail[6]->SetPosition(1.944f, y, 1.108f);
	D3DXVec3Normalize(&vOrientation, &vOrientation);
	pRail[6]->SetOrientation(&vOrientation);
	pRail[6]->SetRotation(&D3DXQUATERNION(0,0,0,1));
	cPoolPhysics.AddEntity(pRail[6]);

	pRail[7] = new CMathObjectLine();
	pRail[7]->SetType(OBJTYPE_LINE);
	pRail[7]->SetFixed(true);
	pRail[7]->SetAngVelocity(&(D3DXVECTOR3(0,0,0)));
	pRail[7]->SyncAngMomentum();
	pRail[7]->SetKineticFriction(0.25f);
	pRail[7]->SetStaticFriction(0.3f);
	pRail[7]->SetTipKineticFriction(0.25f);
	pRail[7]->SetTipStaticFriction(0.3f);
	pRail[7]->SetRestitution(0.82f);
	pRail[7]->SetVelocity(&D3DXVECTOR3(0,0,0));
	vOrientation = D3DXVECTOR3(1.996f-(2.093f), 0, 0.855f-0.964f);
	pRail[7]->SetMagnitude(D3DXVec3Length(&vOrientation));
	pRail[7]->SetPosition(2.093f, y, 0.964f);
	D3DXVec3Normalize(&vOrientation, &vOrientation);
	pRail[7]->SetOrientation(&vOrientation);
	pRail[7]->SetRotation(&D3DXQUATERNION(0,0,0,1));
	cPoolPhysics.AddEntity(pRail[7]);

	pRail[8] = new CMathObjectLine();
	pRail[8]->SetType(OBJTYPE_LINE);
	pRail[8]->SetFixed(true);
	pRail[8]->SetAngVelocity(&(D3DXVECTOR3(0,0,0)));
	pRail[8]->SyncAngMomentum();
	pRail[8]->SetKineticFriction(0.25f);
	pRail[8]->SetStaticFriction(0.3f);
	pRail[8]->SetTipKineticFriction(0.25f);
	pRail[8]->SetTipStaticFriction(0.3f);
	pRail[8]->SetRestitution(0.82f);
	pRail[8]->SetVelocity(&D3DXVECTOR3(0,0,0));
	vOrientation = D3DXVECTOR3(1.996f-(1.996f), 0, -0.849f-0.855f);
	pRail[8]->SetMagnitude(D3DXVec3Length(&vOrientation));
	pRail[8]->SetPosition(1.996f, y, 0.855f);
	D3DXVec3Normalize(&vOrientation, &vOrientation);
	pRail[8]->SetOrientation(&vOrientation);
	pRail[8]->SetRotation(&D3DXQUATERNION(0,0,0,1));
	cPoolPhysics.AddEntity(pRail[8]);

	pRail[9] = new CMathObjectLine();
	pRail[9]->SetType(OBJTYPE_LINE);
	pRail[9]->SetFixed(true);
	pRail[9]->SetAngVelocity(&(D3DXVECTOR3(0,0,0)));
	pRail[9]->SyncAngMomentum();
	pRail[9]->SetKineticFriction(0.25f);
	pRail[9]->SetStaticFriction(0.3f);
	pRail[9]->SetTipKineticFriction(0.25f);
	pRail[9]->SetTipStaticFriction(0.3f);
	pRail[9]->SetRestitution(0.82f);
	pRail[9]->SetVelocity(&D3DXVECTOR3(0,0,0));
	vOrientation = D3DXVECTOR3(1.996f-(2.090f), 0, -0.849f-(-0.962f));
	pRail[9]->SetMagnitude(D3DXVec3Length(&vOrientation));
	pRail[9]->SetPosition(2.090f, y, -0.962f);
	D3DXVec3Normalize(&vOrientation, &vOrientation);
	pRail[9]->SetOrientation(&vOrientation);
	pRail[9]->SetRotation(&D3DXQUATERNION(0,0,0,1));
	cPoolPhysics.AddEntity(pRail[9]);

	pRail[10] = new CMathObjectLine();
	pRail[10]->SetType(OBJTYPE_LINE);
	pRail[10]->SetFixed(true);
	pRail[10]->SetAngVelocity(&(D3DXVECTOR3(0,0,0)));
	pRail[10]->SyncAngMomentum();
	pRail[10]->SetKineticFriction(0.25f);
	pRail[10]->SetStaticFriction(0.3f);
	pRail[10]->SetTipKineticFriction(0.25f);
	pRail[10]->SetTipStaticFriction(0.3f);
	pRail[10]->SetRestitution(0.82f);
	pRail[10]->SetVelocity(&D3DXVECTOR3(0,0,0));
	vOrientation = D3DXVECTOR3(1.837f-(1.945f), 0, -1.017f-(-1.111f));
	pRail[10]->SetMagnitude(D3DXVec3Length(&vOrientation));
	pRail[10]->SetPosition(1.945f, y, -1.111f);
	D3DXVec3Normalize(&vOrientation, &vOrientation);
	pRail[10]->SetOrientation(&vOrientation);
	pRail[10]->SetRotation(&D3DXQUATERNION(0,0,0,1));
	cPoolPhysics.AddEntity(pRail[10]);

	pRail[11] = new CMathObjectLine();
	pRail[11]->SetType(OBJTYPE_LINE);
	pRail[11]->SetFixed(true);
	pRail[11]->SetAngVelocity(&(D3DXVECTOR3(0,0,0)));
	pRail[11]->SyncAngMomentum();
	pRail[11]->SetKineticFriction(0.25f);
	pRail[11]->SetStaticFriction(0.3f);
	pRail[11]->SetTipKineticFriction(0.25f);
	pRail[11]->SetTipStaticFriction(0.3f);
	pRail[11]->SetRestitution(0.82f);
	pRail[11]->SetVelocity(&D3DXVECTOR3(0,0,0));
	vOrientation = D3DXVECTOR3(0.125f-(1.837f), 0, -1.017f-(-1.017f));
	pRail[11]->SetMagnitude(D3DXVec3Length(&vOrientation));
	pRail[11]->SetPosition(1.837f, y, -1.017f);
	D3DXVec3Normalize(&vOrientation, &vOrientation);
	pRail[11]->SetOrientation(&vOrientation);
	pRail[11]->SetRotation(&D3DXQUATERNION(0,0,0,1));
	cPoolPhysics.AddEntity(pRail[11]);

	pRail[12] = new CMathObjectLine();
	pRail[12]->SetType(OBJTYPE_LINE);
	pRail[12]->SetFixed(true);
	pRail[12]->SetAngVelocity(&(D3DXVECTOR3(0,0,0)));
	pRail[12]->SyncAngMomentum();
	pRail[12]->SetKineticFriction(0.25f);
	pRail[12]->SetStaticFriction(0.3f);
	pRail[12]->SetTipKineticFriction(0.25f);
	pRail[12]->SetTipStaticFriction(0.3f);
	pRail[12]->SetRestitution(0.82f);
	pRail[12]->SetVelocity(&D3DXVECTOR3(0,0,0));
	vOrientation = D3DXVECTOR3(0.125f-(0.104f), 0, -1.017f-(-1.105f));
	pRail[12]->SetMagnitude(D3DXVec3Length(&vOrientation));
	pRail[12]->SetPosition(0.104f, y, -1.105f);
	D3DXVec3Normalize(&vOrientation, &vOrientation);
	pRail[12]->SetOrientation(&vOrientation);
	pRail[12]->SetRotation(&D3DXQUATERNION(0,0,0,1));
	cPoolPhysics.AddEntity(pRail[12]);

	pRail[13] = new CMathObjectLine();
	pRail[13]->SetType(OBJTYPE_LINE);
	pRail[13]->SetFixed(true);
	pRail[13]->SetAngVelocity(&(D3DXVECTOR3(0,0,0)));
	pRail[13]->SyncAngMomentum();
	pRail[13]->SetKineticFriction(0.25f);
	pRail[13]->SetStaticFriction(0.3f);
	pRail[13]->SetTipKineticFriction(0.25f);
	pRail[13]->SetTipStaticFriction(0.3f);
	pRail[13]->SetRestitution(0.82f);
	pRail[13]->SetVelocity(&D3DXVECTOR3(0,0,0));
	vOrientation = D3DXVECTOR3(-0.113f-(-0.094f), 0, -1.017f-(-1.108f));
	pRail[13]->SetMagnitude(D3DXVec3Length(&vOrientation));
	pRail[13]->SetPosition(-0.094f, y, -1.108f);
	D3DXVec3Normalize(&vOrientation, &vOrientation);
	pRail[13]->SetOrientation(&vOrientation);
	pRail[13]->SetRotation(&D3DXQUATERNION(0,0,0,1));
	cPoolPhysics.AddEntity(pRail[13]);

	pRail[14] = new CMathObjectLine();
	pRail[14]->SetType(OBJTYPE_LINE);
	pRail[14]->SetFixed(true);
	pRail[14]->SetAngVelocity(&(D3DXVECTOR3(0,0,0)));
	pRail[14]->SyncAngMomentum();
	pRail[14]->SetKineticFriction(0.25f);
	pRail[14]->SetStaticFriction(0.3f);
	pRail[14]->SetTipKineticFriction(0.25f);
	pRail[14]->SetTipStaticFriction(0.3f);
	pRail[14]->SetRestitution(0.82f);
	pRail[14]->SetVelocity(&D3DXVECTOR3(0,0,0));
	vOrientation = D3DXVECTOR3(-1.824f-(-0.113f), 0, -1.017f-(-1.017f));
	pRail[14]->SetMagnitude(D3DXVec3Length(&vOrientation));
	pRail[14]->SetPosition(-0.113f, y, -1.017f);
	D3DXVec3Normalize(&vOrientation, &vOrientation);
	pRail[14]->SetOrientation(&vOrientation);
	pRail[14]->SetRotation(&D3DXQUATERNION(0,0,0,1));
	cPoolPhysics.AddEntity(pRail[14]);

	pRail[15] = new CMathObjectLine();
	pRail[15]->SetType(OBJTYPE_LINE);
	pRail[15]->SetFixed(true);
	pRail[15]->SetAngVelocity(&(D3DXVECTOR3(0,0,0)));
	pRail[15]->SyncAngMomentum();
	pRail[15]->SetKineticFriction(0.25f);
	pRail[15]->SetStaticFriction(0.3f);
	pRail[15]->SetTipKineticFriction(0.25f);
	pRail[15]->SetTipStaticFriction(0.3f);
	pRail[15]->SetRestitution(0.82f);
	pRail[15]->SetVelocity(&D3DXVECTOR3(0,0,0));
	vOrientation = D3DXVECTOR3(-1.824f-(-1.935f), 0, -1.017f-(-1.111f));
	pRail[15]->SetMagnitude(D3DXVec3Length(&vOrientation));
	pRail[15]->SetPosition(-1.935f, y, -1.111f);
	D3DXVec3Normalize(&vOrientation, &vOrientation);
	pRail[15]->SetOrientation(&vOrientation);
	pRail[15]->SetRotation(&D3DXQUATERNION(0,0,0,1));
	cPoolPhysics.AddEntity(pRail[15]);

	pRail[16] = new CMathObjectLine();
	pRail[16]->SetType(OBJTYPE_LINE);
	pRail[16]->SetFixed(true);
	pRail[16]->SetAngVelocity(&(D3DXVECTOR3(0,0,0)));
	pRail[16]->SyncAngMomentum();
	pRail[16]->SetKineticFriction(0.25f);
	pRail[16]->SetStaticFriction(0.3f);
	pRail[16]->SetTipKineticFriction(0.25f);
	pRail[16]->SetTipStaticFriction(0.3f);
	pRail[16]->SetRestitution(0.82f);
	pRail[16]->SetVelocity(&D3DXVECTOR3(0,0,0));
	vOrientation = D3DXVECTOR3(-1.985f-(-2.081f), 0, -0.849f-(-0.968f));
	pRail[16]->SetMagnitude(D3DXVec3Length(&vOrientation));
	pRail[16]->SetPosition(-2.081f, y, -0.968f);
	D3DXVec3Normalize(&vOrientation, &vOrientation);
	pRail[16]->SetOrientation(&vOrientation);
	pRail[16]->SetRotation(&D3DXQUATERNION(0,0,0,1));
	cPoolPhysics.AddEntity(pRail[16]);

	pRail[17] = new CMathObjectLine();
	pRail[17]->SetType(OBJTYPE_LINE);
	pRail[17]->SetFixed(true);
	pRail[17]->SetAngVelocity(&(D3DXVECTOR3(0,0,0)));
	pRail[17]->SyncAngMomentum();
	pRail[17]->SetKineticFriction(0.25f);
	pRail[17]->SetStaticFriction(0.3f);
	pRail[17]->SetTipKineticFriction(0.25f);
	pRail[17]->SetTipStaticFriction(0.3f);
	pRail[17]->SetRestitution(0.82f);
	pRail[17]->SetVelocity(&D3DXVECTOR3(0,0,0));
	vOrientation = D3DXVECTOR3(-1.985f-(-1.985f), 0, 0.855f-(-0.849f));
	pRail[17]->SetMagnitude(D3DXVec3Length(&vOrientation));
	pRail[17]->SetPosition(-1.985f, y, -0.849f);
	D3DXVec3Normalize(&vOrientation, &vOrientation);
	pRail[17]->SetOrientation(&vOrientation);
	pRail[17]->SetRotation(&D3DXQUATERNION(0,0,0,1));
	cPoolPhysics.AddEntity(pRail[17]);

	// add holes to pool table
	// top left
	CMathObjectCircle *hole = new CMathObjectCircle();
	hole->SetType(OBJTYPE_CIRCLE);
	hole->SetAngVelocity(&D3DXVECTOR3(0,0,0));
	hole->SyncAngMomentum();
	hole->SetNormal(&(D3DXVECTOR3(0,1,0)));
	hole->SetPosition(-2.055f, 0.16f-0.053f, 1.086f);
	hole->SetRadius(0.115f);
	hole->SetRotation(&D3DXQUATERNION(0,0,0,1));
	hole->SetVelocity(&D3DXVECTOR3(0,0,0));
	pPoolTable->AddDivElement(hole);
	// top middle
	hole = new CMathObjectCircle();
	hole->SetType(OBJTYPE_CIRCLE);
	hole->SetAngVelocity(&D3DXVECTOR3(0,0,0));
	hole->SyncAngMomentum();
	hole->SetNormal(&(D3DXVECTOR3(0,1,0)));
	hole->SetPosition(0, 0.16f-0.053f, 1.112f);
	hole->SetRadius(0.104f);
	hole->SetRotation(&D3DXQUATERNION(0,0,0,1));
	hole->SetVelocity(&D3DXVECTOR3(0,0,0));
	pPoolTable->AddDivElement(hole);
	// top right
	hole = new CMathObjectCircle();
	hole->SetType(OBJTYPE_CIRCLE);
	hole->SetAngVelocity(&D3DXVECTOR3(0,0,0));
	hole->SyncAngMomentum();
	hole->SetNormal(&(D3DXVECTOR3(0,1,0)));
	hole->SetPosition(2.067f, 0.16f-0.053f, 1.086f);
	hole->SetRadius(0.115f);
	hole->SetRotation(&D3DXQUATERNION(0,0,0,1));
	hole->SetVelocity(&D3DXVECTOR3(0,0,0));
	pPoolTable->AddDivElement(hole);
	// bottom right
	hole = new CMathObjectCircle();
	hole->SetType(OBJTYPE_CIRCLE);
	hole->SetAngVelocity(&D3DXVECTOR3(0,0,0));
	hole->SyncAngMomentum();
	hole->SetNormal(&(D3DXVECTOR3(0,1,0)));
	hole->SetPosition(2.068f, 0.16f-0.053f, -1.087f);
	hole->SetRadius(0.115f);
	hole->SetRotation(&D3DXQUATERNION(0,0,0,1));
	hole->SetVelocity(&D3DXVECTOR3(0,0,0));
	pPoolTable->AddDivElement(hole);
	// bottom middle
	hole = new CMathObjectCircle();
	hole->SetType(OBJTYPE_CIRCLE);
	hole->SetAngVelocity(&D3DXVECTOR3(0,0,0));
	hole->SyncAngMomentum();
	hole->SetNormal(&(D3DXVECTOR3(0,1,0)));
	hole->SetPosition(0, 0.16f-0.053f, -1.172f);
	hole->SetRadius(0.104f);
	hole->SetRotation(&D3DXQUATERNION(0,0,0,1));
	hole->SetVelocity(&D3DXVECTOR3(0,0,0));
	pPoolTable->AddDivElement(hole);
	// bottom left
	hole = new CMathObjectCircle();
	hole->SetType(OBJTYPE_CIRCLE);
	hole->SetAngVelocity(&D3DXVECTOR3(0,0,0));
	hole->SyncAngMomentum();
	hole->SetNormal(&(D3DXVECTOR3(0,1,0)));
	hole->SetPosition(-2.055f, 0.16f-0.053f, -1.087f);
	hole->SetRadius(0.104f);
	hole->SetRotation(&D3DXQUATERNION(0,0,0,1));
	hole->SetVelocity(&D3DXVECTOR3(0,0,0));
	pPoolTable->AddDivElement(hole);
}

//-----------------------------------------------------------------------------
// Name: InitForces()
// Desc: Initialize external forces which act upon pool objects.
//-----------------------------------------------------------------------------
void CGameEngine::InitForces()
{
	// initialize force applied by the cue stick
	// obsolete: cue stick is handled as just another object (which it actually is, of course)
	//pCueStick = new CForce();
	//pCueStick->AssignObject(pPoolBall[0]); // we always strike the cue ball only

	// initialize gravity force
	pGravity = new CForce();
	pGravity->SetType(FORCETYPE_GLOBAL);
	pGravity->SetGravity(true);
	D3DXVECTOR3 v = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pGravity->SetDirection(&v);
	cPoolPhysics.AddForce(pGravity);
}


//-----------------------------------------------------------------------------
// Name: SetupTable()
// Desc: Initial transformation and other object initialization
//-----------------------------------------------------------------------------
void CGameEngine::SetupTable()
{
	// initialize pool table position (for graphics engine)
	pPoolTableCenter->SetFixed(true);  // pool table can not be moved
	pPoolTableCenter->SetPosition(0,0,0); // pool table is at the world center; it must be aligned with camera look-at point for the rotation to work properly
	pPoolTableCenter->SetVelocity(&D3DXVECTOR3(0,0,0));
	pPoolTableCenter->SetAngVelocity(&D3DXVECTOR3(0,0,0));
	pPoolTableCenter->SetType(OBJTYPE_ARBITRARY);

	// initialize pool table
	pPoolTable->SetFixed(true);  // pool table can not be moved
	pPoolTable->SetPosition(0,0.16f-0.053f,0); // pool table is at the world center; it must be aligned with camera look-at point for the rotation to work properly
	pPoolTable->SetVelocity(&D3DXVECTOR3(0,0,0));
	pPoolTable->SetAngVelocity(&D3DXVECTOR3(0,0,0));
	pPoolTable->SetType(OBJTYPE_PLANE);
	pPoolTable->SetKineticFriction(0.07f);
	pPoolTable->SetStaticFriction(0.10f);
	pPoolTable->SetSize(4.1f,2.225f);
	pPoolTable->SetLengthVector(&D3DXVECTOR3(1.0f,0,0));
	pPoolTable->SetWidthVector(&D3DXVECTOR3(0,0,1.0f));
	pPoolTable->SetRestitution(0.8);
	pPoolTable->SetNormal(&D3DXVECTOR3(0,1.0f,0));

	D3DXMATRIX m;

	for (int i = 0; i <= iNumOfBalls; i++)
	{
		pPoolBall[i]->SetRadius(0.053f);
		pPoolBall[i]->SetMass(0.3f);
		pPoolBall[i]->SetVelocity(&D3DXVECTOR3(0,0,0));
		pPoolBall[i]->SetAngVelocity(&D3DXVECTOR3(0,0,0));
		D3DXQUATERNION q = D3DXQUATERNION(0,0,0,1);
		D3DXQuaternionNormalize(&q, &q);
		pPoolBall[i]->SetRotation(&q);
		pPoolBall[i]->SyncAngMomentum();
		pPoolBall[i]->SetFixed(false);
		pPoolBall[i]->SetType(OBJTYPE_SPHERE);
		m = D3DXMATRIX(0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1);
		m._11 = 2*pPoolBall[i]->GetMass()*pPoolBall[i]->GetRadius()*pPoolBall[i]->GetRadius()/5;
		m._22 = m._11;
		m._33 = m._11;
		pPoolBall[i]->SetInertia(&m);
		m._11 = 1/m._11;
		m._22 = m._11;
		m._33 = m._11;
		pPoolBall[i]->SetInertiaInv(&m);
		pPoolBall[i]->SetKineticFriction(0.08f);
		pPoolBall[i]->SetStaticFriction(0.1f);
		pPoolBall[i]->SetRollingFriction(0.05f);
		pPoolBall[i]->SetRestitution(0.92f);
	}
	pPoolBall[0]->SetPosition(-1.0f,0.16f,0);
	pPoolBall[15]->SetPosition(1.2f+8*pPoolBall[1]->GetRadius(), 0.16f, -4*pPoolBall[1]->GetRadius());
	pPoolBall[14]->SetPosition(1.2f+8*pPoolBall[1]->GetRadius(), 0.16f, -2*pPoolBall[1]->GetRadius());
	pPoolBall[13]->SetPosition(1.2f+8*pPoolBall[1]->GetRadius(), 0.16f, 0);
	pPoolBall[12]->SetPosition(1.2f+8*pPoolBall[1]->GetRadius(), 0.16f, 2*pPoolBall[1]->GetRadius());
	pPoolBall[11]->SetPosition(1.2f+8*pPoolBall[1]->GetRadius(), 0.16f, 4*pPoolBall[1]->GetRadius());
	pPoolBall[10]->SetPosition(1.2f+6*pPoolBall[1]->GetRadius(), 0.16f, -3*pPoolBall[1]->GetRadius());
	pPoolBall[9]->SetPosition(1.2f+6*pPoolBall[1]->GetRadius(), 0.16f, -1*pPoolBall[1]->GetRadius());
	pPoolBall[8]->SetPosition(1.2f+6*pPoolBall[1]->GetRadius(), 0.16f, 1*pPoolBall[1]->GetRadius());
	pPoolBall[7]->SetPosition(1.2f+6*pPoolBall[1]->GetRadius(), 0.16f, 3*pPoolBall[1]->GetRadius());
	pPoolBall[6]->SetPosition(1.2f+4*pPoolBall[1]->GetRadius(), 0.16f, -2*pPoolBall[1]->GetRadius());
	pPoolBall[5]->SetPosition(1.2f+4*pPoolBall[1]->GetRadius(), 0.16f, 0);
	pPoolBall[4]->SetPosition(1.2f+4*pPoolBall[1]->GetRadius(), 0.16f, 2*pPoolBall[1]->GetRadius());
	pPoolBall[3]->SetPosition(1.2f+2*pPoolBall[1]->GetRadius(), 0.16f, pPoolBall[1]->GetRadius());
	pPoolBall[2]->SetPosition(1.2f+2*pPoolBall[1]->GetRadius(), 0.16f, -pPoolBall[1]->GetRadius());
	pPoolBall[1]->SetPosition(1.2f, 0.16f, 0);

	pCueStick->SetMagnitude(3.109);
	pCueStick->SetPosition(0,0,0);
	pCueStick->SetOrientation(&(D3DXVECTOR3(0.0f, 0.0f, 1.0f)));
	pCueStick->SetFixed(false);
	pCueStick->SetMass(1.0f);
	pCueStick->SetVelocity(&D3DXVECTOR3(0,0,0));
	pCueStick->SetAngVelocity(&D3DXVECTOR3(0,0,0));
	pCueStick->SyncAngMomentum();
	pCueStick->SetType(OBJTYPE_LINE);
	pCueStick->SetRestitution(1.0f);
	pCueStick->SetStaticFriction(0.7f);
	pCueStick->SetKineticFriction(0.6f);
	m = D3DXMATRIX(0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1);
	m._11 = (1/(float)12.0f)*pCueStick->GetMass()*(pCueStick->GetMagnitude()*pCueStick->GetMagnitude());
	m._22 = m._11;
	m._33 = 0.01f; // "almost" zero
	pCueStick->SetInertia(&m);
	D3DXMATRIX mInv;
	D3DXMatrixInverse(&mInv, NULL, &m);
	pCueStick->SetInertiaInv(&mInv);

	ResetCuestick();
}

//-----------------------------------------------------------------------------
// Name: RotateView()
// Desc: Apply new view transformation according to last mouse movement.
//-----------------------------------------------------------------------------
void CGameEngine::RotateView(LONG ofsX, LONG ofsY, LONG ofsZ, BYTE buttons[8])
{
	if (!bActive) return;
	if (eGameMode == Aim)
	{
		if (buttons[0] == 0 && buttons[1] == 0 && buttons[2] == 0)
		{
			// rotation
			if (ofsX != 0)
			{
				RotateCameraY(-ofsX); // rotate camera about the world Y axis
				if (eGameMode != Running)
					ResetCuestick();
			}
			if (ofsY != 0)
			{
				RotateCameraXZ(-ofsY); // rotate camera about an axis in the world XZ plane, perpendicular to camera axis
				if (eGameMode != Running)
					ResetCuestick();
			}
		}
	}
	if (buttons[0] > 0 && buttons[1] > 0 && eGameMode == Aim)
	{
		// zoom in/out
		ZoomCamera(ofsY);
	}

	if (eGameMode == Stroke)
	{
		// when in stroke mode, move cue stick in and out towards the cue ball, until the cue ball is hit
		if (ofsY > 0)
			MoveCuestick(-ofsY);
		else
		{
			for (int iStep = 0; iStep > ofsY; iStep--)
			{
				// check if the cue stick has hit the cue ball
				MoveCuestick(1);
				D3DXVECTOR3 vIntersection, vNormal;
				bool bEndPoint;
				float fProx = pPoolBall[0]->LineIntersection(pCueStick, &vIntersection, &bEndPoint, &vNormal);
				if (vIntersection.x != INFINITY && fProx < 0.005f)
				{
					// time to start the simulation
					// choose an initial velocity of the cue stick at the time of impact
					D3DXVECTOR3 vVelocity = pCueStick->GetDirection();
					vVelocity *= -ofsY * SENSITIVITY_STROKE * 3;
					pCueStick->SetVelocity(&vVelocity);
					vImpactCueStickPosition = pCueStick->GetPosition();
					cPoolPhysics.StartSim();
					eGameMode = Running;
					break;
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Name: GotoStrokeMode()
// Desc: Signal game engine we are goint to or from "stroke" mode.
//-----------------------------------------------------------------------------
void CGameEngine::SwitchStrokeMode()
{
	if (eGameMode == Aim)
	{
		eGameMode = Stroke; // continue in "stroke" mode
	}
	else if (eGameMode == Stroke)
	{
		eGameMode = Aim; // move back to "aim" mode
		ResetCuestick();
	}

}

//-----------------------------------------------------------------------------
// Name: MoveCuestick()
// Desc: Move cue stick in "stroke" mode with accordance to last mouse movement.
//-----------------------------------------------------------------------------
void CGameEngine::MoveCuestick(float offset)
{
	pCueStick->SetPosition( &(pCueStick->GetPosition() + pCueStick->GetDirection()*offset*SENSITIVITY_STROKE) );
}

//-----------------------------------------------------------------------------
// Name: ResetCuestick()
// Desc: Reset cue stick position to cue ball and camera eye point position.
//-----------------------------------------------------------------------------
void CGameEngine::ResetCuestick()
{
	pCueStick->SetAngVelocity(&D3DXVECTOR3(0,0,0));
	pCueStick->SyncAngMomentum();
	// find a "table" projection of the eye point
	D3DXVECTOR3 vEyeProjection = g_vEyePt;
	vEyeProjection.y = pPoolBall[0]->GetPosition().y + 0.0f;
	D3DXVECTOR3 vEyeTableProjection = g_vEyePt;
	vEyeTableProjection.y = pPoolBall[0]->GetPosition().y;
	D3DXVECTOR3 vDirection = vEyeTableProjection-pPoolBall[0]->GetPosition();
	D3DXVec3Normalize(&vDirection, &vDirection);
	D3DXVECTOR3 vEndPoint = pPoolBall[0]->GetPosition() + vDirection * pPoolBall[0]->GetRadius()*1.0f;
	vDirection = vEndPoint - vEyeProjection; // new cue stick direction
	D3DXVec3Normalize(&vDirection, &vDirection);
	pCueStick->SetPosition( &(vEndPoint - vDirection*pCueStick->GetMagnitude()) );
	// calculate rotation needed to transform original direction vector to new one
	D3DXVECTOR3 vOrigDirection = pCueStick->GetDirection();
	D3DXVECTOR3 vRotationAxis;
	D3DXVec3Cross(&vRotationAxis, &vOrigDirection, &vDirection);
	if (D3DXVec3Length(&vRotationAxis) > 0.001f)
	{
		// calculate angle between old and new direction vectors
		float fAngle = asin( D3DXVec3Length(&vRotationAxis) / (D3DXVec3Length(&vOrigDirection)*D3DXVec3Length(&vDirection)));
		D3DXVec3Normalize(&vRotationAxis, &vRotationAxis);
		D3DXQUATERNION qRotation/* = pCueStick->GetRotation()*/;
		D3DXQuaternionRotationAxis(&qRotation, &vRotationAxis, fAngle);
		D3DXQuaternionMultiply(&qRotation, &(pCueStick->GetRotation()), &qRotation);
		// HACK: limit rotation to world Y axis
		qRotation.x = 0;
		qRotation.z = 0;
		D3DXQuaternionNormalize(&qRotation, &qRotation);
		pCueStick->SetRotation(&qRotation);
	}

	// TODO: if there is an intersection with the cue stick, repeat the process with increased vEyeProjection.y component
}


//-----------------------------------------------------------------------------
// Name: StepSimulation()
// Desc: Step physics simulator for a given time interval.
// Par.: dtime... time in [s]
//-----------------------------------------------------------------------------
void CGameEngine::StepSimulation(float dtime)
{
	if (eGameMode == Running)
	{
		if (cPoolPhysics.GetTime() < 100)
		{
			// at the beginning of the stroke, leave the cue stick free to move,
			// but let it not fall under influence of gravity
			D3DXVECTOR3 vPosition = pCueStick->GetPosition();
			vPosition.y = vImpactCueStickPosition.y;
		}
		else if (pCueStick->GetPosition().y < 100.0f)
		{
			pCueStick->SetPosition(&(D3DXVECTOR3(0,100.0f,0)));
			pCueStick->SetVelocity(&(D3DXVECTOR3(0,0,0)));
			pCueStick->SetAngVelocity(&(D3DXVECTOR3(0,0,0)));
			pCueStick->SyncAngMomentum();
			pCueStick->SetFixed(true);
		}
		float fMaxVelocity = cPoolPhysics.StepSimulation(dtime);
		if (fMaxVelocity <= THRESHOLD_STOPVELOCITY)
		{
			// objects on table have stopped - prepare table for next stroke
			pCueStick->SetFixed(false);
			ResetDynamics();
			g_vLookatPt = pPoolBall[0]->GetPosition(); // point camera towards the cue ball
			eGameMode = Aim;
			ResetCuestick();
		}
		// stop the balls which have fallen through the table
		for (int i = 0; i < iNumOfBalls; i++)
		{
			if (pPoolBall[i]->GetPosition().y < -0.4)
			{
				if (i == 0)
				{
					// return cue ball on the table
					pPoolBall[i]->SetPosition(-0.6f,0.16f,0);
				}
				else
				{
					pPoolBall[i]->SetPosition(0,10,0);
					pPoolBall[i]->SetFixed(true);
				}
				pPoolBall[i]->SetVelocity(&D3DXVECTOR3(0,0,0));	
				pPoolBall[i]->SetAngVelocity(&D3DXVECTOR3(0,0,0));
				pPoolBall[i]->SyncAngMomentum();
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Name: ResetDynamics()
// Desc: Reset all object velocities.
//-----------------------------------------------------------------------------
void CGameEngine::ResetDynamics()
{
	for (int i = 0; i < iNumOfBalls; i++)
	{
		pPoolBall[i]->SetVelocity(&D3DXVECTOR3(0,0,0));
		pPoolBall[i]->SetAngVelocity(&D3DXVECTOR3(0,0,0));
		pPoolBall[i]->SyncAngMomentum();
	}
	pCueStick->SetVelocity(&D3DXVECTOR3(0,0,0));
	pCueStick->SetAngVelocity(&D3DXVECTOR3(0,0,0));
	pCueStick->SyncAngMomentum();
	pCueStick->SetForceSum(&D3DXVECTOR3(0,0,0));
	pCueStick->SetMomentSum(&D3DXVECTOR3(0,0,0));
}


//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Release all memory reserved by the module.
//-----------------------------------------------------------------------------
void CGameEngine::Cleanup()
{
	free(pPoolTable);
	free(pPoolBall[0]);
	cPoolPhysics.Cleanup();
}