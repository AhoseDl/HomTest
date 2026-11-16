#ifndef PBCEQUATION_H
#define PBCEQUATION_H

#include <cstddef>
#include <vector>

struct pbcEquation{
//private:
    std::vector<size_t> ptIds; //<tn, rt1, rt2, ..., rt_nRet> ==> tn = coeff1*rt1 + coeff2*rt2 + ... + coeffN*rtN
    std::vector<double> coeffs; //nRet coefficients

//public:
    size_t nRet;
    pbcEquation():nRet(0), ptIds(), coeffs(){}

    pbcEquation(const pbcEquation& eq):nRet(eq.nRet), ptIds(eq.ptIds), coeffs(eq.coeffs){
//        nRet=eq.getNRet();
//        coeffs=eq.getCoeff();
//        ptIds=eq.getPtIds();
    }
    pbcEquation(size_t _nRet, std::vector<size_t> _ptIds, std::vector<double> _coeffs):nRet(_nRet), ptIds(_ptIds), coeffs(_coeffs){
//nRet=_nRet;
//ptIds.resize(_ptIds.size(), 0); ptIds=_ptIds;
//coeffs.resize(_coeffs.size(),0); coeffs=_coeffs;
    }
    size_t getTiedId() const{return ptIds.at(0);}
    size_t getRetId(size_t i) const {return ptIds.at(i+1);}  // i=0 returns the tied id
    double getCoeff(size_t i) const {return coeffs.at(i);}  //returns the coeff of ret with id=i
    size_t getId(size_t i){return ptIds.at(i);} // returns the ith id
    size_t size(){return ptIds.size();}

//    size_t getNRet() const {return nRet;}
//    std::vector<size_t> getPtIds() const {return ptIds;}
//    std::vector<double> getCoeff() const {return coeffs;}

   //~pbcEquation(){ptIds.clear(); coeffs.clear();}

};

#endif
