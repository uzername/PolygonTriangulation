#ifndef TRIANGULATIONFILE_GUARD
#define TRIANGULATIONFILE_GUARD

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <ctype.h>
#include <array>
#include <vector>
#include <tuple>
#include "mapbox/earcut.hpp"

// The number type to use for tessellation
using Coord = double;

// The index type. Defaults to uint32_t, but you can also pass uint16_t if you know that your
// data won't have more than 65536 vertices.
using eNochka = uint32_t;

// Create array
using Point = std::array<Coord, 2>;
extern std::vector<std::vector<Point>> polygon;

typedef    struct returnTriangularStruct
    {
    std::vector<std::vector<Point>> initialvertlist;
    std::vector<eNochka> finalvertlist;
    } returnTriangularStructType;

returnTriangularStructType performtesttriangulation();
// the first itm in tuple is the return value, second itm in tuple is the 
std::tuple<eNochka,eNochka> getCoordinateByIndice(eNochka in_indice);

extern returnTriangularStructType aFinalTriangulation;



#endif