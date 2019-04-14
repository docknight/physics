/* ******************************************************************************** */
/* MathObjects.h                                                                    */
/*                                                                                  */
/* managed math objects interfaces to be used for user interface manipulation       */
/* of math objects data                                                             */
/*                                                                                  */
/* ******************************************************************************** */
using namespace System::ComponentModel::Design;

// forward declarations
__gc class UIObject;
__gc class UIForce;


// read-only 3x3 matrix class, expandable for proper display in the property grid control
[TypeConverter(__typeof(ExpandableObjectConverter))]
public __gc class CMatrixRO
{
public:
	[ReadOnly(true)]
	__property float get_A11() { return a11; }
	__property void set_A11(float value) { a11 = value; }
	[ReadOnly(true)]
	__property float get_A12() { return a12; }
	__property void set_A12(float value) { a12 = value; }
	[ReadOnly(true)]
	__property float get_A13() { return a13; }
	__property void set_A13(float value) { a13 = value; }
	[ReadOnly(true)]
	__property float get_A21() { return a21; }
	__property void set_A21(float value) { a21 = value; }
	[ReadOnly(true)]
	__property float get_A22() { return a22; }
	__property void set_A22(float value) { a22 = value; }
	[ReadOnly(true)]
	__property float get_A23() { return a23; }
	__property void set_A23(float value) { a23 = value; }
	[ReadOnly(true)]
	__property float get_A31() { return a31; }
	__property void set_A31(float value) { a31 = value; }
	[ReadOnly(true)]
	__property float get_A32() { return a32; }
	__property void set_A32(float value) { a32 = value; }
	[ReadOnly(true)]
	__property float get_A33() { return a33; }
	__property void set_A33(float value) { a33 = value; }

	virtual System::String __gc* ToString()
	{
		return (S"3x3 matrix");
	}

	// class constructor
	CMatrixRO()
	{
		Clear();
	}

	// clear all data components to zero
	void Clear()
	{
		a11 = a12 = a13 = a21 = a22 = a23 = a31 = a32 = a33 = 0;
	}

protected:
	float a11,a12,a13;
	float a21,a22,a23;
	float a31,a32,a33;
};

// 3x3 matrix class, expandable for proper display in the property grid control
[TypeConverter(__typeof(ExpandableObjectConverter))]
public __gc class CMatrix
{
public:
	__property float get_A11() { return a11; }
	__property void set_A11(float value) { a11 = value; }
	__property float get_A12() { return a12; }
	__property void set_A12(float value) { a12 = value; }
	__property float get_A13() { return a13; }
	__property void set_A13(float value) { a13 = value; }
	__property float get_A21() { return a21; }
	__property void set_A21(float value) { a21 = value; }
	__property float get_A22() { return a22; }
	__property void set_A22(float value) { a22 = value; }
	__property float get_A23() { return a23; }
	__property void set_A23(float value) { a23 = value; }
	__property float get_A31() { return a31; }
	__property void set_A31(float value) { a31 = value; }
	__property float get_A32() { return a32; }
	__property void set_A32(float value) { a32 = value; }
	__property float get_A33() { return a33; }
	__property void set_A33(float value) { a33 = value; }

	virtual System::String __gc* ToString()
	{
		return (S"3x3 matrix");
	}

	// class constructor
	CMatrix()
	{
		Clear();
	}

	// clear all data components to zero
	void Clear()
	{
		a11 = a12 = a13 = a21 = a22 = a23 = a31 = a32 = a33 = 0;
	}

protected:
	float a11,a12,a13;
	float a21,a22,a23;
	float a31,a32,a33;
};

// read-only column 3D vector class, expandable for proper display in the property grid control
[TypeConverter(__typeof(ExpandableObjectConverter))]
public __gc class CVectorRO
{
public:
	[ReadOnly(true)]
	__property float get_X() { return x; }
	__property void set_X(float value) { x = value; }
	[ReadOnly(true)]
	__property float get_Y() { return y; }
	__property void set_Y(float value) { y = value; }
	[ReadOnly(true)]
	__property float get_Z() { return z; }
	__property void set_Z(float value) { z = value; }

	virtual System::String __gc* ToString()
	{
		return (S"3x1 column vector");
	}

	// class constructor
	CVectorRO()
	{
		Clear();
	}

	// clear all data components to zero
	void Clear()
	{
		x = y = z = 0;
	}

protected:
	float x;
	float y;
	float z;
};

// column 3D vector class, expandable for proper display in the property grid control
[TypeConverter(__typeof(ExpandableObjectConverter))]
public __gc class CVector
{
public:
	__property float get_X() { return x; }
	__property void set_X(float value) { x = value; }
	__property float get_Y() { return y; }
	__property void set_Y(float value) { y = value; }
	__property float get_Z() { return z; }
	__property void set_Z(float value) { z = value; }

	// class constructor
	CVector()
	{
		Clear();
	}

	virtual System::String __gc* ToString()
	{
		return (S"3x1 column vector");
	}

	// clear all data components to zero
	void Clear()
	{
		x = y = z = 0;
	}

protected:
	float x;
	float y;
	float z;
};

// quaternion class, expandable for proper display in the property grid control
// properties are divided into those user-friendly and their quaternion-counterparts,
// which are actually used for the calculations
[TypeConverter(__typeof(ExpandableObjectConverter))]
public __gc class CQuaternion
{
public:
	[Browsable(false)]
	__property float get_W() { return w; }
	__property void set_W(float value) { w = value; }
	[Browsable(false)]
	__property float get_X() { return x; }
	__property void set_X(float value) { x = value; }
	[Browsable(false)]
	__property float get_Y() { return y; }
	__property void set_Y(float value) { y = value; }
	[Browsable(false)]
	__property float get_Z() { return z; }
	__property void set_Z(float value) { z = value; }

	// properties for user interaction
	[Description("3D rotation axis X component")]
	__property float get_RotationX() { return fRotationX; }
	__property void set_RotationX(float value) { fRotationX = value; AlignToRotation(); }
	[Description("3D rotation axis Y component")]
	__property float get_RotationY() { return fRotationY; }
	__property void set_RotationY(float value) { fRotationY = value; AlignToRotation(); }
	[Description("3D rotation axis Z component")]
	__property float get_RotationZ() { return fRotationZ; }
	__property void set_RotationZ(float value) { fRotationZ = value; AlignToRotation(); }
	[Description("rotation angle [rad] about rotation axis")]
	__property float get_Angle() { return fRotationAngle; }
	__property void set_Angle(float value) { fRotationAngle = value; AlignToRotation(); }
	
	// class constructor
	CQuaternion()
	{
		Clear();
	}

	virtual System::String __gc* ToString()
	{
		return (S"quaternion");
	}

	// clear all data components to zero
	void Clear()
	{
		//x = y = z = w = 0;
		fRotationAngle = 0;
		fRotationX = fRotationY = fRotationZ = 0;
		fRotationX = 1; // must be greater than zero to avoid normalization singularities
		AlignToRotation();
	}

protected:
	// quaternion components
	float w;
	float x;
	float y;
	float z;
	// user manageable quaternion data
	float fRotationX; // 3D rotation axis
	float fRotationY; // 3D rotation axis
	float fRotationZ; // 3D rotation axis
	float fRotationAngle; // rotation angle about rotation axis

	// align quaternion components to rotation axis and angle; also, quaternion is normalized
	void AlignToRotation()
	{
		// normalize axis vector
		D3DXVECTOR3 vNAxis = D3DXVECTOR3(fRotationX, fRotationY, fRotationZ);
		D3DXVec3Normalize(&vNAxis, &vNAxis);
		// build a rotational quaternion from axis and angle data
		float sinAngle = sin(fRotationAngle/2);
		D3DXQUATERNION q = D3DXQUATERNION(vNAxis.x*sinAngle, vNAxis.y*sinAngle, vNAxis.z*sinAngle, cos(fRotationAngle/2));
		// quaternion must remain normalized (TODO: is this required, considering the axis vector has just been normalized above?)
		D3DXQuaternionNormalize(&q, &q);
		x = q.x;
		y = q.y;
		z = q.z;
		w = q.w;
	}
};

