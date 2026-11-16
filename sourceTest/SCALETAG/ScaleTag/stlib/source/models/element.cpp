#include "element.h"

Element::Element(){}

Element::Element(const Element& e){
elementType=e.elementType;
elementSize=e.elementSize;
connectivity=new size_t[e.elementSize];
for(size_t i=0; i<e.elementSize; i++) connectivity[i]=e.connectivity[i];
}

Element::Element(scaletag::fem::eltType eType, size_t eSize, std::vector<size_t> _connectivity){

if (_connectivity.size() != eSize) return;
elementType = eType;
elementSize = eSize;
connectivity = new size_t[eSize];
for(size_t i=0; i<eSize ; i++) connectivity[i] = _connectivity[i];

}

Element::~Element(){if(connectivity!=nullptr)delete[] connectivity;}

size_t/*eltType*/ Element::getElementType() const {return elementType;}

size_t Element::getElementSize() const {return elementSize;}

size_t* Element::nodelist(){
size_t* nodeList=new size_t[elementSize];
for(size_t i=0; i<elementSize; i++) nodeList[i]=connectivity[i];
return nodeList;
}

size_t* Element::getConnectivity(){
    return connectivity;
}



void Element::readMarcElement(ifstream& entree){
    size_t skipdata; size_t marcEltType;
entree>>skipdata;
entree>>marcEltType;
if(marcEltType==134) {connectivity=new size_t[elementSize=4];
    for(size_t i=0; i<4; i++) entree>>connectivity[i]; elementType = scaletag::fem::eltMarcToScaletag(marcEltType);}

if(marcEltType==157) {connectivity=new size_t[elementSize=4];
 for(size_t i=0; i<4; i++) entree>>connectivity[i];
 elementType = scaletag::fem::eltMarcToScaletag(marcEltType); entree>>skipdata; //To skip the fith node
}
}

void Element::affiche(ostream& sortie){
sortie<<elementType<<" ";
for(size_t i=0; i<elementSize-1; i++) sortie<<connectivity[i]<<" ";
sortie<<connectivity[elementSize-1];
}

