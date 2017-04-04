// Copyright (C) 2008-2009 Ashraful Kadir.
// Licensed under the GNU LGPL Version 2.1.
//
// Modified by Tazrian Khan, 2008.
//
// First added:  2008-09-27
// Last changed: 2008-10-02
//------------------------------------------------------------------------------
#ifndef MATERIALPROPERTY_H
#define MATERIALPROPERTY_H
//------------------------------------------------------------------------------
#include "color.h"
//------------------------------------------------------------------------------
class TMaterialProperty
{
	TIntensity Ambient;
	TIntensity Diffusion;
	TIntensity Specular;
	TIntensity Shininess;
	TIntensity Emission;

	float Reflection;
	float Refraction;
	float Density;
	int Power;
	
public:
	TMaterialProperty(
		float refl = 0.0f,
		float refr = 0.0,
		TIntensity amb = TIntensity(0.0f, 0.0f, 0.0f),
		TIntensity diff = TIntensity(0.0f, 0.0f, 0.0f),
		TIntensity spec = TIntensity(0.0f, 0.0f, 0.0f),
		TIntensity shin = TIntensity(0.0f, 0.0f, 0.0f),
		TIntensity emi = TIntensity(0.0f, 0.0f, 0.0f)
	)
		: 	Reflection(refl), Refraction(refr), Ambient(amb), Diffusion(diff),
			Specular(spec), Shininess(shin), Emission(emi)
	{
	}
	
	~TMaterialProperty()
	{
	}
	
	inline void SetAmbient(TIntensity v) { this->Ambient = v; }
	inline void SetDiffusion(TIntensity v) { this->Diffusion = v; }
	inline void SetSpecular(TIntensity v) { this->Specular = v; }
	inline void SetShininess(TIntensity v) { this->Shininess = v; }
	inline void SetEmission(TIntensity v) { this->Emission = v; }
	inline void SetReflection(float v) { this->Reflection = v; }
	inline void SetRefraction(float v) { this->Refraction = v; }
	inline void SetDensity(float v) { this->Density = v; }
	inline void SetPower(int v) { this->Power = v; }

	inline TIntensity GetAmbient() { return this->Ambient; }
	inline TIntensity GetDiffusion() { return this->Diffusion; }
	inline TIntensity GetSpecular() { return this->Specular; }
	inline TIntensity GetShininess() { return this->Shininess; }
	inline TIntensity GetEmission() { return this->Emission; }
	inline float GetReflection() { return this->Reflection; }
	inline float GetRefraction() { return this->Refraction; }
	inline float GetDensity() { return this->Density; }
	inline int GetPower() { return this->Power; }
};
//------------------------------------------------------------------------------
TMaterialProperty* GetRandomMaterialProperty();
//------------------------------------------------------------------------------
#endif // MATERIALPROPERTY_H
//------------------------------------------------------------------------------