// type converter that transforms an integer object type to its string representation
public __gc class ObjectTypeConverter : public /*ExpandableObjectConverter*/TypeConverter
{
public:
	// first overload of the ConvertTo() function
	virtual Object *ConvertTo(Object *value, Type *destType )
    {
        if( destType == __typeof(String) ) // TODO: add safety checking that value is indeed of a type Int32
        {
			System::Int32 *val = (System::Int32*)value;
			if (*val == OBJTYPE_SPHERE)
				return (S"SPHERE");
			else
			if (*val == OBJTYPE_BOX)
				return (S"BOX");
        }
		return __super::ConvertTo(value, destType);
    }
	// second overload of the ConvertTo() function
    virtual Object* ConvertTo(ITypeDescriptorContext* context, 
							  System::Globalization::CultureInfo* culture, Object* value, Type* destType)
    {
        if( destType == __typeof(String) )
        {
			System::Int32 *val = (System::Int32*)value;
			if (*val == OBJTYPE_SPHERE)
				return (S"SPHERE");
			else
			if (*val == OBJTYPE_BOX)
				return (S"BOX");
			else
			if (*val == OBJTYPE_EDGE)
				return (S"EDGE");
			else
			if (*val == OBJTYPE_LINE)
				return (S"LINE");
			else
			if (*val == OBJTYPE_PLANE)
				return (S"COMPOSITE BOUNDED PLANE");
			else
			if (*val == OBJTYPE_CIRCLE)
				return (S"CIRCLE");
			else
			if (*val == OBJTYPE_RECT)
				return (S"RECTANGLE");
        }
		return __super::ConvertTo(context, culture, value, destType);
    }
};

// type converter that transforms an integer force type to its string representation
public __gc class ForceTypeConverter : public /*ExpandableObjectConverter*/TypeConverter
{
public:
	// first overload of the ConvertTo() function
	virtual Object *ConvertTo(Object *value, Type *destType )
    {
        if( destType == __typeof(String) ) // TODO: add safety checking that value is indeed of a type Int32
        {
			System::Int32 *val = (System::Int32*)value;
			if (*val == FORCETYPE_GLOBAL)
				return (S"GLOBAL");
			else
			if (*val == FORCETYPE_OBJECT)
				return (S"LOCAL (OBJECT RELATIVE)");
        }
		return __super::ConvertTo(value, destType);
    }
	// second overload of the ConvertTo() function
    virtual Object* ConvertTo(ITypeDescriptorContext* context, 
							  System::Globalization::CultureInfo* culture, Object* value, Type* destType)
    {
        if( destType == __typeof(String) )
        {
			System::Int32 *val = (System::Int32*)value;
			if (*val == FORCETYPE_GLOBAL)
				return (S"GLOBAL");
			else
			if (*val == FORCETYPE_OBJECT)
				return (S"LOCAL (OBJECT RELATIVE)");
        }
		return __super::ConvertTo(context, culture, value, destType);
    }
};



/* --------------- MATHEMATICAL ENTITY OBJECTS --------------- */

public __gc class MMathObject
{
public:
	[Browsable(false)]
	__property String* get_Name() { return m_name; }
	__property void set_Name(String *value) { m_name = value; }

	[Category("Geometry properties")]
	[Description("Geometry type of the object")]
	[ReadOnly(true)]
	[Browsable(true)]
	[TypeConverter(__typeof(ObjectTypeConverter))]
	__property int get_Type() { return iType; }
	__property void set_Type(int value) { iType = value; }

	[Category("Kinetics properties")]
	[Description("Object mass [kg]")]
	__property float get_Mass() { return fMass; }
	__property void set_Mass(float value) { fMass = value; }

	[Category("Kinetics properties")]
	[Description("whether the object is fixed in the world and not influenced by forces")]
	__property bool get_Fixed() { return bFixed; }
	__property void set_Fixed(bool value) { bFixed = value; }

	[Category("Kinematics properties")]
	[Description("position in the world coord. (if a plane, this is a plane center point)")]
	__property CVector* get_Position() { return vPosition; }
	__property void set_Position(CVector* value) { vPosition = value; }

	[Category("Kinematics properties")]
	[Description("object rotational quaternion")]
	__property CQuaternion* get_Rotation() { return qRotation; }
	__property void set_Rotation(CQuaternion* value) { qRotation = value; }

	[Category("Kinematics properties")]
	[Description("linear velocity of the mass center")]
	__property CVector* get_Velocity() { return vVelocity; }
	__property void set_Velocity(CVector* value) { vVelocity = value; }

	[Category("Kinematics properties")]
	[Description("angular velocity vector")]
	__property CVector* get_AngVel() { return vAngVel; }
	__property void set_AngVel(CVector* value) { vAngVel = value; }

	[Category("Kinetics properties")]
	[Description("static coefficient of friction")]
	__property float get_StaticFriction() { return fFrictionStatic; }
	__property void set_StaticFriction(float value) { fFrictionStatic = value; }

	[Category("Kinetics properties")]
	[Description("kinetic (sliding) coefficient of friction")]
	__property float get_KineticFriction() { return fFrictionKinetic; }
	__property void set_KineticFriction(float value) { fFrictionKinetic = value; }

	[Category("Kinetics properties")]
	[Description("coefficient of collision restitution (kinetic energy loss)")]
	__property float get_Restitution() { return fRestitution; }
	__property void set_Restitution(float value) { fRestitution = value; }

	[Category("Graphics properties")]
	[Description("Object color")]
	__property System::Drawing::Color get_Color() { return color; }
	__property void set_Color(System::Drawing::Color value) { color = value; }

	// class constructor
	MMathObject()
	{
		vPosition = new CVector();
		qRotation = new CQuaternion();
		vVelocity = new CVector();
		vAngVel = new CVector();
		// default values
		fMass = 1;
		bFixed = false;
	}

	//-----------------------------------------------------------------------------
	// Name: SyncMathObject()
	// Desc: Synchronize CMathObject instance with data from MMathObject as defined through UI.
	// Par.: pMathObject... math object to synchronize
	//       bKinematics... set to TRUE, if kinematics quantities should be sycnhronized, too
	// Ret.: TRUE, if differences are such that the graphics data for the object must 
	//       be re-initialized; FALSE, if there is no such difference between objects.
	//-----------------------------------------------------------------------------
	virtual bool SyncMathObject(CMathObject *pMathObject, bool bKinematics)
	{
		bool bNeedReload = false;
		pMathObject->SetType(iType);
		pMathObject->SetMass(fMass);
		pMathObject->SetFixed(bFixed);
		pMathObject->SetStaticFriction(fFrictionStatic);
		pMathObject->SetKineticFriction(fFrictionKinetic);
		pMathObject->SetRestitution(fRestitution);
		if (bKinematics)
		{
			pMathObject->SetPosition(vPosition->X, vPosition->Y, vPosition->Z);
			D3DXQUATERNION q = D3DXQUATERNION( qRotation->X, qRotation->Y, qRotation->Z, qRotation->W );
			pMathObject->SetRotation(&q);
			D3DXVECTOR3 v = D3DXVECTOR3( vVelocity->X, vVelocity->Y, vVelocity->Z );
			pMathObject->SetVelocity(&v);
			v = D3DXVECTOR3( vAngVel->X, vAngVel->Y, vAngVel->Z );
			pMathObject->SetAngVelocity(&v);
			//pMathObject->SyncAngMomentum();
		}
		return bNeedReload;
	}

protected:
	String			*m_name;			// object name as defined in the UI
	int				iType;				// object type
	bool			bFixed;				// whether the object is fixed in the world and is not influenced by the forces
	float			fMass;				// object mass
	CVector			*vPosition;			// object position in the world coordinates (if the object is a plane, this is a center point on the bounded plane)
	CQuaternion		*qRotation;			// object rotational quaternion (normalized)
	CVector			*vVelocity;			// linear velocity of the mass center
	CVector			*vAngVel;			// angular velocity vector
	float			fFrictionStatic;	// static coefficient of friction
	float			fFrictionKinetic;	// kinetic (sliding) coefficient of friction
	float			fRestitution;		// coefficient of collision restitution
	System::Drawing::Color color;		// object color
};

