// Copyright (C) 2008-2009 Tazrian Khan.
// Licensed under the GNU LGPL Version 2.1.
//
// Modified by Ashraful Kadir, 2008.
//
// First added: 2008-09-27
// Last changed: 2008-09-29
//------------------------------------------------------------------------------
#ifndef LIGHT_H
#define LIGHT_H
//------------------------------------------------------------------------------
#include <iostream>
//------------------------------------------------------------------------------
#include "color.h"
#include "svector.h"
//------------------------------------------------------------------------------
class TLight
{
   TPoint Origin;
   TIntensity Intensity;

public:
	TLight(TPoint o = TPoint(0.0f, 0.0f, SRAY_NEG_INFINITY), TIntensity i = TIntensity(0.0f, 0.0f, 0.0f))
		: Origin(o), Intensity(i)
  
	{
	}
	
	~TLight()
	{
	}
	
	inline TPoint GetOrigin() { return this->Origin; }
	inline void SetOrigin(TPoint o) { this->Origin = o; }
	inline TIntensity GetIntensity() { return this->Intensity; }
	inline void SetIntensity(TIntensity i) { this->Intensity = i; }
//	friend inline TLight* GetRandomLight(float lightBoundaryX,
//		float lightBoundaryY, float lightBoundaryZ);
};
//------------------------------------------------------------------------------
TLight* GetRandomLight(float lightBoundaryX,
	float lightBoundaryY, float lightBoundaryZ);
//------------------------------------------------------------------------------
#endif // LIGHT_H
//------------------------------------------------------------------------------
