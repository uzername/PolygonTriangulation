#ifndef TRIANGULATIONFILE_GUARD
#define TRIANGULATIONFILE_GUARD
#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include "triangle.h"
#ifdef SINGLE
#define REAL float
#else /* not SINGLE */
#define REAL double
#endif /* not SINGLE */

typedef    struct returnTriangularStruct
    {
        REAL* initialvertlist;
        int vertcount;
        REAL* finalvertlist;
        int finalvertcount;
        // ALL triangle vertices
        int* triangleindiceslist;
        // total number of ALL triangle vertices
        int triangleindiceslistcount;
        char free_required;
    } returnTriangularStructType;

returnTriangularStructType performtesttriangulation();

extern returnTriangularStructType aFinalTriangulation;

#ifdef __cplusplus
}
#endif

#endif