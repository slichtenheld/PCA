// Copyright (C) 2008-2009 Ashraful Kadir.
// Licensed under the GNU LGPL Version 2.1.
//
// First added:  2008-09-27
// Last changed: 2008-09-27
//------------------------------------------------------------------------------
#ifndef RAY_H
#define RAY_H
//------------------------------------------------------------------------------
#include "defs.h"
#include "svector.h"
//------------------------------------------------------------------------------
class TRay
{
	TPoint Origin;
	TVector Direction;

public:
	TRay(TPoint o = TPoint(0.0f, 0.0f, SRAY_NEG_INFINITY), TVector d = TVector(0.0f, 0.0f, 1.0f))
		: Origin(o), Direction(d)
	{
	}

	~TRay()
	{
	}

	inline TPoint GetOrigin() { return Origin; }
	inline TVector GetDirection() { return Direction; }

	inline void SetOrigin(TPoint origin) { this->Origin = origin; }
	inline void SetDirection(TVector direction) { this->Direction = direction; }
};
//------------------------------------------------------------------------------
#endif // RAY_H
//------------------------------------------------------------------------------
