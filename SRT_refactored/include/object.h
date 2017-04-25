// Copyright (C) 2008-2009 Ashraful Kadir.
// Licensed under the GNU LGPL Version 2.1.
//
// Modified by Tazrian Khan, 2008.
//
// First added:  2008-09-27
// Last changed: 2008-10-03
//------------------------------------------------------------------------------
#ifndef OBJECT_H
#define OBJECT_H
//------------------------------------------------------------------------------
#include "svector.h"
#include "shape.h"
#include "materialproperty.h"
//------------------------------------------------------------------------------
class TObject
{
	TMaterialProperty *MaterialProperty;
	TShape *Shape;

public:
	TObject(TMaterialProperty *mp = NULL, TShape *s = NULL)
		: MaterialProperty(mp), Shape(s)
	{
	}

	~TObject()
	{
		if(MaterialProperty) delete MaterialProperty;
		if(Shape) delete Shape;
	}

	inline void SetMaterialProperty(TMaterialProperty *p) { this->MaterialProperty = p; }

	inline TMaterialProperty* GetMaterialProperty()
	{
		if(MaterialProperty == NULL)
			MaterialProperty = new TMaterialProperty;
			
		return MaterialProperty;
	}

	inline void SetShape(TShape *s) { this->Shape = s; }
	
	bool RayIntersection(TRay r, float &t, int &cLoad)
	{
		return Shape->RayIntersection(r, t, cLoad);
	}
		
	inline TShape* GetShape()
	{
		return this->Shape;
	}
};	
//------------------------------------------------------------------------------
TObject* GetRandomObject(float boundarySX, float boundaryEX, float boundarySY,
   float boundaryEY, float boundarySZ, float boundaryEZ, float minShapeSize,
   float maxShapeSize);
//------------------------------------------------------------------------------
#endif // OBJECT_H
//------------------------------------------------------------------------------
