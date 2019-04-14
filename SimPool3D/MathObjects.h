/* ******************************************************************************** */
/* MathObjects.h                                                                    */
/*                                                                                  */
/* Mathematicaly represented game entities                                          */
/*                                                                                  */
/* ******************************************************************************** */
#include "stdafx.h"

#pragma once

#define INFINITY				1000000
#define INTERSECTION_THRESHOLD	0.01f

// different object types have different properties
#define OBJTYPE_SPHERE		1
#define OBJTYPE_BOX			2
#define OBJTYPE_EDGE		3
#define OBJTYPE_LINE		4
#define OBJTYPE_CIRCLE		5
#define OBJTYPE_RECT		6
#define OBJTYPE_PLANE		7
#define OBJTYPE_ARBITRARY	8	// for non-uniform objects with custom inertia tensor (mass distribution)


class	CMathObjectPlane;	// forward declaration of the plane object
class	CMathObjectLine;	// forward declaration of the line object
class	CMathObjectSphere;	// forward declaration of the sphere object

// methods and data required to represent and interact with a general object in 3D space
class	CMathObject
{
protected:
	int				iType;				// object type
	bool			bFixed;				// whether the object is fixed in the world and is not influenced by the forces
	float			fMass;				// object mass
	D3DXMATRIX		mInertia;			// inertia tensor in local (body) coordinates; must be changed to world coords. during integration process for complex objects (see GDMPHYS4.pdf, Eq.20)
	D3DXMATRIX		mInertiaInv;		// inverse of the inertia tensor in body coordinates
	D3DXVECTOR3		vPosition;			// object position in the world coordinates (if the object is a plane, this is a center point on the bounded plane)
	D3DXQUATERNION	qRotation;			// object rotational quaternion (normalized)
	D3DXVECTOR3		vVelocity;			// linear velocity of the mass center
	D3DXVECTOR3		vAngVel;			// TODO: might not be necessary (get it from ang.momentum, GDMPHYS4,Eq.18);angular velocity vector
	D3DXVECTOR3		vAngMomentum;		// angular momentum of the object
	D3DXVECTOR3		vForceSum;			// sum of forces acting on the body mass center
	D3DXVECTOR3		vMomentSum;			// sum of torques (moments) acting on the body mass center (assuming this is the point of rotation)
	float			fFrictionStatic;	// static friction of the object when it is not moving relative to the object that it is penetrating with
	float			fFrictionKinetic;	// kinetic friction of the object when tangential contact velocity is not zero
	float			fRestitution;		// coefficient of restitution defining the kinetic energy loss during collision

public:
	CMathObject();
	void			SetType(int type);
	void			SetPosition(D3DXVECTOR3 *pvPosition);
	void			SetPosition(float x, float y, float z);
	void			SetRotation(D3DXQUATERNION *pqRotation);
	void			SetVelocity(D3DXVECTOR3 *pvVelocity);
	void			SetAngVelocity(D3DXVECTOR3 *pvAngVel);
	void			SetAngMomentum(D3DXVECTOR3 *pvAngMomentum);
	void			SetMass(float m);
	void			SetFixed(bool fixed);
	void			SetInertia(D3DXMATRIX *pmInertia);
	void			SetInertiaInv(D3DXMATRIX *pmInertiaInv);
	void			SetForceSum(D3DXVECTOR3 *pvForce);
	void			SetMomentSum(D3DXVECTOR3 *pvMoment);
	void			SetStaticFriction(float friction);
	void			SetKineticFriction(float friction);
	void			SetRestitution(float epsilon);
	D3DXVECTOR3		GetPosition();
	D3DXVECTOR3		GetVelocity();
	D3DXVECTOR3		GetAngVelocity();
	D3DXVECTOR3		GetAngMomentum();
	int				GetType();
	bool			IsFixed();
	float			GetMass();
	D3DXMATRIX		GetInertia(bool bWorld);
	D3DXMATRIX		GetInertiaInv(bool bWorld);
	D3DXQUATERNION	GetRotation();
	D3DXVECTOR3		GetForceSum();
	D3DXVECTOR3		GetMomentSum();
	float			GetStaticFriction();
	float			GetKineticFriction();
	float			GetRestitution();
	void			SyncAngMomentum();
	void			SyncAngVelocity();
	void			AddForce(D3DXVECTOR3 *pvForce);
	void			AddMoment(D3DXVECTOR3 *pvMoment);
	virtual float	PlaneIntersection(CMathObjectPlane *plane, D3DXVECTOR3 *vpIntersection, bool *bEndPoint, D3DXVECTOR3 *vNormal); // determine intersection of the object with a plane
	virtual float	LineIntersection(CMathObjectLine *line, D3DXVECTOR3 *vpIntersection, bool *bEndPoint, D3DXVECTOR3 *vNormal); // determine intersection of the object with a line
	virtual float   SphereIntersection(CMathObjectSphere *sphere, D3DXVECTOR3 *vpIntersection); // determine intersection of the object with a sphere
};

