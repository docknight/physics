/* ******************************************************************************** */
/* Physics.h                                                                        */
/*                                                                                  */
/* Physics simulation of the objects in the game world                              */
/*                                                                                  */
/* ******************************************************************************** */
#include "stdafx.h"

#pragma once

#include "MathObjects.h"

#define	g	(9.8f)	// gravitational acceleration
#define coefRestitution (0.92f)	// kinetic coefficient of restituion, currently equal for all objects
#define kineticfriction (0.15f)
#define slidingfriction (0.2f)
#define rollingfriction (0.7f)
#define staticfriction (0.2f)

#define epsilon (0.0001f) // zero value tolerance


// different forces have different properties
#define FORCETYPE_GLOBAL		1
#define FORCETYPE_OBJECT		2

enum CollisionStage
	{
		StageCollision,
		StageContact
	};


// external force representation (cue stick, gravity, for instance)
class CForce
{
private:
	int				iType;			// global forces are distinguished from those acting on a certain object
	D3DXVECTOR3		vDirection;		// normalized direction of the force
	D3DXVECTOR3		vPosition;		// force application point, relative to pMathObject's position (if type is GLOBAL, this property is not used)
	float			fMagnitude;		// force magnitude in Newtons (discarded if this is a gravitational force)
	CMathObject		*pMathObject;	// object that the force is acting on; not used if force type is GLOBAL
	int				iTimeStart;		// time offset in ms after the force starts to act
	int				iDuration;		// time in ms for the force to remain active (can be set to INFINITE)
	bool			bGravity;		// whether this is a gravitational force which depends on the object mass
public:
	CForce();
	void			SetType(int type);
	int				GetType();
	void			SetDirection(D3DXVECTOR3 *dir);
	void			SetPosition(D3DXVECTOR3 *pos);
	void			SetMagnitude(float mag);
	void			AssignObject(CMathObject *obj);		// assign a specific math object to be susceptive to this force
	void			SetGravity(bool gravity);
	void			SetStart(int time);					// set time offset when the force awakens
	void			SetDuration(int time);				// set duration of the active force
	D3DXVECTOR3		GetDirection();
	D3DXVECTOR3		GetPosition();
	float			GetMagnitude();
	CMathObject*	GetObject();
	bool			IsGravity();
	int				GetStart();
	int				GetDuration();
};


typedef struct ForceList* ForceListPtr;
// list of forces
struct ForceList
{
	CForce			*pForce;
	ForceListPtr	next;
};


typedef struct ObjDynamicsList* ObjDynamicsListPtr;

// data describing a single collision between two objects
struct CollisionInfo
{
	CMathObject*		pObject1;
	CMathObject*		pObject2;
	ObjDynamicsListPtr	pObjDynamics1;	// velocity data that changes during collision
	ObjDynamicsListPtr	pObjDynamics2;	// velocity data that changes during collision
	D3DXVECTOR3			vIntersection;	// collision point
	D3DXVECTOR3			vNormal;		// collision normal, pointing towards pObject1 by convention
	bool				bFriction;		// specifies whether there is a friction to be resolved during the collision of these objects
	bool				bEndPoint;		// true, if collision occured at endpoint of one of the objects (line or plane); required by collision-resolve to choose proper coef. of friction
};

typedef struct CollisionList* CollisionListPtr;
// list of collisions data
struct CollisionList
{
	CollisionInfo		*pCollision;
	CollisionListPtr	next;
};


// list of per-object data which is stored and used during dynamics step calculation
// it is maintained in the exact same order as 
struct ObjDynamicsList
{
	D3DXVECTOR3			vVelInitial; // initial velocity of the object at the beginning of the time interval
	D3DXVECTOR3			vAngVelInitial;
	D3DXVECTOR3			vVelEnd; // velocity at the end of the time interval, changed due to external forces acting on object
	D3DXVECTOR3			vAngVelEnd;
	D3DXVECTOR3			vPosInitial; // position at the beginning of the time interval
	D3DXQUATERNION		qRotInitial; // orientation at the beginning of the time interval
	D3DXVECTOR3			vjCollision; // impulse vector, which is added to velocity at the end of interval due to any collisions that occur
	D3DXVECTOR3			vjVel;		// velocity obtained due to collision impulse above
	D3DXVECTOR3			vjAngVel;	// angular velocity obtained due to collsion impulse above
	D3DXVECTOR3			vjContact; // impulse vector, which is added to velocity at the beginning of time interval due to any contact forces that occur
	int					iObjIndex; // index of the object in the object list, maintained by CPhysics class
	ObjDynamicsListPtr	next;
};



// container for a list of objects which are transformed through common physical laws
class	CPhysics
{
protected:
	MathObjectListPtr	pEntities;	// list of game entities that comply to physical laws
	ForceListPtr		pExtForces; // list of external forces that act on one or more game entities
	ObjDynamicsListPtr	pObjDynamics; // list of dynamic object data; it is maintained in the exact same order as the entities list
	int					iTimeStamp; // current time of the simulation passed [miliseconds]
private:
	MathObjectListPtr	pEntitiesBack;	// backup of entities
	CollisionListPtr	DetectCollision(float *fminprox);	
	int					ResolveCollision(CollisionListPtr collisions, CollisionStage stage);
	void				ApplyForces(); // apply (external) forces to entities
	void				InitForces(); // initialize forces and moments of all objects to zero 
	float				Impulse(CMathObject* obj1, CMathObject* obj2, D3DXVECTOR3 *vIntersection, D3DXVECTOR3 *vNormal, D3DXVECTOR3 *vRelVelocity, float fCoefRestitution);		// calculate collision impulse in the collision normal direction
	void				InitTimeStep(); // make a backup of all entities and initialize object dynamics data
	void				RestoreBackup(); // restore saved objects' data
	void				ReleaseBackup();
public:
	CPhysics();	 // class constructor
	void				StartSim(); // starts the simulation by initializing time values etc.
	void				AddEntity(CMathObject * pMathObject); // add pre-initialized object to list of simulated objects
	void				AddForce(CForce * pForce); // add pre-initialized force object to list of forces
	float				StepSimulation(float dt);
	int					GetTime(); // return current time of the simulation passed [ms]
	void				ClearForces(); // clear the list of external forces
	void				Cleanup();	// release all memory reserved by the module
};

