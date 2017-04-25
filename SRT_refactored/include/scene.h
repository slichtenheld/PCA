// Copyright (C) 2008-2009 Tazrian Khan.
// Licensed under the GNU LGPL Version 2.1.
//
// Modified by Ashraful Kadir, 2008.
// Modified by Tazrian Khan, 2008.
//
// First added: 2008-09-27
// Last changed: 2008-10-06
//------------------------------------------------------------------------------
#ifndef SCENE_H
#define SCENE_H
//------------------------------------------------------------------------------
#include <vector>
#include "light.h"
#include "object.h"
#include "color.h"
//------------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------------
class TScene
{
public:
	vector<TLight*> Light;
	vector<TObject*> Object;	

	TScene()
	{
	}

	~TScene()
	{
      int i;
      for (i = 0; i < this->Light.size(); i++)
         delete Light[i];

      for (i = 0; i < this->Object.size(); i++)
         delete Object[i];
	}

	inline void AddObject(TObject *v) { this->Object.push_back(v); }
	
	inline void AddLight(TLight *v) { this->Light.push_back(v); }
	inline void AddLight(TPoint origin, TIntensity intensity)
	{
		TLight *v = new TLight(origin, intensity);
		this->Light.push_back(v);
	}
	
	TObject* IntersectingObject(TRay ray, float &t, bool any, int &cLoad);
	bool IsObjectInShadlow(int objectId);
	TColor Raytrace(TRay ray, int &cLoad);
	void GenerateRandomScene(int totalObjects, int totalLights,
         float boundarySX, float boundaryEX, float boundarySY, float boundaryEY,
         float boundarySZ, float boundaryEZ,
         float minShapeSize, float maxShapeSize,
	      float lightBoundaryX, float lightBoundaryY, float lightBoundaryZ);
};
//------------------------------------------------------------------------------
#endif // SCENE_H
//------------------------------------------------------------------------------