// math object array list (used to properly display class properties in the collection editor)
public __gc class MMathElements : public CollectionBase 
{
public:
   __property MMathObject* get_Item( int index )
   {
      return __try_cast<MMathObject*>(List->Item[index]);
   }
   __property void set_Item( int index, MMathObject* value )
   {
      List->Item[index] = value;
   }

   int Add(MMathObject* value)
   {
      return(List->Add(value));
   }

   int IndexOf(MMathObject* value) 
   {
      return(List->IndexOf(value));
   }

   void Insert(int index, MMathObject* value) 
   {
      List->Insert(index, value);
   }

   void Remove(MMathObject* value)
   {
      List->Remove(value);
   }

   bool Contains(MMathObject* value)
   {
      return(List->Contains(value));
   }
   
/*
protected:
   void OnInsert(int index, Object* value) 
   {
      //if (value->GetType() != Type::GetType(S"System.Int16"))
      //   throw new ArgumentException(S"value must be of type Int16.", S"value");
   }

   void OnRemove(int index, Object* value) 
   {
      //if (value->GetType() != Type::GetType(S"System.Int16"))
      //   throw new ArgumentException(S"value must be of type Int16.", S"value");
   }

   void OnSet(int index, Object* oldValue, Object* newValue) 
   {
      //if (newValue->GetType() != Type::GetType(S"System.Int16"))
      //   throw new ArgumentException(S"newValue must be of type Int16.", S"newValue");
   }

   void OnValidate(Object* value) 
   {
     // if (value->GetType() != Type::GetType(S"System.Int16"))
     //    throw new ArgumentException(S"value must be of type Int16.");
   }*/
};



public __gc class MMathObjectSphere : public MMathObject
{
public:
	[Category("Kinetics properties")]
	[Description("Predefined sphere inertia tensor in local (body) coordinates")]
	[ReadOnly(true)]
	[Browsable(true)]
	__property CMatrixRO* get_Inertia() { return mInertia; }
	__property void set_Inertia(CMatrixRO *value) { mInertia = value; }

	[Category("Kinetics properties")]
	[Description("Predefined inverse of sphere inertia tensor in local (body) coordinates")]
	[ReadOnly(true)]
	[Browsable(true)]
	__property CMatrixRO* get_InertiaInv() { return mInertiaInv; }
	__property void set_InertiaInv(CMatrixRO *value) { mInertiaInv = value; }

	[Category("Geometry properties")]
	[Description("Sphere radius [m]")]
	__property float get_Radius() { return fRadius; }
	__property void set_Radius(float value) { fRadius = value; SetInertia(); }

	[Category("Kinetics properties")]
	[Description("Object mass [kg]")]
	__property float get_Mass() { return fMass; }
	__property void set_Mass(float value) { fMass = value; SetInertia(); }

	[Category("Kinetics properties")]
	[Description("rolling coefficient of friction")]
	__property float get_RollingFriction() { return fFrictionRolling; }
	__property void set_RollingFriction(float value) { fFrictionRolling = value; }

	MMathObjectSphere()
	{
		// TODO: choose an unused name
		m_name = "sphere001";
		iType = OBJTYPE_SPHERE;
		mInertia = new CMatrixRO();
		mInertiaInv = new CMatrixRO();
		fRadius = 1;
		SetInertia();
		fFrictionStatic = 0.1;
		fFrictionKinetic = 0.15;
		fFrictionRolling = 0.08;
		fRestitution = 0.92;
	}

	// class constructor
	MMathObjectSphere(String *name)
	{
		m_name = name;
		iType = OBJTYPE_SPHERE;
		mInertia = new CMatrixRO();
		mInertiaInv = new CMatrixRO();
		fRadius = 1;
		SetInertia();
		fFrictionStatic = 0.1;
		fFrictionKinetic = 0.15;
		fFrictionRolling = 0.08;
		fRestitution = 0.92;
	}
	//-----------------------------------------------------------------------------
	// Name: SyncMathObject()
	// Desc: Synchronize CMathObject instance with data from MMathObject as defined through UI.
	// Par.: pMathObject... math object to synchronize
	//       bKinematics... set to TRUE, if kinematics quantities should be sycnhronized, too
	// Ret.: TRUE, if differences are such that the graphics data for the object must 
	//       be re-initialized; FALSE, if there is no such difference between objects.
	//-----------------------------------------------------------------------------
	virtual bool SyncMathObject(CMathObject *pMathObject, bool bKinematics)
	{
		bool bNeedReload = __super::SyncMathObject(pMathObject, bKinematics);
		CMathObjectSphere *pmathobj = (CMathObjectSphere*)pMathObject;
		pmathobj->SetRollingFriction(fFrictionRolling);
		if (bKinematics)
		{
			D3DXMATRIX m = D3DXMATRIX(mInertia->A11, mInertia->A12, mInertia->A13, 0,
									  mInertia->A21, mInertia->A22, mInertia->A23, 0,
									  mInertia->A31, mInertia->A32, mInertia->A33, 0,
									  0,0,0,0);
			pmathobj->SetInertia(&m);
			m = D3DXMATRIX(mInertiaInv->A11, mInertiaInv->A12, mInertiaInv->A13, 0,
									  mInertiaInv->A21, mInertiaInv->A22, mInertiaInv->A23, 0,
									  mInertiaInv->A31, mInertiaInv->A32, mInertiaInv->A33, 0,
									  0,0,0,0);
			pmathobj->SetInertiaInv(&m);
			pMathObject->SyncAngMomentum();
		}
		if (fRadius != pmathobj->GetRadius())
		{
			pmathobj->SetRadius(fRadius);
			bNeedReload = true;
		}
		return bNeedReload;
	}

protected:
	CMatrixRO			*mInertia;			// inertia tensor in local (body) coordinates; must be changed to world coords. during integration process for complex objects (see GDMPHYS4.pdf, Eq.20)
	CMatrixRO			*mInertiaInv;		// inverse of inertia tensor in local (body) coordinates; must be changed to world coords. during integration process for complex objects (see GDMPHYS4.pdf, Eq.20)
	float				fRadius;			// sphere radius
	float				fFrictionRolling;	// sphere rolling friction coefficient
	
	// re-calculate inertia tensor based on sphere radius and mass
	void				SetInertia()
	{
		mInertia->A11 = 2*fMass*fRadius*fRadius/5;
		mInertia->A22 = mInertia->A11;
		mInertia->A33 = mInertia->A11;
		mInertiaInv->A11 = 1/mInertia->A11;
		mInertiaInv->A22 = mInertiaInv->A11;
		mInertiaInv->A33 = mInertiaInv->A11;
	}
};

