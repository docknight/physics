/* ******************************************************************************** */
/* MathObjects.h                                                                    */
/*                                                                                  */
/* Mathematicaly represented game entities                                          */
/*                                                                                  */
/* ******************************************************************************** */
#include "stdafx.h"

#include "MathObjects.h"
#include <assert.h>

//-----------------------------------------------------------------------------
// Name: CMathObject()
// Desc: class constructor; initialize data
//-----------------------------------------------------------------------------
CMathObject::CMathObject()
{
	vPosition.x = 0;
	vPosition.y = 0;
	vPosition.z = 0;
	vAngMomentum.x = 0;
	vAngMomentum.y = 0;
	vAngMomentum.z = 0;
	D3DXQuaternionIdentity( &qRotation );
}

//-----------------------------------------------------------------------------
// Name: GetPosition()
// Desc: returns the current position vector
//-----------------------------------------------------------------------------
D3DXVECTOR3 CMathObject::GetPosition()
{
	return vPosition;
}

//-----------------------------------------------------------------------------
// Name: GetVelocity()
// Desc: returns the current velocity vector
//-----------------------------------------------------------------------------
D3DXVECTOR3 CMathObject::GetVelocity()
{
	return vVelocity;
}

//-----------------------------------------------------------------------------
// Name: GetAngVelocity()
// Desc: returns the current angular velocity vector.
//-----------------------------------------------------------------------------
D3DXVECTOR3 CMathObject::GetAngVelocity()
{
	return vAngVel;
}

//-----------------------------------------------------------------------------
// Name: GetAngMomentum()
// Desc: returns the current angular momentum vector.
//-----------------------------------------------------------------------------
D3DXVECTOR3 CMathObject::GetAngMomentum()
{
	return vAngMomentum;
}

//-----------------------------------------------------------------------------
// Name: SetType()
// Desc: Set the object type.
//-----------------------------------------------------------------------------
void CMathObject::SetType(int type)
{
	iType = type;
}

//-----------------------------------------------------------------------------
// Name: SetPosition()
// Desc: set new object position vector
//-----------------------------------------------------------------------------
void CMathObject::SetPosition(D3DXVECTOR3 *pvPosition)
{
	vPosition = *pvPosition;
}


//-----------------------------------------------------------------------------
// Name: SetPosition()
// Desc: set new object position vector.
//-----------------------------------------------------------------------------
void CMathObject::SetPosition(float x, float y, float z)
{
	vPosition.x = x;
	vPosition.y = y;
	vPosition.z = z;
}

//-----------------------------------------------------------------------------
// Name: SetRotation()
// Desc: set new object rotation quaternion.
//-----------------------------------------------------------------------------
void CMathObject::SetRotation(D3DXQUATERNION *pqRotation)
{
	qRotation = *pqRotation;
}


//-----------------------------------------------------------------------------
// Name: SetVelocity()
// Desc: set new object linear velocity vector.
//-----------------------------------------------------------------------------
void CMathObject::SetVelocity(D3DXVECTOR3 *pvVelocity)
{
	vVelocity = *pvVelocity;
}

//-----------------------------------------------------------------------------
// Name: SetAngVelocity()
// Desc: set new object angular velocity vector.
//-----------------------------------------------------------------------------
void CMathObject::SetAngVelocity(D3DXVECTOR3 *pvAngVel)
{
	vAngVel = *pvAngVel;
}

//-----------------------------------------------------------------------------
// Name: SetAngMomentum()
// Desc: set new object angular momentum vector.
//-----------------------------------------------------------------------------
void CMathObject::SetAngMomentum(D3DXVECTOR3 *pvAngMomentum)
{
	vAngMomentum = *pvAngMomentum;
}

//-----------------------------------------------------------------------------
// Name: SetMass()
// Desc: Set new object mass.
//-----------------------------------------------------------------------------
void CMathObject::SetMass(float m)
{
	fMass = m;
}

//-----------------------------------------------------------------------------
// Name: SetFixed()
// Desc: Set whether the object is fixed in the world.
//-----------------------------------------------------------------------------
void CMathObject::SetFixed(bool fixed)
{
	bFixed = fixed;
}

//-----------------------------------------------------------------------------
// Name: SetForceSum(pvForce)
// Desc: Set sum of the forces acting on the object.
//-----------------------------------------------------------------------------
void CMathObject::SetForceSum(D3DXVECTOR3 *pvForce)
{
	vForceSum = *pvForce;
}

//-----------------------------------------------------------------------------
// Name: SetMomentSum(pvMoment)
// Desc: Set sum of the moments acting on the object.
//-----------------------------------------------------------------------------
void CMathObject::SetMomentSum(D3DXVECTOR3 *pvMoment)
{
	vMomentSum = *pvMoment;
}

//-----------------------------------------------------------------------------
// Name: SetStaticFriction(friction)
// Desc: Set static friction coefficient for the object.
//-----------------------------------------------------------------------------
void CMathObject::SetStaticFriction(float friction)
{
	fFrictionStatic = friction;
}

//-----------------------------------------------------------------------------
// Name: SetKineticFriction(friction)
// Desc: Set kinetic (sliding) friction coefficient for the object.
//-----------------------------------------------------------------------------
void CMathObject::SetKineticFriction(float friction)
{
	fFrictionKinetic = friction;
}

//-----------------------------------------------------------------------------
// Name: SetRestitution(epsilon)
// Desc: Set collision restitution coefficient for the object.
//-----------------------------------------------------------------------------
void CMathObject::SetRestitution(float epsilon)
{
	fRestitution = epsilon;
}

//-----------------------------------------------------------------------------
// Name: GetType()
// Desc: Returns type of the object.
//-----------------------------------------------------------------------------
int CMathObject::GetType()
{
	return iType;
}

//-----------------------------------------------------------------------------
// Name: IsFixed()
// Desc: Returns true if the object is fixed in the world and not influenced by the forces.
//-----------------------------------------------------------------------------
bool CMathObject::IsFixed()
{
	return bFixed;
}

//-----------------------------------------------------------------------------
// Name: GetMass()
// Desc: Returns mass of the object.
//-----------------------------------------------------------------------------
float CMathObject::GetMass()
{
	if (bFixed) 
		return INFINITY;
	else
		return fMass;
}

//-----------------------------------------------------------------------------
// Name: GetInertia()
// Desc: Inertia tensor in local (body) or world coordinates;
//-----------------------------------------------------------------------------
D3DXMATRIX CMathObject::GetInertia(bool bWorld)
{
	//bWorld = false; // TESTING
	if (bFixed) 
		return D3DXMATRIX(INFINITY, 0, 0, 0,
						  0, INFINITY, 0, 0,
						  0, 0, INFINITY, 0,
						  0,0,0,0);
	else
	{
		if (bWorld)
		{
			if (iType != OBJTYPE_SPHERE)
			{
				D3DXMATRIX mInertiaWorld;
				D3DXMATRIX rotMatrix;
				D3DXMatrixRotationQuaternion(&rotMatrix, &qRotation);
				D3DXMATRIX rotMatrixT;
				D3DXMatrixTranspose(&rotMatrixT, &rotMatrix);
				D3DXMATRIX m;
				D3DXMatrixMultiply(&m, &rotMatrix, &mInertia);
				D3DXMatrixMultiply(&mInertiaWorld, &m, &rotMatrixT);
				return mInertiaWorld;
			}
		}
		return mInertia;
	}
}

//-----------------------------------------------------------------------------
// Name: SetInertia()
// Desc: Inertia tensor in local (body) coordinates; must be changed to world coords. during integration process for complex objects (see GDMPHYS4.pdf, Eq.20).
//-----------------------------------------------------------------------------
void CMathObject::SetInertia(D3DXMATRIX *pmInertia)
{
	mInertia = *pmInertia;
}

//-----------------------------------------------------------------------------
// Name: GetInertiaInv()
// Desc: Inverse of the inertia tensor in body or world coordinates.
//-----------------------------------------------------------------------------
D3DXMATRIX CMathObject::GetInertiaInv(bool bWorld)
{
	//bWorld = false; // TESTING
	if (bFixed) 
		return D3DXMATRIX(0, 0, 0, 0,
						  0, 0, 0, 0,
						  0, 0, 0, 0,
						  0, 0, 0, 0);
	else
	{
		if (bWorld)
		{
			if (iType != OBJTYPE_SPHERE)
			{
				D3DXMATRIX mInertiaInvWorld;
				D3DXMATRIX rotMatrix;
				D3DXMatrixRotationQuaternion(&rotMatrix, &qRotation);
				D3DXMATRIX rotMatrixT;
				D3DXMatrixTranspose(&rotMatrixT, &rotMatrix);
				D3DXMATRIX m;
				D3DXMatrixMultiply(&m, &rotMatrix, &mInertiaInv);
				D3DXMatrixMultiply(&mInertiaInvWorld, &m, &rotMatrixT);
				return mInertiaInvWorld;
			}
		}
		return mInertiaInv;
	}
}

//-----------------------------------------------------------------------------
// Name: SetInertiaInv()
// Desc: Inverse of the inertia tensor in body coordinates.
//-----------------------------------------------------------------------------
void CMathObject::SetInertiaInv(D3DXMATRIX *pmInertiaInv)
{
	mInertiaInv = *pmInertiaInv;
}

//-----------------------------------------------------------------------------
// Name: GetRotation()
// Desc: Returns object's rotational quaternion.
//-----------------------------------------------------------------------------
D3DXQUATERNION	CMathObject::GetRotation()
{
	return qRotation;
}

//-----------------------------------------------------------------------------
// Name: SyncAngMomentum()
// Desc: Synchronize angular momentum value with angular velocity.
//-----------------------------------------------------------------------------
void CMathObject::SyncAngMomentum()
{
	// L = I * w
	// get inertia tensor inverse in world coordinates;
	// TODO: for non-homogene objects, we should not just use the
	// body-space tensor, but we should similarity-transform the tensor
	// using orientation matrix (or quaternion)
	D3DXMATRIX mIntertiaWorld = GetInertia(true);
	vAngMomentum.x = mIntertiaWorld._11 * vAngVel.x + mIntertiaWorld._12 * vAngVel.y +  mIntertiaWorld._13 * vAngVel.z;
	vAngMomentum.y = mIntertiaWorld._21 * vAngVel.x + mIntertiaWorld._22 * vAngVel.y +  mIntertiaWorld._23 * vAngVel.z;
	vAngMomentum.z = mIntertiaWorld._31 * vAngVel.x + mIntertiaWorld._32 * vAngVel.y +  mIntertiaWorld._33 * vAngVel.z;
}

