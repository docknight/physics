/* ******************************************************************************** */
/* Physics.cpp                                                                      */
/*                                                                                  */
/* Physics simulation of the objects in the game world                              */
/*                                                                                  */
/* ******************************************************************************** */
#include "stdafx.h"
#include "Physics.h"
#include <assert.h>


/* -------------- Class CForce -------------- */

//-----------------------------------------------------------------------------
// Name: CForce()
// Desc: Initialize force
//-----------------------------------------------------------------------------
CForce::CForce()
{
	// set force to act globally at all times
	iTimeStart = 0;
	iDuration = INFINITY;
	pMathObject = NULL;
}

//-----------------------------------------------------------------------------
// Name: SetType(type)
// Desc: Set force type, either global or object-local.
//-----------------------------------------------------------------------------
void CForce::SetType(int type)
{
	iType = type;
}

//-----------------------------------------------------------------------------
// Name: GetType()
// Desc: Get force type.
//-----------------------------------------------------------------------------
int CForce::GetType()
{
	return iType;
}

//-----------------------------------------------------------------------------
// Name: SetDirection(dir)
// Desc: Set the force direction vector in world coordinate system. Vector is normalized.
//-----------------------------------------------------------------------------
void CForce::SetDirection(D3DXVECTOR3 *dir)
{
	D3DXVec3Normalize(&vDirection, dir);
}

//-----------------------------------------------------------------------------
// Name: SetPosition(pos)
// Desc: Set the force position vector in body coordinates.
//-----------------------------------------------------------------------------
void CForce::SetPosition(D3DXVECTOR3 *pos)
{
	vPosition = *pos;
}

//-----------------------------------------------------------------------------
// Name: SetMagnitude(pos)
// Desc: Set the force magnitude in the direction of vDirection.
//-----------------------------------------------------------------------------
void CForce::SetMagnitude(float mag)
{
	fMagnitude = mag;
}

//-----------------------------------------------------------------------------
// Name: AssignObject(obj)
// Desc: Assign a specific math object to be susceptive to this force.
//-----------------------------------------------------------------------------
void CForce::AssignObject(CMathObject *obj)
{
	pMathObject = obj;
}

//-----------------------------------------------------------------------------
// Name: GetObject()
// Desc: Return reference to object that has been assigned to this force.
//-----------------------------------------------------------------------------
CMathObject* CForce::GetObject()
{
	return pMathObject;
}


//-----------------------------------------------------------------------------
// Name: SetStart(time)
// Desc: Set time offset when the force awakens.
//-----------------------------------------------------------------------------
void CForce::SetStart(int time)
{
	iTimeStart = time;
}

//-----------------------------------------------------------------------------
// Name: SetDuration(time)
// Desc: Set duration of the active force.
//-----------------------------------------------------------------------------
void CForce::SetDuration(int time)
{
	iDuration = time;
}

//-----------------------------------------------------------------------------
// Name: SetGravity(gravity)
// Desc: Sets the gravity flag for special handling of the force's magnitude calculation.
//-----------------------------------------------------------------------------
void CForce::SetGravity(bool gravity)
{
	bGravity = gravity;
}

//-----------------------------------------------------------------------------
// Name: IsGravity()
// Desc: Returns true if the force is a gravitational force
//-----------------------------------------------------------------------------
bool CForce::IsGravity()
{
	return bGravity;
}

//-----------------------------------------------------------------------------
// Name: GetPosition()
// Desc: Returns the force application point in body local coordinates. Valid
//       only if force type is OBJECT.
//-----------------------------------------------------------------------------
D3DXVECTOR3 CForce::GetPosition()
{
	return vPosition;
}

//-----------------------------------------------------------------------------
// Name: GetDirection()
// Desc: Returns the normalized direction vector of the force
//-----------------------------------------------------------------------------
D3DXVECTOR3 CForce::GetDirection()
{
	return vDirection;
}

//-----------------------------------------------------------------------------
// Name: GetMagnitude()
// Desc: Returns the magnitude of the force.
//-----------------------------------------------------------------------------
float CForce::GetMagnitude()
{
	return fMagnitude;
}

//-----------------------------------------------------------------------------
// Name: GetStart()
// Desc: Return time offset when the force awakens.
//-----------------------------------------------------------------------------
int CForce::GetStart()
{
	return iTimeStart;
}

//-----------------------------------------------------------------------------
// Name: GetDuration()
// Desc: Return duration of the active force.
//-----------------------------------------------------------------------------
int CForce::GetDuration()
{
	return iDuration;
}



/* -------------- Class CPhysics -------------- */


//-----------------------------------------------------------------------------
// Name: CPhysics()
// Desc: Initialize entities list
//-----------------------------------------------------------------------------
CPhysics::CPhysics()
{
	// initialize entities list
	pEntities = (MathObjectListPtr)malloc(sizeof(MathObjectList));
	pEntities->pMathObject = NULL;
	pEntities->next = NULL;
	// initialize external forces list
	pExtForces = (ForceListPtr)malloc(sizeof(ForceList));
	pExtForces->pForce = NULL;
	pExtForces->next = NULL;
	// initialize object dynamics list
	pObjDynamics = (ObjDynamicsListPtr)malloc(sizeof(ObjDynamicsList));
	pObjDynamics->vjCollision = D3DXVECTOR3(0,0,0);
	pObjDynamics->vjContact = D3DXVECTOR3(0,0,0);
	pObjDynamics->vVelEnd = D3DXVECTOR3(0,0,0);
	pObjDynamics->vVelInitial = D3DXVECTOR3(0,0,0);
	pObjDynamics->iObjIndex = -1;
	pObjDynamics->next = NULL;

}

//-----------------------------------------------------------------------------
// Name: StartSim()
// Desc: Starts the simulation by initializing time values etc.
//-----------------------------------------------------------------------------
void CPhysics::StartSim()
{
	iTimeStamp = 0;
}

//-----------------------------------------------------------------------------
// Name: AddEntity()
// Desc: Add pre-initialized object to list of simulated objects (entities).
//-----------------------------------------------------------------------------
void CPhysics::AddEntity(CMathObject * pMathObject)
{
	int iEntityCount = 1;
	MathObjectListPtr entlist;
	entlist = pEntities;
	// seek to the end of the entities list
	while (entlist->next != NULL)
	{
		entlist = entlist->next;
		iEntityCount++;
	}
	// we add a new element, unless this is the first object, in which case the element has already been created
	if (entlist->pMathObject != NULL)
	{
		entlist->next = (MathObjectListPtr)malloc(sizeof(MathObjectList));
		entlist = entlist->next;
		entlist->next = NULL;
		iEntityCount++;
	}
	// store object's reference into the list
	entlist->pMathObject = pMathObject;

	// also, add new element to object-dynamics list
	ObjDynamicsListPtr objdynlist = pObjDynamics;
	// seek to the end of the list
	while (objdynlist->next != NULL)
		objdynlist = objdynlist->next;
	// we add a new element, unless this is the first object, in which case the element has already been created
	if (objdynlist->iObjIndex >= 0)
	{
		objdynlist->next = (ObjDynamicsListPtr)malloc(sizeof(ObjDynamicsList));
		objdynlist = objdynlist->next;
		objdynlist->next = NULL;
	}
	// store object's data into the list
	objdynlist->iObjIndex = iEntityCount-1;
	objdynlist->vjCollision = D3DXVECTOR3(0,0,0);
	objdynlist->vjContact = D3DXVECTOR3(0,0,0);
	objdynlist->vVelEnd = D3DXVECTOR3(0,0,0);
	objdynlist->vVelInitial = D3DXVECTOR3(0,0,0);
}

//-----------------------------------------------------------------------------
// Name: AddForce(pForce)
// Desc: Add pre-initialized force object to list of forces.
//-----------------------------------------------------------------------------
void CPhysics::AddForce(CForce * pForce)
{
	ForceListPtr flist;
	flist = pExtForces;
	// seek to the end of the forces list
	while (flist->next != NULL)
		flist = flist->next;
	// we add a new element, unless this is the first object, in which case the element has already been created
	if (flist->pForce != NULL)
	{
		flist->next = (ForceListPtr)malloc(sizeof(ForceList));
		flist = flist->next;
		flist->next = NULL;
	}
	// store object's reference into the list
	flist->pForce = pForce;
}

