#ifndef COORDINATES_H
#define COORDINATES_H

#include<iostream>
#include<vector>
//#include<iomanip>
//#include<fstream>
//#include<string>
//#include<sstream>

//#include<bitset>
//#include<algorithm>
//#include<cmath>
//#include<cassert>

#include "point.h"

using namespace std;

template<typename T, size_t D>
class Coordinates : public vector<Point<T,D>>{

protected:
size_t NPts, dim;
vector<Point<T,D> > coo;

public:
	Coordinates();
	
	Coordinates(const Coordinates<T,D>& co);
	
	int getNPts() const;
	
	const Point<T,D>& getPoint(size_t i) const ;

	
	template<typename U, size_t Di>
	friend ostream& operator <<(ostream& sortie, const Coordinates<U,Di>& Coo);
	
	template<typename U, size_t Di>
	friend istream& operator >>(istream& sortie, Coordinates<U,Di>& Coo);
	

};

template<typename T, size_t D> Coordinates<T,D>::coordinates(){}

template<typename T, size_t D> Coordinates<T,D>::coordinates(const Coordinates<T,D>& co){
NPts=co.NPts; dim=co.dim;
coo.resize(NPts);
for(size_t i=0; i<NPts; i++)
coo[i]=co.getPoint(i);}

template<typename T, size_t D> int Coordinates<T,D>::getNPts() const {return NPts;}

template<typename T, size_t D> const Point<T,D>&  Coordinates<T,D>::getPoint(size_t i) const {return coo[i];}

template<typename T, size_t D>
ostream& operator <<(ostream& sortie, const Coordinates<T,D>& Coo){
for (int i=0; i<Coo.size()-1; i++){sortie<<i+1<<" "<<Coo[i]<<"\n";}
sortie<<Coo.size()<<" "<<Coo[Coo.size()-1];
return sortie;
}

template<typename T, size_t D>
istream& operator >>(istream& entree, Coordinates<T,D>& Coo){
int NumPoint; int skipdata;
entree>> Coo.dim >> Coo.NPts >>skipdata>>skipdata;
Coo.resize(Coo.NPts);
for (int i=0; i<Coo.size(); i++){entree>>NumPoint>>Coo[i];}
return entree;
}

#endif