//-----------------------------------------------------------------------------
// Name: SyncAngVelocity()
// Desc: Synchronize angular velocity value with angular momentum.
//-----------------------------------------------------------------------------
void CMathObject::SyncAngVelocity()
{
	// w = I^(-1) * L
	// get inertia tensor inverse in world coordinates;
	// TODO: for non-homogene objects, we should not just use the
	// body-space tensor, but we should similarity-transform the tensor
	// using orientation matrix (or quaternion)
	D3DXMATRIX mIntertiaWorldInv = GetInertiaInv(true);
	if (iType != OBJTYPE_SPHERE)
	{
		D3DXMATRIX rotMatrix;
		D3DXMatrixRotationQuaternion(&rotMatrix, &qRotation);
		D3DXMATRIX rotMatrixT;
		D3DXMatrixTranspose(&rotMatrixT, &rotMatrix);
		D3DXMATRIX m;
		D3DXMatrixMultiply(&m, &rotMatrix, &mInertiaInv);
		D3DXMatrixMultiply(&mIntertiaWorldInv, &m, &rotMatrixT);
	}
	vAngVel.x = mIntertiaWorldInv._11 * vAngMomentum.x + mIntertiaWorldInv._12 * vAngMomentum.y +  mIntertiaWorldInv._13 * vAngMomentum.z;
	vAngVel.y = mIntertiaWorldInv._21 * vAngMomentum.x + mIntertiaWorldInv._22 * vAngMomentum.y +  mIntertiaWorldInv._23 * vAngMomentum.z;
	vAngVel.z = mIntertiaWorldInv._31 * vAngMomentum.x + mIntertiaWorldInv._32 * vAngMomentum.y +  mIntertiaWorldInv._33 * vAngMomentum.z;
}

//-----------------------------------------------------------------------------
// Name: GetForceSum()
// Desc: Returns total acting forces on the object's center of mass.
//-----------------------------------------------------------------------------
D3DXVECTOR3 CMathObject::GetForceSum()
{
	return vForceSum;
}

//-----------------------------------------------------------------------------
// Name: GetMomentSum()
// Desc: Returns total moments on the object.
//-----------------------------------------------------------------------------
D3DXVECTOR3 CMathObject::GetMomentSum()
{
	return vMomentSum;
}

//-----------------------------------------------------------------------------
// Name: GetStaticFriction()
// Desc: Returns static friction coefficient for the object.
//-----------------------------------------------------------------------------
float CMathObject::GetStaticFriction()
{
	return fFrictionStatic;
}

//-----------------------------------------------------------------------------
// Name: GetRestitution()
// Desc: Returns collision restitution coefficient for the object.
//-----------------------------------------------------------------------------
float CMathObject::GetRestitution()
{
	return fRestitution;
}

//-----------------------------------------------------------------------------
// Name: GetKineticFriction()
// Desc: Returns kinetic (sliding) friction coefficient for the object.
//-----------------------------------------------------------------------------
float CMathObject::GetKineticFriction()
{
	return fFrictionKinetic;
}

//-----------------------------------------------------------------------------
// Name: AddForce(pvForce)
// Desc: Adds a new force to the force sum.
//-----------------------------------------------------------------------------
void CMathObject::AddForce(D3DXVECTOR3* pvForce)
{
	vForceSum += *pvForce;
}

//-----------------------------------------------------------------------------
// Name: AddMoment(pvMoment)
// Desc: Adds a new force to the force sum.
//-----------------------------------------------------------------------------
void CMathObject::AddMoment(D3DXVECTOR3* pvMoment)
{
	vMomentSum += *pvMoment;
}

//-----------------------------------------------------------------------------
// Name: PlaneIntersection()
// Desc: determine intersection of the object with a plane.
//-----------------------------------------------------------------------------
float CMathObject::PlaneIntersection(CMathObjectPlane *plane, D3DXVECTOR3 *vpIntersection, bool *bEndPoint, D3DXVECTOR3 *vNormal)
{
	*vpIntersection = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
	return 0;
}

//-----------------------------------------------------------------------------
// Name: LineIntersection()
// Desc: determine intersection of the object with a line.
//-----------------------------------------------------------------------------
float CMathObject::LineIntersection(CMathObjectLine *line, D3DXVECTOR3 *vpIntersection, bool *bEndPoint, D3DXVECTOR3 *vNormal)
{
	*vpIntersection = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
	return 0;
}

//-----------------------------------------------------------------------------
// Name: SphereIntersection()
// Desc: determine intersection of the object with a sphere.
//-----------------------------------------------------------------------------
float CMathObject::SphereIntersection(CMathObjectSphere *sphere, D3DXVECTOR3 *vpIntersection)
{
	*vpIntersection = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
	return 0;
}


/* -------------- Class CMathObjectSphere -------------- */


//-----------------------------------------------------------------------------
// Name: SetRadius(r)
// Desc: Set sphere radius.
//-----------------------------------------------------------------------------
void CMathObjectSphere::SetRadius(float r)
{
	fRadius = r;
}

//-----------------------------------------------------------------------------
// Name: SetRollingFriction(friction)
// Desc: Set sphere rolling kinetic friction.
//-----------------------------------------------------------------------------
void CMathObjectSphere::SetRollingFriction(float friction)
{
	fFrictionRolling = friction;
}

//-----------------------------------------------------------------------------
// Name: GetRadius()
// Desc: Returns sphere radius.
//-----------------------------------------------------------------------------
float CMathObjectSphere::GetRadius()
{
	return fRadius;
}

//-----------------------------------------------------------------------------
// Name: GetRollingFriction()
// Desc: Returns sphere rolling kinetic friction.
//-----------------------------------------------------------------------------
float CMathObjectSphere::GetRollingFriction()
{
	return fFrictionRolling;
}

