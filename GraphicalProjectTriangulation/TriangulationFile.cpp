#include "TriangulationFile.h"


returnTriangularStructType performtesttriangulation() {
    returnTriangularStructType value_output;       
    // Fill polygon structure with actual data. Any winding order works.
    // The first polyline defines the main polygon.
    value_output.initialvertlist.push_back({ {100, 0}, {100, 100}, {0, 100}, {0, 50}, {20,50}, {20,20}, {0,20}, {0, 0} });
    // Following polylines define holes.
    value_output.initialvertlist.push_back({ {75, 25}, {75, 75}, {25, 75}, {25, 25} });
    // Run tessellation
    // Returns array of indices that refer to the vertices of the input polygon.
    // e.g: the index 6 would refer to {25, 75} in this example.
    // Three subsequent indices form a triangle. Output triangles are clockwise.
    std::vector<eNochka> indices = mapbox::earcut<eNochka>(value_output.initialvertlist);
    value_output.finalvertlist = indices;


	return value_output;
}
std::tuple<eNochka, eNochka> getCoordinateByIndice(eNochka in_indice) {
    eNochka INDEXPTR =0 ;
    eNochka numberOfContour = -1; eNochka numberOfPoint = -1;
    for (int i = 0; i < aFinalTriangulation.initialvertlist.size(); i++) {
        for (int j = 0; j < aFinalTriangulation.initialvertlist[i].size(); j++) {
            if (in_indice == INDEXPTR) {
                numberOfContour = i; numberOfPoint = j;
                std::tuple<eNochka, eNochka> out_tuple(numberOfContour, numberOfPoint);
                return out_tuple;
            }
            INDEXPTR++;
        }
    }
    std::tuple<eNochka, eNochka> out_tuple(numberOfContour, numberOfPoint);
    return out_tuple;
}
returnTriangularStructType aFinalTriangulation;
std::vector<std::vector<Point>> polygon;