public __gc class MMathObjectRect : public MMathObject
{
public:
	[Category("Geometry properties")]
	[Description("Length along the world X axis [m]")]
	__property float get_Length() { return fLength; }
	__property void set_Length(float value) { fLength = value; SetInertia(); }

	[Category("Geometry properties")]
	[Description("Width along the world Z axis [m]")]
	__property float get_Width() { return fWidth; }
	__property void set_Width(float value) { fWidth = value; SetInertia(); }

	[Category("Geometry properties")]
	[Description("initial direction of the rectangle length axis")]
	__property CVector* get_LengthVector() { return vLength; }
	__property void set_LengthVector(CVector* value) { vLength = value; }

	[Category("Geometry properties")]
	[Description("initial direction of the rectangle width axis")]
	__property CVector* get_WidthVector() { return vWidth; }
	__property void set_WidthVector(CVector* value) { vWidth = value; }

	[Category("Kinetics properties")]
	[Description("Predefined rect. inertia tensor in local (body) coordinates")]
	[ReadOnly(true)]
	[Browsable(true)]
	__property CMatrixRO* get_Inertia() { return mInertia; }
	__property void set_Inertia(CMatrixRO *value) { mInertia = value; }

	[Category("Kinetics properties")]
	[Description("Predefined inverse of rect. inertia tensor in local (body) coordinates")]
	[ReadOnly(true)]
	[Browsable(true)]
	__property CMatrixRO* get_InertiaInv() { return mInertiaInv; }
	__property void set_InertiaInv(CMatrixRO *value) { mInertiaInv = value; }

	[Category("Kinetics properties")]
	[Description("Object mass [kg]")]
	__property float get_Mass() { return fMass; }
	__property void set_Mass(float value) { fMass = value; SetInertia(); }

	MMathObjectRect()
	{
		// TODO: choose an unused name
		m_name = "rect001";
		iType = OBJTYPE_RECT;
		fLength = 1;
		fWidth = 1;
		vLength = new CVector();
		vWidth = new CVector();
		// initial height and width vectors
		vLength->X = 1.0f;
		vLength->Y = 0;
		vLength->Z = 0;
		vWidth->X = 0;
		vWidth->Y = 0;
		vWidth->Z = 1.0f;
		mInertia = new CMatrixRO();
		mInertiaInv = new CMatrixRO();
		SetInertia();
		fFrictionStatic = 0.15;
		fFrictionKinetic = 0.07;
		fRestitution = 0.8;
	}

	// class constructor
	MMathObjectRect(String *name)
	{
		m_name = name;
		iType = OBJTYPE_RECT;
		fLength = 1;
		fWidth = 1;
		vLength = new CVector();
		vWidth = new CVector();
		// initial height and width vectors
		vLength->X = 1.0f;
		vLength->Y = 0;
		vLength->Z = 0;
		vWidth->X = 0;
		vWidth->Y = 0;
		vWidth->Z = 1.0f;
		mInertia = new CMatrixRO();
		mInertiaInv = new CMatrixRO();
		SetInertia();
		fFrictionStatic = 0.15;
		fFrictionKinetic = 0.07;
		fRestitution = 0.8;
	}

	//-----------------------------------------------------------------------------
	// Name: GetNormal()
	// Desc: Helper function that calculates a normal vector from length and width vectors.
	//-----------------------------------------------------------------------------
	virtual CVector* GetNormal()
	{
		// calculate normal vector (cross product: [vLength] X [vWidth])
		D3DXVECTOR3 v = D3DXVECTOR3(vLength->X, vLength->Y, vLength->Z);
		D3DXVECTOR3 v2 = D3DXVECTOR3(vWidth->X, vWidth->Y, vWidth->Z);
		// cross product operands are reversed, since D3D coordinate system in which the
		// v1 and v2 vectors are defined is left-handed, while D3DXVec3Cross requires a
		// right-handed set of coordinates
		D3DXVec3Cross(&v, &v2, &v);
		CVector* normal = new CVector();
		normal->X = v.x;
		normal->Y = v.y;
		normal->Z = v.z;
		return normal;
	}

	//-----------------------------------------------------------------------------
	// Name: SyncMathObject()
	// Desc: Synchronize CMathObject instance with data from MMathObject as defined through UI.
	// Par.: pMathObject... math object to synchronize
	//       bKinematics... set to TRUE, if kinematics quantities should be sycnhronized, too
	// Ret.: TRUE, if differences are such that the graphics data for the object must 
	//       be re-initialized; FALSE, if there is no such difference between objects.
	//-----------------------------------------------------------------------------
	virtual bool SyncMathObject(CMathObject *pMathObject, bool bKinematics)
	{
		bool bNeedReload = __super::SyncMathObject(pMathObject, bKinematics);
		CMathObjectRect *pmathobj = (CMathObjectRect*)pMathObject;
		if (fLength != pmathobj->GetLength())
		{
			pmathobj->SetSize(fLength, pmathobj->GetWidth());
			bNeedReload = true;
		}
		if (fWidth != pmathobj->GetWidth())
		{
			pmathobj->SetSize(pmathobj->GetLength(), fWidth);
			bNeedReload = true;
		}
		D3DXVECTOR3 v = pmathobj->GetLengthVector(false);
		if (v.x != vLength->X || v.y != vLength->Y || v.z != vLength->Z)
		{
			v.x = vLength->X;
			v.y = vLength->Y;
			v.z = vLength->Z;
			pmathobj->SetLengthVector(&v);
			bNeedReload = true;
		}
		v = pmathobj->GetWidthVector(false);
		if (v.x != vWidth->X || v.y != vWidth->Y || v.z != vWidth->Z)
		{
			v.x = vWidth->X;
			v.y = vWidth->Y;
			v.z = vWidth->Z;
			pmathobj->SetWidthVector(&v);
			bNeedReload = true;
		}

		if (bKinematics)
		{
			D3DXMATRIX m = D3DXMATRIX(mInertia->A11, mInertia->A12, mInertia->A13, 0,
									  mInertia->A21, mInertia->A22, mInertia->A23, 0,
									  mInertia->A31, mInertia->A32, mInertia->A33, 0,
									  0,0,0,0);
			pmathobj->SetInertia(&m);
			m = D3DXMATRIX(mInertiaInv->A11, mInertiaInv->A12, mInertiaInv->A13, 0,
									  mInertiaInv->A21, mInertiaInv->A22, mInertiaInv->A23, 0,
									  mInertiaInv->A31, mInertiaInv->A32, mInertiaInv->A33, 0,
									  0,0,0,0);
			pmathobj->SetInertiaInv(&m);
			pMathObject->SyncAngMomentum();
		}

		// calculate normal vector (cross product: [vLength] X [vWidth])
		CVector* normal = GetNormal();
		v.x = normal->X;
		v.y = normal->Y;
		v.z = normal->Z;
		pmathobj->SetNormal(&v);
		return bNeedReload;
	}

protected:
	float			fLength;		// length of the rectangle (along the rotated vLength local axis)
	float			fWidth;			// width of the rectangle (along the rotated vWidth local axis)
	CVector*		vLength;		// vLength and vWidth are perpendicular normalized vectors, aligned with world axes X and Z, respectively
	CVector*		vWidth;
	CMatrixRO		*mInertia;		// inertia tensor in local (body) coordinates; must be changed to world coords. during integration process for complex objects (see GDMPHYS4.pdf, Eq.20)
	CMatrixRO		*mInertiaInv;	// inverse of inertia tensor in local (body) coordinates; must be changed to world coords. during integration process for complex objects (see GDMPHYS4.pdf, Eq.20)

	// re-calculate inertia tensor based on rectangle mass
	void				SetInertia()
	{
		// NOTE: it is assumed that vLength is along world X axis and vWidth is along world Z axis
		// TODO: move this initialization to CMathObject and make more "context-independent"
		mInertia->A11 = (1/(float)12.0f)*fMass*(fWidth*fWidth);
		mInertia->A22 = (1/(float)12.0f)*fMass*(fLength*fLength + fWidth*fWidth);
		mInertia->A33 = (1/(float)12.0f)*fMass*(fLength*fLength);
		D3DXMATRIX m = D3DXMATRIX(0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1);
		m._11 = mInertia->A11;
		m._22 = mInertia->A22;
		m._33 = mInertia->A33;
		D3DXMATRIX mInv;
		D3DXMatrixInverse(&mInv, NULL, &m);
		mInertiaInv->A11 = mInv._11; mInertiaInv->A12 = mInv._12; mInertiaInv->A13 = mInv._13;
		mInertiaInv->A21 = mInv._21; mInertiaInv->A22 = mInv._22; mInertiaInv->A23 = mInv._23;
		mInertiaInv->A31 = mInv._31; mInertiaInv->A32 = mInv._32; mInertiaInv->A33 = mInv._33;
	}
};