//-----------------------------------------------------------------------------
// Name: ClearForces()
// Desc: Clear the list of external forces.
//-----------------------------------------------------------------------------
void CPhysics::ClearForces()
{
	ForceListPtr objlist = pExtForces;
	ForceListPtr objlistnext;
	while (objlist != NULL)
	{
		objlistnext = objlist->next;
		// release list element
		free(objlist);
		objlist = objlistnext;
	}
	// re-initialize external forces list
	pExtForces = (ForceListPtr)malloc(sizeof(ForceList));
	pExtForces->pForce = NULL;
	pExtForces->next = NULL;
}

//-----------------------------------------------------------------------------
// Name: ApplyForces(dt)
// Desc: Apply (external) forces to entities.
//-----------------------------------------------------------------------------
void CPhysics::ApplyForces()
{
	// external forces (gravity) are first applied to the objects
	// for rest of the forces, calculate the point of application and determine moments, too
	// TODO: only apply forces that are still active (compare force duration and simulation time stamp)
	// TODO: plane->GetNormal() is expensive; save the result at the beginning
	ForceListPtr forces = pExtForces;
	CForce* force;
	MathObjectListPtr entities;
	CMathObject* entity;
	while (forces->pForce != NULL) // traverse the forces list
	{
		force = forces->pForce;
		if (force->GetType() == FORCETYPE_GLOBAL)
		{
			// global force is applied to all non-fixed objects			
			entities = pEntities;
			while (entities->pMathObject != NULL)
			{
				entity = entities->pMathObject;
				if ( !(entity->IsFixed()) ) // we assume fixed objects are immune to gravity
				{
					D3DXVECTOR3 vGForce;
					// if force is a gravity, calculate the gravitational force
					float fMagnitude;
					if (force->IsGravity())
						fMagnitude = entity->GetMass() * (float)g;
					else
						fMagnitude = force->GetMagnitude();
					vGForce = force->GetDirection() * fMagnitude;
					entity->AddForce(&vGForce); // set the global force to be acting on the non-fixed object
				}
				
				entities = entities->next;
				if (entities == NULL) break;
			} // while
		}
		
		else
		// force is not global; use its application point and find intersection
		// with any of the objects; when such an intersection is found, apply the force
		// (and momentum, if the force is not directed at the mass centrum) to the object 
		// CHANGE: since local forces already have assigned an object (and are always
		// moving with it), no intersection calculation is needed;
		// we could have another type of forces though, some kind of a force field...
		{
			entity = force->GetObject();
			// take force activation time into account
			if ( force->GetStart() <= iTimeStamp && (force->GetStart()+force->GetDuration() >= iTimeStamp || force->GetDuration() == INFINITY) )
			{
				// assign the force to object
				D3DXVECTOR3 vForce = force->GetDirection() * force->GetMagnitude();
				entity->AddForce(&vForce);
				// calculate torque on the object; it is assumed that the force position
				// vector is relative to object mass center position and can therefore
				// be used directly in the torque equation
				D3DXVECTOR3 vMoment;
				D3DXVec3Cross(&vMoment, &(force->GetPosition()), &vForce);
				entity->AddMoment(&vMoment);
			}
		}

		forces = forces->next;
		if (forces == NULL) break;
	}
}

//-----------------------------------------------------------------------------
// Name: InitForces()
// Desc: Initialize forces and moments of all objects to zero. Done at the beginning 
//       of every simulation step, before external forces are applied.
//-----------------------------------------------------------------------------
void CPhysics::InitForces()
{
	MathObjectListPtr entities = pEntities;
	D3DXVECTOR3 vZero = D3DXVECTOR3(0,0,0);
	while (entities != NULL)
	{
		if (entities->pMathObject != NULL)
		{
			entities->pMathObject->SetForceSum(&vZero);
			entities->pMathObject->SetMomentSum(&vZero);
		}
		entities = entities->next;
	}
} 