//-----------------------------------------------------------------------------
// Name: PlaneIntersection()
// Desc: Determine intersection of the sphere with a plane.
//       Returns the amount by which the objects penetrate.
// Par.: plane... plane to which the intersection is measured;
//       vpIntersection... intersection point; set to INFINITY if there is none;
//		 bEndPoint... TRUE if there is an intersection with an edge
//       vNormal... collision normal, applicable if bEndPoint is TRUE (otherwise,
//                  collision normal is always the plane normal)
//-----------------------------------------------------------------------------
float CMathObjectSphere::PlaneIntersection(CMathObjectPlane *plane, D3DXVECTOR3 *vpIntersection, bool *bEndPoint, D3DXVECTOR3 *vNormal)
{
	if (iType != OBJTYPE_SPHERE) return 0;
	float fProximity = INFINITY; // distance from the closest point if there is no collision
	float fIntersection; // amount by which the objects collide
	*vpIntersection = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
	*bEndPoint = false;
	// first, find the point on the sphere that is closest to the bounding plane
	// if the ray from the sphere center and perpendicular to the plane intersects the
	// plane, then a point on this ray is closest to the sphere
	// first, get two perpendicular vectors on the plane
	// TODO: verify this: the only constraint is: n * n' = 0, which is a underdetermined system (infinite solutions)
	*vNormal = plane->GetNormal(true);

	D3DXVECTOR3 v1 = plane->GetLengthVector(true);
	D3DXVECTOR3 v2 = plane->GetWidthVector(true);
	// v1 and v2 now represent two perpendicular plane axes which are aligned with
	// original bounded plane borders
	// now check from plane center in both direction to see if a ray from the sphere center goes
	// through the plane; otherwise, check if there is an off-center point on the sphere that
	// might get through (center is less than radius away from borders in one of the axis
	// directions); finally, when a candidate point is found, check its distance from the
	// plane in the plane normal direction
	D3DXVECTOR3 vPlaneCenter = plane->GetPosition();
	D3DXVECTOR3 vIntersection; // possible intersection point
	D3DXVECTOR3 vDistance = vPosition-vPlaneCenter;
	// if the plane had been infinite, intersection point would simply be the projection on the plane
	vIntersection = vPosition - D3DXVec3Dot(&vDistance, vNormal) * *vNormal;
	// now, since the plane is bounded, intersection point can't exceed borders
	D3DXVECTOR3 vDistance2 = vIntersection-vPlaneCenter;
	int iEdge = 0;
	if (fabs(D3DXVec3Dot(&vDistance2, &v1)) > plane->GetLength()/2)
	{
		D3DXVECTOR3 v1r;
		if (D3DXVec3Dot(&vDistance2, &v1) < 0)
			v1r = -v1;
		else
			v1r = v1;
		vIntersection = vIntersection - v1r * ((float)fabs(D3DXVec3Dot(&vDistance2, &v1))-plane->GetLength()/2);
		*bEndPoint = true;
		// OBSOLETE: if border is hit, normal is along the plane and pointing towards the sphere
		//*vNormal = v1r;
		// normal is pointing towards the sphere center
		*vNormal = vPosition - vIntersection;
		iEdge++;
	}
	if (fabs(D3DXVec3Dot(&vDistance2, &v2)) > plane->GetWidth()/2)
	{
		D3DXVECTOR3 v2r;
		if (D3DXVec3Dot(&vDistance2, &v2) < 0)
			v2r = -v2;
		else
			v2r = v2;
		vIntersection = vIntersection - v2r * ((float)fabs(D3DXVec3Dot(&vDistance2, &v2))-plane->GetWidth()/2);
		*bEndPoint = true;
		// OBSOLETE: if border is hit, normal is along the plane and pointing towards the sphere
		//*vNormal = v2r;
		*vNormal = vPosition - vIntersection;
		iEdge++;
	}
	// OBSOLETE: if a plane corner was hit, both conditions above will be true and normal will point
	// towards the sphere center
	//if (iEdge == 2)
	//{
	//	vDistance = vIntersection-vPosition;
	//	D3DXVec3Cross(vNormal, &vDistance, &(plane->GetNormal(true)));
	//	D3DXVec3Cross(vNormal, vNormal, &(plane->GetNormal(true)));
	//}

	// finally, check if the candidate intersection point is actually within reach of the sphere
	vDistance = vPosition-vIntersection;
	// amount by which the sphere penetrates the plane
	fIntersection = D3DXVec3Length(&vDistance) - fRadius;
	if (fIntersection > INTERSECTION_THRESHOLD)
		return fIntersection; // sphere does not penetrate - return immediately

	// ok, sphere penetrates the plane at the intersection point vIntersection;
	// now check if the plane is solid at that point; additional element of the plane define
	// discontinueties, which cause additional, inner borders; therefore, we might have to
	// search for different intersection point, or conclude, there isn't any, after all;
	bool bSolidPoint = true;
	CMathObject* pInnerObj = NULL;
	pInnerObj = plane->GetNextElement(pInnerObj);
	while (pInnerObj != NULL)
	{
		if (pInnerObj->GetType() == OBJTYPE_CIRCLE)  // currently, only circles are supported
		{
			vDistance = pInnerObj->GetPosition()-vIntersection;
			if (D3DXVec3Length(&vDistance) < ((CMathObjectCircle*)pInnerObj)->GetRadius() )
			{
				// intersection point is within a circle
				bSolidPoint = false;
				break;
			}
		}
		pInnerObj = plane->GetNextElement(pInnerObj);
	}
	if (!bSolidPoint)
	{
		// plane is indeed not solid at the intersection point; find if there is another
		// intersection point with any of the circles
		pInnerObj = NULL;
		pInnerObj = plane->GetNextElement(pInnerObj);
		while (pInnerObj != NULL)
		{
			if (pInnerObj->GetType() == OBJTYPE_CIRCLE)  // currently, only circles are supported
			{
				// find the closest point on the circle to the sphere center
				// 1.,2.) define two planes, first is the one of the circle;
				//        second has a normal in the direction of a vector from sphere center
				//        to circle center; note that circle normal is equal to the plane normal
				vDistance = pInnerObj->GetPosition() - vPosition;
				D3DXVECTOR3 n1;
				D3DXVec3Normalize(&n1, &vDistance);
				D3DXVECTOR3 n2 = ((CMathObjectCircle*)pInnerObj)->GetNormal(true);
				// 3.) calculate intersection of the two planes (which is a line)
				D3DXVECTOR3 a;
				// intersection is defined as a cross product between the two plane normals
				// note, that the result is a line direction, which is enough for our purposes
				D3DXVec3Cross(&a, &n1, &n2);
				// 4.) define a plane using normal [a] and a point which is either the circle
				//     center or sphere center (the both lie on a same plane with normal [a])
				// 5.) this plane intersects circle at two point: one is at a minimum distance and
				//     the second one is at a maximum distance
				//     it comes down to solving an equation of type A*sin(alpha) + B*cos(alpha) = 0
				//     which again simplifies into sin(alpha+gama) = 0, where gama is a certain constant
				//     expression on A and B
				float r = ((CMathObjectCircle*)pInnerObj)->GetRadius();
				float A = a.x*r*v2.x + a.y*r*v2.y + a.z*r*v2.z;
				float B = a.x*r*v1.x + a.y*r*v1.y + a.z*r*v1.z;
				float gama;
				if (A > 0)
					gama = (float)atan(B/A);
				else
					gama = (float)3.14 + atan(B/A); // TODO: declare pi
				// finally (from sin(alpha+gama)=0), declare two points on circle using alpha parametrization with
				// values of alpha = -gama and alpha = pi-gama, and choose the closer one
				D3DXVECTOR3 p1,p2;
				p1 = pInnerObj->GetPosition();
				p1.x += r*cos(-gama)*v1.x + r*sin(-gama)*v2.x;
				p1.y += r*cos(-gama)*v1.y + r*sin(-gama)*v2.y;
				p1.z += r*cos(-gama)*v1.z + r*sin(-gama)*v2.z;
				p2 = pInnerObj->GetPosition();
				p2.x += r*cos((float)3.14-gama)*v1.x + r*sin((float)3.14-gama)*v2.x;
				p2.y += r*cos((float)3.14-gama)*v1.y + r*sin((float)3.14-gama)*v2.y;
				p2.z += r*cos((float)3.14-gama)*v1.z + r*sin((float)3.14-gama)*v2.z;
				// now choose the closer one
				vDistance = p1 - vPosition;
				vDistance2 = p2 - vPosition;
				if (D3DXVec3Length(&vDistance) > D3DXVec3Length(&vDistance2))
					vIntersection = p2;
				else
					vIntersection = p1;
				// we got a new candidate intersection point; however, before checking
				// if it is really close enough to actually apply for an intersection,
				// we must again check if the point is within the plane (as the circle
				// center might be positioned somewhere close to the border
				vDistance = vIntersection - plane->GetPosition();
				if (fabs(D3DXVec3Dot(&vDistance, &v1)) > plane->GetLength()/2 ||
					fabs(D3DXVec3Dot(&vDistance, &v2)) > plane->GetWidth()/2 )
				{
					// candidate intersection point is outside the plane borders; find
					// all points on the plane borders that intersect with the circle;
					// the one closest to the old intersection candidate is the new
					// intersection point candidate
					// we setup a system of equasions for each intersection (there are
					// 4 plane borders and therefore 4 equation systems; we solve each one
					// separately and then combine the results and filter out the new
					// candidate point;
					// line is defined as [p](t) = [L0] + t*[N], where L0 is a point on the
					// line and N is a line direction vector
					// circle is defined as [p] = [p0] + R*cos(alpha)*[U] + R*sin(alpha)*[V],
					// where U and V are perpendicular vectors on the circle plane (and aligned
					// with plane borders);
					// after some simplification, it comes down to solving the equation
					// [t] = C - R*sin(alpha),
					// where alpha is the solution to the equation alpha = +/-acos(D/R) and
					// C = ([p0]-[L0])*[V], D = ([p0]-[L0])*[U]
					D3DXVECTOR3 L0[4] = { 
										plane->GetPosition() - (plane->GetLength()/2)*v1 - (plane->GetWidth()/2)*v2, // point on the first line : bottom left plane corner
										plane->GetPosition() + (plane->GetLength()/2)*v1 - (plane->GetWidth()/2)*v2, // point on the second line : bottom right plane corner
										plane->GetPosition() - (plane->GetLength()/2)*v1 - (plane->GetWidth()/2)*v2, // point on the third line : bottom left plane corner
										plane->GetPosition() - (plane->GetLength()/2)*v1 + (plane->GetWidth()/2)*v2, // point on the fourth line : top left plane corner
										};
					D3DXVECTOR3 N[4] = { v2,v2,v1,v1};
					D3DXVECTOR3 T[8];
					float C,D;
					float alpha;
					float t1,t2;

					for (int i=0; i<4; i++)
					{
						T[i*2] = D3DXVECTOR3(INFINITY,INFINITY,INFINITY);
						T[i*2+1] = D3DXVECTOR3(INFINITY,INFINITY,INFINITY);
						vDistance = pInnerObj->GetPosition() - L0[i];
						C = D3DXVec3Dot(&vDistance, &v2);
						D = D3DXVec3Dot(&vDistance, &v1);
						// equation simplification differs slightly for lines in direction
						// of v1(=U) from lines in direction of v2(=V); specifically, sinuses and
						// cosinuses are reversed
						if (N[i] == v2)
						{
							if (fabs(D/r) >= 1)
								continue; // circle center is more than radius away from the line -> no intersection
							alpha = acos(-D/r); // note: sign probably isn't necessary?
							t1 = C - r*sin(alpha);
							t2 = C + r*sin(alpha);
						}
						else // N[i] == v1
						{
							if (fabs(C/r) >= 1)
								continue; // circle center is more than radius away from the line -> no intersection
							alpha = asin(-C/r); // note: sign probably isn't necessary?
							t1 = D - r*cos(alpha);
							t2 = D + r*cos(alpha);
						}
						T[i*2] = L0[i] + t1*N[i]; // actual point on the line
						// since we were checking the intersection with an infinite line,
						// we must now verify that the intersection is meaningfull for our
						// purposes
						vDistance = plane->GetPosition() - T[i*2];
						if ( fabs(D3DXVec3Dot(&vDistance, &v1)) > plane->GetLength()/2 ||
							 fabs(D3DXVec3Dot(&vDistance, &v2)) > plane->GetWidth()/2 )
							 T[i*2] = D3DXVECTOR3(INFINITY,INFINITY,INFINITY); // invalid intersection
						T[i*2+1] = L0[i] + t2*N[i]; // actual point on the line
						vDistance = plane->GetPosition() - T[i*2+1];
						if ( fabs(D3DXVec3Dot(&vDistance, &v1)) > plane->GetLength()/2 ||
							 fabs(D3DXVec3Dot(&vDistance, &v2)) > plane->GetWidth()/2 )
							 T[i*2+1] = D3DXVECTOR3(INFINITY,INFINITY,INFINITY); // invalid intersection
					}

					// almost done, now; choose the closest point from the sphere center
					float fMinDistance = INFINITY;
					vIntersection = T[0];
					for (int i=0; i<7; i++)
					{
						if (T[i].x == INFINITY) 
							continue;
						vDistance = T[i] - vPosition;
						if (D3DXVec3Length(&vDistance) < fMinDistance)
						{
							fMinDistance = D3DXVec3Length(&vDistance);
							vIntersection = T[i];
						}
					}
					if (vIntersection.x != INFINITY)
					{
						// we have a candidate intersection; now check if it is close enough
						if (fMinDistance - fRadius < INTERSECTION_THRESHOLD)
						{
							// intersection confirmed!
							*vpIntersection = vIntersection;
							*bEndPoint = true;
							// OBSOLETE: normal is tangent on the circle in the intersection point
							//vDistance = vIntersection - pInnerObj->GetPosition();
							//D3DXVec3Cross(vNormal, &vDistance, &(plane->GetNormal(true)));
							//// also, normal should point towards the sphere
							//if (D3DXVec3Dot(&vDistance, vNormal) < 0)
							//	*vNormal = -(*vNormal);
							*vNormal = vPosition - vIntersection; // normal is pointing towards the sphere center
							D3DXVec3Normalize(vNormal, vNormal);
							return (fMinDistance - fRadius);
						}
						else
						if (fMinDistance - fRadius < fProximity) // maintain best (closest) point distance
							fProximity = fMinDistance - fRadius;
					}
				}
				else // initial candidate intersection point is within borders; now check if it is close enough
				{
					if (vIntersection.x != INFINITY)
					{
						vDistance = vIntersection - vPosition;
						if (D3DXVec3Length(&vDistance) - fRadius < INTERSECTION_THRESHOLD)
						{
							*vpIntersection = vIntersection;
							*bEndPoint = true;
							// OBSOLETE: normal is pointing from circle center towards the sphere
							//*vNormal = vIntersection - pInnerObj->GetPosition();
							*vNormal = vPosition - vIntersection; // normal is pointing towards sphere center
							D3DXVec3Normalize(vNormal, vNormal);
							return (D3DXVec3Length(&vDistance) - fRadius);
						}
						else
						if (D3DXVec3Length(&vDistance) - fRadius < fProximity) // maintain best (closest) point distance
							fProximity = D3DXVec3Length(&vDistance) - fRadius;
					}
				}

			}
			pInnerObj = plane->GetNextElement(pInnerObj);
		}
		// if we came here, initial point was refused and no other appropriate was found; return the closest distance found
		*vpIntersection = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
		return fProximity;
	}
	
	else // geometry is solid at the intersection
	
	if (vIntersection.x != INFINITY)
	{
		*vpIntersection = vIntersection;
		vDistance = vIntersection - vPosition;
		if (*bEndPoint)
			D3DXVec3Normalize(vNormal, vNormal);
		return (D3DXVec3Length(&vDistance) - fRadius); // (intersection was already confirmed, just return the penetration amount
	}

	*vpIntersection = vIntersection;

	return 0; // TODO: what's my purpose?
}