public __gc class MMathObjectCircle : public MMathObject
{
public:
	[Category("Geometry properties")]
	[Description("Circle radius [m]")]
	__property float get_Radius() { return fRadius; }
	__property void set_Radius(float value) { fRadius = value; SetInertia(); }

	[Category("Kinematics properties")]
	[Description("Circle normal vector (defines the orientation in conjunction with rotation quaternion)")]
	__property CVector* get_Normal() { return vNormal; }
	__property void set_Normal(CVector* value) { vNormal = value; }

	[Category("Kinetics properties")]
	[Description("Predefined circle. inertia tensor in local (body) coordinates")]
	[ReadOnly(true)]
	[Browsable(true)]
	__property CMatrixRO* get_Inertia() { return mInertia; }
	__property void set_Inertia(CMatrixRO *value) { mInertia = value; }

	[Category("Kinetics properties")]
	[Description("Predefined inverse of rect. inertia tensor in local (body) coordinates")]
	[ReadOnly(true)]
	[Browsable(true)]
	__property CMatrixRO* get_InertiaInv() { return mInertiaInv; }
	__property void set_InertiaInv(CMatrixRO *value) { mInertiaInv = value; }

	[Category("Kinetics properties")]
	[Description("Object mass [kg]")]
	__property float get_Mass() { return fMass; }
	__property void set_Mass(float value) { fMass = value; SetInertia(); }

	MMathObjectCircle()
	{
		//// TODO: choose an unused name
		//m_name = "circle001";
		//iType = OBJTYPE_CIRCLE;
		//fRadius = 1;
		//vNormal = new CVector();
		//// initial normal vector
		//vNormal->X = 0;
		//vNormal->Y = 0;
		//vNormal->Z = -1.0f;
		//mInertia = new CMatrixRO();
		//mInertiaInv = new CMatrixRO();
		//SetInertia();
		//fFrictionStatic = 0.15;
		//fFrictionKinetic = 0.07;
		//fRestitution = 0.8;
	}

	// class constructor
	MMathObjectCircle(String *name)
	{
		m_name = name;
		iType = OBJTYPE_CIRCLE;
		fRadius = 1;
		vNormal = new CVector();
		// initial height and width vectors
		vNormal->X = 0;
		vNormal->Y = 0;
		vNormal->Z = -1.0f;
		mInertia = new CMatrixRO();
		mInertiaInv = new CMatrixRO();
		SetInertia();
		fFrictionStatic = 0.15;
		fFrictionKinetic = 0.07;
		fRestitution = 0.8;
	}

	//-----------------------------------------------------------------------------
	// Name: SyncMathObject()
	// Desc: Synchronize CMathObject instance with data from MMathObject as defined through UI.
	// Par.: pMathObject... math object to synchronize
	//       bKinematics... set to TRUE, if kinematics quantities should be sycnhronized, too
	// Ret.: TRUE, if differences are such that the graphics data for the object must 
	//       be re-initialized; FALSE, if there is no such difference between objects.
	//-----------------------------------------------------------------------------
	virtual bool SyncMathObject(CMathObject *pMathObject, bool bKinematics)
	{
		bool bNeedReload = __super::SyncMathObject(pMathObject, bKinematics);
		CMathObjectCircle *pmathobj = (CMathObjectCircle*)pMathObject;
		if (fRadius != pmathobj->GetRadius())
		{
			pmathobj->SetRadius(fRadius);
			bNeedReload = true;
		}
		D3DXVECTOR3 v = pmathobj->GetNormal(false);
		if (v.x != vNormal->X || v.y != vNormal->Y || v.z != vNormal->Z)
		{
			v.x = vNormal->X;
			v.y = vNormal->Y;
			v.z = vNormal->Z;
			pmathobj->SetNormal(&v);
			bNeedReload = true;
		}

		if (bKinematics)
		{
			D3DXMATRIX m = D3DXMATRIX(mInertia->A11, mInertia->A12, mInertia->A13, 0,
									  mInertia->A21, mInertia->A22, mInertia->A23, 0,
									  mInertia->A31, mInertia->A32, mInertia->A33, 0,
									  0,0,0,0);
			pmathobj->SetInertia(&m);
			m = D3DXMATRIX(mInertiaInv->A11, mInertiaInv->A12, mInertiaInv->A13, 0,
									  mInertiaInv->A21, mInertiaInv->A22, mInertiaInv->A23, 0,
									  mInertiaInv->A31, mInertiaInv->A32, mInertiaInv->A33, 0,
									  0,0,0,0);
			pmathobj->SetInertiaInv(&m);
			pMathObject->SyncAngMomentum();
		}

		return bNeedReload;
	}

protected:
	float			fRadius;		// circle radius
	CVector*		vNormal;		// circle normal vector (note, that two opposing directions define the same circle)
	CMatrixRO		*mInertia;		// inertia tensor in local (body) coordinates; must be changed to world coords. during integration process for complex objects (see GDMPHYS4.pdf, Eq.20)
	CMatrixRO		*mInertiaInv;	// inverse of inertia tensor in local (body) coordinates; must be changed to world coords. during integration process for complex objects (see GDMPHYS4.pdf, Eq.20)

	// re-calculate inertia tensor based on rectangle mass
	void				SetInertia()
	{
		mInertia->A11 = 2*fMass*fRadius*fRadius/5; // TODO: this is a circle!
		mInertia->A22 = mInertia->A11;
		mInertia->A33 = mInertia->A11;
		mInertiaInv->A11 = 1/mInertia->A11;
		mInertiaInv->A22 = mInertiaInv->A11;
		mInertiaInv->A33 = mInertiaInv->A11;
	}
};


