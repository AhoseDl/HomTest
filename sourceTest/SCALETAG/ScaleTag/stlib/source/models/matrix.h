#ifndef SCALETAGMATRIX_H
#define SCALETAGMATRIX_H

#include<iostream>
#include<valarray>
#include <vector>
#include <list>
#include <cassert>
#include<algorithm>
#include<functional>



namespace scaletag{
namespace fem{


using namespace std;

template <typename T>
class matrix{

protected:
	valarray<T> val;
	valarray<size_t> idx;
	valarray<size_t> ptr;
	size_t idxmax;

public:
	matrix();
	matrix(const matrix<T>&);
    matrix(const std::valarray<size_t>& ptr,const valarray<size_t>& idx, const valarray<T>& val, size_t ncol);
	matrix<T>& operator = (const matrix<T>&);
    valarray<T> operator * (const valarray<T>& ) const;
    /*const T&*/ T operator () (size_t, size_t)  const;
	T& at(size_t, size_t);
	void resize(size_t, size_t, size_t);
	void resize(const vector<list<size_t>>&s, size_t ncol/*, size_t ndof=1*/);
	size_t nrow() const;
	size_t ncol() const;
	size_t nnz() const;	
	template <typename U> friend
	ostream & operator << (ostream&, const matrix<U>&);
	template <typename U> friend
	istream& operator >>(istream&, matrix<U>&);

    valarray<size_t> getLinesIndex() const; //This function returns the line index of values in val;
    valarray<size_t>getColumnsIndex() const{return idx;}
    valarray<size_t> getPtr() const{return ptr;}
    const valarray<T> getVal() const{return val;}
    valarray<T>& getVal() {return val;}

    matrix<T> transpose() const;
    matrix<T> mult(const matrix<T>& term2) const;
    matrix<T> plus(const matrix<T>& term2) const;
    matrix<T> minus(const matrix<T>& term2) const;
    matrix<T> operator +(const matrix<T>& mat2);
    matrix<T> operator -(const matrix<T>& mat2);
    matrix<T> operator *(const matrix<T>& mat2);
    matrix<T> operator *(const T& alpha) const;
    void getRowStruct(std::vector<std::list<size_t> >&s, size_t& nrow)const;
    void getColStruct(std::vector<std::list<size_t> >&s, size_t& col)const;
    void nFirstLines(size_t n);
    void nFirstColumns(size_t n);
    void nLastLines(size_t n);
    void nLastColumns(size_t n);
    void appendMatrixToBottom(const matrix<T>& mat);
    void appendMatrixToRight(const matrix<T>& mat);
    void zero(size_t N, size_t M){
        //this makes the matrix NxM with 0s on the diagonal
        //which are counted as non zero values
        size_t NN = (N<M ? N : M);
        //size_t MM = (N<M ? M : N);
        idxmax = M;
        ptr.resize(N+1);
        idx.resize(NN);
        val.resize(NN, 0.);
        for(size_t i=0; i<NN; i++){
            ptr[i]=i;
            idx[i]=i;
            val[i]=0;
            }
        ptr[NN] = NN;
        for(size_t i=NN; i<N; i++)
        ptr[i+1]=ptr[i];
    }
    T min()const {return val.min();}
    T max()const {return val.max();}

    void getDiagonal(std::valarray<T>& res){

        size_t nl=nrow();
        res.resize(nl, 0.);
        for(size_t i=0; i<nl; i++)
            for(size_t p=ptr[i]; p != ptr[i+1]; p++)
                if(idx[p]==i) res[i]=val[p];
    }