//-----------------------------------------------------------------------------
// Name: LineIntersection()
// Desc: determine intersection of the object with a line.
// Par.: bEndPoint - true if the sphere intersects the line at one of the endpoints;
//       vNormal - if bEndPoint is false, vNormal defines the perpendicular distance 
//                 from the line to the sphere center
//-----------------------------------------------------------------------------
float CMathObjectSphere::LineIntersection(CMathObjectLine *line, D3DXVECTOR3 *vpIntersection, bool *bEndPoint, D3DXVECTOR3 *vNormal)
{
	D3DXVECTOR3 vIntersection = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
	float		fProximity = INFINITY;
	D3DXQUATERNION r,rn,rnc, q;
	// define start and end point on the line
	D3DXVECTOR3 vP1 = line->GetPosition();
	// for end point, first calculate line direction using its orientation and rotation
	D3DXVECTOR3 vDirection = line->GetDirection();
	// define line ending point
	D3DXVECTOR3 vP2 = vP1 + vDirection*line->GetMagnitude();

	// line equation, using the defined points above, becomes [P] = [P1] + u * ([P2]-[P1]), where 'u' is between [0, 1]
	// sphere is defined by (x-x3)^2 + (y-y3)^2 + (z-z3)^2 = r^2, where [P3] = (x3,y3,z3) is the sphere center point
	// after substituting the equation of the line into the sphere we get an equation in the form
	// a*u^2 + b*u + c = 0,   where a,b and c are defined as below:
	float a = pow(vP2.x-vP1.x,2) + pow(vP2.y-vP1.y,2) + pow(vP2.z-vP1.z,2);
	float b = 2 * ( (vP2.x-vP1.x)*(vP1.x-vPosition.x) + (vP2.y-vP1.y)*(vP1.y-vPosition.y) + (vP2.z-vP1.z)*(vP1.z-vPosition.z) );
	float c = pow(vPosition.x,2) + pow(vPosition.y,2) + pow(vPosition.z,2) + pow(vP1.x,2) + pow(vP1.y,2) + pow(vP1.z,2) - 2 * (vPosition.x*vP1.x + vPosition.y*vP1.y + vPosition.z*vP1.z) - pow(fRadius,2);
	float D = pow(b,2) - 4*a*c;
	if (D < 0/*-INTERSECTION_THRESHOLD*/)
	{
		// no intersection
		*vpIntersection = vIntersection;
		return INFINITY;
	}
	float u1 = INFINITY, u2 = INFINITY;
	*bEndPoint = true; // default, we assume collision with line endpoint, unless proven differently
	if (fabs(D) <= INTERSECTION_THRESHOLD/5)
	{
		// line is tangent to the sphere
		u1 = -b/(2*a);
		*bEndPoint = false;
		// calculate perpendicular distance from the line to the sphere
		D3DXVECTOR3 vTmp1, vTmp2;
		vTmp1 = vPosition - vP1;
		vTmp2 = vP2 - vP1;
		float u = D3DXVec3Dot(&vTmp1, &vTmp2) / D3DXVec3Dot(&vTmp2, &vTmp2);
		D3DXVECTOR3 vP = vP1 + u*(vP2-vP1);
		D3DXVECTOR3 vD = vPosition - vP; // vector defining the perpendicular distance from the line to the sphere center
		*vNormal = vD;
	}
	else
	{
		// two intersection points
		u1 = (-b - sqrt(D)) / (2*a);
		u2 = (-b + sqrt(D)) / (2*a);
	}
	// only those intersection points are valid that are actually on the line (since our line is not infinite)
	// therefore, points with u > 1 are beyond the line limits
	if (u1 < 0-INTERSECTION_THRESHOLD || u1 > 1+INTERSECTION_THRESHOLD)
	{
		D3DXVECTOR3 vDistance = vP1 + u1*vP2 - vPosition;
		u1 = INFINITY;
		// also, maintain closest distance, even if there is no intersection
		fProximity = min(fProximity, D3DXVec3Length(&vDistance)-fRadius);
	}
	if (u2 < 0-INTERSECTION_THRESHOLD || u2 > 1+INTERSECTION_THRESHOLD)
	{
		D3DXVECTOR3 vDistance = vP1 + u2*vP2 - vPosition;
		u2 = INFINITY;
		// also, maintain closest distance, even if there is no intersection
		fProximity = min(fProximity, D3DXVec3Length(&vDistance)-fRadius);
	}
	if (u1 != INFINITY && u2 != INFINITY)
	{
		// there are still two points that intersect the sphere; this would occur if the
		// integration step was too big or the line (or sphere) velocity too large;
		// it can also occur if the sphere closes in to the line in the line normal direction
		// in that case, the point that hits the sphere first must be returned, together with
		// a detected type of collision
		// (1) first, calculate the exact time offset when the sphere hit the line plane, if it ever did
		float t = INFINITY;
		D3DXVECTOR3 vTmp1, vTmp2;
		vTmp1 = vPosition - vP1;
		vTmp2 = vP2 - vP1;
		float u = D3DXVec3Dot(&vTmp1, &vTmp2) / D3DXVec3Dot(&vTmp2, &vTmp2);
		D3DXVECTOR3 vP = vP1 + u*(vP2-vP1);
		D3DXVECTOR3 vD = vPosition - vP; // vector defining the perpendicular distance from the line to the sphere center
		*vNormal = vD;
		// calculate relative velocity in the line direction
		D3DXVECTOR3 vLineVelocity = line->GetVelocity();
		D3DXVECTOR3 vRelVel = vVelocity - vLineVelocity;
		D3DXVec3Normalize(&vRelVel, &vRelVel);
		D3DXVec3Normalize(&vD, &vD);
		float alfa = acos(D3DXVec3Dot(&vD, &vRelVel)); // angle between distance and velocity vector
		float d; // distance for the objects to backtrace in the direction of their relative velocity
		if (cos(alfa) < 0)
		{
			d = (fRadius - D3DXVec3Length(&vD)) / cos(alfa);
			t = d / D3DXVec3Length(&vRelVel);
			*bEndPoint = false; // not a vertex collision
			u = min(u1,u2)+fabs(u2-u1)/2; // u is a middle point, not actually on the line; TODO: this is not a valid intersection point
		}
		if (cos(alfa) > 0)
		{
			d = (fRadius + D3DXVec3Length(&vD)) / cos(alfa);
			t = d / D3DXVec3Length(&vRelVel);
			*bEndPoint = false;
			u = min(u1,u2)+fabs(u2-u1)/2; // u is a middle point, not actually on the line; TODO: this is not a valid intersection point
		}
		// (2) second, calculate the exact time offset when the sphere hit the line end point, if it ever did
		float v = D3DXVec3Dot(&vRelVel, &(line->GetDirection())); // we are only interested for the velocity along the line here
		if (v > 0)
		{
			vTmp1 = vP2-vP1;
			d = D3DXVec3Length(&vTmp1)*u2;
			if (d/v < t)
			{
				*bEndPoint = true;
				// vP1 is the point that actually collided with the sphere, however, since
				// we must return the intersection point that is on the sphere, we return
				// u1 which is closest to vP1
				u = u1;
			}
		}
		if (v < 0)
		{
			vTmp1 = vP2-vP1;
			d = D3DXVec3Length(&vTmp1)*(1-u1);
			if (d/v < t)
			{
				*bEndPoint = true;
				// vP2 is the point that actually collided with the sphere, however, since
				// we must return the intersection point that is on the sphere, we return
				// u2 which is closest to vP2
				u = u2;
			}
		}

		u1 = u; // u1 now holds the chosen intersection point

		//// first, calculate relative velocity in the line direction
		//D3DXVECTOR3 vLineVelocity = line->GetVelocity();
		//D3DXVECTOR3 vRelVel = vLineVelocity - vVelocity;
		//float fRelVel = D3DXVec3Dot(&vRelVel, &vDirection);
		//// special case: if the line is stationary, then the sphere must have penetrated
		//// the line directly in between the intersection points
		//if (D3DXVec3Length(&vLineVelocity) < 0.001)
		//{
		//	u1 = min(u1,u2)+fabs(u2-u1)/2;
		//	*bEndPoint = false; // not a vertex collision
		//}
		//else
		//{
		//	// if relative velocity in the line direction is positive, then the collision first
		//	// occured in the direction of line direction and the first point to intersect the
		//	// sphere was the point furthest along the line direction vector;
		//	if (fRelVel >= 0)
		//		u1 = max(u1,u2);
		//	else
		//		u1 = min(u1,u2);
		//}

		//// u1 now holds the chosen intersection point
	}
	else
	{
		// only one intersection point
		u1 = min(u1,u2);
	}

	if (u1 < INFINITY)
	{
		if (u1 > 1.0f) u1 = 1.0f; // if intersection is just beyond the line, choose the line endpoint as the intersection point
		if (u1 < 0.0f) u1 = 0.0f;
		vIntersection = vP1 + u1 * (vP2-vP1);
		D3DXVECTOR3 vDistance = vPosition - vIntersection;
		// also, maintain closest distance
		fProximity = D3DXVec3Length(&vDistance)-fRadius;
		// normal is a vector pointing towards the sphere center
		D3DXVec3Normalize(vNormal, &vDistance);
	}

	*vpIntersection = vIntersection;

	return fProximity;
}

