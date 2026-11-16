#ifndef NODE_H
#define NODE_H

#include<iostream>
#include<cassert>
#include<array>
#include<vtkCell.h>
#include<cmath>
#include "../stlib_global.h"

using namespace std;

template<typename T=double, size_t D=3>
class STLIB_EXPORT Node : public array<T,D>{

protected:
	T x[D];
    size_t nodeNumber; //number used in connectivity definition of an element
	
public:
	Node();
	Node(const Node<T,D>& node);
    Node(size_t i, T _x, T _y, T _z){nodeNumber=i; assert(D==3);x[0]=_x; x[1]=_y; x[2]=_z;}
    Node(const double* _x, size_t i=-1){for(size_t ii=0; ii<D; ii++) x[ii]=_x[ii]; nodeNumber=i;}
	
	Node<T,D>& operator=(const Node<T,D>& node);

    template<typename U, size_t Di>
	friend ostream& operator <<(ostream& sortie, const Node<U,Di>& node);

    template<typename U, size_t Di>
	friend istream& operator >>(istream& entree, Node<U,Di>& node);
	
	T* getX();

    T getXi(size_t i) const ; // To return the ith coordinate of the point
	
    size_t getNodeNumber() const;

    const T& operator [](size_t i)const{assert(/*i>=0 && */i<D); return x[i];} //unsigned always >=0
    T& operator [](size_t i){assert(/*i>=0 && */i<D); return x[i];}

    template<typename U, size_t D1>
    friend Node<U,D1> operator + (const Node<U,D1>& node1,const Node<U,D1>& node2);
    template<typename U, size_t D1>
    friend Node<U,D1> operator - (const Node<U,D1>& node1,const Node<U,D1>& node2);
    template<typename U, size_t D1>
    friend U dot(const Node<U,D1>& node1,const Node<U,D1>& node2);
    template<typename U, size_t D1>
    friend Node<U,D1> operator ^ (const Node<U,D1>& node1,const Node<U,D1>& node2);
};

    template<typename T, size_t D> Node<T,D>::Node(){}

    template<typename T, size_t D> Node<T,D>::Node(const Node<T,D>& node){for(size_t i=0; i<D; i++) x[i]=node.x[i]; nodeNumber=node.nodeNumber;}
	
    template<typename T, size_t D> Node<T,D>& Node<T,D>::operator=(const Node<T,D>& node){for(size_t i=0; i<D; i++) x[i]=node.x[i];
	nodeNumber=node.nodeNumber;
	return *this;}
	
    template<typename T, size_t D> T Node<T,D>::getXi(size_t i) const {assert(/*(i>=0) && */(i<D)); return x[i];}
	
	
    template<typename T, size_t D>	T* Node<T,D>::getX(){return x;}
	
	
    template<typename T, size_t D> size_t Node<T,D>::getNodeNumber() const {return nodeNumber;}

// Pour l'affichage des points
    template<typename T, size_t D>
	ostream& operator <<(ostream& sortie, const Node<T,D>& node){
	
	sortie<<node.nodeNumber<<" ";
    for(size_t i=0; i<D-1; i++){
	sortie<<node.x[i]<<" ";}
	sortie<<node.x[D-1];
	return sortie;}

// Pour la lecture des points

    template<typename T, size_t D>
	istream& operator >>(istream& entree, Node<T,D>& node){
	
	entree>>node.nodeNumber;
    for(size_t i=0; i<D; i++){
	entree >> node.x[i];}
	return entree;}

    //Somme mathematical operators or functions
    template<typename T, size_t D>
    Node<T,D> operator + (const Node<T,D>& node1,const Node<T,D>& node2){Node<T,D> res; for(size_t i=0; i<D; i++) res[i]=node1[i]+ node2[i]; res.nodeNumber=-1; return res;}

    template<typename T, size_t D>
    Node<T,D> operator - (const Node<T,D>& node1,const Node<T,D>& node2){Node<T,D> res; for(size_t i=0; i<D; i++) res[i]=node1[i]- node2[i]; res.nodeNumber=-1; return res;}

    template<typename T, size_t D>
    T dot(const Node<T,D>& node1,const Node<T,D>& node2){T res=0; for(size_t i=0; i<D; i++) res +=(node1[i]*node2[i]);  return res;}

    template<typename T, size_t D>
    Node<T,D> operator ^ (const Node<T,D>& node1,const Node<T,D>& node2){
    assert(D==3);
    Node<T,3> res;
    res[0]=node1[1]*node2[2]-node1[2]*node2[1]; res[1]=-node1[0]*node2[2]+ node1[2]*node2[0]; res[2]=node1[0]*node2[1]-node1[1]*node2[0];
    res.nodeNumber=std::pow(2,24)-1; return res;}

	
#endif