    matrix<T> diagRightMult(const std::valarray<T>& D1){
        //This function computes the matrix A*D1 where D1 is a diagonal matrix

        size_t nr=nrow(), jMat;
        matrix<T> res; std::valarray<T> resVal = getVal();

        for(size_t i=0; i<nr ; i++)
            for(size_t p=ptr[i]; p != ptr[i+1]; p++){
                jMat=idx[p];
                resVal[p] = resVal[p] * D1[jMat];
            }
        res=matrix<T>(ptr, idx, resVal, idxmax);
        return res;
    }
    matrix<T> diagLeftMult(const std::valarray<T>& D1){
        //This function computes the matrix D1*A where D1 is a diagonal matrix

        size_t nr=nrow();
        matrix<T> res; std::valarray<T> resVal = getVal();

        for(size_t i=0; i<nr ; i++)
            for(size_t p=ptr[i]; p != ptr[i+1]; p++){
                resVal[p] = resVal[p] * D1[i];
            }
        res=matrix<T>(ptr, idx, resVal, idxmax);
        return res;
    }
    //void setIdx(size_t _idx){idxmax=_idx;}//To be removed
};


template <typename T>
matrix<T>::matrix():val(0), idx(0), ptr(0), idxmax(0){}

template <typename T>
matrix<T>::matrix(const matrix<T>& a) : val(a.val), idx(a.idx), ptr(a.ptr), idxmax(a.idxmax){}

template <typename T>
matrix<T>::matrix(const std::valarray<size_t>& ptr,const valarray<size_t>& idx, const valarray<T>& val, size_t ncol):val(val), idx(idx), ptr(ptr), idxmax(ncol){

}

template <typename T>
matrix<T>& matrix<T>::operator = (const matrix<T>& a){
val=a.val; idx=a.idx; ptr=a.ptr; idxmax=a.idxmax;
return *this;}

template <typename T>
valarray<T> matrix<T>::operator * (const valarray<T>& x) const{

valarray<T> res (T(0), nrow());
for (size_t i=0; i<nrow(); i++)
    for (size_t p=ptr[i]; p<ptr[i+1]; p++)
        res[i] += val[p]*x[idx[p]];
return res;
}

template <typename T>
matrix<T> matrix<T>::operator * (const T& alpha) const{

matrix<T> res =*this;

res.getVal()= alpha*(res.getVal());
return res;
}

template <typename T>
/*const T&*/ T matrix<T>::operator()(size_t i, size_t j) const {

for (size_t p=ptr[i]; p!=ptr[i+1]; p++)
	if(idx[p]==j) return val[p];
//T zero =0;
//assert(0);
return T(0);

}

template <typename T>
T& matrix<T>::at(size_t i, size_t j){

for (size_t p=ptr[i]; p!=ptr[i+1]; p++)
    if (idx[p]==j) return val[p];
//T zero =0;
//assert(0); return zero;
//return;
throw std::runtime_error("Invalid referencing in the matrix");
//return std::nullopt;
}

template <typename T>
void matrix<T>::resize(size_t nr, size_t nc, size_t nnz){
ptr.resize(nr+1); val.resize(nnz); idx.resize(nnz); idxmax=nc;
}

//template <typename T>
//void matrix<T>::resize(const vector<list<size_t>>& s, size_t ncol, size_t ndof){
//size_t nrow=ndof*s.size();
//ptr.resize(/*nrow*/ndof*s.size()+1);

//ptr[0]=0; for(size_t i=1; i<ndof; i++) ptr[i]=ptr[i-1]+(i)*s[0].size();

//for (size_t i=0; i<nrow; i=i+ndof){
//ptr[i+ndof]=ptr[i]+ndof*s[i%ndof].size();

//for (size_t k=1; k<ndof; k++)
//ptr[i+ndof+k]=ptr[i+ndof+k-1]+(k)*s[i%ndof+1].size();
//
//}
//size_t nnz=ptr[nrow];
//idx.resize(nnz);

//for (size_t i=0, p=0; i<nrow; i++)
//	for (list<size_t>::const_iterator iter=s[i].begin(), last=s[i].end() ; iter != last; iter++, p++)
//	idx[p]=*iter;
//val.resize(nnz);
//val=0;
//idxmax=ncol;
//}


template <typename T>
void matrix<T>::resize(const vector<list<size_t> >& s, size_t ncol){
size_t nrow=s.size();
ptr.resize(nrow+1);
ptr[0]=0;

for (size_t i=0; i<nrow; i++)
ptr[i+1]=ptr[i]+s[i].size();

size_t nnz=ptr[nrow];
idx.resize(nnz);

for (size_t i=0, p=0; i<nrow; i++)
	for (list<size_t>::const_iterator iter=s[i].begin(), last=s[i].end() ; iter != last; iter++, p++)
	idx[p]=*iter;
val.resize(nnz);
val=0;
idxmax=ncol;
}


template <typename T>
size_t matrix<T>::nrow() const{
return ptr.size()-1 ;}

template <typename T>
size_t matrix<T>::ncol() const{
return idxmax;}

template <typename T>
size_t matrix<T>::nnz() const{
return idx.size();}

template <typename T>
ostream& operator << (ostream & os, const matrix<T>& a){

os<< "%%MatrixMarket Matrix"<<endl;
os <<a.nrow()<<" "<<a.ncol()<<" "<<a.nnz()<<endl;

for (size_t i=0; i<a.nrow(); i++)
	for (size_t p= a.ptr[i]; p<a.ptr[i+1]; p++)
	os << i+1 <<" "<<a.idx[p]+1<<" "<<a.val[p]<<endl;

return os;}

template <typename T>
istream& operator >> (istream & is, matrix<T>& a){
char c='\0';
is>>ws;
c=is.peek();
if (c=='%') while (c!='\n') is.get(c);
size_t nrow, ncol, nnz;
is>>nrow>>ncol>>nnz;
a.resize(nrow, ncol, nnz);

for (size_t p=0; p<nnz; p++){
size_t i;
is>>i>>a.idx[p]>>a.val[p];
i--; a.idx[p]--;
a.ptr[i+1]++;}

for (size_t i=0; i<nrow; i++)
a.ptr[i+1] += a.ptr[i];

return is;
}

template <typename T>
valarray<size_t> matrix<T>::getLinesIndex() const{
//This function returns the line index of values in val;
valarray<size_t> idxLines;
idxLines.resize(nnz(),0);
for(size_t i=0; i<nrow(); i++)
    for(size_t ptrIndex=0; ptrIndex<ptr[i+1]-ptr[i]; ptrIndex++)
    idxLines[ptr[i]+ptrIndex]=i;
return idxLines;
}

template <typename T>
void matrix<T>::getRowStruct(std::vector<std::list<size_t> >&s, size_t& ncol)const{
size_t nrow=this->nrow();
ncol=this->ncol();

s.resize(nrow);
for(size_t i=0; i<nrow; i++)
    for(size_t j=ptr[i]; j!=ptr[i+1]; j++)
    s[i].insert(s[i].end(), idx[j]);
}

template <typename T>
void matrix<T>::getColStruct(std::vector<std::list<size_t> >&s, size_t& nrow)const{
nrow=this->nrow();
size_t ncol=this->ncol();

s.resize(ncol);

for(size_t i=0; i<nrow; i++)
    for(size_t j=ptr[i]; j!=ptr[i+1]; j++)
    s[this->idx[j]].insert(s[this->idx[j]].end(), i);

}

template <typename T>
void matrix<T>::nFirstLines(size_t n){

if(n>0){
size_t tempnnz;

std::valarray<size_t> tempPtr = ptr[std::slice(0, n+1, 1)];
tempnnz =ptr [n];
std::valarray<size_t> tempIdx = idx[std::slice(0, tempnnz, 1)];
std::valarray<T> tempVal = val[std::slice(0, tempnnz, 1)];

ptr=tempPtr;
idx=tempIdx;
val=tempVal;
}

}


template <typename T>
void matrix<T>::nLastLines(size_t n){

size_t tempnnz;
size_t firstLines =nrow()-n;
size_t firstNnz = ptr[firstLines];
tempnnz = nnz()-firstNnz;

std::valarray<size_t> tempPtr = ptr[std::slice(firstLines, n+1, 1)];
tempPtr -= firstNnz;

std::valarray<size_t> tempIdx = idx[std::slice(firstNnz, tempnnz, 1)];
std::valarray<T> tempVal = val[std::slice(firstNnz, tempnnz, 1)];

ptr=tempPtr;
idx=tempIdx;
val=tempVal;
}


template <typename T>
void matrix<T>::nFirstColumns(size_t n){
*this = transpose();
nFirstLines(n);
*this = transpose();
}

template <typename T>
void matrix<T>::nLastColumns(size_t n){
*this = transpose();
nLastLines(n);
*this = transpose();
}


template <typename T>
void matrix<T>::appendMatrixToRight(const matrix<T>& mat){

std::valarray<size_t> tempPtr;
std::valarray<size_t> tempIdx;
std::valarray<T> tempVal;

tempPtr.resize(nrow()+1, 0);
tempIdx.resize(nnz()+mat.nnz(), 0);
tempVal.resize(nnz()+mat.nnz(), 0.);

 std::valarray<size_t> matPtr = mat.getPtr();
std::valarray<size_t> matIdx = mat.getColumnsIndex(); matIdx+=ncol();
 std::valarray<T> matVal = mat.getVal();

size_t ni=0;
tempPtr[0]=ptr[0];
for(size_t i=0; i<nrow(); i++)
{
    ni += matPtr[i+1]-matPtr[i]  +  ptr[i+1]-ptr[i] ;
    tempPtr[i+1] = ni;
}
 ni=0;
for(size_t i=0; i<nrow(); i++) {
    //size_t ni=ptr[i+1]-ptr[i];
    //prevnnz=ni;
    //size_t matnnz= matPtr[i+1]-matPtr[i];

    tempIdx[ std::slice(ni, ptr[i+1]-ptr[i],1) ] = idx[ std::slice(ptr[i], ptr[i+1]-ptr[i],1) ];
    tempVal[ std::slice(ni, ptr[i+1]-ptr[i],1) ] = val[ std::slice(ptr[i], ptr[i+1]-ptr[i],1) ];
    ni += ptr[i+1]-ptr[i];

    tempIdx[ std::slice(ni, matPtr[i+1]-matPtr[i], 1) ] = matIdx[ std::slice(matPtr[i], matPtr[i+1]-matPtr[i],1) ];
    tempVal[ std::slice(ni, matPtr[i+1]-matPtr[i], 1) ] = matVal[ std::slice(matPtr[i], matPtr[i+1]-matPtr[i],1) ];
    ni += matPtr[i+1]-matPtr[i];
}

idxmax=ncol()+mat.ncol();
ptr=tempPtr;
idx = tempIdx;
val=tempVal;

}


template <typename T>
void matrix<T>:: appendMatrixToBottom(const matrix<T>& mat){

std::valarray<size_t> tempPtr;
std::valarray<size_t> tempIdx;
std::valarray<T> tempVal;

std::valarray<size_t> matPtr = mat.getPtr();
std::valarray<size_t> matIdx = mat.getColumnsIndex();
std::valarray<T> matVal=mat.getVal();

size_t thisnnz = nnz();
matPtr += thisnnz;

tempPtr.resize(nrow()+mat.nrow()+1,0 );
tempIdx.resize(nnz()+mat.nnz(),0);
tempVal.resize(nnz()+mat.nnz(),0.);

//std::copy(std::begin(matPtr), std::end(matPtr), std::end(ptr) );
//std::copy(std::begin(matIdx), std::end(matIdx), std::end(idx) );
//std::copy(std::begin(matVal), std::end(matVal), std::end(val) );

size_t sizePtr=ptr.size();
size_t sizeMatPtr=matPtr.size();
size_t sizeIdx=idx.size();
size_t sizeMatIdx=matIdx.size();

tempPtr[ std::slice(0,sizePtr,1) ]=ptr;
tempPtr[ std::slice(sizePtr,sizeMatPtr-1,1) ] = matPtr[std::slice(1,sizeMatPtr-1,1)];

tempIdx[ std::slice(0,sizeIdx,1) ] = idx;
tempIdx[ std::slice(sizeIdx,sizeMatIdx,1) ] = matIdx;

tempVal[ std::slice(0,sizeIdx,1) ] = val;
tempVal[ std::slice(sizeIdx,sizeMatIdx,1) ] = matVal;

ptr=tempPtr;
idx=tempIdx;
val=tempVal;
idxmax=std::max(idxmax, mat.idxmax);
}


template <typename T>
matrix<T> matrix<T>::transpose() const {
size_t nrow=this->nrow(), ncol=this->ncol();
std::vector<std::list<size_t> > s(ncol);

for(size_t i=0; i<nrow; i++)
    for(size_t j=ptr[i]; j!=ptr[i+1]; j++)
    s[this->idx[j]].insert(s[this->idx[j]].end(), i);

matrix<T> res;
res.resize(s, nrow);

for(size_t i=0; i<ncol; i++)
    for(size_t j=res.ptr[i]; j!=res.ptr[i+1]; j++)
    res.at(i,res.idx[j]) = (*this)(res.idx[j],i);

res.idxmax=nrow;
return res;
}


template <typename T>
matrix<T> matrix<T>::mult(const matrix<T>& term2)const{
// Une façon bien naïve de multiplier les matrices
// Utiliser plus tard peut-être l'algorithme de Strassen dont l'intérêt
// est discutable pour les matrices de tailles modérées.

size_t nrow1=this->nrow();
size_t ncol1; //=this->ncol();
size_t nrow2; //=term2.nrow();
size_t ncol2=term2.ncol();

matrix<T> res;
std::vector<std::list<size_t> > s(nrow1);

std::vector<std::list<size_t> >s1, s2;
this->getRowStruct(s1,ncol1);
term2.getColStruct(s2, nrow2);

for(size_t i=0; i<nrow1; i++)
    for(size_t j=0; j<ncol2; j++)
    {
    std::list<size_t> temp;
    std::set_intersection(s1[i].begin(), s1[i].end(), s2[j].begin(), s2[j].end(),std::back_inserter(temp));
    if(!temp.empty()) s[i].push_back(j);

    }
res.resize(s,ncol2);

//for(size_t i=0; i<nrow1; i++)
//    for(size_t j=res.ptr[i]; j!=res.ptr[i+1]; j++)
//    {
//    std::list<size_t> temp;
//    std::set_intersection(s1[i].begin(), s1[i].end(), s2[res.idx[j]].begin(), s2[res.idx[j]].end(),std::back_inserter(temp));
//    if(!temp.empty()){
//        for(auto p:temp) res.val[j] += ((*this)(i,p))*(term2(p,res.idx[j])) ;}
//    }

//res_ij = this_ik*B_ik

for(size_t i=0; i<res.nrow(); i++)
    for(size_t p= res.ptr[i]; p<res.ptr[i+1]; p++)
    {
    size_t j=res.idx[p];
    for(size_t pA=ptr[i]; pA<ptr[i+1]; pA++)
                res.val[p] += ( val[pA] )*( term2(idx[pA], j) );
    }

return res;
}


template <typename T>
matrix<T> matrix<T>::plus(const matrix<T>& term2) const{
//we suppose lists are sorted
matrix<T> res;
std::vector<std::list<size_t> > s(this->nrow());
size_t nrow=this->nrow();
size_t ncol; /*=this->ncol();*/
std::vector<std::list<size_t> >s1, s2;
this->getRowStruct(s1,ncol);
term2.getRowStruct(s2,ncol);

for(size_t i=0; i<nrow; i++){
    std::list<size_t> temp;
    std::merge(s1[i].begin(), s1[i].end(), s2[i].begin(), s2[i].end(),std::back_inserter(temp));
    auto it=std::unique(temp.begin(), temp.end());
    temp.erase(it,temp.end());
    s[i].assign(temp.begin(),temp.end());
}

res.resize(s,ncol);
for(size_t i=0; i<nrow; i++)
    for(size_t j=res.ptr[i]; j!=res.ptr[i+1]; j++)
    res.val[j] = (*this)(i,res.idx[j]) +term2(i,res.idx[j]);

return res;
}

template <typename T>
matrix<T> matrix<T>::minus(const matrix<T>& term2)const{
matrix<T> res;
res=term2;
res.val=-1*res.val;
return res;

}

template <typename T>
matrix<T> matrix<T>::operator +(const matrix<T>& mat2){return this->plus(mat2);}

template <typename T>
matrix<T> matrix<T>::operator -(const matrix<T>& mat2){return this->minus(mat2);}

template <typename T>
matrix<T> matrix<T>::operator *(const matrix<T>& mat2){return this->mult(mat2);}



template<typename T>
matrix<T> toDiagMatrix(const std::valarray<T>& vec){
/* This function transforms a std::valarray (vec) to a squared matrix<> where vec is the diagonal */
matrix<T> res;
size_t nl = vec.size();
std::vector<std::list<size_t> > s(nl);
for(size_t i=0; i<nl; i++) s[i].push_back(i);
res.resize(s, nl);
for(size_t i=0; i<nl; i++ ) res.at(i, i) = vec[i];
return res;
}

}}

#endif


	