// circle as an element of the composite plane
// NOTE: all its kinematics properties are relative to the parent plane
public __gc class MMathElementCircle : public MMathObjectCircle
{
public:
	[Category("Kinematics properties")]
	[Description("Circle normal vector (defines the orientation in conjunction with rotation quaternion)")]
	[ReadOnly(true)]
	__property CVectorRO* get_Normal() { return vNormal; }
	__property void set_Normal(CVectorRO* value) { vNormal = value; __super::Normal->X = value->X; __super::Normal->Y = value->Y; __super::Normal->Z = value->Z;}

	[Browsable(false)]
	__property bool get_NameInitialized() { return bNameInitialized; }
	__property void set_NameInitialized(bool value) { bNameInitialized = value; }

	MMathElementCircle()
	{
		bNameInitialized = false;
		iType = OBJTYPE_CIRCLE;
		fRadius = 1;
		__super::vNormal = new CVector();
		vNormal = new CVectorRO();
		mInertia = new CMatrixRO();
		mInertiaInv = new CMatrixRO();
		SetInertia();
		fFrictionStatic = 0.15;
		fFrictionKinetic = 0.07;
		fRestitution = 0.8;
		pGrObject = NULL;
	}

	CGrObject*		pGrObject;			// reference to grobject is kept here since elements are not present in the root UIObject list

protected:
	bool			bNameInitialized;	// used by plane collection editor to automatically choose an unused name after object has been instanced
	CVectorRO*		vNormal;
};



__gc class PlaneCollectionEditor;

public __gc class MMathObjectPlane : public MMathObjectRect
{
public:
	[Category("Elements")]
	[Description("Array of sub-elements which are subtracted from the plane")]
	[EditorAttribute(__typeof(PlaneCollectionEditor), // specify type of collection editor
                     __typeof(System::Drawing::Design::UITypeEditor))]
	__property MMathElements* get_Elements() { return listDivElements; }
	__property void set_Elements(MMathElements *value) { listDivElements = value; }

	// class constructor
	MMathObjectPlane(String *name)
	{
		m_name = name;
		iType = OBJTYPE_PLANE;
		listDivElements = new MMathElements();
	}
	//-----------------------------------------------------------------------------
	// Name: SyncMathObject()
	// Desc: Synchronize CMathObject instance with data from MMathObject as defined through UI.
	// Par.: pMathObject... math object to synchronize
	//       bKinematics... set to TRUE, if kinematics quantities should be sycnhronized, too
	// Ret.: TRUE, if differences are such that the graphics data for the object must 
	//       be re-initialized; FALSE, if there is no such difference between objects.
	//-----------------------------------------------------------------------------
	virtual bool SyncMathObject(CMathObject *pMathObject, bool bKinematics)
	{
		bool bNeedReload = __super::SyncMathObject(pMathObject, bKinematics);
		CMathObjectPlane *pmathobj = (CMathObjectPlane*)pMathObject;
		// copy sub-elements into CMathObject instance; the easiest way to do it is to
		// clear the math object list and then create it again.
		// TODO: this procedure can't detect whether a change has occured that requires
		// re-initialization of grobject; therefore, bNeedReload is always set to TRUE;
		bNeedReload = true;
		pmathobj->ClearDivElements(true);
		for (int i=0; i<this->Elements->Count; i++)
		{
			CMathObject *pmathobjdiv;
			MMathObject *pmmathobj = this->Elements->Item[i];
			switch (pmmathobj->Type)
			{
				case OBJTYPE_RECT:
					pmathobjdiv = new CMathObjectRect();
					break;
				case OBJTYPE_SPHERE:
					pmathobjdiv = new CMathObjectSphere();
					break;
				case OBJTYPE_CIRCLE:
					{
						pmathobjdiv = new CMathObjectCircle();
						// additional processing: circle elements have a read-only normal vector
						// which must always match the plane normal vector
						CVector* normal = this->GetNormal();
						((MMathElementCircle*)(pmmathobj))->Normal->X = normal->X;
						((MMathElementCircle*)(pmmathobj))->Normal->Y = normal->Y;
						((MMathElementCircle*)(pmmathobj))->Normal->Z = normal->Z;
						((MMathObjectCircle*)(pmmathobj))->Normal->X = normal->X;
						((MMathObjectCircle*)(pmmathobj))->Normal->Y = normal->Y;
						((MMathObjectCircle*)(pmmathobj))->Normal->Z = normal->Z;
					}
					break;
				// TODO: add others if necessary
				default:
					pmathobjdiv = new CMathObject();
			}

			pmmathobj->SyncMathObject(pmathobjdiv, bKinematics);
			// additionally, since the element kinematics properties are only relative to the
			// parent plane properties, add the plane property values to get absolute values
			D3DXVECTOR3 v = pmathobjdiv->GetVelocity();
			v += pmathobj->GetVelocity();
			pmathobjdiv->SetVelocity(&v);
			v = pmathobjdiv->GetAngVelocity();
			v += pmathobj->GetAngVelocity();
			pmathobjdiv->SetAngVelocity(&v);
			D3DXQUATERNION q = pmathobjdiv->GetRotation();
			q += pmathobj->GetRotation();
			pmathobjdiv->SetRotation(&q);
			v = pmathobjdiv->GetPosition();
			v += pmathobj->GetPosition();
			pmathobjdiv->SetPosition(&v);

			pmathobj->AddDivElement(pmathobjdiv);
		}
		if (bKinematics)
		{
		}
		return bNeedReload;
	}

protected:
	MMathElements	*listDivElements;	// additional math objects which are subtracted from the plane to enable holes, round edges etc.
	
};