//-----------------------------------------------------------------------------
// Name: SphereIntersection()
// Desc: determine intersection of the sphere with a sphere.
//-----------------------------------------------------------------------------
float CMathObjectSphere::SphereIntersection(CMathObjectSphere *sphere, D3DXVECTOR3 *vpIntersection)
{
	D3DXVECTOR3 vDistance = sphere->GetPosition() - vPosition; // distance between the two spheres' centers
	float fProximity = D3DXVec3Length(&vDistance) - fRadius - sphere->GetRadius();
	if (fProximity > INTERSECTION_THRESHOLD)
		*vpIntersection = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
	else
	{
		// spheres intersect - calculate intersection point
		// if the velocity along the distance vector is negative, then the intersection
		// is the closest point on the sphere 2 (could be 1 - we must choose a point on 
		// one of the objects, we chose 2nd here), else it is the furthest point on sphere 2
		D3DXVECTOR3 vRelVel = sphere->GetVelocity() - vVelocity;
		D3DXVECTOR3 vNDistance;
		D3DXVec3Normalize(&vNDistance, &vDistance);
		if (D3DXVec3Dot(&vRelVel, &vNDistance) <= 0)
			*vpIntersection = sphere->GetPosition() - vNDistance*sphere->GetRadius();
		else
			*vpIntersection = sphere->GetPosition() + vNDistance*sphere->GetRadius();
		// TODO: this won't look pretty for object 1 in case object 2 is penetrating too
		// far (intersection point will be on the wrong side of object 1);
		// to resolve this, intersection routine should return some kind of failure if
		// the objects are penetrating too far, so that the simulation would backtrace to
		// a moment in time when the spheres were distant enough;
	}
	return fProximity;
}


/* -------------- Class CMathObjectCircle -------------- */

//-----------------------------------------------------------------------------
// Name: SetRadius()
// Desc: Set circle radius.
//-----------------------------------------------------------------------------
void CMathObjectCircle::SetRadius(float r)
{
	fRadius = r;
}

//-----------------------------------------------------------------------------
// Name: GetRadius()
// Desc: Returns circle radius.
//-----------------------------------------------------------------------------
float CMathObjectCircle::GetRadius()
{
	return fRadius;
}

//-----------------------------------------------------------------------------
// Name: SetNormal(pvNormal)
// Desc: Set normal of the circle. If the object is part of a composite plane,
// 5 DOF must be taken into account and no additional checking is performed.
//-----------------------------------------------------------------------------
void CMathObjectCircle::SetNormal(D3DXVECTOR3 *pvNormal)
{
	D3DXVec3Normalize(&vNormal, pvNormal);
}

//-----------------------------------------------------------------------------
// Name: GetNormal(bTransformed)
// Desc: Returns (rotated) normal of the circle plane by combining initial 
//       normal vector and current object rotation.
// Par.: bTransformed... whether normal vector should be rotated according to
//       object rotation quaternion before returned
//-----------------------------------------------------------------------------
D3DXVECTOR3 CMathObjectCircle::GetNormal(bool bTransformed)
{
	if (!bTransformed)
		return vNormal;
	D3DXQUATERNION rn,rnc, q;
	q.x = vNormal.x;		// quaternion of the vector to rotate
	q.y = vNormal.y;
	q.z = vNormal.z;
	q.w = 0;
	rn = qRotation;
	// TODO: I have no idea why I had to negate the quaternion in order to return
	// correct vector; must have something to do with left/right handed coordinate
	// system confusion...
	// TODO: verify: should only Z coordinate be negated?
	rn.x = -rn.x;
	rn.y = -rn.y;
	rn.z = -rn.z;
	D3DXQuaternionConjugate(&rnc, &rn);
	// quaternion rotation: q_new = rn * q * rn'
	D3DXQuaternionMultiply(&rn, &rn, &q);
	D3DXQuaternionMultiply(&q, &rn, &rnc);
	// q now holds the rotated (normalized) vNormal vector
	return D3DXVECTOR3(q.x, q.y, q.z);
}

//-----------------------------------------------------------------------------
// Name: PlaneIntersection()
// Desc: determine intersection of the object with a plane.
//-----------------------------------------------------------------------------
float CMathObjectCircle::PlaneIntersection(CMathObjectPlane *plane, D3DXVECTOR3 *vpIntersection, bool *bEndPoint, D3DXVECTOR3 *vNormal)
{
	*vpIntersection = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
	return 0;
}

//-----------------------------------------------------------------------------
// Name: LineIntersection()
// Desc: determine intersection of the object with a line.
//-----------------------------------------------------------------------------
float CMathObjectCircle::LineIntersection(CMathObjectLine *line, D3DXVECTOR3 *vpIntersection, bool *bEndPoint, D3DXVECTOR3 *vNormal)
{
	*vpIntersection = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
	return 0;
}

//-----------------------------------------------------------------------------
// Name: SphereIntersection()
// Desc: determine intersection of the object with a sphere.
//-----------------------------------------------------------------------------
float CMathObjectCircle::SphereIntersection(CMathObjectSphere *sphere, D3DXVECTOR3 *vpIntersection)
{
	*vpIntersection = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
	return 0;
}



/* -------------- Class CMathObjectRect -------------- */


//-----------------------------------------------------------------------------
// Name: SetSize(length, width)
// Desc: Set length and width of the rectangle (along the X and Z axis, respectively).
//-----------------------------------------------------------------------------
void CMathObjectRect::SetSize(float length, float width)
{
	fLength = length;
	fWidth = width;
}

//-----------------------------------------------------------------------------
// Name: GetLength()
// Desc: Get current rectangle length.
//-----------------------------------------------------------------------------
float CMathObjectRect::GetLength()
{
	return fLength;
}

//-----------------------------------------------------------------------------
// Name: GetWidth()
// Desc: Get current rectangle width.
//-----------------------------------------------------------------------------
float CMathObjectRect::GetWidth()
{
	return fWidth;
}

//-----------------------------------------------------------------------------
// Name: SetNormal(pvNormal)
// Desc: Set initial normal of the rectangle.
//-----------------------------------------------------------------------------
void CMathObjectRect::SetNormal(D3DXVECTOR3 *pvNormal)
{
	D3DXVec3Normalize(&vNormal, pvNormal);
}

//-----------------------------------------------------------------------------
// Name: GetNormal(bTransformed)
// Desc: Returns rotated normal of the rectangle plane by combining initial 
//       normal vector and current object rotation.
//-----------------------------------------------------------------------------
D3DXVECTOR3 CMathObjectRect::GetNormal(bool bTransformed)
{
	if (!bTransformed)
		return vNormal;
	D3DXQUATERNION rn,rnc, q;
	q.x = vNormal.x;		// quaternion of the vector to rotate
	q.y = vNormal.y;
	q.z = vNormal.z;
	q.w = 0;
	rn = qRotation;
	// TODO: I have no idea why I had to negate the quaternion in order to return
	// correct vector; must have something to do with left/right handed coordinate
	// system confusion...
	// TODO: verify: should only Z coordinate be negated?
	rn.x = -rn.x;
	rn.y = -rn.y;
	rn.z = -rn.z;
	D3DXQuaternionConjugate(&rnc, &rn);
	// quaternion rotation: q_new = rn * q * rn'
	D3DXQuaternionMultiply(&rn, &rn, &q);
	D3DXQuaternionMultiply(&q, &rn, &rnc);
	// q now holds the rotated (normalized) vNormal vector
	return D3DXVECTOR3(q.x, q.y, q.z);
}

//-----------------------------------------------------------------------------
// Name: SetLengthVector(pvLength)
// Desc: Set initial value for the normalized length vector (like [1,0,0]).
//-----------------------------------------------------------------------------
void CMathObjectRect::SetLengthVector(D3DXVECTOR3 *pvLength)
{
	D3DXVec3Normalize(&vLength, pvLength);
}

//-----------------------------------------------------------------------------
// Name: SetWidthVector(pvWidth)
// Desc: Set initial value for the normalized width vector (like [0,0,1]).
//-----------------------------------------------------------------------------
void CMathObjectRect::SetWidthVector(D3DXVECTOR3 *pvWidth)
{
	D3DXVec3Normalize(&vWidth, pvWidth);
}

//-----------------------------------------------------------------------------
// Name: GetLengthVector(bRotated)
// Desc: Get current normalized length vector by combining initial length vector 
//       and current object rotation.
// Par.: bRotated... whether the length vector is rotated according to rotation
//                   quaternion, before returned 
//-----------------------------------------------------------------------------
D3DXVECTOR3	CMathObjectRect::GetLengthVector(bool bRotated)
{
	if (bRotated == false)
		return vLength;
	else
	{
		D3DXQUATERNION rn,rnc, q;
		q.x = vLength.x;		// quaternion of the vector to rotate
		q.y = vLength.y;
		q.z = vLength.z;
		q.w = 0;
		rn = qRotation;
		// TODO: I have no idea why I had to negate the quaternion in order to return
		// correct vector; must have something to do with left/right handed coordinate
		// system confusion...
		// TODO: verify: should only Z coordinate be negated?
		rn.x = -rn.x;
		rn.y = -rn.y;
		rn.z = -rn.z;
		D3DXQuaternionConjugate(&rnc, &rn);
		// quaternion rotation: q_new = rn * q * rn'
		D3DXQuaternionMultiply(&rn, &rn, &q);
		D3DXQuaternionMultiply(&q, &rn, &rnc);
		// q now holds the rotated (normalized) vLength vector
		return D3DXVECTOR3(q.x, q.y, q.z);
	}
}

//-----------------------------------------------------------------------------
// Name: GetWidthVector(bRotated)
// Desc: Get current width vector by combining initial width vector and current
//       object rotation.
// Par.: bRotated... whether the width vector is rotated according to rotation
//                   quaternion, before returned 
//-----------------------------------------------------------------------------
D3DXVECTOR3	CMathObjectRect::GetWidthVector(bool bRotated)
{
	if (bRotated == false)
		return vWidth;
	else
	{
		D3DXQUATERNION rn,rnc, q;
		q.x = vWidth.x;		// quaternion of the vector to rotate
		q.y = vWidth.y;
		q.z = vWidth.z;
		q.w = 0;
		rn = qRotation;
		// TODO: I have no idea why I had to negate the quaternion in order to return
		// correct vector; must have something to do with left/right handed coordinate
		// system confusion...
		// TODO: verify: should only Z coordinate be negated?
		rn.x = -rn.x;
		rn.y = -rn.y;
		rn.z = -rn.z;
		D3DXQuaternionConjugate(&rnc, &rn);
		// quaternion rotation: q_new = rn * q * rn'
		D3DXQuaternionMultiply(&rn, &rn, &q);
		D3DXQuaternionMultiply(&q, &rn, &rnc);
		// q now holds the rotated (normalized) vWidth vector
		return D3DXVECTOR3(q.x, q.y, q.z);
	}
}

