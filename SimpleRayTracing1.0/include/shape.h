// Copyright (C) 2008-2009 Ashraful Kadir.
// Licensed under the GNU LGPL Version 2.1.
//
// Modified By Tazrian Khan, 2008.
//
// First added:  2008-09-27
// Last changed: 2008-10-02
//------------------------------------------------------------------------------
#ifndef SHAPE_H
#define SHAPE_H
//------------------------------------------------------------------------------
#include "defs.h"
#include "svector.h"
#include "ray.h"
//------------------------------------------------------------------------------
class TShape
{
protected:
	TPoint Origin;
	float Size;
   eShapeType Type;

public:
	TShape(TPoint o = TPoint(0.0, 0.0, 0.0), float s = 0.0, eShapeType st = ST_UNDEFINED)
		: Origin(o), Size(s), Type(st)
	{
	}

	~TShape() {}

	virtual bool RayIntersection(TRay r, float &t, int &cLoad) = 0;

	virtual inline void SetOrigin(TPoint origin) { Origin = origin; }
	virtual inline TPoint GetOrigin() { return Origin; }

   virtual inline void SetSize(float size) { Size = size; }
   virtual inline float GetSize() { return Size; }

   virtual inline void SetType(eShapeType type) { Type = type; }
   virtual inline eShapeType GetType() { return Type; }
};
//------------------------------------------------------------------------------
#endif // SHAPE_H
//------------------------------------------------------------------------------
