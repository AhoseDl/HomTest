#include "elementtype.h"

namespace scaletag{
namespace fem{

/* Scaletag uses the same enumeration for elements type as GMSH*/
size_t eltMarcToScaletag(size_t i){
    if(i==134) return TET4;
    if(i==157) return HERM5;
}


eltType eltGmshToScaletag(size_t i){
    //return i;

    if(i==1) return LIN2; //=1,
    if(i==3) return QUAD4; //=3,
    if(i==4) return TET4; //=4,
    if(i==5) return HEX8; //=5,
    if(i==8) return LIN3; //=8 ,
    if(i==11) return TET10; //=11,
    if(i==15) return VERTEX1; //=15,
    if(i==16) return QUAD8; //=16,
    if(i==17) return HEX20; //=17,
    if(i==50) return HERM5; //=50
}

//size_t eltScaletagToMarc(size_t);
}}