//-----------------------------------------------------------------------------
// Name: DetectCollision(*fminprox)
// Desc: Detect collisions between objects and return list of collision data
//       to be used by collision resolver. Also, minimum proximity among all
//       the collided objects is returned.
//-----------------------------------------------------------------------------
CollisionListPtr CPhysics::DetectCollision(float *fminprox)
{
	D3DXVECTOR3			vIntersection;
	float				fProx;
	CollisionListPtr	pCollisions, pCollisionStart;
	// initialize list header
	pCollisions = (CollisionListPtr)malloc(sizeof(CollisionList));
	pCollisions->pCollision = NULL;
	pCollisions->next = NULL;
	pCollisionStart = pCollisions;

	// now iterate through entities to detect collisions between them
	MathObjectListPtr	entlist = pEntities;
	MathObjectListPtr	entlist2;
	CMathObject*		pEntityA;
	CMathObject*		pEntityB;
	ObjDynamicsListPtr  objdynlist = pObjDynamics;
	ObjDynamicsListPtr	objdynlist2;

	*fminprox = INFINITY;

	// seek to the end of the entities list
	while (entlist != NULL)
	{
		pEntityA = entlist->pMathObject;
		if (pEntityA != NULL)
		{
			// seek through remainder of entity list to find another object to check for collision
			entlist2 = entlist->next;
			objdynlist2 = objdynlist->next;
			while (entlist2 != NULL)
			{
				pEntityB = entlist2->pMathObject;
				if (pEntityB != NULL)
				{
					// ok, got two objects; now, check for collision!
					if ( (pEntityA->GetType() == OBJTYPE_LINE && pEntityB->GetType() == OBJTYPE_SPHERE) ||
						 (pEntityA->GetType() == OBJTYPE_SPHERE && pEntityB->GetType() == OBJTYPE_LINE) )
					{
						bool bEndPoint;
						D3DXVECTOR3 vNormal;
						bool bReversed = false;
						if (pEntityA->GetType() == OBJTYPE_SPHERE && pEntityB->GetType() == OBJTYPE_LINE)
						{
							// reverse object order (procedure below assumes pEntityB is the sphere)
							CMathObject* pEntity = pEntityB;
							pEntityB = pEntityA;
							pEntityA = pEntity;
							bReversed = true;
						}
						// check for collision between a line and a sphere (for instance, cue stick and pool ball)
						fProx = pEntityB->LineIntersection((CMathObjectLine*)pEntityA, &vIntersection, &bEndPoint, &vNormal);
						*fminprox = min(fProx, *fminprox);
						if (vIntersection.x != INFINITY)
						{
							// intersection confirmed -> add collision data to the list
							CollisionInfo* colinfo;
							colinfo = (CollisionInfo*)malloc(sizeof(CollisionInfo));
							colinfo->pObject1 = pEntityB;
							colinfo->pObject2 = pEntityA;
							colinfo->pObjDynamics1 = bReversed ? objdynlist : objdynlist2;
							colinfo->pObjDynamics2 = bReversed ? objdynlist2 : objdynlist;
							colinfo->vIntersection = vIntersection;
							colinfo->bEndPoint = bEndPoint;
							// OBSOLETE
							/*
							if (bEndPoint)
							{
								// calculate collision normal, which is aligned on the line
								D3DXVECTOR3 vDirection = ((CMathObjectLine*)pEntityA)->GetDirection();
								// let the normal, by convention, point towards the sphere (object 1 in the collision pair)
								if ( D3DXVec3Dot(&(pEntityB->GetPosition()), &vDirection) > D3DXVec3Dot(&vIntersection, &vDirection) )
									colinfo->vNormal = vDirection;
								else
									colinfo->vNormal = -vDirection;
								colinfo->bFriction = false;
							}
							else*/
							{
								// if the sphere hit the line along its sides, the collision normal is perpendicular to
								// the line and points towards the sphere center
								D3DXVec3Normalize(&(colinfo->vNormal), &vNormal);
								colinfo->bFriction = true;
							}
							pCollisions->pCollision = colinfo;
							// expand the list
							pCollisions->next = (CollisionListPtr)malloc(sizeof(CollisionList));
							pCollisions = pCollisions->next;
							pCollisions->pCollision = NULL;
							pCollisions->next = NULL;
						}
						if (bReversed)
						{
							// reverse object order back to previous
							CMathObject* pEntity = pEntityB;
							pEntityB = pEntityA;
							pEntityA = pEntity;
						}
					}
					if ( (pEntityA->GetType() == OBJTYPE_PLANE && pEntityB->GetType() == OBJTYPE_SPHERE) ||
						 (pEntityA->GetType() == OBJTYPE_SPHERE && pEntityB->GetType() == OBJTYPE_PLANE) )
					{
						// check for collision between a plane and a sphere (for instance, pool table surface and pool ball)
						bool bReversed = false;
						if (pEntityA->GetType() == OBJTYPE_SPHERE && pEntityB->GetType() == OBJTYPE_PLANE)
						{
							// reverse object order (procedure below assumes pEntityB is the sphere)
							CMathObject* pEntity = pEntityB;
							pEntityB = pEntityA;
							pEntityA = pEntity;
							bReversed = true;
						}
						bool bEndPoint;
						D3DXVECTOR3 vNormal;
						fProx = ((CMathObjectSphere*)pEntityB)->PlaneIntersection((CMathObjectPlane*)pEntityA, &vIntersection, &bEndPoint, &vNormal);
						*fminprox = min(fProx, *fminprox);
						if (vIntersection.x != INFINITY)
						{			
							// intersection confirmed -> add collision data to the list
							CollisionInfo* colinfo;
							colinfo = (CollisionInfo*)malloc(sizeof(CollisionInfo));
							colinfo->pObject1 = pEntityB;
							colinfo->pObject2 = pEntityA;
							colinfo->pObjDynamics1 = bReversed ? objdynlist : objdynlist2;
							colinfo->pObjDynamics2 = bReversed ? objdynlist2 : objdynlist;
							colinfo->vIntersection = vIntersection;
							colinfo->bEndPoint = bEndPoint;
							// calculate collision normal, which is directed from intersection point towards the sphere
							//if (!bEndPoint)
							{
								D3DXVECTOR3 vDirection = pEntityB->GetPosition() - vIntersection;
								D3DXVec3Normalize( &(colinfo->vNormal), &vDirection);	
								colinfo->bFriction = true;
							}
							// OBSOLETE
							/*else
							{
								// if there was an end-point collision (intersection with a plane edge),
								// normal is along the plane and its direction has already been calculated
								colinfo->vNormal = vNormal;
								colinfo->bFriction = false;
								// however, if the bodies are flying apart in that direction,
								// we might still have a collision in the plane normal direction;
								// we think of that collision as a regular sphere/plane face collision
								D3DXVECTOR3 vPointAngVel; // velocity of the contact point due to object's angular velocity
								D3DXVECTOR3 vRadiusVector = colinfo->vIntersection - pEntityB->GetPosition();
								D3DXVec3Cross(&vPointAngVel, &( ((CMathObjectSphere*)(pEntityB))->GetAngVelocity() ), &vRadiusVector);
								D3DXVECTOR3 vVelBP = pEntityB->GetVelocity() + vPointAngVel; // sphere contact point velocity
								D3DXVECTOR3 vVelAP = pEntityA->GetVelocity(); // plane contact point velocity
								D3DXVECTOR3 vVelAB = vVelBP - vVelAP;
								if (D3DXVec3Dot(&vVelAB, &(colinfo->vNormal)) >= 0)
								{
									// no collision along the plane -> try with normal as usual
									D3DXVECTOR3 vDirection = pEntityB->GetPosition() - vIntersection;
									D3DXVec3Normalize( &(colinfo->vNormal), &vDirection);	
									colinfo->bFriction = true;
								}
							}
							*/
							pCollisions->pCollision = colinfo;
							// expand the list
							pCollisions->next = (CollisionListPtr)malloc(sizeof(CollisionList));
							pCollisions = pCollisions->next;
							pCollisions->pCollision = NULL;
							pCollisions->next = NULL;
						}
						if (bReversed)
						{
							// reverse object order back to previous
							CMathObject* pEntity = pEntityB;
							pEntityB = pEntityA;
							pEntityA = pEntity;
						}
					}
					if (pEntityA->GetType() == OBJTYPE_SPHERE && pEntityB->GetType() == OBJTYPE_SPHERE) 
					{
						D3DXVECTOR3 vNormal;
						// check for collision between two spheres (pool balls, for instance)
						fProx = pEntityA->SphereIntersection((CMathObjectSphere*)pEntityB, &vIntersection);
						*fminprox = min(fProx, *fminprox);
						if (vIntersection.x != INFINITY)
						{
							// intersection confirmed -> add collision data to the list
							CollisionInfo* colinfo;
							colinfo = (CollisionInfo*)malloc(sizeof(CollisionInfo));
							colinfo->pObject1 = pEntityA;
							colinfo->pObject2 = pEntityB;
							colinfo->pObjDynamics1 = objdynlist;
							colinfo->pObjDynamics2 = objdynlist2;
							colinfo->vIntersection = vIntersection;
							// collision normal is a vector from sphere B to sphere A (always pointing towards first object, by convention)
							colinfo->vNormal = pEntityA->GetPosition() - pEntityB->GetPosition();
							D3DXVec3Normalize(&(colinfo->vNormal), &(colinfo->vNormal));
							colinfo->bFriction = true; // there is a friction to be handled between spheres
							colinfo->bEndPoint = false;
							pCollisions->pCollision = colinfo;
							// expand the list
							pCollisions->next = (CollisionListPtr)malloc(sizeof(CollisionList));
							pCollisions = pCollisions->next;
							pCollisions->pCollision = NULL;
							pCollisions->next = NULL;
						}
					}
					if (pEntityA->GetType() == OBJTYPE_PLANE && pEntityB->GetType() == OBJTYPE_PLANE) 
					{
						D3DXVECTOR3 vNormal;
						bool bEndPoint;
						// check for collision between two planes
						fProx = pEntityA->PlaneIntersection((CMathObjectPlane*)pEntityB, &vIntersection, &bEndPoint, &vNormal);
						*fminprox = min(fProx, *fminprox);
						if (vIntersection.x != INFINITY)
						{
							// intersection confirmed -> add collision data to the list
							CollisionInfo* colinfo;
							colinfo = (CollisionInfo*)malloc(sizeof(CollisionInfo));
							colinfo->pObject1 = pEntityB;
							colinfo->pObject2 = pEntityA;
							colinfo->pObjDynamics1 = objdynlist2;
							colinfo->pObjDynamics2 = objdynlist;
							colinfo->vIntersection = vIntersection;
							// collision normal is a vector from plane A to plane B (always pointing towards first object, by convention)

							colinfo->vNormal = vNormal;
							colinfo->bFriction = true; // there is a friction to be handled between spheres
							colinfo->bEndPoint = bEndPoint;
							pCollisions->pCollision = colinfo;
							// expand the list
							pCollisions->next = (CollisionListPtr)malloc(sizeof(CollisionList));
							pCollisions = pCollisions->next;
							pCollisions->pCollision = NULL;
							pCollisions->next = NULL;
						}
					}
				}
				entlist2 = entlist2->next;
				objdynlist2 = objdynlist2->next;
			}
		}
		
		entlist = entlist->next;
		objdynlist = objdynlist->next;
	}

	return pCollisionStart;
}