public __gc class MMathObjectLine : public MMathObject
{
public:
	[Category("Geometry properties")]
	[Description("length along the orientation vector [m]")]
	__property float get_Magnitude() { return fMagnitude; }
	__property void set_Magnitude(float value) { fMagnitude = value; SetInertia(); }

	[Category("Geometry properties")]
	[Description("initial direction of the line in world coordinates")]
	__property CVector* get_Orientation() { return vOrientation; }
	__property void set_Orientation(CVector* value) { vOrientation = value; }

	[Category("Kinetics properties")]
	[Description("Predefined line inertia tensor in local (body) coordinates")]
	[ReadOnly(true)]
	[Browsable(true)]
	__property CMatrixRO* get_Inertia() { return mInertia; }
	__property void set_Inertia(CMatrixRO *value) { mInertia = value; }

	[Category("Kinetics properties")]
	[Description("Predefined inverse of line inertia tensor in local (body) coordinates")]
	[ReadOnly(true)]
	[Browsable(true)]
	__property CMatrixRO* get_InertiaInv() { return mInertiaInv; }
	__property void set_InertiaInv(CMatrixRO *value) { mInertiaInv = value; }

	[Category("Kinetics properties")]
	[Description("Object mass [kg]")]
	__property float get_Mass() { return fMass; }
	__property void set_Mass(float value) { fMass = value; SetInertia(); }

	[Category("Kinetics properties")]
	[Description("static friction of the line tip point")]
	__property float get_FrictionTipStatic() { return fFrictionTipStatic; }
	__property void set_FrictionTipStatic(float value) { fFrictionTipStatic = value; }

	[Category("Kinetics properties")]
	[Description("kinetic (sliding) friction of the line tip point")]
	__property float get_FrictionTipKinetic() { return fFrictionTipKinetic; }
	__property void set_FrictionTipKinetic(float value) { fFrictionTipKinetic = value; }

	MMathObjectLine()
	{
		// TODO: choose an unused name
		m_name = "line001";
		iType = OBJTYPE_LINE;
		fMagnitude = 1;
		vOrientation = new CVector();
		// initial orientation (direction) vector
		vOrientation->X = 1.0f;
		vOrientation->Y = 0;
		vOrientation->Z = 0;
		mInertia = new CMatrixRO();
		mInertiaInv = new CMatrixRO();
		SetInertia();
		fFrictionStatic = 0.15;
		fFrictionKinetic = 0.07;
		fFrictionTipStatic = 0.6;
		fFrictionTipKinetic = 0.5;
		fRestitution = 0.8;
	}

	// class constructor
	MMathObjectLine(String *name)
	{
		m_name = name;
		iType = OBJTYPE_LINE;
		fMagnitude = 1;
		vOrientation = new CVector();
		// initial orientation (direction) vector
		vOrientation->X = 1.0f;
		vOrientation->Y = 0;
		vOrientation->Z = 0;
		mInertia = new CMatrixRO();
		mInertiaInv = new CMatrixRO();
		SetInertia();
		fFrictionStatic = 0.15;
		fFrictionKinetic = 0.07;
		fFrictionTipStatic = 0.6;
		fFrictionTipKinetic = 0.5;
		fRestitution = 0.8;
	}

	//-----------------------------------------------------------------------------
	// Name: SyncMathObject()
	// Desc: Synchronize CMathObject instance with data from MMathObject as defined through UI.
	// Par.: pMathObject... math object to synchronize
	//       bKinematics... set to TRUE, if kinematics quantities should be sycnhronized, too
	// Ret.: TRUE, if differences are such that the graphics data for the object must 
	//       be re-initialized; FALSE, if there is no such difference between objects.
	//-----------------------------------------------------------------------------
	virtual bool SyncMathObject(CMathObject *pMathObject, bool bKinematics)
	{
		bool bNeedReload = __super::SyncMathObject(pMathObject, bKinematics);
		CMathObjectLine *pmathobj = (CMathObjectLine*)pMathObject;
		if (fMagnitude != pmathobj->GetMagnitude())
		{
			pmathobj->SetMagnitude(fMagnitude);
			bNeedReload = true;
		}
		D3DXVECTOR3 v = pmathobj->GetOrientation();
		if (v.x != vOrientation->X || v.y != vOrientation->Y || v.z != vOrientation->Z)
		{
			v.x = vOrientation->X;
			v.y = vOrientation->Y;
			v.z = vOrientation->Z;
			D3DXVec3Normalize(&v, &v);
			pmathobj->SetOrientation(&v);
			bNeedReload = true;
		}

		if (bKinematics)
		{
			D3DXMATRIX m = D3DXMATRIX(mInertia->A11, mInertia->A12, mInertia->A13, 0,
									  mInertia->A21, mInertia->A22, mInertia->A23, 0,
									  mInertia->A31, mInertia->A32, mInertia->A33, 0,
									  0,0,0,0);
			pmathobj->SetInertia(&m);
			m = D3DXMATRIX(mInertiaInv->A11, mInertiaInv->A12, mInertiaInv->A13, 0,
									  mInertiaInv->A21, mInertiaInv->A22, mInertiaInv->A23, 0,
									  mInertiaInv->A31, mInertiaInv->A32, mInertiaInv->A33, 0,
									  0,0,0,0);
			pmathobj->SetInertiaInv(&m);
			pMathObject->SyncAngMomentum();
		}

		return bNeedReload;
	}

protected:
	float			fFrictionTipStatic;	// static friction of the line tip
	float			fFrictionTipKinetic;// kinetic (sliding) friction of the line tip
	float			fMagnitude;			// lines are represented by a normalized vector vOrientation from vPosition which is then rotated by qRotation and scaled by fMagnitude
	CVector*		vOrientation;		// normalized line orientation; line direction = vOrientation rotated by qRotation
	CMatrixRO		*mInertia;			// inertia tensor in local (body) coordinates; must be changed to world coords. during integration process for complex objects (see GDMPHYS4.pdf, Eq.20)
	CMatrixRO		*mInertiaInv;		// inverse of inertia tensor in local (body) coordinates; must be changed to world coords. during integration process for complex objects (see GDMPHYS4.pdf, Eq.20)

	// re-calculate inertia tensor based on rectangle mass
	void				SetInertia()
	{
		// NOTE: it is assumed that vOrientation is along world X axis
		// TODO: move this initialization to CMathObject and make it more "context-independent"
		mInertia->A11 = 0.01f; // "almost" zero
		mInertia->A22 = (1/(float)12.0f)*fMass*(fMagnitude*fMagnitude);
		mInertia->A33 = (1/(float)12.0f)*fMass*(fMagnitude*fMagnitude);
		D3DXMATRIX m = D3DXMATRIX(0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1);
		m._11 = mInertia->A11;
		m._22 = mInertia->A22;
		m._33 = mInertia->A33;
		D3DXMATRIX mInv;
		D3DXMatrixInverse(&mInv, NULL, &m);
		mInertiaInv->A11 = mInv._11; mInertiaInv->A12 = mInv._12; mInertiaInv->A13 = mInv._13;
		mInertiaInv->A21 = mInv._21; mInertiaInv->A22 = mInv._22; mInertiaInv->A23 = mInv._23;
		mInertiaInv->A31 = mInv._31; mInertiaInv->A32 = mInv._32; mInertiaInv->A33 = mInv._33;
	}
};



__gc class ObjectNameConverter; // forward decl.

/* --------------- MATHEMATICAL FORCE CLASS --------------- */


