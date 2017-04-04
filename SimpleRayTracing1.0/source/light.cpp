// Copyright (C) 2008-2009 Ashraful Kadir.
// Licensed under the GNU LGPL Version 2.1.
//
// First added:  2008-09-29
// Last changed: 2008-09-29
//------------------------------------------------------------------------------
#include "light.h"
#include "defs.h"
//------------------------------------------------------------------------------
TLight* GetRandomLight(float lightBoundaryX, float lightBoundaryY,
	float lightBoundaryZ)
{
	TLight *aLight = new TLight(TPoint(
		pseudo(-lightBoundaryX, lightBoundaryX),
		pseudo(-lightBoundaryY, lightBoundaryY),
		pseudo(-lightBoundaryZ, lightBoundaryZ)),
		TIntensity(pseudo(0.0f, 1.0f),pseudo(0.0f, 1.0f),pseudo(0.0f, 1.0f)));
		
	return aLight;
}
//------------------------------------------------------------------------------