//-----------------------------------------------------------------------------
// Name: ResolveCollision(collisions, stage)
// Desc: Iterates through CollisionInfo list populated by collision detection
//       algorithm and applies impulses to objects to change their velocities.
// Ret.: number of collisions actually resolved (if relative velocity is > 0,
//       objects are already flying apart and there is no collision to handle);
//-----------------------------------------------------------------------------
int CPhysics::ResolveCollision(CollisionListPtr collisions, CollisionStage stage)
{
	CollisionInfo* colinfo;
	D3DXVECTOR3 vVelAP, vVelBP; // contact point velocities for both bodies
	D3DXVECTOR3 vVelAB;			// relative velocity at the contact point
	D3DXVECTOR3 vVelA, vVelB;
	D3DXVECTOR3 vAngVelA, vAngVelB;

	// velocities at the end of time interval, when the collision actually occured
	D3DXVECTOR3 vVelAPEnd, vVelBPEnd; // contact point velocities for both bodies
	D3DXVECTOR3 vVelABEnd;			// relative velocity at the contact point
	D3DXVECTOR3 vVelAEnd, vVelBEnd;
	D3DXVECTOR3 vAngVelAEnd, vAngVelBEnd;
	bool bPreviousCollision;

	int iCount = 0;

	while (collisions != NULL)
	{
		colinfo = collisions->pCollision;
		if (colinfo != NULL)
		{
			// check if any object is already moving under the influence of another collision that occured at this same instant
			if ( fabs(D3DXVec3Dot(&(colinfo->pObjDynamics1->vjCollision), &(colinfo->vNormal))) > epsilon ||
				 fabs(D3DXVec3Dot(&(colinfo->pObjDynamics2->vjCollision), &(colinfo->vNormal))) > epsilon )
				bPreviousCollision = true;
			else
				bPreviousCollision = false;
			// first, calculate velocity of the contact point on both objects
			switch (colinfo->pObject1->GetType())
			{
				case OBJTYPE_SPHERE:
					{
						D3DXVECTOR3 vPointAngVel; // velocity of the contact point due to object's angular velocity
						D3DXVECTOR3 vRadiusVector = colinfo->vIntersection - colinfo->pObject1->GetPosition();
						D3DXVec3Cross(&vPointAngVel, &( ((CMathObjectSphere*)(colinfo->pObject1))->GetAngVelocity() ), &vRadiusVector);
						vVelAP = colinfo->pObject1->GetVelocity() + vPointAngVel;
						// the same for velocity at the end of time interval
						if (stage == StageCollision)
						{
							D3DXVec3Cross(&vPointAngVel, &(colinfo->pObjDynamics1->vAngVelEnd), &vRadiusVector);
							vVelAPEnd = colinfo->pObjDynamics1->vVelEnd + vPointAngVel;
							D3DXVec3Cross(&vPointAngVel, &( colinfo->pObjDynamics1->vjAngVel ), &vRadiusVector);
							vVelAPEnd += colinfo->pObjDynamics1->vjVel + vPointAngVel; // add impulse velocities obtained from other simultaneous collisions
						}
						else // use collision impulse in contact collision verification, otherwise a collision might be processed as a contact also
						{
							D3DXVec3Cross(&vPointAngVel, &( colinfo->pObjDynamics1->vjAngVel ), &vRadiusVector);
							vVelAPEnd = vVelAP + colinfo->pObjDynamics1->vjVel + vPointAngVel;
						}
					}
					break;
				case OBJTYPE_LINE:
					{
						D3DXVECTOR3 vPointAngVel; // velocity of the contact point due to object's angular velocity
						D3DXVECTOR3 vRadiusVector = colinfo->vIntersection - ((CMathObjectLine*)colinfo->pObject1)->GetCenterPosition();
						D3DXVec3Cross(&vPointAngVel, &( ((CMathObjectLine*)(colinfo->pObject1))->GetAngVelocity() ), &vRadiusVector);
						vVelAP = colinfo->pObject1->GetVelocity() + vPointAngVel;
						// the same for velocity at the end of time interval
						if (stage == StageCollision)
						{
							D3DXVec3Cross(&vPointAngVel, &(colinfo->pObjDynamics1->vAngVelEnd), &vRadiusVector);
							vVelAPEnd = colinfo->pObjDynamics1->vVelEnd + vPointAngVel;
							D3DXVec3Cross(&vPointAngVel, &( colinfo->pObjDynamics1->vjAngVel ), &vRadiusVector);
							vVelAPEnd += colinfo->pObjDynamics1->vjVel + vPointAngVel; // add impulse velocities obtained from other simultaneous collisions
						}
						else // use collision impulse in contact collision verification, otherwise a collision might be processed as a contact also
						{
							D3DXVec3Cross(&vPointAngVel, &( colinfo->pObjDynamics1->vjAngVel ), &vRadiusVector);
							vVelAPEnd = vVelAP + colinfo->pObjDynamics1->vjVel + vPointAngVel;
						}
					}
					break;
				case OBJTYPE_PLANE:
					{
						D3DXVECTOR3 vPointAngVel; // velocity of the contact point due to object's angular velocity
						D3DXVECTOR3 vRadiusVector = colinfo->vIntersection - colinfo->pObject1->GetPosition();
						D3DXVec3Cross(&vPointAngVel, &( ((CMathObjectPlane*)(colinfo->pObject1))->GetAngVelocity() ), &vRadiusVector);
						vVelAP = colinfo->pObject1->GetVelocity() + vPointAngVel;
						// the same for velocity at the end of time interval
						if (stage == StageCollision)
						{
							D3DXVec3Cross(&vPointAngVel, &(colinfo->pObjDynamics1->vAngVelEnd), &vRadiusVector);
							vVelAPEnd = colinfo->pObjDynamics1->vVelEnd + vPointAngVel;
							D3DXVec3Cross(&vPointAngVel, &( colinfo->pObjDynamics1->vjAngVel ), &vRadiusVector);
							vVelAPEnd += colinfo->pObjDynamics1->vjVel + vPointAngVel; // add impulse velocities obtained from other simultaneous collisions
						}
						else // use collision impulse in contact collision verification, otherwise a collision might be processed as a contact also
						{
							D3DXVec3Cross(&vPointAngVel, &( colinfo->pObjDynamics1->vjAngVel ), &vRadiusVector);
							vVelAPEnd = vVelAP + colinfo->pObjDynamics1->vjVel + vPointAngVel;
						}
					}
					break;
				default:
					{
						// for rest of the objects, contact point velocity equals mass center velocity
						vVelAP = colinfo->pObject1->GetVelocity();
						if (stage == StageCollision)
						{
							vVelAPEnd = colinfo->pObjDynamics1->vVelEnd;
							vVelAPEnd += colinfo->pObjDynamics1->vjVel; // add impulse velocities obtained from other simultaneous collisions
						}
						else // use collision impulse in contact collision verification, otherwise a collision might be processed as a contact also
							vVelAPEnd = vVelAP + colinfo->pObjDynamics1->vjVel;
					}
					break;
			}
			switch (colinfo->pObject2->GetType())
			{
				case OBJTYPE_SPHERE:
					{
						D3DXVECTOR3 vPointAngVel; // velocity of the contact point due to object's angular velocity
						D3DXVECTOR3 vRadiusVector = colinfo->vIntersection - colinfo->pObject2->GetPosition();
						D3DXVec3Cross(&vPointAngVel, &( ((CMathObjectSphere*)(colinfo->pObject2))->GetAngVelocity() ), &vRadiusVector);
						vVelBP = colinfo->pObject2->GetVelocity() + vPointAngVel;
						// the same for velocity at the end of time interval
						if (stage == StageCollision)
						{
							D3DXVec3Cross(&vPointAngVel, &(colinfo->pObjDynamics2->vAngVelEnd), &vRadiusVector);
							vVelBPEnd = colinfo->pObjDynamics2->vVelEnd + vPointAngVel;
							D3DXVec3Cross(&vPointAngVel, &( colinfo->pObjDynamics2->vjAngVel ), &vRadiusVector);
							vVelBPEnd += colinfo->pObjDynamics2->vjVel + vPointAngVel; // add impulse velocities obtained from other simultaneous collisions
						}
						else // use collision impulse in contact collision verification, otherwise a collision might be processed as a contact also
						{
							D3DXVec3Cross(&vPointAngVel, &( colinfo->pObjDynamics2->vjAngVel ), &vRadiusVector);
							vVelBPEnd = vVelBP + colinfo->pObjDynamics2->vjVel + vPointAngVel;
						}
					}
					break;
				case OBJTYPE_LINE:
					{
						D3DXVECTOR3 vPointAngVel; // velocity of the contact point due to object's angular velocity
						D3DXVECTOR3 vRadiusVector = colinfo->vIntersection - ((CMathObjectLine*)colinfo->pObject2)->GetPosition();
						D3DXVec3Cross(&vPointAngVel, &( ((CMathObjectLine*)(colinfo->pObject2))->GetAngVelocity() ), &vRadiusVector);
						vVelBP = colinfo->pObject2->GetVelocity() + vPointAngVel;
						// the same for velocity at the end of time interval
						if (stage == StageCollision)
						{
							D3DXVec3Cross(&vPointAngVel, &(colinfo->pObjDynamics2->vAngVelEnd), &vRadiusVector);
							vVelBPEnd = colinfo->pObjDynamics2->vVelEnd + vPointAngVel;
							D3DXVec3Cross(&vPointAngVel, &( colinfo->pObjDynamics2->vjAngVel ), &vRadiusVector);
							vVelBPEnd += colinfo->pObjDynamics2->vjVel + vPointAngVel; // add impulse velocities obtained from other simultaneous collisions
						}
						else // use collision impulse in contact collision verification, otherwise a collision might be processed as a contact also
						{
							D3DXVec3Cross(&vPointAngVel, &( colinfo->pObjDynamics2->vjAngVel ), &vRadiusVector);
							vVelBPEnd = vVelBP + colinfo->pObjDynamics2->vjVel + vPointAngVel;
						}
					}
					break;
				case OBJTYPE_PLANE:
					{
						D3DXVECTOR3 vPointAngVel; // velocity of the contact point due to object's angular velocity
						D3DXVECTOR3 vRadiusVector = colinfo->vIntersection - colinfo->pObject2->GetPosition();
						D3DXVec3Cross(&vPointAngVel, &( ((CMathObjectPlane*)(colinfo->pObject2))->GetAngVelocity() ), &vRadiusVector);
						vVelBP = colinfo->pObject2->GetVelocity() + vPointAngVel;
						// the same for velocity at the end of time interval
						if (stage == StageCollision)
						{
							D3DXVec3Cross(&vPointAngVel, &(colinfo->pObjDynamics2->vAngVelEnd), &vRadiusVector);
							vVelBPEnd = colinfo->pObjDynamics2->vVelEnd + vPointAngVel;
							D3DXVec3Cross(&vPointAngVel, &( colinfo->pObjDynamics2->vjAngVel ), &vRadiusVector);
							vVelBPEnd += colinfo->pObjDynamics2->vjVel + vPointAngVel; // add impulse velocities obtained from other simultaneous collisions
						}
						else // use collision impulse in contact collision verification, otherwise a collision might be processed as a contact also
						{
							D3DXVec3Cross(&vPointAngVel, &( colinfo->pObjDynamics2->vjAngVel ), &vRadiusVector);
							vVelBPEnd = vVelBP + colinfo->pObjDynamics2->vjVel + vPointAngVel;
						}
					}
					break;
				default:
					{
						// for rest of the objects, contact point velocity equals mass center velocity
						vVelBP = colinfo->pObject2->GetVelocity();
						if (stage == StageCollision)
						{
							vVelBPEnd = colinfo->pObjDynamics2->vVelEnd;
							vVelBPEnd += colinfo->pObjDynamics2->vjVel; // add impulse velocities obtained from other simultaneous collisions
						}
						else // use collision impulse in contact collision verification, otherwise a collision might be processed as a contact also
							vVelBPEnd = vVelBP + colinfo->pObjDynamics2->vjVel;
					}
					break;
			}
			// calculate relative velocity at the contact point
			vVelAB = vVelAP - vVelBP;
			vVelABEnd = vVelAPEnd - vVelBPEnd;

			// if any impulse has already been raised on either of the objects in the
			// collision normal direction, then the velocity at the end of time interval,
			// including that impulse, must be used when checking for collision condition;
			// otherwise, velocity at the beginning is used first, and then the ending
			// velocity is used to verify collision;
			// collision condition: assuming that the normal vector points towards 
			// object A (!!), collision only occurs when relative velocity in the 
			// normal direction is < 0; if it is > 0, objects are actually flying apart;
			if ( (stage == StageCollision && bPreviousCollision && D3DXVec3Dot(&vVelABEnd, &(colinfo->vNormal)) < 0-epsilon) ||
				 (stage == StageCollision && !bPreviousCollision && D3DXVec3Dot(&vVelAB, &(colinfo->vNormal)) < 0-epsilon && D3DXVec3Dot(&vVelABEnd, &(colinfo->vNormal)) < 0-epsilon) ||
				 (stage == StageContact && D3DXVec3Dot(&vVelAB, &(colinfo->vNormal)) && D3DXVec3Dot(&vVelABEnd, &(colinfo->vNormal)) < 0-epsilon) )
			{
				// collision confirmed
				iCount++;
				
				// calculate the impulse
				float j;
				if (stage == StageCollision)
					j = Impulse(colinfo->pObject1, colinfo->pObject2, &(colinfo->vIntersection), &(colinfo->vNormal), &vVelABEnd, min(colinfo->pObject1->GetRestitution(), colinfo->pObject2->GetRestitution()) );
				else
					j = Impulse(colinfo->pObject1, colinfo->pObject2, &(colinfo->vIntersection), &(colinfo->vNormal), &vVelAB, 0.0f );
				// apply impulse to the objects, changing direction and magnitude of their velocities at the end of time interval
				vVelA = colinfo->pObject1->GetVelocity();
				vVelB = colinfo->pObject2->GetVelocity();
				D3DXVECTOR3 jn = j * colinfo->vNormal;
				if (stage == StageCollision)
				{
					// add to object dynamics data
					colinfo->pObjDynamics1->vjVel += (colinfo->vNormal*j)/colinfo->pObject1->GetMass();
					colinfo->pObjDynamics2->vjVel += (colinfo->vNormal*(-j))/colinfo->pObject2->GetMass();
					colinfo->pObjDynamics1->vjCollision += jn;
					colinfo->pObjDynamics2->vjCollision += -jn;
				}
				else
				{
					// add directly to entity kinematic data
					vVelA += (colinfo->vNormal*j)/colinfo->pObject1->GetMass();
					vVelB += (colinfo->vNormal*(-j))/colinfo->pObject2->GetMass();
				}
				D3DXVECTOR3 rAP = colinfo->vIntersection - colinfo->pObject1->GetPosition();
				D3DXVECTOR3 rBP = colinfo->vIntersection - colinfo->pObject2->GetPosition();
				D3DXVec3Cross(&vAngVelA, &rAP, &jn);
				jn = -jn;
				D3DXVec3Cross(&vAngVelB, &rBP, &jn);
				// multiply by inertia inverse
				D3DXMATRIX mInertiaInv = colinfo->pObject1->GetInertiaInv(true);
				vAngVelA = D3DXVECTOR3(mInertiaInv(0,0)*vAngVelA.x + mInertiaInv(0,1)*vAngVelA.y + mInertiaInv(0,2)*vAngVelA.z,
									   mInertiaInv(1,0)*vAngVelA.x + mInertiaInv(1,1)*vAngVelA.y + mInertiaInv(1,2)*vAngVelA.z,
									   mInertiaInv(2,0)*vAngVelA.x + mInertiaInv(2,1)*vAngVelA.y + mInertiaInv(2,2)*vAngVelA.z);
				mInertiaInv = colinfo->pObject2->GetInertiaInv(true);
				vAngVelB = D3DXVECTOR3(mInertiaInv(0,0)*vAngVelB.x + mInertiaInv(0,1)*vAngVelB.y + mInertiaInv(0,2)*vAngVelB.z,
									   mInertiaInv(1,0)*vAngVelB.x + mInertiaInv(1,1)*vAngVelB.y + mInertiaInv(1,2)*vAngVelB.z,
									   mInertiaInv(2,0)*vAngVelB.x + mInertiaInv(2,1)*vAngVelB.y + mInertiaInv(2,2)*vAngVelB.z);
				// add angular velocity caused by impulse to total impulse angular velocity
				if (stage == StageCollision)
				{
					colinfo->pObjDynamics1->vjAngVel += vAngVelA;
					colinfo->pObjDynamics2->vjAngVel += vAngVelB;
				}
				else
				{
					vAngVelA += colinfo->pObject1->GetAngVelocity();
					vAngVelB += colinfo->pObject2->GetAngVelocity();
				}

				// for certain types of objects there is a friction during the collision that
				// must be accounted for; friction acts in a direction perpendicular to normal force;
				if (colinfo->bFriction || stage == StageContact)
				{
					// assumptions: velocity does not change during time interval;
					// therefore, either static or kinetic friction acts during the 
					// entire interval; also, transition from sliding to rolling friction
					// of a sphere does not occur somewhere in the middle of time interval;
					// all friction transitions always occur at the time interval beginning;

					// calculate tangential vector, perpendicular to normal vector and aligned with relative velocity vector
					D3DXVECTOR3 vTangent;
					if (stage == StageCollision)
						D3DXVec3Cross(&vTangent, &vVelABEnd, &(colinfo->vNormal));
					else
						D3DXVec3Cross(&vTangent, &vVelAB, &(colinfo->vNormal));
					D3DXVec3Cross(&vTangent, &vTangent, &(colinfo->vNormal));
					if (D3DXVec3Length(&vTangent) > epsilon)
					{
						D3DXVec3Normalize(&vTangent, &vTangent);
						// calculate tangential impulse using Coulomb friction law
						// first, assume that the objects are sticking and apply static friction
						// maximum friction coefficient of the two objects is used
						float jt = max(colinfo->pObject1->GetStaticFriction(), colinfo->pObject2->GetStaticFriction()) * j;
						if (colinfo->pObject1->GetType() == OBJTYPE_LINE && colinfo->bEndPoint)
							jt = max(jt, ((CMathObjectLine*)(colinfo->pObject1))->GetTipStaticFriction());
						if (colinfo->pObject2->GetType() == OBJTYPE_LINE && colinfo->bEndPoint)
							jt = max(jt, ((CMathObjectLine*)(colinfo->pObject2))->GetTipStaticFriction());
						// friction does not work after the relative tangential velocity reaches zero;
						// to verify that, first calculate tangential impulse that is necessary to stop the
						// contact point in the collision tangential direction; to do that, simply use
						// the impulse formula, supplying it with a zero coefficient of restitution (as there
						// is no remaining velocity after the collision)
						float jtmax;
						if (stage == StageCollision)
							jtmax = Impulse(colinfo->pObject1, colinfo->pObject2, &(colinfo->vIntersection), &vTangent, &vVelABEnd, 0.0f);
						else
							jtmax = Impulse(colinfo->pObject1, colinfo->pObject2, &(colinfo->vIntersection), &vTangent, &vVelAB, 0.0f);
						if (fabs(jtmax) > fabs(jt))
						{
							// impulse is not in the friction cone and static friction can not 
							// hold collision point at rest - sliding friction is used instead
							jt = max(colinfo->pObject1->GetKineticFriction(), colinfo->pObject2->GetKineticFriction()) * j;
							if (colinfo->pObject1->GetType() == OBJTYPE_LINE && colinfo->bEndPoint)
								jt = max(jt, ((CMathObjectLine*)(colinfo->pObject1))->GetKineticFriction());
							if (colinfo->pObject2->GetType() == OBJTYPE_LINE && colinfo->bEndPoint)
								jt = max(jt, ((CMathObjectLine*)(colinfo->pObject2))->GetKineticFriction());
							jt = min(jt, jtmax); // friction should not reverse relative velocity direction
						}
						else
							jt = jtmax; // friction should not reverse relative velocity direction
						
						D3DXVECTOR3 vjt = vTangent*jt;
						// apply the impulse to objects, again changing their velocities
						// friction impulse acts in the direction opposite to tangential relative velocity:
						// in the direction of vector vTangent, velocity of object A is increased while the
						// velocity of object B is decreased
						// friction impulses also satisfy 3rd Newton's law
						if (stage == StageCollision)
						{
							colinfo->pObjDynamics1->vjVel += (vjt)/colinfo->pObject1->GetMass();
							colinfo->pObjDynamics2->vjVel += (-vjt)/colinfo->pObject2->GetMass();
							colinfo->pObjDynamics1->vjCollision += vjt;
							colinfo->pObjDynamics2->vjCollision += -vjt;
						}
						else
						{
							vVelA += (vjt)/colinfo->pObject1->GetMass();
							vVelB += (-vjt)/colinfo->pObject2->GetMass();
						}
						D3DXVECTOR3 vTmp;
						D3DXVec3Cross(&vTmp, &rAP, &vjt);
						// multiply by inertia inverse
						mInertiaInv = colinfo->pObject1->GetInertiaInv(true);
						if (stage == StageCollision)
							colinfo->pObjDynamics1->vjAngVel += D3DXVECTOR3(mInertiaInv(0,0)*vTmp.x + mInertiaInv(0,1)*vTmp.y + mInertiaInv(0,2)*vTmp.z,
													mInertiaInv(1,0)*vTmp.x + mInertiaInv(1,1)*vTmp.y + mInertiaInv(1,2)*vTmp.z,
													mInertiaInv(2,0)*vTmp.x + mInertiaInv(2,1)*vTmp.y + mInertiaInv(2,2)*vTmp.z);
						else
							vAngVelA += D3DXVECTOR3(mInertiaInv(0,0)*vTmp.x + mInertiaInv(0,1)*vTmp.y + mInertiaInv(0,2)*vTmp.z,
													mInertiaInv(1,0)*vTmp.x + mInertiaInv(1,1)*vTmp.y + mInertiaInv(1,2)*vTmp.z,
													mInertiaInv(2,0)*vTmp.x + mInertiaInv(2,1)*vTmp.y + mInertiaInv(2,2)*vTmp.z);
						D3DXVec3Cross(&vTmp, &rBP, &(-vjt));
						// multiply by inertia inverse
						mInertiaInv = colinfo->pObject2->GetInertiaInv(true);
						if (stage == StageCollision)
							colinfo->pObjDynamics2->vjAngVel += D3DXVECTOR3(mInertiaInv(0,0)*vTmp.x + mInertiaInv(0,1)*vTmp.y + mInertiaInv(0,2)*vTmp.z,
													mInertiaInv(1,0)*vTmp.x + mInertiaInv(1,1)*vTmp.y + mInertiaInv(1,2)*vTmp.z,
													mInertiaInv(2,0)*vTmp.x + mInertiaInv(2,1)*vTmp.y + mInertiaInv(2,2)*vTmp.z);
						else
							vAngVelB += D3DXVECTOR3(mInertiaInv(0,0)*vTmp.x + mInertiaInv(0,1)*vTmp.y + mInertiaInv(0,2)*vTmp.z,
													mInertiaInv(1,0)*vTmp.x + mInertiaInv(1,1)*vTmp.y + mInertiaInv(1,2)*vTmp.z,
													mInertiaInv(2,0)*vTmp.x + mInertiaInv(2,1)*vTmp.y + mInertiaInv(2,2)*vTmp.z);
					}
					else if (stage == StageContact)
					{
						// apply rolling friction to a sphere when relative tangential velocity equals zero
						if (colinfo->pObject1->GetType() == OBJTYPE_SPHERE && colinfo->pObject2->GetType() == OBJTYPE_PLANE ||
							colinfo->pObject1->GetType() == OBJTYPE_PLANE && colinfo->pObject2->GetType() == OBJTYPE_SPHERE)
						{
							CMathObjectSphere *sphere;
							CMathObjectPlane *plane;
							if (colinfo->pObject1->GetType() == OBJTYPE_SPHERE)
							{
								sphere = (CMathObjectSphere*)colinfo->pObject1;
								plane = (CMathObjectPlane*)colinfo->pObject2;
							}
							else
							{
								sphere = (CMathObjectSphere*)colinfo->pObject2;
								plane = (CMathObjectPlane*)colinfo->pObject1;
							}
							// project translational velocity onto the collision tangential plane
							D3DXVECTOR3 vVelCenter = sphere->GetVelocity() - plane->GetVelocity();
							D3DXVec3Cross(&vVelCenter, &vVelCenter, &(colinfo->vNormal));
							D3DXVec3Cross(&vVelCenter, &(colinfo->vNormal), &vVelCenter);

							// now add the friction force which opposes the tangential linear velocity of the sphere mass center
							D3DXVECTOR3 vFriction = -vVelCenter; // TODO: verify the sign is necessary
							if (D3DXVec3Length(&vFriction) > epsilon)
								D3DXVec3Normalize(&vFriction, &vFriction);
							else
								vFriction = D3DXVECTOR3(0.0f,0.0f,0.0f);
							// we simulate rolling friction using two impulses
							D3DXVECTOR3 vjt = vFriction * j * sphere->GetRollingFriction(); // TODO: rolling friction only depends on sphere material, which is not true
							vVelA += (vjt)/colinfo->pObject1->GetMass();
							vVelB += (-vjt)/colinfo->pObject2->GetMass();
							// since there is no velocity at the contact point, there is no sliding friction that could cause rotation
							// there is a static friction, however, and it opposes rolling friction force
							// also, since there is no velocity at the contact point, there is no energy dissipation -
							// sphere kinetic energy remains the same; static friction magnitude is 2/7 of rolling friction (see Shepard, APAPP):
							vjt = -vjt*(2.0f/7);
							vVelA += (vjt)/colinfo->pObject1->GetMass();
							vVelB += (-vjt)/colinfo->pObject2->GetMass();
							// TODO: if the friction force will reverse velocity direction in the
							// given time interval, the force is invalid and we must signal the simulator to
							// reduce time interval
							D3DXVECTOR3 vTmp;
							D3DXVec3Cross(&vTmp, &rAP, &vjt);
							// multiply by inertia inverse
							mInertiaInv = colinfo->pObject1->GetInertiaInv(true);
							vAngVelA += D3DXVECTOR3(mInertiaInv(0,0)*vTmp.x + mInertiaInv(0,1)*vTmp.y + mInertiaInv(0,2)*vTmp.z,
													mInertiaInv(1,0)*vTmp.x + mInertiaInv(1,1)*vTmp.y + mInertiaInv(1,2)*vTmp.z,
													mInertiaInv(2,0)*vTmp.x + mInertiaInv(2,1)*vTmp.y + mInertiaInv(2,2)*vTmp.z);
							D3DXVec3Cross(&vTmp, &rBP, &(-vjt));
							// multiply by inertia inverse
							mInertiaInv = colinfo->pObject2->GetInertiaInv(true);
							vAngVelB += D3DXVECTOR3(mInertiaInv(0,0)*vTmp.x + mInertiaInv(0,1)*vTmp.y + mInertiaInv(0,2)*vTmp.z,
													mInertiaInv(1,0)*vTmp.x + mInertiaInv(1,1)*vTmp.y + mInertiaInv(1,2)*vTmp.z,
													mInertiaInv(2,0)*vTmp.x + mInertiaInv(2,1)*vTmp.y + mInertiaInv(2,2)*vTmp.z);
						}
					}

				}

				// save new velocities
				if (colinfo->pObject1->IsFixed() == false && stage == StageContact)
				{
					colinfo->pObject1->SetVelocity(&vVelA);
					colinfo->pObject1->SetAngVelocity(&vAngVelA);
					// synchronize angular momentum with updated angular velocity
					colinfo->pObject1->SyncAngMomentum();
				}
				if (colinfo->pObject2->IsFixed() == false && stage == StageContact)
				{
					colinfo->pObject2->SetVelocity(&vVelB);
					colinfo->pObject2->SetAngVelocity(&vAngVelB);
					// synchronize angular momentum with updated angular velocity
					colinfo->pObject2->SyncAngMomentum();
				}
			}
		}
		collisions = collisions->next;
	}

	return iCount;
}

