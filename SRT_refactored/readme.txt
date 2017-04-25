Release 1.0, 23-Oct-2008

Authors
=======
* Ashraful Kadir, http://www.csc.kth.se/~smakadir
* Tazrian Khan

How to run
==========
* To compile with MPI: Include "-DSRAY_MPI" in CFLAGS
* To compile with OpenMP: Include "-DSRAY_OPENMP" in CFLAGS
* Enable load balancing: Include "-DSRAY_LB1" in CFLAGS
* Enable load measurement: Include "-DLOAD_MEASUREMENT" in CFLAGS
* Command line parameters: Input config file name, output file name and (optional) output config file name.
* Use Makefile in the source directory

Acknowledgement
===============
* Michael Chourdakis, for the nice XML parser. Visit http://www.turboirc.com/forum to learn more.
