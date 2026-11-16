#ifndef ELEMENTTYPE_H
#define ELEMENTTYPE_H

#include<cstddef>

namespace scaletag{
namespace fem{

/* Scaletag uses the same enumeration for elements type as GMSH*/

enum eltType{
LIN2=1, QUAD4=3, TET4=4, HEX8=5,
LIN3=8, TET10=11, VERTEX1=15, QUAD8=16, HEX20=17,
HERM5=50/*marc element 157*/
};

size_t eltMarcToScaletag(size_t i);

eltType eltGmshToScaletag(size_t i);

//size_t eltScaletagToMarc(size_t);

}}

#endif