//-----------------------------------------------------------------------------
// Name: Impulse(obj1, obj2, vNormal, vRelVelocity, fCoefRestitution)
// Desc: Calculate collision impulse in the collision normal direction.
// Par.: obj1, obj2 - collision objects
//       vIntersection - collision point
//       vNormal - collision normal, pointing towards obj1
//		 vRelVelocity - relative velocity of the collision objects
//       fCoefRestitution - coef. of restitution, defining elasticity of collision
//-----------------------------------------------------------------------------
float CPhysics::Impulse(CMathObject* obj1, CMathObject* obj2, D3DXVECTOR3 *vIntersection, D3DXVECTOR3 *vNormal, D3DXVECTOR3 *vRelVelocity, float fCoefRestitution)
{
	float j;
	D3DXVECTOR3 vDenom1, vDenom2, vDenom3;
	// TODO: if object is line, use its center position
	D3DXVECTOR3 rAP = *vIntersection - obj1->GetPosition();
	D3DXVECTOR3 rBP = *vIntersection - obj2->GetPosition();
	j = -D3DXVec3Dot(vRelVelocity, vNormal) * (1.0f+fCoefRestitution);
	vDenom1 = (1/obj1->GetMass() + 1/obj2->GetMass()) * *vNormal;
	D3DXVec3Cross(&vDenom2, &rAP, vNormal);
	D3DXMATRIX mInertiaInv = obj1->GetInertiaInv(true);
	vDenom2 = D3DXVECTOR3(mInertiaInv(0,0)*vDenom2.x + mInertiaInv(0,1)*vDenom2.y + mInertiaInv(0,2)*vDenom2.z,
		                  mInertiaInv(1,0)*vDenom2.x + mInertiaInv(1,1)*vDenom2.y + mInertiaInv(1,2)*vDenom2.z,
						  mInertiaInv(2,0)*vDenom2.x + mInertiaInv(2,1)*vDenom2.y + mInertiaInv(2,2)*vDenom2.z);
	D3DXVec3Cross(&vDenom2, &vDenom2, &rAP);
	D3DXVec3Cross(&vDenom3, &rBP, vNormal);
	mInertiaInv = obj2->GetInertiaInv(true);
	vDenom3 = D3DXVECTOR3(mInertiaInv(0,0)*vDenom3.x + mInertiaInv(0,1)*vDenom3.y + mInertiaInv(0,2)*vDenom3.z,
		                  mInertiaInv(1,0)*vDenom3.x + mInertiaInv(1,1)*vDenom3.y + mInertiaInv(1,2)*vDenom3.z,
						  mInertiaInv(2,0)*vDenom3.x + mInertiaInv(2,1)*vDenom3.y + mInertiaInv(2,2)*vDenom3.z);
	D3DXVec3Cross(&vDenom3, &vDenom3, &rBP);
	vDenom2 = vDenom2+vDenom3;
	j = j / ( D3DXVec3Dot(&vDenom1, vNormal) + D3DXVec3Dot(&vDenom2, vNormal) );
	return j;
}


