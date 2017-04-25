// Copyright (C) 2008-2009 Ashraful Kadir.
// Licensed under the GNU LGPL Version 2.1.
//
// Modified by Tazrian Khan, 2008. 
//
// First added:  2008-09-26
// Last changed: 2008-10-09
//------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <ctime>
#include "svector.h"
#include "color.h"
#include "object.h"
#include "sphere.h"
#include "viewport.h"
#include "light.h"
#include "defs.h"
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
//------------------------------------------------------------------------------
#ifdef SRAY_MPI
#	include <mpi.h>
#elif defined (SRAY_OPENMP)
#	include <omp.h>
#endif
//------------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------------
/**********************************************************************/
/* timef for linux - return millisecond wall clock time		      */
/**********************************************************************/
double timef_()
{
	double msec;
	struct timeval tv;
	gettimeofday(&tv,0);
	msec = tv.tv_sec * 1.0e3 + tv.tv_usec * 1.0e-3;
	return msec;
}
//------------------------------------------------------------------------------
/**********************************************************************/
/* second for linux - return second CPU clock time		      */
/**********************************************************************/
float second_()
{
  return (float) clock()/CLOCKS_PER_SEC;
}
//------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		cout << "usage: ./simpleray [config_file_name] [output_file_name] [(optional)config_output_file_name]" << endl;
		return 1;
	}

#if (defined(SRAY_MPI) && defined(SRAY_OPENMP))
	cout << "Hybrid model not supported." << endl;
	return 1;
#endif

	TViewport *aViewport = new TViewport;
	double t, t1, t2;
	int size = -1, rank = -1;

#ifdef SRAY_MPI   
   MPI_Status status;         /* status of communication          */
   MPI_Request request;       /* handle for pending communication */

   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &rank );
   MPI_Comm_size( MPI_COMM_WORLD, &size );
#endif

//Random scene
#ifdef SRAY_RANDOM
#	if (defined(SRAY_MPI) || defined(SRAY_OPENMP))
		cout << "Random scene generation is applicable for single processor only" << endl;
		return 1;
#	endif
	srand((unsigned)time(0));
	aViewport->GetScene()->GenerateRandomScene(
		10 /*int totalObjects*/,
		5 /* int totalLights */,
   		100.0f, /* float boundarySX */
		(float)(aViewport->GetWidth() - 100.0f) /* float boundaryEX */,
   		100.0f, /* float boundarySY */
		(float)(aViewport->GetHeight() - 100.0f) /* float boundaryEY */,
   		NEAREST_VPOINT_DIST+100.0f, /* float boundarySZ */
		(float)(SRAY_INFINITY - 100.0f) /* float boundaryEZ */,
   		30.0f /* float minShapeSize */,
	   	100.0f /* float maxShapeSize */,
		10000.0f /* float lightBoundaryX */,
   		10000.0f /* float lightBoundaryY */,
	   	10000.0f /* float lightBoundaryZ */);
#else
	aViewport->ConfigureFromFile(argv[1]);
#endif

#ifdef SRAY_MPI   
	t1 = MPI_Wtime();
#else
	t1 = timef_() / 1000.0;
#endif

#ifdef SRAY_OPENMP
#	pragma omp parallel shared (aViewport, t) private (rank, size) default(none)
	{
		rank = omp_get_thread_num();
		size = omp_get_num_threads();
#endif
		aViewport->Render(size, rank);

#ifdef SRAY_OPENMP
	}
#endif

#ifdef SRAY_MPI
	t1 = MPI_Wtime() - t1;

	if (rank == 0)
		MPI_Reduce (MPI_IN_PLACE, &t1, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	else 
		MPI_Reduce (&t1, &t1, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	if(!rank)
		cout << "Rendering time required " << t1 << " seconds." << endl;
#else
	t2 = timef_() / 1000.0;
	cout << "Rendering time required " << t2-t1 << " seconds." << endl;
#endif

#ifdef SRAY_MPI
	t2 = MPI_Wtime();
	if(!aViewport->DataCollection(rank, size))
	{
		delete aViewport;
		MPI_Finalize();
		return 1;
	}
	
	t2 = MPI_Wtime() - t2;
	if (rank==0)
		MPI_Reduce (MPI_IN_PLACE, &t2, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	else 
		MPI_Reduce (&t1, &t1, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	
	if(!rank)
	{
		cout << "Data communication time required " << t2 << " seconds." << endl;
		cout << "Total time required " << t1+t2 << " seconds." << endl;
	}

	if(!rank)   // if MPI enabled, master will save the image tga file
#endif
	{
		aViewport->SaveToTGAFile(argv[2], size);
	}

#ifdef LOAD_MEASUREMENT
#	ifdef SRAY_MPI
		if(!aViewport->LoadDataCollection(rank, size))
		{
			delete aViewport;
			MPI_Finalize();
			return 1;
		}

		if(!rank)  // if MPI enabled, master will save the load measurement file
#  endif
		{
			aViewport->SaveLoadToFile("loadfile.txt", size, false);
#	if defined(SRAY_MPI) || defined(SRAY_OPENMP)
			aViewport->SaveLoadToFile("load_breakdown.txt", size, true);
#	endif	
		}
#endif

	if (argc == 4)
	{
#ifdef SRAY_MPI
		if (!rank)	// if MPI enabled, master will save the config file
#endif
		{
			aViewport->SaveConfigToFile(argv[3]);
		}
	}

	delete aViewport;

#ifdef SRAY_MPI   
	MPI_Finalize();
#endif

	return 0;
}
//------------------------------------------------------------------------------
