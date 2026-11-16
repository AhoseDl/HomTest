#ifndef ELEMENT_H
#define ELEMENT_H

#include<iostream>
#include<fstream>
#include<vtkCell.h>
#include<node.h>
#include<elementtype.h>
#include "../stlib_global.h"

using namespace std;

class STLIB_EXPORT Element{
    size_t /*scaletag::fem::eltType*/ elementType;
    size_t elementSize;
    size_t *connectivity; // list of nodes in the element;  this is not renumbered

	
	public:
	Element();
	Element(const Element& e);
    Element(scaletag::fem::eltType eType, size_t eSize, std::vector<size_t> _connectivity);
	~Element();
    size_t getElementType() const ;
    //size_t getElementSize();
    size_t getElementSize() const; /*const {return elementSize;}*/
    size_t* getConnectivity();
    size_t* getConnectivity() const{return connectivity;}
    size_t* nodelist(); //return the list of node (connectivity)
    const size_t & operator[](size_t i) const {return connectivity[i];}
	

void readMarcElement(ifstream& entree);

void affiche(ostream&);

};

#endif
