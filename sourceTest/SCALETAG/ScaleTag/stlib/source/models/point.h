#ifndef POINT_H
#define POINT_H

#include<iostream>
#include<cassert>
#include<array>

using namespace std;

template<typename T=double, size_t D=3>
class Point : public array<T,D>{

protected:
	T x[D];
	
public:
	Point();
	Point(const Point<T,D>& Pt);
	
	Point<T,D>& operator=(const Point<T,D>& Pt);

	template<typename U, size_t Di>
	friend ostream& operator <<(ostream& sortie, const Point<U,Di>& Pt);

	template<typename U, size_t Di>
	friend istream& operator >>(istream& entree, Point<U,Di>& Pt);

	T getXi(int i) const ; // To return the ith coordinate of the point

};


	template<typename T, size_t D> Point<T,D>::Point(){}

	template<typename T, size_t D> Point<T,D>::Point(const Point<T,D>& Pt){for(size_t i=0; i<D; i++) x[i]=Pt.x[i];}
	
	template<typename T, size_t D> Point<T,D>& Point<T,D>::operator=(const Point<T,D>& Pt){for(size_t i=0; i<D; i++) x[i]=Pt.x[i];
	return *this;}
	
	template<typename T, size_t D> T Point<T,D>::getXi(int i) const {assert((i>=0)||(i<D-1)); return x[i];}

// Pour l'affichage des points
	template<typename T, size_t D>
	ostream& operator <<(ostream& sortie, const Point<T,D>& Pt){
	
	for(size_t i=0; i<D-1; i++){
	sortie<<Pt.x[i]<<" ";}
	sortie<<Pt.x[D-1];
	return sortie;}

// Pour la lecture des points

	template<typename T, size_t D>
	istream& operator >>(istream& entree, Point<T,D>& Pt){
	
	for(size_t i=0; i<D; i++){
	entree >> Pt.x[i];}
	return entree;}
	
#endif