//-----------------------------------------------------------------------------
// Name: PlaneIntersection(plane)
// Desc: TODO: Determine intersection of the rectangle with a plane.
//-----------------------------------------------------------------------------
float CMathObjectRect::PlaneIntersection(CMathObjectPlane *plane, D3DXVECTOR3 *vpIntersection, bool *bEndPoint, D3DXVECTOR3 *vNormal)
{
	*vpIntersection = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
	return 0;
}

//-----------------------------------------------------------------------------
// Name: LineIntersection(line)
// Desc: TODO: Determine intersection of the rectangle with a line.
//-----------------------------------------------------------------------------
float CMathObjectRect::LineIntersection(CMathObjectLine *line, D3DXVECTOR3 *vpIntersection, bool *bEndPoint, D3DXVECTOR3 *vNormal)
{
	*vpIntersection = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
	return 0;
}

//-----------------------------------------------------------------------------
// Name: SphereIntersection()
// Desc: determine intersection of the object with a sphere.
//-----------------------------------------------------------------------------
float CMathObjectRect::SphereIntersection(CMathObjectSphere *sphere, D3DXVECTOR3 *vpIntersection)
{
	*vpIntersection = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
	return 0;
}


/* -------------- Class CMathObjectLine -------------- */


//-----------------------------------------------------------------------------
// Name: SetMagnitude()
// Desc: Set magnitude (length) of the line in the direction of qRotation.
//-----------------------------------------------------------------------------
void CMathObjectLine::SetMagnitude(float mag)
{
	fMagnitude = mag;
}

//-----------------------------------------------------------------------------
// Name: GetMagnitude()
// Desc: Returns line length.
//-----------------------------------------------------------------------------
float CMathObjectLine::GetMagnitude()
{
	return fMagnitude;
}

//-----------------------------------------------------------------------------
// Name: SetOrientation(pvOrientation)
// Desc: Set line orientation vector. No checking for normalization is performed.
//-----------------------------------------------------------------------------
void CMathObjectLine::SetOrientation(D3DXVECTOR3 *pvOrientation )
{
	vOrientation = *pvOrientation;
}

//-----------------------------------------------------------------------------
// Name: GetOrientation()
// Desc: Returns line orientation vector.
//-----------------------------------------------------------------------------
D3DXVECTOR3	CMathObjectLine::GetOrientation()
{
	return vOrientation;
}

//-----------------------------------------------------------------------------
// Name: SetTipStaticFriction(friction)
// Desc: Set static friction for the line tip (end-point).
//-----------------------------------------------------------------------------
void CMathObjectLine::SetTipStaticFriction(float friction)
{
	fFrictionTipStatic = friction;
}

//-----------------------------------------------------------------------------
// Name: SetTipKineticFriction(friction)
// Desc: Set kinetic (sliding) friction for the line tip (end-point).
//-----------------------------------------------------------------------------
void CMathObjectLine::SetTipKineticFriction(float friction)
{
	fFrictionTipKinetic = friction;
}

//-----------------------------------------------------------------------------
// Name: GetTipStaticFriction()
// Desc: Get static friction for the line tip (end-point).
//-----------------------------------------------------------------------------
float CMathObjectLine::GetTipStaticFriction()
{
	return fFrictionTipStatic;
}

//-----------------------------------------------------------------------------
// Name: GetTipKineticFriction()
// Desc: Get kinetic (sliding) friction for the line tip (end-point).
//-----------------------------------------------------------------------------
float CMathObjectLine::GetTipKineticFriction()
{
	return fFrictionTipKinetic;
}

//-----------------------------------------------------------------------------
// Name: GetDirection()
// Desc: Function calculates line normalized direction vector, using orientation 
//       and rotation vector and quaternion, respectively.
//-----------------------------------------------------------------------------
D3DXVECTOR3	CMathObjectLine::GetDirection()
{
	D3DXQUATERNION r,rn,rnc, q;
	q.x = vOrientation.x;		// quaternion of the vector to rotate
	q.y = vOrientation.y;
	q.z = vOrientation.z;
	q.w = 0;
	rn = qRotation;
	// TODO: I have no idea why I had to negate the quaternion in order to return
	// correct vector; must have something to do with left/right handed coordinate
	// system confusion...
	// TODO: verify: should only Z coordinate be negated?
	rn.x = -rn.x;
	rn.y = -rn.y;
	rn.z = -rn.z;
	D3DXQuaternionConjugate(&rnc, &rn);
	// quaternion rotation: q_new = rn * q * rn'
	D3DXQuaternionMultiply(&rn, &rn, &q);
	D3DXQuaternionMultiply(&q, &rn, &rnc);
	// q now holds the rotated (normalized) vOrientation vector, therefore representing normalized line direction
	return D3DXVECTOR3(q.x, q.y, q.z);
}

//-----------------------------------------------------------------------------
// Name: GetDirection()
// Desc: Function calculates position of the line center using its starting
//       point, length and direction vector
//-----------------------------------------------------------------------------
D3DXVECTOR3	CMathObjectLine::GetCenterPosition()
{
	D3DXVECTOR3 v = this->GetPosition();
	v = v + this->GetDirection()*this->fMagnitude/2;
	return v;
}




/* -------------- Class CMathObjectPlane -------------- */

//-----------------------------------------------------------------------------
// Name: CMathObjectPlane()
// Desc: Initialize object list.
//-----------------------------------------------------------------------------
CMathObjectPlane::CMathObjectPlane()
{
	// initialize elements-to-subtract list
	pDivElements = (MathObjectListPtr)malloc(sizeof(MathObjectList));
	pDivElements->pMathObject = NULL;
	pDivElements->next = NULL;
}

//-----------------------------------------------------------------------------
// Name: ~CMathObjectPlane()
// Desc: Free object list (not objects!) from memory.
//-----------------------------------------------------------------------------
CMathObjectPlane::~CMathObjectPlane()
{
	// clear math-object lists
	MathObjectListPtr objlist = pDivElements;
	MathObjectListPtr objlistnext;
	while (objlist != NULL)
	{
		objlistnext = objlist->next;
		// release list element
		free(objlist);
		objlist = objlistnext;
	}
}

//-----------------------------------------------------------------------------
// Name: AddDivElement()
// Desc: Add pre-initialized object to list of boolean subtraction elements
//-----------------------------------------------------------------------------
void CMathObjectPlane::AddDivElement(CMathObject * pobject)
{
	MathObjectListPtr elements = pDivElements;
	// seek to the end of the elements list
	while (elements->next != NULL)
		elements = elements->next;
	// we add a new element, unless this is the first object, in which case the element has already been created
	if (elements->pMathObject != NULL)
	{
		elements->next = (MathObjectListPtr)malloc(sizeof(MathObjectList));
		elements = elements->next;
		elements->next = NULL;
	}
	// store object's reference into the list
	elements->pMathObject = pobject;
}

