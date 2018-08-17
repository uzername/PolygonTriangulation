#include "TriangulationFile.h"

#ifdef __cplusplus
extern "C" {
#endif
returnTriangularStructType performtesttriangulation() {
    struct triangulateio in, mid, out, vorout;
     returnTriangularStructType value_output;
    /* Define input points. */

    in.numberofpoints = 4;
    #ifdef useadvancedattributes
    in.numberofpointattributes = 1;
    #else
    in.numberofpointattributes = 0;
    #endif

    in.pointlist = (REAL *)malloc(in.numberofpoints * 2 * sizeof(REAL));
    in.pointlist[0] = 0.0;
    in.pointlist[1] = 0.0;
    in.pointlist[2] = 5.0;
    in.pointlist[3] = 0.0;
    in.pointlist[4] = 3.0;
    in.pointlist[5] = 10.0;
    in.pointlist[6] = 0.0;
    in.pointlist[7] = 10.0;
    in.pointattributelist = (REAL *)malloc(in.numberofpoints *
        in.numberofpointattributes *
        sizeof(REAL));
    #ifdef useadvancedattributes
    in.pointattributelist[0] = 0.0;
    in.pointattributelist[1] = 1.0;
    in.pointattributelist[2] = 11.0;
    in.pointattributelist[3] = 10.0;
    #endif
    in.pointmarkerlist = (int *)malloc(in.numberofpoints * sizeof(int));
    in.pointmarkerlist[0] = 0;
    in.pointmarkerlist[1] = 0;
    in.pointmarkerlist[2] = 0;
    in.pointmarkerlist[3] = 0;

    in.numberofsegments = 0;
    in.numberofholes = 0;
    #ifdef useadvancedattributes
    in.numberofregions = 1;
    #else
    in.numberofregions = 0;
    #endif
    in.regionlist = (REAL *)malloc(in.numberofregions * 4 * sizeof(REAL));
#ifdef useadvancedattributes
    in.regionlist[0] = 0.5;
    in.regionlist[1] = 5.0;
    in.regionlist[2] = 7.0;            /* Regional attribute (for whole mesh). */
    in.regionlist[3] = 0.1;          /* Area constraint that will not be used. */
    #endif

    /* Make necessary initializations so that Triangle can return a */
    /*   triangulation in `mid' and a voronoi diagram in `vorout'.  */

    mid.pointlist = (REAL *)NULL;            /* Not needed if -N switch used. */
                                             /* Not needed if -N switch used or number of point attributes is zero: */
    mid.pointattributelist = (REAL *)NULL;
    mid.pointmarkerlist = (int *)NULL; /* Not needed if -N or -B switch used. */
    mid.trianglelist = (int *)NULL;          /* Not needed if -E switch used. */
                                             /* Not needed if -E switch used or number of triangle attributes is zero: */
    mid.triangleattributelist = (REAL *)NULL;
    mid.neighborlist = (int *)NULL;         /* Needed only if -n switch used. */
                                            /* Needed only if segments are output (-p or -c) and -P not used: */
    mid.segmentlist = (int *)NULL;
    /* Needed only if segments are output (-p or -c) and -P and -B not used: */
    mid.segmentmarkerlist = (int *)NULL;
    mid.edgelist = (int *)NULL;             /* Needed only if -e switch used. */
    mid.edgemarkerlist = (int *)NULL;   /* Needed if -e used and -B not used. */

    vorout.pointlist = (REAL *)NULL;        /* Needed only if -v switch used. */
                                            /* Needed only if -v switch used and number of attributes is not zero: */
    vorout.pointattributelist = (REAL *)NULL;
    vorout.edgelist = (int *)NULL;          /* Needed only if -v switch used. */
    vorout.normlist = (REAL *)NULL;         /* Needed only if -v switch used. */

                                            /* Triangulate the points.  Switches are chosen to read and write a  */
                                            /*   PSLG (p), preserve the convex hull (c), number everything from  */
                                            /*   zero (z), assign a regional attribute to each element (A), and  */
                                            /*   produce an edge list (e), a Voronoi diagram (v), and a triangle */
                                            /*   neighbor list (n).                                              */

    triangulate((char*) "pczAevn", &in, &mid, &vorout);


    /* Attach area constraints to the triangles in preparation for */
    /*   refining the triangulation.                               */
    /* Needed only if -r and -a switches used: */
    mid.trianglearealist = (REAL *)malloc(mid.numberoftriangles * sizeof(REAL));
    mid.trianglearealist[0] = 3.0;
    mid.trianglearealist[1] = 1.0;
    /* Make necessary initializations so that Triangle can return a */
    /*   triangulation in `out'.                                    */
    out.pointlist = (REAL *)NULL;            /* Not needed if -N switch used. */
                                             /* Not needed if -N switch used or number of attributes is zero: */
    out.pointattributelist = (REAL *)NULL;
    out.trianglelist = (int *)NULL;          /* Not needed if -E switch used. */
                                             /* Not needed if -E switch used or number of triangle attributes is zero: */
    out.triangleattributelist = (REAL *)NULL;

    /* Refine the triangulation according to the attached */
    /*   triangle area constraints.                       */

    triangulate((char*)"prazBP", &mid, &out, (struct triangulateio *) NULL);

    // printf("Refined triangulation:\n\n");
    // report(&out, 0, 1, 0, 0, 0, 0);

    /* Free all allocated arrays, including those allocated by Triangle. */

	value_output.initialvertlist = (REAL *)malloc(in.numberofpoints * 2 * sizeof(REAL));
	memcpy(value_output.initialvertlist, in.pointlist, in.numberofpoints * 2 * sizeof(REAL));
    value_output.vertcount = in.numberofpoints;
    #define userefined
    #ifdef userefined
    value_output.triangleindiceslistcount = out.numberoftriangles;
    value_output.triangleindiceslist = (int*)malloc(out.numberoftriangles*3*sizeof(int));
    memcpy(value_output.triangleindiceslist,out.trianglelist, out.numberoftriangles * 3 * sizeof(int));
    value_output.finalvertcount = out.numberofpoints;
    value_output.finalvertlist = (REAL *)malloc(out.numberofpoints*2*sizeof(REAL));
    memcpy(value_output.finalvertlist,out.pointlist, out.numberofpoints * 2 * sizeof(REAL));
    #else
    value_output.triangleindiceslistcount = mid.numberoftriangles;
    value_output.triangleindiceslist = (int*)malloc(mid.numberoftriangles * 3 * sizeof(int));
    memcpy(value_output.triangleindiceslist, mid.trianglelist, mid.numberoftriangles * 3 * sizeof(int));
    value_output.finalvertcount = mid.numberofpoints;
    value_output.finalvertlist = (REAL *)malloc(mid.numberofpoints * 2 * sizeof(REAL));
    memcpy(value_output.finalvertlist, mid.pointlist, mid.numberofpoints * 2 * sizeof(REAL));
    #endif
	value_output.free_required = 1;


    free(in.pointlist);
    free(in.pointattributelist);
    free(in.pointmarkerlist);
    free(in.regionlist);
    free(mid.pointlist);
    free(mid.pointattributelist);
    free(mid.pointmarkerlist);
    free(mid.trianglelist);
    free(mid.triangleattributelist);
    free(mid.trianglearealist);
    free(mid.neighborlist);
    free(mid.segmentlist);
    free(mid.segmentmarkerlist);
    free(mid.edgelist);
    free(mid.edgemarkerlist);
    free(vorout.pointlist);
    free(vorout.pointattributelist);
    free(vorout.edgelist);
    free(vorout.normlist);
    free(out.pointlist);
    free(out.pointattributelist);
    free(out.trianglelist);
    free(out.triangleattributelist);
	
	return value_output;
}

returnTriangularStructType aFinalTriangulation;
#ifdef __cplusplus
}
#endif