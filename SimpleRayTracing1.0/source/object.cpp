// Copyright (C) 2008-2009 Ashraful Kadir.
// Licensed under the GNU LGPL Version 2.1.
//
// Modified by Tazrian Khan, 2008.
//
// First added:  2008-09-29
// Last changed: 2008-10-03
//------------------------------------------------------------------------------
#include "object.h"
#include "defs.h"
#include "sphere.h"
//------------------------------------------------------------------------------
TObject* GetRandomObject(float boundarySX, float boundaryEX, float boundarySY,
   float boundaryEY, float boundarySZ, float boundaryEZ, float minShapeSize,
   float maxShapeSize)
{
	TObject *aObject = new TObject;
	aObject->SetShape(GetRandomSphere(boundarySX, boundaryEX,
      boundarySY, boundaryEY, boundarySZ, boundaryEZ,
		minShapeSize, maxShapeSize));
	aObject->SetMaterialProperty(GetRandomMaterialProperty());

	return aObject;	
}
//------------------------------------------------------------------------------