// methods and data required to represent and interact with a sphere in 3D space
class	CMathObjectSphere : public CMathObject
{
private:
	float			fRadius;			// sphere radius
	float			fFrictionRolling;	// rolling kinetic friction when sphere reaches rolling equilibrium during contact
public:
	CMathObjectSphere() { iType = OBJTYPE_SPHERE; }
	void			SetRadius(float r);
	void			SetRollingFriction(float friction);
	float			GetRadius();
	float			GetRollingFriction();
	float			PlaneIntersection(CMathObjectPlane *plane, D3DXVECTOR3 *vpIntersection, bool *bEndPoint, D3DXVECTOR3 *vNormal); // determine intersection of the sphere with a plane
	float			LineIntersection(CMathObjectLine *line, D3DXVECTOR3 *vpIntersection, bool *bEndPoint, D3DXVECTOR3 *vNormal); // determine intersection of the object with a line
	float			SphereIntersection(CMathObjectSphere *sphere, D3DXVECTOR3 *vpIntersection); // determine intersection of the object with a sphere
};

// methods and data required to represent and interact with a 2D circle in 3D space
class	CMathObjectCircle : public CMathObject
{
private:
	float			fRadius;		// circle radius
	D3DXVECTOR3		vNormal;		// circle normal vector (note, that two opposing directions define the same circle)
public:
	void			SetRadius(float r);
	float			GetRadius();
	void			SetNormal(D3DXVECTOR3 *pvNormal);
	D3DXVECTOR3		GetNormal(bool bTransformed);
	float			PlaneIntersection(CMathObjectPlane *plane, D3DXVECTOR3 *vpIntersection, bool *bEndPoint, D3DXVECTOR3 *vNormal); // determine intersection of the circle with a plane
	float			LineIntersection(CMathObjectLine *line, D3DXVECTOR3 *vpIntersection, bool *bEndPoint, D3DXVECTOR3 *vNormal); // determine intersection of the object with a line
	float			SphereIntersection(CMathObjectSphere *sphere, D3DXVECTOR3 *vpIntersection); // determine intersection of the object with a sphere
};

