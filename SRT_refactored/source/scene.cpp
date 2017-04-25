// Copyright (C) 2008-2009 Ashraful Kadir.
// Licensed under the GNU LGPL Version 2.1.
//
// First added:  2008-09-27
// Last changed: 2008-10-08
//------------------------------------------------------------------------------
#include "scene.h"
#include "defs.h"
//------------------------------------------------------------------------------
TObject* TScene::IntersectingObject(TRay ray, float &t, bool any, int &cLoad)
{
	t = SRAY_INFINITY;
	TObject *retValue = NULL;

	for (int i = 0; i < this->Object.size(); i++)
	{
#ifdef LOAD_MEASUREMENT
      cLoad += 1;
#endif

		if(this->Object[i]->RayIntersection(ray, t, cLoad))
		{
        	if(any)
        		return Object[i];
        		
        	retValue = Object[i];
        }
    }

    return retValue;
}
//------------------------------------------------------------------------------
TColor TScene::Raytrace(TRay ray, int &cLoad)
{
	TColor color(0.0f, 0.0f, 0.0f);
	float coef = 1.0f;
	int level = 0;

	do 
	{
		float t = SRAY_INFINITY;
		TObject *object = this->IntersectingObject(ray, t, false, cLoad);
		if(object == NULL)
			break;

		TPoint newOrigin = ray.GetOrigin() + t * ray.GetDirection();
		TVector n = newOrigin - object->GetShape()->GetOrigin();

#ifdef LOAD_MEASUREMENT
      cLoad += 9;
#endif

      if (!n.Normalize())
         break;

#ifdef LOAD_MEASUREMENT
      cLoad += 13;
#endif

		for(int i = 0; i < this->Light.size(); i++)
		{
			TVector d = this->Light[i]->GetOrigin() - newOrigin;
			float lightProjection = dot(n, d);

#ifdef LOAD_MEASUREMENT
         cLoad += 3 + 5;
#endif
			if(lightProjection <= 0.0f) continue;

			float t = d.Norm();

#ifdef LOAD_MEASUREMENT
         cLoad += 15;
#endif
			if(t == 0.0f) continue;

			t = 1.0 / t; //InvSqrt(t);
			d = t * d;
			lightProjection = t * lightProjection;
//			if(!d.Normalize(t)) continue;
			
			TRay lightRay(newOrigin, d);

			bool objInShadow = (NULL != this->IntersectingObject(lightRay, t, true, cLoad));

#ifdef LOAD_MEASUREMENT
         cLoad += 12;
#endif
			if(objInShadow) continue;

			color.Diffuse(lightRay.GetDirection(), n, Light[i]->GetIntensity(),
				object->GetMaterialProperty()->GetDiffusion(), coef, cLoad);

//	inline void Specular(TVector lightDir, TVector rayDir, TVector surfNormal,
//		float lightProjection, int matPower, float coef, TColor specular, TColor lightIntensity)

			color.Specular(lightRay.GetDirection(), ray.GetDirection(),
				n, lightProjection, object->GetMaterialProperty()->GetPower(), coef,
				object->GetMaterialProperty()->GetSpecular(), this->Light[i]->GetIntensity(), cLoad);
		}

		coef *= object->GetMaterialProperty()->GetReflection();
		float refl = 2.0f * dot(ray.GetDirection(), n);
		ray.SetOrigin(newOrigin);
		ray.SetDirection(ray.GetDirection() - refl * n);
		level++;

#ifdef LOAD_MEASUREMENT
      cLoad += 16;
#endif
	} 
   while ((coef > 0.0f) && (level < SRAY_MAX_LEVEL));

   return color;
}
//------------------------------------------------------------------------------
void TScene::GenerateRandomScene(int totalObjects, int totalLights,
   float boundarySX, float boundaryEX, float boundarySY, float boundaryEY,
   float boundarySZ, float boundaryEZ,
   float minShapeSize, float maxShapeSize,
	float lightBoundaryX, float lightBoundaryY, float lightBoundaryZ)
{
	for(int i = 0; i < totalLights; i++)
	{
		this->AddLight(GetRandomLight(lightBoundaryX, lightBoundaryY,
			lightBoundaryZ));
	}
	for(int i = 0; i < totalObjects; i++)
	{
		this->AddObject(GetRandomObject(boundarySX, boundaryEX,
         boundarySY, boundaryEY, boundarySZ, boundaryEZ,
         minShapeSize, maxShapeSize));
	}
}
//------------------------------------------------------------------------------