public __gc class MForce
{
public:
	[Category("Basic properties")]
	[Description("type of the force (global or body-local)")]
	[ReadOnly(true)]
	[Browsable(true)]
	[TypeConverter(__typeof(ForceTypeConverter))]
	__property int get_Type() { return iType; }
	__property void set_Type(int value) { iType = value; }

	[Category("Kinetics properties")]
	[Description("force direction in the world")]
	__property CVector* get_Direction() { return vDirection; }
	__property void set_Direction(CVector *value) { vDirection = value; }

	[Category("Kinetics properties")]
	[Description("force magnitude [Newton]")]
	__property float get_Magnitude() { return fMagnitude; }
	__property void set_Magnitude(float value) { fMagnitude = value; }

	[Category("Kinetics properties")]
	[Description("simulation time offset after the force starts acting [ms]")]
	__property int get_TimeStart() { return iTimeStart; }
	__property void set_TimeStart(int value) { iTimeStart = value; }

	[Category("Kinetics properties")]
	[Description("amount of time that the force is active (starting from TimeStart value) [ms]")]
	__property int get_Duration() { return iDuration; }
	__property void set_Duration(int value) { iDuration = value; }

	[Category("Local (body) force properties")]
	[Description("position in body coordinates (relative to attached body position)")]
	__property CVector* get_Position() { return vPosition; }
	__property void set_Position(CVector* value) { vPosition = value; }

	[Category("Local (body) force properties")]
	[Description("Name of the object that the force is attached to")]
	[TypeConverter(typeof(ObjectNameConverter))] 
	__property String* get_Object() { return objname; }
	__property void set_Object(String* value) { objname = value; }

	[Category("Global force type properties")]
	[Description("If set, force is acting differently on each object")]
	__property bool get_Gravity() { return bGravity; }
	__property void set_Gravity(bool value) { bGravity = value; if (iType == FORCETYPE_OBJECT) bGravity = false; }

	// class constructor
	MForce(int type, ArrayList *uiobjects)
	{
		// each MForce instance stores a link to UIObject list in order to be able to build
		// a list of object names which the user can select from in a combo box
		objects = uiobjects; 
		vPosition = new CVector();
		vDirection = new CVector();
		// default values
		iType = type;
		fMagnitude = 0;
		bGravity = false;
		iDuration = 0;
		iTimeStart = 0;
	}

	ArrayList		*objects;		// link to UIObject objects from which a drop-down selection property for object name is built

protected:
	int				iType;			// global forces are distinguished from those acting on a certain object
	CVector			*vDirection;	// normalized direction of the force
	CVector			*vPosition;		// force application point, relative to pMathObject's position (if type is GLOBAL, this property is not used)
	float			fMagnitude;		// force magnitude in Newtons (discarded if this is a gravitational force)
	String			*objname;		// name of the object that the force is acting on; not used if force type is GLOBAL
	int				iTimeStart;		// time offset in ms after the force starts to act
	int				iDuration;		// time in ms for the force to remain active (can be set to INFINITE)
	bool			bGravity;		// whether this is a gravitational force which depends on the object mass
};



	// list of objects as recognized from the user interface
	__gc class UIObject
	{
	public:
		UIObject() {}
		__property String* get_Name() { return m_name; }
		__property void set_Name(String *value) { m_name = value; }
		int					iObjType; // type of math object
		CMathObject			*pMathObject;
		MMathObject			*pMMathObject;
		CGrObject			*pGrObject;

	protected:
		String				*m_name;
	};



	// list of forces as seen from the user interface
	__gc class UIForce
	{
	public:
		UIForce() {}
		__property String* get_Name() { return m_name; }
		__property void set_Name(String *value) { m_name = value; }
		int					iForceType;	// type of force (global or body-local)
		CForce				*pForce;
		MForce				*pMForce;

		//-----------------------------------------------------------------------------
		// Name: SyncForce()
		// Desc: Synchronize CForce instance with data from MForce as defined through UI.
		//-----------------------------------------------------------------------------
		void SyncForce()
		{
			pForce->SetType(pMForce->Type);
			D3DXVECTOR3 v = D3DXVECTOR3( pMForce->Position->X, pMForce->Position->Y, pMForce->Position->Z );
			pForce->SetPosition(&v);
			v = D3DXVECTOR3( pMForce->Direction->X, pMForce->Direction->Y, pMForce->Direction->Z );
			pForce->SetDirection(&v);
			pForce->SetStart(pMForce->TimeStart);
			if (pMForce->Duration == 0) // when user selects 0 as duration, we assume he means INFINITY
				pForce->SetDuration(INFINITY);
			else
				pForce->SetDuration(pMForce->Duration);
			pForce->SetMagnitude(pMForce->Magnitude);
			pForce->SetGravity(pMForce->Gravity);
			// find the UIObject with a matching name of the MForce object and assign its CMathObject to the force
			if (pMForce->Object != NULL && pMForce->Type == FORCETYPE_OBJECT)
			{
				for (int i = 0; i < pMForce->objects->Count; i++)
				{
					UIObject* obj = (UIObject*)((pMForce->objects->Item[i]));
					if (obj->Name == pMForce->Object)
					{
						pForce->AssignObject(obj->pMathObject);
						break;
					}
				}
			}
		}

	protected:
		String				*m_name;
	};


// auxilary class to transform UI objects into a list of their names to show in the drop-down combo box
public __gc class ObjectNameConverter: public StringConverter 
{
public:
	virtual bool GetStandardValuesSupported(ITypeDescriptorContext *context)
	{
		return true;
	}
	virtual StandardValuesCollection* GetStandardValues(ITypeDescriptorContext *context)
	{
		ArrayList *alist = new ArrayList();
		Object *obj = context->Instance; // get calling object (MForce assumed)
		MForce* forceobj = (MForce*)obj;
		for (int i=0; i < forceobj->objects->Count; i++)
		{
			alist->Add( ((UIObject*)(forceobj->objects->Item[i]))->Name );
		}
		StandardValuesCollection* col = new StandardValuesCollection(alist);
		return col;
	} 
};

// used to specify types of new objects added to MMathObjectPlane elements collection
public __gc class PlaneCollectionEditor : public System::ComponentModel::Design::CollectionEditor
{
private:
	int iObjIndex;

	// set unique name for the object, using its type and current element count in the list
	// bClash is set to true if a name has already been assigned and a new one must be assigned
	// since the current one already exists
	void SetName(MMathObject* obj, Object* list __gc[], bool bClash)
	{
		if (!bClash)
		{
			switch (obj->Type)
			{
				case OBJTYPE_CIRCLE:
					obj->Name = "circle";
					break;
				case OBJTYPE_RECT:
					obj->Name = "rect";
					break;
				default:
					obj->Name = "object";
					break;
			}
			obj->Name = String::Format(S"{0}{1}", obj->Name, __box(list->Length+1));
			// check if the assigned name already exists
			for (int i = 0; i < list->Length; i++)
			{
				if (list[i] != obj)
				{
					if ( ((MMathObject*)(list[i]))->Name == obj->Name )
					{
						SetName(obj, list, true);
						break;
					}
				}
			}
		}
		else
		{
			// append something and try again
			obj->Name = String::Format(S"{0}a", obj->Name);
			for (int i = 0; i < list->Length; i++)
			{
				if (list[i] != obj)
				{
					if ( ((MMathObject*)(list[i]))->Name == obj->Name )
					{
						SetName(obj, list, true);
						break;
					}
				}
			}
		}
	}
public:
	PlaneCollectionEditor(Type* type) : CollectionEditor(type) { iObjIndex = 0; }

	MMathObjectPlane*	plane;

public: Object* EditValue(
   ITypeDescriptorContext* context,
   System::IServiceProvider* provider,
   Object* value
						)
		{
			// get the MMathObjectPlane instance that started this collection editor
			plane = (MMathObjectPlane*)context->Instance;
			MMathElements *elements = (MMathElements*)value;
			// when collection list is changed, property grid does not recognize the change
			// unless the collection itself is changed; therefore, we use a dirty inefficient
			// trick of copying the entire collection
			MMathElements *el2 = new MMathElements();
			for (int i=0; i<elements->Count; i++)
				el2->Add(elements->Item[i]);
			elements->Clear();
			value = el2;
			// work with the copy from now on (old collection thereby loses all references and becomes victim of GC)
			return __super::EditValue(context, provider, value);
		}

protected: 
	virtual Object* SetItems(Object* editValue, Object* value __gc[])
	{
		// when new elements are created, change common properties, like normal vectors...
		for (int i = 0; i < value->Length; i++)
		{
			MMathObject* obj = (MMathObject*)value[i];
			switch (obj->Type)
			{
				case OBJTYPE_CIRCLE:
					{
						// because of the read-only property of normal vector in element collection,
						// normal is duplicated in the inherited object; therefore, parent normal
						// should be corrected, too
						MMathElementCircle* circle = (MMathElementCircle*)obj;
						MMathObjectCircle* objcircle = (MMathObjectCircle*)obj;
						CVector* normal = plane->GetNormal();
						circle->Normal->X = normal->X;
						circle->Normal->Y = normal->Y;
						circle->Normal->Z = normal->Z;
						objcircle->Normal->X = circle->Normal->X;
						objcircle->Normal->Y = circle->Normal->Y;
						objcircle->Normal->Z = circle->Normal->Z;
						if (circle->NameInitialized == false)
						{
							SetName(obj, value, false);
							circle->NameInitialized = true;
						}
					}
					break;
				case OBJTYPE_RECT:
					{
						MMathObjectRect* rect = (MMathObjectRect*)obj; // TODO: change to MMathElementRect when impl.
						rect->LengthVector = plane->LengthVector;
						rect->WidthVector = plane->WidthVector;
					}
					break;
			}
		}
		return __super::SetItems(editValue, value);
	}

	virtual System::Type* CreateNewItemTypes()[]
	{
		Type *types[] = new Type * [2];
		types[0] = __typeof(MMathElementCircle);
		types[1] = __typeof(MMathObjectRect);

		return types;

	}
};