//-----------------------------------------------------------------------------
// Name: StepSimulation(dt)
// Desc: Main dynamics algorithm which simulates the physics world for a portion
//       of time; objects are moved under influence of the forces and torques and
//       collisions are accounted for;
// Ret.: velocity magnitude of the fastest object at the end of time step;
//-----------------------------------------------------------------------------
float CPhysics::StepSimulation(float dt)
{
	float				dTime = dt;
	MathObjectListPtr	entities;
	CMathObject			*entity;
	ObjDynamicsListPtr	objdynamics;
	D3DXVECTOR3			vValue;
	float				fProx;
	float				fTime;
	float				fMaxVelocity = 0.0f;

	iTimeStamp += (int)(dt*1000);

	while (dt)
	{
		dTime = min(0.001, dt);
		// summon external forces into the simulated world
		InitForces();
		ApplyForces();

		// save object data in case we will have to back-up later and shorten the interval;
		// also, initial kinematics data will be restored later when a 2nd integration will be performed
		InitTimeStep();

		// integration of kinematic quantities
		// only positions are actually updated since old velocites are to be used in collision detection
		entities = pEntities;
		objdynamics = pObjDynamics;
		while (entities != NULL)
		{
			entity = entities->pMathObject;
			if (entity != NULL)
			{
				// update angular momentum and angular velocity
				vValue = entity->GetAngMomentum() + dTime*entity->GetMomentSum();
				entity->SetAngMomentum(&vValue);
				entity->SyncAngVelocity();

				// update linear velocity
				vValue = entity->GetVelocity() + dTime*entity->GetForceSum()/entity->GetMass();
				entity->SetVelocity(&vValue);

				// update entity object position
				vValue = entity->GetPosition() + dTime*entity->GetVelocity();
				entity->SetPosition(&vValue);

				// update orientation quaternion (see Physics for Game Developers, p.229)
				D3DXQUATERNION qRotationNew, qAngVel;
				vValue = entity->GetAngVelocity();
				qAngVel.x = vValue.x;
				qAngVel.y = vValue.y;
				qAngVel.z = vValue.z;
				qAngVel.w = 0;
				D3DXQuaternionMultiply(&qRotationNew, &qAngVel, &(entity->GetRotation()));
				qRotationNew = entity->GetRotation() + dTime * 0.5f * qRotationNew;
				D3DXQuaternionNormalize(&qRotationNew, &qRotationNew);
				entity->SetRotation(&qRotationNew);

				// store new velocities into object dynamics data while leaving current entity velocities intact
				// TODO: this is not computationaly effective! (duplicate, to and fro, matrix multiplication)
				objdynamics->vAngVelEnd = entity->GetAngVelocity();
				entity->SetAngVelocity(&(objdynamics->vAngVelInitial));
				entity->SyncAngMomentum();
				objdynamics->vVelEnd = entity->GetVelocity();
				entity->SetVelocity(&(objdynamics->vVelInitial));

			}
			entities = entities->next;
			objdynamics = objdynamics->next;
		}

		// detect and resolve collison
		CollisionListPtr collisions = DetectCollision(&fProx);
		// halve integration step if any of the collisions was too deep, but never below 1ms (TODO: should not be hardcoded)
		if (fProx < 0 && dTime >= 0.002)
		{
			dTime = dTime/2;
			RestoreBackup();
			ReleaseBackup();
			continue;
		}
		// while there are any objects still on the collision course, resolve their 
		// collision (note, that when a velocity of the object switches direction, it may 
		// then be colliding with another object that is close enough)
		int iIteration = 0;
		while (ResolveCollision(collisions, StageCollision) && iIteration < 50) // TODO: parametrize this
		{
			iIteration++;
		}

		// now that all collision have been found and resolved, we may update velocities
		// using the previously executed integration step; note, that the impulses are
		// not applied yet, since these are not active until the end of time interval and
		// we have yet to apply contact impulses to velocities during the time interval
		entities = pEntities;
		objdynamics = pObjDynamics;
		while (entities != NULL)
		{
			entity = entities->pMathObject;
			if (entity != NULL)
			{
				entity->SetVelocity(&(objdynamics->vVelEnd));
				entity->SetAngVelocity(&(objdynamics->vAngVelEnd));
				entity->SyncAngMomentum();
			}
			entities = entities->next;
			objdynamics = objdynamics->next;
		}

		// contact resolution stage; all collisions still occuring are actually contacts
		// which are handled as inelastic collisions
		while (ResolveCollision(collisions, StageContact)) ;

		// final integration of velocities into new entity positions; again, collision
		// impulses are omitted from integration since they are only activated at the end of time interval
		entities = pEntities;
		objdynamics = pObjDynamics;
		while (entities != NULL)
		{
			entity = entities->pMathObject;
			if (entity != NULL)
			{
				// restore initial positions from the time interval start
				entity->SetPosition(&(objdynamics->vPosInitial));
				entity->SetRotation(&(objdynamics->qRotInitial));

				// update entity object position
				vValue = entity->GetPosition() + dTime*entity->GetVelocity();
				entity->SetPosition(&vValue);

				// update orientation quaternion (see Physics for Game Developers, p.229)
				D3DXQUATERNION qRotationNew, qAngVel, q;
				q = entity->GetRotation();
				vValue = entity->GetAngVelocity();
				qAngVel.x = vValue.x;
				qAngVel.y = vValue.y;
				qAngVel.z = -vValue.z; // must negate, since quaternion D3DX methods are written for right-handed system; (otherwise rotation goes berserk after a while)
				qAngVel.w = 0;
				D3DXQuaternionMultiply(&qRotationNew, &qAngVel, &q);
				qRotationNew = q + dTime * 0.5f * qRotationNew;
				D3DXQuaternionNormalize(&qRotationNew, &qRotationNew);
				entity->SetRotation(&qRotationNew);
				
				// finally, apply collision impulse, which instantly changes velocity at the end of current time interval
				vValue = entity->GetVelocity();
				vValue += objdynamics->vjVel;
				entity->SetVelocity(&vValue);
				if (dt == dTime)
					fMaxVelocity = max(fMaxVelocity, D3DXVec3Length(&vValue));
				vValue = entity->GetAngVelocity();
				vValue += objdynamics->vjAngVel;
				//if (dt == dTime)
				//	fMaxVelocity = max(fMaxVelocity, D3DXVec3Length(&vValue));
				entity->SetAngVelocity(&vValue);
				entity->SyncAngMomentum();
			}
			entities = entities->next;
			objdynamics = objdynamics->next;
		}

		ReleaseBackup();
		dt -= dTime;
		dTime = dt; // if there is any time remaining, simulate it during the next iteration
	}

	return fMaxVelocity;
}

