// Copyright (C) 2008-2009 Ashraful Kadir.
// Licensed under the GNU LGPL Version 2.1.
//
// First added:  2008-09-29
// Last changed: 2008-09-29
//------------------------------------------------------------------------------
#include "materialproperty.h"
#include "defs.h"
//------------------------------------------------------------------------------
TMaterialProperty* GetRandomMaterialProperty()
{
	TMaterialProperty *aMP = new TMaterialProperty;
	
	aMP->SetDiffusion(TIntensity(pseudo(0.0f, 1.0f), pseudo(0.0f, 1.0f),
		pseudo(0.0f, 1.0f)));
	aMP->SetSpecular(TIntensity(pseudo(0.0f, 1.0f), pseudo(0.0f, 1.0f),
		pseudo(0.0f, 1.0f)));
	aMP->SetReflection(pseudo(0.0f, 1.0f));
	aMP->SetPower((int)pseudo(0.0f, 100.0f));

	return aMP;	
}
//------------------------------------------------------------------------------