//-----------------------------------------------------------------------------
// Name: GetNextElement(current)
// Desc: Returns data of the next element in the additional object list, after the 'current' element.
//       If the 'current' is set to NULL, data of the first element is retrieved.
//		 If 'current' is equal to the data of the last element, NULL is returned.
//		 If 'current' is not in the list, NULL is returned.
//-----------------------------------------------------------------------------
CMathObject* CMathObjectPlane::GetNextElement(CMathObject* current)
{
	MathObjectListPtr elements = pDivElements;
	if (current == NULL) return elements->pMathObject;
	while (elements->next != NULL)
	{
		if (elements->pMathObject == current)
		{
			return elements->next->pMathObject;
		}
		elements = elements->next;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// Name: ClearDivElements()
// Desc: Clear the list of elements.
// Par.: bClearEl... when TRUE, elements themselves are released, too.
//-----------------------------------------------------------------------------
void CMathObjectPlane::ClearDivElements(bool bClearEl)
{
	// clear math-object lists
	MathObjectListPtr objlist = pDivElements;
	MathObjectListPtr objlistnext;
	while (objlist != NULL)
	{
		objlistnext = objlist->next;
		// release math object
		if (objlist->pMathObject != NULL)
			free(objlist->pMathObject);
		// release list element
		free(objlist);
		objlist = objlistnext;
	}
	// initialize list to have at least one element
	pDivElements = (MathObjectListPtr)malloc(sizeof(MathObjectList));
	pDivElements->pMathObject = NULL;
	pDivElements->next = NULL;
}

//-----------------------------------------------------------------------------
// Name: PlaneIntersection(plane)
// Desc: Determine intersection of the plane with another plane.
// Par.:
//       vpIntersection... intersection point; set to INFINITY if there is none;
//		 bEndPoint... FALSE, if we've got face/face collision;
//       vNormal... collision normal, always set to one of the plane normal vectors;
//-----------------------------------------------------------------------------
float CMathObjectPlane::PlaneIntersection(CMathObjectPlane *plane, D3DXVECTOR3 *vpIntersection, bool *bEndPoint, D3DXVECTOR3 *vNormal)
{
	float fProx = INFINITY; // distance between the two planes
	*vpIntersection = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
	*bEndPoint = true;
	// calculate distance of each plane from origin, as used in Hesse normal form plane definition
	D3DXVECTOR3 vNormal1 = GetNormal(true);
	D3DXVECTOR3 vNormal2 = plane->GetNormal(true);
	float p1 = -D3DXVec3Dot(&vNormal1, &(GetPosition()));
	float p2 = -D3DXVec3Dot(&vNormal2, &(plane->GetPosition()));
	D3DXVECTOR3 va;
	D3DXVec3Cross(&va, &vNormal1, &vNormal2); // direction vector of the intersection line
	if (D3DXVec3Length(&va) >= 0.05f) // TESTING: reduce this?
		D3DXVec3Normalize(&va, &va);
	else
		va = D3DXVECTOR3(0.0f,0.0f,0.0f);
	// now find a point that is on both planes and therefore on the line we are looking for
	float x0,y0,z0;
	if (fabs(va.z) > 0.001f)
	{
		z0 = 0;
		if (fabs(vNormal1.x) <= 0.001f)
		{
			y0 = -p1 / vNormal1.y;
			x0 = (-p2 - vNormal2.y * y0) / vNormal2.x;
		}
		else
		{
			y0 = (-p2 + (vNormal2.x*p1/vNormal1.x)) / (-vNormal2.x*vNormal1.y/vNormal1.x + vNormal2.y);
			x0 = (-p1 - vNormal1.y*y0) / vNormal1.x;
		}
	}
	else if (fabs(va.y) > 0.001f)
	{
		y0 = 0;
		if (fabs(vNormal1.x) <= 0.001f)
		{
			z0 = -p1 / vNormal1.z;
			x0 = (-p2 - vNormal2.z * z0) / vNormal2.x;
		}
		else
		{
			z0 = (-p2 + (vNormal2.x*p1/vNormal1.x)) / (-vNormal2.x*vNormal1.z/vNormal1.x + vNormal2.z);
			x0 = (-p1 - vNormal1.z*z0) / vNormal1.x;
		}
	}
	else if (fabs(va.x) > 0.001f)
	{
		x0 = 0;
		if (fabs(vNormal1.z) <= 0.001f)
		{
			y0 = -p1 / vNormal1.y;
			z0 = (-p2 - vNormal2.y * y0) / vNormal2.z;
		}
		else
		{
			y0 = (-p2 + (vNormal2.z*p1/vNormal1.z)) / (-vNormal2.z*vNormal1.y/vNormal1.z + vNormal2.y);
			z0 = (-p1 - vNormal1.y*y0) / vNormal1.z;
		}
	}
	else
	{
		// planes do not intersect;
		fProx = fabs(D3DXVec3Dot(&(GetPosition()-plane->GetPosition()), &vNormal1));
		if (fProx <= INTERSECTION_THRESHOLD*10)
		{
			// planes are close enough though, to count as being in contact
			// only thing left to do here is to choose an intersection point; 
			// for that purpose it makes sense to choose the point that lies in the
			// center of the intersection area; so, let's find the whole intersection area
			D3DXVECTOR3 vLength = GetLengthVector(true);
			D3DXVECTOR3 vWidth = GetWidthVector(true);
			D3DXVECTOR3 vLeft; // left edge of the intersection, in the direction of vLength vector
			D3DXVECTOR3 vRight; // left edge of the intersection, in the direction of vLength vector
			if ( D3DXVec3Dot(&(GetPosition()-vLength*GetLength()/2), &vLength) > D3DXVec3Dot(&(plane->GetPosition()-vLength*plane->GetLength()/2), &vLength) )
				vLeft = GetPosition() - vLength*GetLength()/2;
			else
				vLeft = plane->GetPosition() - vLength*plane->GetLength()/2;
			if ( D3DXVec3Dot(&(GetPosition()+vLength*GetLength()/2), &vLength) < D3DXVec3Dot(&(plane->GetPosition()+vLength*plane->GetLength()/2), &vLength) )
				vRight = GetPosition() + vLength*GetLength()/2;
			else
				vRight = plane->GetPosition() + vLength*plane->GetLength()/2;
			D3DXVECTOR3 vBottom; // bottom edge of the intersection, in the direction of Width vector
			D3DXVECTOR3 vTop; // top edge of the intersection, in the direction of vWidth vector
			if ( D3DXVec3Dot(&(GetPosition()-vWidth*GetWidth()/2), &vWidth) > D3DXVec3Dot(&(plane->GetPosition()-vWidth*plane->GetWidth()/2), &vWidth) )
				vBottom = GetPosition() - vWidth*GetWidth()/2;
			else
				vBottom = plane->GetPosition() - vWidth*plane->GetWidth()/2;
			if ( D3DXVec3Dot(&(GetPosition()+vWidth*GetWidth()/2), &vWidth) < D3DXVec3Dot(&(plane->GetPosition()+vWidth*plane->GetWidth()/2), &vWidth) )
				vTop = GetPosition() + vWidth*GetWidth()/2;
			else
				vTop = plane->GetPosition() + vWidth*plane->GetWidth()/2;
			D3DXVECTOR3 vIntersectionLocal;
			vIntersectionLocal = vLeft + (vRight-vLeft)*0.5;
			vIntersectionLocal += vWidth * D3DXVec3Dot(&(vBottom-vLeft), &vWidth);
			vIntersectionLocal += (vTop-vBottom)*0.5; //vWidth * D3DXVec3Dot(&(vTop-vBottom), &vWidth) * 0.5;
			// intersection point in world coordinates
			vpIntersection->x = D3DXVec3Dot(&vIntersectionLocal, &D3DXVECTOR3(1.0f,0.0f,0.0f));
			vpIntersection->y = D3DXVec3Dot(&vIntersectionLocal, &D3DXVECTOR3(0.0f,1.0f,0.0f));
			vpIntersection->z = D3DXVec3Dot(&vIntersectionLocal, &D3DXVECTOR3(0.0f,0.0f,1.0f));
			// normal vector, pointing towards second plane
			if ( D3DXVec3Dot(&(plane->GetPosition()-GetPosition()), &vNormal1) >= 0 )
				*vNormal = vNormal1;
			else
				*vNormal = -vNormal1;
			*bEndPoint = false;
		}
		return fProx;
	}

	// we got the plane intersection equation : [y] = [x0,y0,z0] + t*[va]
	// this line must be truncated since the planes are not infinite; in order to achieve
	// this we first find the intersection of the line with all plane edges (there are 8 of those)
	D3DXVECTOR3 vIntersection[8];

	// basically we perform a similar calculation for each one of the edges
	float t1;
	float fTmp;
	D3DXVECTOR3 vX0;
	D3DXVECTOR3 vY0 = D3DXVECTOR3(x0,y0,z0);
	D3DXVECTOR3 vDirection;
	D3DXVECTOR3 vLength = GetLengthVector(true);
	D3DXVECTOR3 vWidth = GetWidthVector(true);
	D3DXVECTOR3 vTemp, vTemp2;

	// 1st edge of the 1st plane, its equation is [x] = [vX0] + t1*[vDirection]
	vDirection = vLength;
	vX0 = GetPosition() - vWidth*GetWidth()/2;
	// intersection equation, using (Hill 1994)
	D3DXVec3Cross(&vTemp, &(vY0-vX0), &va);
	D3DXVec3Cross(&vTemp2, &vDirection, &va);
	fTmp = D3DXVec3Length(&vTemp2);
	if (fTmp > 0.0001)
	{
		t1 = D3DXVec3Dot(&vTemp, &vTemp2) / (fTmp*fTmp);
		vIntersection[0] = vX0 + t1*vDirection;
	}
	else // lines are parallel to each other
		vIntersection[0] = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);

	// 2nd edge of the 1st plane, its equation is [x] = [vX0] + t1*[vDirection]
	vDirection = vLength;
	vX0 = GetPosition() + vWidth*GetWidth()/2;
	// intersection equation, using (Hill 1994)
	D3DXVec3Cross(&vTemp, &(vY0-vX0), &va);
	D3DXVec3Cross(&vTemp2, &vDirection, &va);
	fTmp = D3DXVec3Length(&vTemp2);
	if (fTmp > 0.0001)
	{
		t1 = D3DXVec3Dot(&vTemp, &vTemp2) / (fTmp*fTmp);
		vIntersection[1] = vX0 + t1*vDirection;
	}
	else // lines are parallel to each other
		vIntersection[1] = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);

	// 3rd edge of the 1st plane, its equation is [x] = [vX0] + t1*[vDirection]
	vDirection = vWidth;
	vX0 = GetPosition() - vLength*GetLength()/2;
	// intersection equation, using (Hill 1994)
	D3DXVec3Cross(&vTemp, &(vY0-vX0), &va);
	D3DXVec3Cross(&vTemp2, &vDirection, &va);
	fTmp = D3DXVec3Length(&vTemp2);
	if (fTmp > 0.0001)
	{
		t1 = D3DXVec3Dot(&vTemp, &vTemp2) / (fTmp*fTmp);
		vIntersection[2] = vX0 + t1*vDirection;
	}
	else // lines are parallel to each other
		vIntersection[2] = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);

	// 4th edge of the 1st plane, its equation is [x] = [vX0] + t1*[vDirection]
	vDirection = vWidth;
	vX0 = GetPosition() + vLength*GetLength()/2;
	// intersection equation, using (Hill 1994)
	D3DXVec3Cross(&vTemp, &(vY0-vX0), &va);
	D3DXVec3Cross(&vTemp2, &vDirection, &va);
	fTmp = D3DXVec3Length(&vTemp2);
	if (fTmp > 0.0001)
	{
		t1 = D3DXVec3Dot(&vTemp, &vTemp2) / (fTmp*fTmp);
		vIntersection[3] = vX0 + t1*vDirection;
	}
	else // lines are parallel to each other
		vIntersection[3] = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);

	vLength = plane->GetLengthVector(true);
	vWidth = plane->GetWidthVector(true);
	// 1st edge of the 2nd plane, its equation is [x] = [vX0] + t1*[vDirection]
	vDirection = vLength;
	vX0 = plane->GetPosition() - vWidth*plane->GetWidth()/2;
	// intersection equation, using (Hill 1994)
	D3DXVec3Cross(&vTemp, &(vY0-vX0), &va);
	D3DXVec3Cross(&vTemp2, &vDirection, &va);
	fTmp = D3DXVec3Length(&vTemp2);
	if (fTmp > 0.0001)
	{
		t1 = D3DXVec3Dot(&vTemp, &vTemp2) / (fTmp*fTmp);
		vIntersection[4] = vX0 + t1*vDirection;
	}
	else // lines are parallel to each other
		vIntersection[4] = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);

	// 2nd edge of the 2nd plane, its equation is [x] = [vX0] + t1*[vDirection]
	vDirection = vLength;
	vX0 = plane->GetPosition() + vWidth*plane->GetWidth()/2;
	// intersection equation, using (Hill 1994)
	D3DXVec3Cross(&vTemp, &(vY0-vX0), &va);
	D3DXVec3Cross(&vTemp2, &vDirection, &va);
	fTmp = D3DXVec3Length(&vTemp2);
	if (fTmp > 0.0001)
	{
		t1 = D3DXVec3Dot(&vTemp, &vTemp2) / (fTmp*fTmp);
		vIntersection[5] = vX0 + t1*vDirection;
	}
	else // lines are parallel to each other
		vIntersection[5] = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);

	// 3rd edge of the 2nd plane, its equation is [x] = [vX0] + t1*[vDirection]
	vDirection = vWidth;
	vX0 = plane->GetPosition() - vLength*plane->GetLength()/2;
	// intersection equation, using (Hill 1994)
	D3DXVec3Cross(&vTemp, &(vY0-vX0), &va);
	D3DXVec3Cross(&vTemp2, &vDirection, &va);
	fTmp = D3DXVec3Length(&vTemp2);
	if (fTmp > 0.0001)
	{
		t1 = D3DXVec3Dot(&vTemp, &vTemp2) / (fTmp*fTmp);
		vIntersection[6] = vX0 + t1*vDirection;
	}
	else // lines are parallel to each other
		vIntersection[6] = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);

	// 4th edge of the 2nd plane, its equation is [x] = [vX0] + t1*[vDirection]
	vDirection = vWidth;
	vX0 = plane->GetPosition() + vLength*plane->GetLength()/2;
	// intersection equation, using (Hill 1994)
	D3DXVec3Cross(&vTemp, &(vY0-vX0), &va);
	D3DXVec3Cross(&vTemp2, &vDirection, &va);
	fTmp = D3DXVec3Length(&vTemp2);
	if (fTmp > 0.0001)
	{
		t1 = D3DXVec3Dot(&vTemp, &vTemp2) / (fTmp*fTmp);
		vIntersection[7] = vX0 + t1*vDirection;
	}
	else // lines are parallel to each other
		vIntersection[7] = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);

	// now filter intersection points of both planes: only the points that are
	// actually within both plane borders are valid; this will leave us with exactly two
	// points if the planes intersect within their borders; 
	// TODO: avoid calculating same values more than once
	D3DXVECTOR3 vPoints[4];
	vLength = GetLengthVector(true);
	vWidth = GetWidthVector(true);
	bool bVertex[8]; // whether an intersection point is likely a vertex of a plane
	for (int i = 0; i < 8; i++)
		bVertex[i] = false;
	int iCount = 0;
	for (int i = 0; i < 8; i++)
	{
		bool bOut = false;
		if ( fabs(D3DXVec3Dot(&(vIntersection[i]-GetPosition()), &vWidth))-INTERSECTION_THRESHOLD*3 > GetWidth()/2 )
		{
			bOut = true;
			fProx = min(fProx, fabs(D3DXVec3Dot(&(vIntersection[i]-GetPosition()), &vWidth)) - GetWidth()/2);
		}
		if ( fabs(D3DXVec3Dot(&(vIntersection[i]-GetPosition()), &vLength))-INTERSECTION_THRESHOLD*3 > GetLength()/2 )
		{
			bOut = true;
			fProx = min(fProx, fabs(D3DXVec3Dot(&(vIntersection[i]-GetPosition()), &vLength)) - GetLength()/2);
		}
		if (bOut)
			vIntersection[i] = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
		if (!bOut && fabs(D3DXVec3Dot(&(vIntersection[i]-GetPosition()), &vWidth)) > (GetWidth()/2)*0.95 &&
			fabs(D3DXVec3Dot(&(vIntersection[i]-GetPosition()), &vLength)) > (GetLength()/2)*0.95)
			bVertex[i] = true;
	}

	// do the same with intersection points on the second plane
	vLength = plane->GetLengthVector(true);
	vWidth = plane->GetWidthVector(true);
	for (int i = 0; i < 8; i++)
	{
		bool bOut = false;
		if ( fabs(D3DXVec3Dot(&(vIntersection[i]-plane->GetPosition()), &vWidth))-INTERSECTION_THRESHOLD*3 > plane->GetWidth()/2 )
		{
			bOut = true;
			fProx = min(fProx, fabs(D3DXVec3Dot(&(vIntersection[i]-plane->GetPosition()), &vWidth)) - plane->GetWidth()/2);
		}
		if ( fabs(D3DXVec3Dot(&(vIntersection[i]-plane->GetPosition()), &vLength))-INTERSECTION_THRESHOLD*3 > plane->GetLength()/2 )
		{
			bOut = true;
			fProx = min(fProx, fabs(D3DXVec3Dot(&(vIntersection[i]-plane->GetPosition()), &vLength)) - plane->GetLength()/2);
		}
		if (bOut)
			vIntersection[i] = D3DXVECTOR3(INFINITY, INFINITY, INFINITY);
		if (!bOut && fabs(D3DXVec3Dot(&(vIntersection[i]-plane->GetPosition()), &vWidth)) > (plane->GetWidth()/2)*0.95 &&
			fabs(D3DXVec3Dot(&(vIntersection[i]-plane->GetPosition()), &vLength)) > (plane->GetLength()/2)*0.95)
			bVertex[i] = true;
	}

	int iVertexPlane[2]; // if any of the points is on a vertex, define its plane (0.. first plane, 1.. second plane)
	// extract two remaining valid points
	for (int i = 0; i < 8; i++)
	{
		if (vIntersection[i].x != INFINITY)
		{
			if (iCount < 2)
			{
				vPoints[iCount] = vIntersection[i];
				bVertex[iCount] = bVertex[i]; // assign vertex flags to accepted points
				iVertexPlane[iCount] = i < 4 ? 0 : 1; 
			}
			// it is possible to have 4 valid points, if the two planes of matching edge
			// length intersect along the edge; in that case, the same point has been added
			// twice
			iCount++;
			if (iCount > 2 && fabs(vIntersection[i].x-vPoints[0].x) <= 0.005 && fabs(vIntersection[i].y-vPoints[0].y) <= 0.005 && fabs(vIntersection[i].z-vPoints[0].z) <= 0.005 ||
				iCount > 2 && fabs(vIntersection[i].x-vPoints[1].x) <= 0.005 && fabs(vIntersection[i].y-vPoints[1].y) <= 0.005 && fabs(vIntersection[i].z-vPoints[1].z) <= 0.005)
				iCount--;
		}
	}

	// intersection line, if it exists, is bounded by two intersection points in vPoints[]
	if (iCount == 2)
	{
		// let's define intersection point in the middle of the intersection line
		*vpIntersection = vPoints[0] + (vPoints[1]-vPoints[0])*0.5;
		// to define the collision normal, we must estimate which plane hit the other; we are
		// assuming that one of the planes hit the other along the edge; we must isolate that
		// edge and thereupon define the collision normal as the normal vector of the other
		// plane (since we have a edge/face collision); therefore, we must find the edge that
		// is closest to the intersection point, relative to plane size;
		float fRelDist1 = INFINITY;
		float fRelDist2 = INFINITY;
		float fProx1 = INFINITY;
		float fProx2 = INFINITY;
		int iEdge1,iEdge2; // edge id on both planes (0 for length, 1 for width)
		fRelDist1 = fabs(D3DXVec3Dot(&(*vpIntersection-GetPosition()), &(GetWidthVector(true)))) / (GetWidth()/2);
		fProx1 = fabs(D3DXVec3Dot(&(*vpIntersection-GetPosition()), &(GetWidthVector(true)))) - GetWidth()/2;
		iEdge1 = 1;
		if ( fabs(D3DXVec3Dot(&(*vpIntersection-GetPosition()), &(GetLengthVector(true)))) / (GetLength()/2) > fRelDist1 )
		{
			fRelDist1 = fabs(D3DXVec3Dot(&(*vpIntersection-GetPosition()), &(GetLengthVector(true)))) / (GetLength()/2);
			fProx1 = fabs(D3DXVec3Dot(&(*vpIntersection-GetPosition()), &(GetLengthVector(true)))) - GetLength()/2;
			iEdge1 = 0;
		}
		fRelDist2 = fabs(D3DXVec3Dot(&(*vpIntersection-plane->GetPosition()), &(plane->GetWidthVector(true)))) / (plane->GetWidth()/2);
		fProx2 = fabs(D3DXVec3Dot(&(*vpIntersection-plane->GetPosition()), &(plane->GetWidthVector(true)))) - plane->GetWidth()/2;
		iEdge2 = 1;
		if ( fabs(D3DXVec3Dot(&(*vpIntersection-plane->GetPosition()), &(plane->GetLengthVector(true)))) / (plane->GetLength()/2) > fRelDist2 )
		{
			fRelDist2 = fabs(D3DXVec3Dot(&(*vpIntersection-plane->GetPosition()), &(plane->GetLengthVector(true)))) / (plane->GetLength()/2);
			fProx2 = fabs(D3DXVec3Dot(&(*vpIntersection-plane->GetPosition()), &(plane->GetLengthVector(true)))) - plane->GetLength()/2;
			iEdge2 = 0;
		}
		D3DXVECTOR3	vVector1;
		D3DXVECTOR3	vVector2;
		if (iEdge1 == 0) vVector1 = GetLengthVector(true);
		else
			vVector1 = GetWidthVector(true);
		if (iEdge2 == 0) vVector2 = plane->GetLengthVector(true);
		else
			vVector2 = plane->GetWidthVector(true);
		if (fabs(fRelDist1-fRelDist2) < 2*INTERSECTION_THRESHOLD && fRelDist1 > 0.92 &&
			fabs(D3DXVec3Dot(&vVector1, &vVector2)) <= 0.9)
		{
			// seems like we have an edge/edge collision of two edges that are not
			// aligned, resulting in a vertex/vertex collision
			// in that case normal points from first plane to second plane, 
			// which forces objects to fly apart after the impact
			D3DXVec3Cross(vNormal, &vNormal1, &vNormal2);
			D3DXVec3Normalize(vNormal, vNormal);
		}
		else
			*vNormal = fRelDist1 >= fRelDist2 ? plane->GetNormal(true) : GetNormal(true);
		// override normal if one of the planes is hit along its vertex
		if (bVertex[0])
			*vNormal = (iVertexPlane[0] == 0) ? plane->GetNormal(true) : GetNormal(true);
		if (bVertex[1])
			*vNormal = (iVertexPlane[1] == 0) ? plane->GetNormal(true) : GetNormal(true);

		if (D3DXVec3Dot(&(plane->GetPosition()-*vpIntersection), vNormal) < 0)
			*vNormal = -(*vNormal);
		fProx = fRelDist1 >= fRelDist2 ? fProx1 : fProx2;

	}

	return fProx;
}

