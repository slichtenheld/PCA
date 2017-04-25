// Copyright (C) 2008-2009 Ashraful Kadir.
// Licensed under the GNU LGPL Version 2.1.
//
// First added:  2008-09-26
// Last changed: 2008-09-28
//------------------------------------------------------------------------------
#ifndef SVECTOR_H
#define SVECTOR_H
//------------------------------------------------------------------------------
#include <iostream>
#include <math.h>
//------------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------------
class TVector
{
	float X, Y, Z;

public:
	TVector() { X = Y = Z = 0.0; }
	TVector(float x, float y, float z) { X = x; Y = y; Z = z; }

	inline float GetX() { return X; }
	inline float GetY() { return Y; }
	inline float GetZ() { return Z; }

	inline void SetX(float x) { X = x; }
	inline void SetY(float y) { Y = y; }
	inline void SetZ(float z) { Z = z; }

   TVector& operator += (const TVector &v)
   {
	   this->X += v.X;
       this->Y += v.Y;
       this->Z += v.Z;
	   return *this;
   }

   inline float Norm()
   {
   	return sqrt(dot(*this, *this));
   }

   inline bool Normalize()
   {
		float norm = this->Norm();
    	if(norm == 0.0) return false;

    	float invnorm = 1.0f / norm;
    	this->X *= invnorm;
    	this->Y *= invnorm;
    	this->Z *= invnorm;

    	return true;
   }

   inline bool Normalize(float norm)
   {
      if(norm == 0.0) return false;

    	float invnorm = 1.0f / norm;
    	this->X *= invnorm;
    	this->Y *= invnorm;
    	this->Z *= invnorm;

    	return true;
   }

   friend inline float dot (TVector v1, TVector v2);
   friend inline TVector operator + (const TVector &v1, const TVector &v2);
   friend inline TVector operator - (const TVector &v1, const TVector &v2);
   friend inline TVector operator * (const float n, const TVector &v);

   friend inline ostream& operator << (ostream &os, const TVector &v);
};
//------------------------------------------------------------------------------
typedef TVector TPoint;
//------------------------------------------------------------------------------
inline TVector operator + (const TVector &v1, const TVector &v2)
{
	return TVector(v1.X + v2.X, v1.Y + v2.Y, v1.Z + v2.Z);
}
//------------------------------------------------------------------------------
inline TVector operator - (const TVector &v1, const TVector &v2)
{
	return TVector(v1.X - v2.X, v1.Y - v2.Y, v1.Z - v2.Z);
}
//------------------------------------------------------------------------------
inline TVector operator * (const float n, const TVector &v)
{
	return TVector(n * v.X, n * v.Y, n * v.Z);
}
//------------------------------------------------------------------------------
inline float dot (TVector v1, TVector v2)
{
	return (v1.X * v2.X + v1.Y * v2.Y + v1.Z * v2.Z);
}
//------------------------------------------------------------------------------
inline ostream& operator << (ostream &os, const TVector &v)
{
  os << "(" << v.X << ", " << v.Y << ", " << v.Z << ")";
  return os;
}
//------------------------------------------------------------------------------
#endif // SVECTOR_H
//------------------------------------------------------------------------------
