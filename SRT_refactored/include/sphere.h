// Copyright (C) 2008-2009 Ashraful Kadir.
// Licensed under the GNU LGPL Version 2.1.
//
// Modified by Tazrian Khan, 2008
//
// First added:  2008-09-27
// Last changed: 2008-10-02
//------------------------------------------------------------------------------
#ifndef SPHERE_H
#define SPHERE_H
//------------------------------------------------------------------------------
#include "shape.h"
//------------------------------------------------------------------------------
class TSphere : public TShape
{
public:
	TSphere(TPoint o = TPoint(0.0, 0.0, 0.0), float s = 0.0)
		: TShape(o, s, ST_SPHERE)
	{
	}
	
	~TSphere()
	{
	}
	
	virtual bool RayIntersection(TRay r, float &t, int &cLoad);
};	
//------------------------------------------------------------------------------
TShape* GetRandomSphere(float boundarySX, float boundaryEX, float boundarySY,
   float boundaryEY, float boundarySZ, float boundaryEZ, 
	float minShapeSize, float maxShapeSize);
//------------------------------------------------------------------------------
#endif // SPHERE_H
//------------------------------------------------------------------------------
