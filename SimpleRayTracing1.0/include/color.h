// Copyright (C) 2008-2009 Ashraful Kadir.
// Licensed under the GNU LGPL Version 2.1.
//
// First added:  2008-09-26
// Last changed: 2008-09-28
//------------------------------------------------------------------------------
#ifndef COLOR_H
#define COLOR_H
//------------------------------------------------------------------------------
#include "defs.h"
#include "svector.h"
#include <cmath>
#include <iostream>
#include <algorithm>
//------------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------------
class TColor
{
    float R, G, B;

public:
	TColor(float r = 0.0, float g = 0.0, float b = 0.0)
		: R(r), G(g), B(b)
	{
	}
	
	~TColor()
	{
	}

	inline float GetR() { return R; }
	inline float GetG() { return G; }
	inline float GetB() { return B; }
	
	inline void SetR(float r) { R = r; }
	inline void SetG(float g) { G = g; }
	inline void SetB(float b) { B = b; }

	inline TColor sRGBEncode()
	{
		this->R = (this->R <= 0.0031308f) ? (12.92f * this->R) :
			(1.055f * pow(this->R, 0.4166667f) - 0.055f);
	
		this->G = (this->G <= 0.0031308f) ? (12.92f * this->G) :
			(1.055f * pow(this->G, 0.4166667f) - 0.055f);
	
		this->B = (this->B <= 0.0031308f) ? (12.92f * this->B) :
			(1.055f * pow(this->B, 0.4166667f) - 0.055f);
			
		return *this;
	}
	
   inline TColor & operator += (const TColor &c)
   {
		this->R += c.R;
		this->G += c.G;
		this->B += c.B;

		return *this;
   }

   inline float & getChannel(eColorOffset offset) { return reinterpret_cast<float*>(this)[offset]; }
   inline float getChannel(eColorOffset offset) const { return reinterpret_cast<const float*>(this)[offset]; }

	inline void Diffuse(TVector lightDir, TVector surfNormal,
		TColor lcolor, TColor diffusion, float coef, int &cLoad)
	{
	   float lambert = dot(lightDir, surfNormal) * coef;
		this->R += lambert * lcolor.R * diffusion.R;
		this->G += lambert * lcolor.G * diffusion.G;
		this->B += lambert * lcolor.B * diffusion.B;

#ifdef LOAD_MEASUREMENT
      cLoad += 15;
#endif
	}

	inline void Specular(TVector lightDir, TVector rayDir, TVector surfNormal,
		float lightProjection, int matPower, float coef, TColor specular, TColor lightIntensity, int &cLoad)
	{
		float viewProjection = dot(rayDir, surfNormal);
		TVector BlinnDir = lightDir - rayDir;
		float innerProduct = dot(BlinnDir, BlinnDir);

#ifdef LOAD_MEASUREMENT
      cLoad += 11;
#endif      
		if(innerProduct == 0.0f)
			return;

		float Blinn = InvSqrt(innerProduct) * max(lightProjection - viewProjection, 0.0f);
		Blinn = coef * pow(Blinn, matPower);

		(*this) += Blinn * (specular * lightIntensity);

#ifdef LOAD_MEASUREMENT
      cLoad += 24;
#endif
	}

	inline TColor Exposure()
	{
		float exposure = -1.00f;

		this->R = 1.0f - exp(this->R * exposure);
		this->G = 1.0f - exp(this->G * exposure);
		this->B = 1.0f - exp(this->B * exposure);

		return *this;
	}

	friend inline TColor operator * (const TColor &c1, const TColor &c2)
	{
		return TColor(c1.R * c2.R, c1.G * c2.G, c1.B * c2.B);
	}

	friend inline TColor operator + (const TColor &c1, const TColor &c2)
	{
		return TColor(c1.R + c2.R, c1.G + c2.G, c1.B + c2.B);
	}

	friend inline TColor operator * (float coef, const TColor &c)
	{
		return TColor(coef * c.R, coef * c.G, coef * c.B);
	}

	friend ostream& operator << (ostream &os, const TColor &c)
	{
      os << "(" << c.R << ", " << c.G << ", " << c.B << ")";
      return os;
	}
};
//------------------------------------------------------------------------------
typedef TColor TIntensity;
//------------------------------------------------------------------------------
#endif // COLOR_H
//------------------------------------------------------------------------------