//-----------------------------------------------------------------------------
// Name: InitTimeStep()
// Desc: Make a backup of all entities and initialize object dynamics data.
//-----------------------------------------------------------------------------
void CPhysics::InitTimeStep()
{
	pEntitiesBack = (MathObjectListPtr)malloc(sizeof(MathObjectList));
	MathObjectListPtr pEntitiesBackStart = pEntitiesBack;
	MathObjectListPtr entities = pEntities;
	pEntitiesBack->pMathObject = NULL;
	pEntitiesBack->next = NULL;
	CMathObject *entity;
	ObjDynamicsListPtr objdynamics = pObjDynamics;

	// for each element in object list, make a new element in the backup list and copy
	// data between them
	while (entities != NULL)
	{
		entity = entities->pMathObject;
		if (entity != NULL)
		{
			if (pEntitiesBack->pMathObject != NULL)
			{
				pEntitiesBack->next = (MathObjectListPtr)malloc(sizeof(MathObjectList));
				pEntitiesBack = pEntitiesBack->next;
			}
			pEntitiesBack->pMathObject = new CMathObject();
			// copy all primary data
			// TODO: verify this works
			*(pEntitiesBack->pMathObject) = *entity;
			pEntitiesBack->next = NULL;
		}

		// also, initialize object dynamics data that applies to the beginning of time interval
		// note: it is assumed objdynamics list is in the same order as the entity list
		if (objdynamics != NULL)
		{
			objdynamics->vVelInitial = entity->GetVelocity();
			objdynamics->vAngVelInitial = entity->GetAngVelocity();
			objdynamics->vPosInitial = entity->GetPosition();
			objdynamics->qRotInitial = entity->GetRotation();
			objdynamics->vjCollision = D3DXVECTOR3(0,0,0);
			objdynamics->vjContact = D3DXVECTOR3(0,0,0);
			objdynamics->vjVel = D3DXVECTOR3(0,0,0);
			objdynamics->vjAngVel = D3DXVECTOR3(0,0,0);
			objdynamics->vAngVelEnd = D3DXVECTOR3(0,0,0);
			objdynamics = objdynamics->next;
		}

		entities = entities->next;
	}

	pEntitiesBack = pEntitiesBackStart;
}

