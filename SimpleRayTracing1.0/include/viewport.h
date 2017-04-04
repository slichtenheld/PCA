// Copyright (C) 2008-2009 Ashraful Kadir.
// Licensed under the GNU LGPL Version 2.1.
//
// Modified by Tazrian Khan, 2008.
//
// First added:  2008-09-27
// Last changed: 2008-10-06
//------------------------------------------------------------------------------
#ifndef VIEWPORT_H
#define VIEWPORT_H
//------------------------------------------------------------------------------
#include "scene.h"
#include "defs.h"
//------------------------------------------------------------------------------
class TViewport
{
	int Width;
	int Height;

	int locHeight;
   	
	COLOR *ImageMatrix;
	int *LoadMatrix;

	TScene *Scene;

	int *storeRcvDatasize;
	int storeMaxDatasize;

public:
	TViewport(int w = RES_COL, int h = RES_ROW)
		: Width(w), Height(h)
	{
		ImageMatrix = NULL; //new COLOR [Width * Height * 3];

#ifdef LOAD_MEASUREMENT
		LoadMatrix = NULL; //new int [Width * Height];
#endif

		storeMaxDatasize = 0;
		storeRcvDatasize = NULL;
      
		Scene = new TScene;
	}

	~TViewport()
	{
		delete [] ImageMatrix;
#ifdef LOAD_MEASUREMENT
		delete [] LoadMatrix;
#endif
		delete Scene;
#ifdef SRAY_MPI
		delete [] storeRcvDatasize;
#endif
	}
	
	inline TScene* GetScene() { return this->Scene; }
	inline COLOR* GetImageMatrix() { return this->ImageMatrix; }
	inline int* GetStoreRcvDatasize() { return this->storeRcvDatasize; }
	inline int GetStoreMaxDatasize() { return this->storeMaxDatasize; }
	
	inline int GetWidth() { return this->Width; }
	inline int GetHeight() { return this->Height; }

	inline int GetLocHeight() { return this->locHeight; }

	inline void SetWidth(int width) { this->Width = width; }
	inline void SetHeight(int height) { this->Height = height; }
	
	void SetColor(int x, int y, TColor c);

	inline void SetLoad(int x, int y, int load) { this->LoadMatrix[y*this->Width + x] = load; }
	inline int GetLoad(int x, int y) { return this->LoadMatrix[y*this->Width + x]; }
	bool SaveLoadToFile(char *filename, int size, bool breakdown);

	bool AllocateMemory(int starty, int endy, int yincr, int rank, int size);

	bool MetadataCollection(int &datasize, int p, int P);
	bool DataCollection(int rank, int size);
	bool LoadDataCollection(int rank, int size);
	bool SaveToTGAFile(char *filename, int size);

	bool Render(int size, int rank);
	
	bool ConfigureFromFile(char *filename);
	bool SaveConfigToFile(char *filename);
};
//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------