// methods and data required to represent and interact with a 2D rectangle in 3D space
class	CMathObjectRect : public CMathObject
{
private:
	float			fLength;		// length of the rectangle (along the rotated vLength local axis)
	float			fWidth;			// width of the rectangle (along the rotated vWidth local axis)
	// rectangle orientation data; note that to get current vectors, these must be combined with object rotation quaternion
	D3DXVECTOR3		vNormal;		// rectangle normal vector, defined as [N] = [vLength] x [vWidth] (note, that two opposing directions define the same rectangle)
	D3DXVECTOR3		vLength;		// vLength and vWidth are perpendicular normalized vectors, aligned with world axes X and Z, respectively
	D3DXVECTOR3		vWidth;
public:
	void			SetSize(float length, float width);
	float			GetLength();
	float			GetWidth();
	void			SetNormal(D3DXVECTOR3 *pvNormal);	// set initial value for the normal
	D3DXVECTOR3		GetNormal(bool bTransformed);	// get (rotated) normal vector
	void			SetLengthVector(D3DXVECTOR3 *pvLength);		// set initial value for the length vector
	void			SetWidthVector(D3DXVECTOR3 *pvWidth);		// set initial value for the width vector
	D3DXVECTOR3		GetLengthVector(bool bRotated);	// get normalized (rotated) length vector
	D3DXVECTOR3		GetWidthVector(bool bRotated);	// get normalized (rotated) width vector
	float			PlaneIntersection(CMathObjectPlane *plane, D3DXVECTOR3 *vpIntersection, bool *bEndPoint, D3DXVECTOR3 *vNormal); // determine intersection of the rectangle with a plane
	float			LineIntersection(CMathObjectLine *line, D3DXVECTOR3 *vpIntersection, bool *bEndPoint, D3DXVECTOR3 *vNormal); // determine intersection of the object with a line
	float			SphereIntersection(CMathObjectSphere *sphere, D3DXVECTOR3 *vpIntersection); // determine intersection of the object with a sphere
};

// methods and data required to represent and interact with a line in 3D space
class	CMathObjectLine : public CMathObject
{
private:
	D3DXMATRIX		mInertia;			// inertia tensor in local (body) coordinates; must be changed to world coords. during integration process for complex objects (see GDMPHYS4.pdf, Eq.20)
	D3DXMATRIX		mInertiaInv;		// inverse of the inertia tensor in body coordinates
	D3DXVECTOR3		vOrientation;		// normalized line orientation; line direction = vOrientation rotated by qRotation; TODO: could we not just use qRotation.x,.y and .z as the normalized line orientation ?? we could, but since there is a possibility of more vectors to be added to line object in the future (like, normal) it is better to have a separate orientation to define all of their orientation
	float			fMagnitude;			// lines are represented by a normalized vector vOrientation from vPosition which is then rotated by qRotation and scaled by fMagnitude
	float			fFrictionTipStatic; // static friction of the line tip
	float			fFrictionTipKinetic;// kinetic (sliding) friction of the line tip
public:
	void			SetMagnitude(float mag);
	void			SetOrientation(D3DXVECTOR3 *pvOrientation );
	void			SetTipStaticFriction(float friction);
	void			SetTipKineticFriction(float friction);
	D3DXVECTOR3		GetCenterPosition();	// get position of the line center
	float			GetMagnitude();
	D3DXVECTOR3		GetOrientation();
	D3DXVECTOR3		GetDirection(); // function calculates line normalized direction vector, using orientation and rotation vector and quaternion, respectively
	float			GetTipStaticFriction();
	float			GetTipKineticFriction();
};



// list of math objects is used for two reasons: 
// first, physics module uses the list to store all entities which are present in the simulated world
// second, plane object (below) is a composite of objects, which is represented by a list
typedef struct MathObjectList* MathObjectListPtr;
// mathematical object list
struct MathObjectList
{
	CMathObject			*pMathObject;
	MathObjectListPtr	next;
};


// methods and data required to represent and interact with a plane in 3D space;
// plane is a special composite of the objects; all objects are 2D and are assumed to
// reside in the same plane (they have the same normal and their points reside on the same
// plane in 3D); 
class	CMathObjectPlane : public CMathObjectRect
{
private:
	MathObjectListPtr	pDivElements;	// additional math objects which are subtracted from the plane to enable holes, round edges etc.
public:
	CMathObjectPlane();
	~CMathObjectPlane();
	void				AddDivElement(CMathObject *pobject);
	CMathObject*		GetNextElement(CMathObject* current); // returns next element in the additional object list, after the 'current' element
	void				ClearDivElements(bool bClearEl); // clear the list of elements
	float				PlaneIntersection(CMathObjectPlane *plane, D3DXVECTOR3 *vpIntersection, bool *bEndPoint, D3DXVECTOR3 *vNormal); // determine intersection of the plane with another plane
};