//-----------------------------------------------------------------------------
// Name: RestoreBackup()
// Desc: Restore saved objects' data.
//-----------------------------------------------------------------------------
void CPhysics::RestoreBackup()
{
	MathObjectListPtr entities = pEntities;
	MathObjectListPtr entitiesback = pEntitiesBack;

	while (entitiesback != NULL)
	{
		if (entities != NULL) // just to be safe
		{
			if (entitiesback->pMathObject != NULL)
			{
				if (entities->pMathObject != NULL) // just to be safe
				{
					*(entities->pMathObject) = *(entitiesback->pMathObject);
				}
			}
		}
		entitiesback = entitiesback->next;
		if (entities != NULL)
			entities = entities->next;
	}
}

//-----------------------------------------------------------------------------
// Name: ReleaseBackup()
// Desc: Dispose all backup data.
//-----------------------------------------------------------------------------
void CPhysics::ReleaseBackup()
{
	MathObjectListPtr entitiesback = pEntitiesBack;
	MathObjectListPtr entity;

	while (entitiesback != NULL)
	{
		if (entitiesback->pMathObject != NULL)
			free(entitiesback->pMathObject);
		entity = entitiesback;
		entitiesback = entitiesback->next;
		free(entity);
	}
	pEntitiesBack = NULL;
}

//-----------------------------------------------------------------------------
// Name: GetTime()
// Desc: Return current time of the simulation passed [ms].
//-----------------------------------------------------------------------------
int	CPhysics::GetTime()
{
	return iTimeStamp;
}


//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Release all memory reserved by the module.
//-----------------------------------------------------------------------------
void CPhysics::Cleanup()
{
	// clear math-object and external-force lists
	MathObjectListPtr objlist = pEntities;
	MathObjectListPtr objlistnext;
	while (objlist != NULL)
	{
		objlistnext = objlist->next;
		// release list element
		free(objlist);
		objlist = objlistnext;
	}
	ForceListPtr objlist2 = pExtForces;
	ForceListPtr objlist2next;
	while (objlist2 != NULL)
	{
		objlist2next = objlist2->next;
		// release list element
		free(objlist2);
		objlist2 = objlist2next;
	}
	ObjDynamicsListPtr objdynlist = pObjDynamics;
	ObjDynamicsListPtr objdynlistnext;
	while (objdynlist != NULL)
	{
		objdynlistnext = objdynlist->next;
		// release list element
		free(objdynlist);
		objdynlist = objdynlistnext;
	}

}
