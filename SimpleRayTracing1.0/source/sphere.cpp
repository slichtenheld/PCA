// Copyright (C) 2008-2009 Ashraful Kadir.
// Licensed under the GNU LGPL Version 2.1.
//
// Modified by Tazrian Khan, 2008.
//
// First added:  2008-09-27
// Last changed: 2008-10-08
//------------------------------------------------------------------------------
#include "sphere.h"
#include "defs.h"
//------------------------------------------------------------------------------
bool TSphere::RayIntersection(TRay r, float &t, int &cLoad)
{
	bool retValue = false;

	TVector v = r.GetOrigin() - this->Origin;

	float a = dot(r.GetDirection(), r.GetDirection());
	float b = 2 * dot(r.GetDirection(), v);
	float c = dot(v, v) - (this->Size * this->Size);

	float D = b * b - 4 * a * c;

#ifdef LOAD_MEASUREMENT
   cLoad += 25;
#endif

   if (D < 0.0f) return false;

	float d = sqrt(D);
   float t0 = (- b - d) / (2 * a);
   float t1 = (- b + d) / (2 * a);

   if (t0 > 0.1f && t0 < t)
   {
      t = t0;
       retValue = true;
   }

   if (t1 > 0.1f && t1 < t)
   {
      t = t1;
      retValue = true;
   }

#ifdef LOAD_MEASUREMENT
   cLoad += 34;
#endif
   
   return retValue; 	
}
//------------------------------------------------------------------------------
TShape* GetRandomSphere(float boundarySX, float boundaryEX, float boundarySY,
   float boundaryEY, float boundarySZ, float boundaryEZ, 
	float minShapeSize, float maxShapeSize)
{
	TShape *aSphere = new TSphere(TPoint(pseudo(boundarySX, boundaryEX), pseudo(boundarySY, boundaryEY),
		pseudo(boundarySZ, boundaryEZ)), pseudo(minShapeSize, maxShapeSize));

	return aSphere;
}
//------------------------------------------------------------------------------
