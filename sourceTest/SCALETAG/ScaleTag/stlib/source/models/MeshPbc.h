#ifndef MeshPbc_H
#define MeshPbc_H

#include<iostream>
#include<vector>
#include<node.h>
#include<element.h>
#include<pbcFunctions.h>
#include<cctype> //for isdigit() function
#include<fstream>
#include <valarray>
#include<algorithm>

#include<pbcmeshtools.h>

using namespace std;


template<typename T, size_t D> class MeshPbc
{
    vector<Node<T,D> > nodes;
	vector<Element> elements;
    size_t numberOfNodes;
    vector<size_t> renumberedNodesId;
    size_t d=3; // for space dimension
    bool referenceDofsOK = false;

    std::vector<size_t> renum;
    std::valarray<bool> eltMasq;
    vector<size_t> sommets;
    std::map<size_t, size_t> num;  //<label, associated number from 0>
    std::vector<size_t> matType;

    scaletag::fem::MeshData meshData;


	public:
		void marcInitialize(const string& marcFile);
		void abaqusInitialize(const string& abaqusFile);
		void ansysInitialize(const string& ansysFile);
        void gmshInitialize(const scaletag::fem::MeshData& _meshData);

		vector<Node<T,D>>& getNodes(){return nodes;}
        const vector<Node<T,D>>& getNodes()const{return nodes;}
		vector<Element>& getElements(){return elements;}
        const vector<Element>& getElements() const{return elements;}
        size_t getNumberOfNodes() const{return /*numberOfNodes*/nodes.size();}
        size_t getNumberOfElements() const{return elements.size();}

        const Element& operator[](size_t e)const{return elements[e];}
        void reorder(const std::vector<size_t>& _sommets);
        void order();
        bool setReferenceDofsOK(bool ok=false);
        bool getReferenceDofsOK();
        void addNode(const size_t i, T x, T y, T z){ if(referenceDofsOK==true) return;    getNodes().push_back(Node<T,D>(i,x,y,z)); ++numberOfNodes; num.insert({i,numberOfNodes-1});}
        void setSommets(const std::vector<size_t>& _sommets){sommets.clear(); sommets=_sommets;}
        size_t getNum(size_t _lab) const {return num.at(_lab);}//gives the numerotation from 0

        std::vector<size_t> getConnectivity(size_t _e) const; // returns nodes lists renumbered from 0



        size_t getMatType(size_t e) const {
            /*
             *  types : HooKe/Linear Elasticity, Not defined, user defined
             *  //Vector of material object must be furnished this function indexes in the vector
            */

            return matType[e];
        }

        void assignMat(const size_t& matTypeNum, const std::vector<size_t>& ve={} ){

            size_t ne = getNumberOfElements();
            matType.resize(ne, 0);

            if(ve.empty()) matType.assign(ne, matTypeNum);// Assign matTypeNum to the whole set of elements
            if(!ve.empty()) for(size_t mt=0; mt<ve.size(); mt++) matType[ ve[mt] ] = matTypeNum;

}


scaletag::fem::MeshData getMeshData() const {return meshData;}
void setMeshData(const scaletag::fem::MeshData& _meshData) {meshData = _meshData;}

};


template<typename T, size_t D> void MeshPbc<T,D>::gmshInitialize(const scaletag::fem::MeshData& _meshData){


nodes.clear();
elements.clear();

d=3;
numberOfNodes = _meshData.getNodeTags().size();
nodes.reserve(numberOfNodes);

for(size_t i=0; i<numberOfNodes; i++){

            T x[D];
            size_t numNode = _meshData.getNodeTags()[i];
            for(size_t kn=0; kn<D; kn++) x[kn] = _meshData.getNodesCoord()[D*i+kn];
            Node<T,D> node(numNode, x[0], x[1], x[2]);
            //node.nodeNumber = _meshData.getNodeTags()[i];
            //for(size_t kn=0; kn<D; kn++) node.x[kn] = _meshData.getNodesCoord()[D*i+kn];
            nodes.push_back(node);
}

//size_t nelts = _meshData.getElementTags().size();

for( size_t elmentTypeSize = 0; elmentTypeSize<_meshData.getElementTypes().size(); elmentTypeSize++ ){
            size_t gEltType = _meshData.getElementTypes()[elmentTypeSize];
            size_t eSize = _meshData.getElementSizes()[elmentTypeSize];
            size_t nEType = _meshData.getElementTags()[elmentTypeSize].size();

            for(size_t e=0; e<nEType; e++){
                std::vector<size_t> eConnectivity(eSize);
                for (size_t i= 0; i < eSize; i++) {
                    eConnectivity[i] = _meshData.getConnectivity()[elmentTypeSize][eSize*e+i];
                }

                Element element( scaletag::fem::eltGmshToScaletag(gEltType), eSize, eConnectivity);
                elements.push_back(element);

            }
}
order();
meshData = _meshData;
}

template<typename T, size_t D> void MeshPbc<T,D>::marcInitialize(const string& marcFile){
int skipdata; string line="";
string WorkFile = createMarcWorkFile(marcFile);  //return x, y, z marc parsed file
ifstream entree(WorkFile, ios::in|ios::binary);

nodes.clear();
elements.clear();

{
            //This section is for reading nodes
            Node<T,D> node; int nnode;//=numberOfNodes;
            getline(entree, line, '\n');
            entree>>d>>numberOfNodes>>skipdata>>skipdata;
            nnode=numberOfNodes;
            //nodes.resize(numberOfNodes);
            for (int i=0; i<nnode; i++){entree>>node;nodes.push_back(node);}  //Finish reading nodes
            //}while(!entree.eof());
}

{
            //This section is for reading elements
            string ligne; char c='\0'; Element e;
            ifstream entree2(marcFile, ios::in|ios::binary);
            do{
                getline(entree2, ligne, '\n');
                if(ligne.find("connectivity") != string::npos){
                    getline(entree2, ligne, '\n');
                    do{
                        entree2>>std::ws;
                        c=entree2.peek();
                        if(isdigit(c)){e.readMarcElement(entree2); elements.push_back(e);}
                    }while(isdigit(c));
                }
            }while(!entree2.eof());
}
//reorder(sommets);
//setSommets(sommets);
order();
}

template<typename T, size_t D> void MeshPbc<T,D>::abaqusInitialize(const string& abaqusFile){}

template<typename T, size_t D> void MeshPbc<T,D>::ansysInitialize(const string& ansysFile){}

template<typename T, size_t D> void MeshPbc<T,D>::reorder(const std::vector<size_t>& _sommets){

/*
 * As the appearing order of nodes is not generally the one used in the element connectivity definition,
 * we have to make a difference between the numbering used in connectivity and the one defined by the
 * appearing order
 *
 * This function gives another numbering from 0 of nodes needed in the computation.
 * eltMasq defines nodes numbering needed in the computation
 * renum stores the numbering
*/
size_t nnode=getNumberOfNodes();
size_t ne=getNumberOfElements();
std::vector<size_t> tempOrder;

renum.clear();
renum.resize(getNumberOfNodes());
eltMasq.resize(getNumberOfNodes(),0);
setSommets(_sommets);

for(size_t e=0; e<ne; e++){
    size_t nv=(*this)[e].getElementSize();
    for(size_t i=0; i<nv; i++){ tempOrder.push_back( (*this)[e][i] ); }
}

for(size_t nv=0; nv<sommets.size(); nv++) tempOrder.push_back(sommets[nv]);

std::sort(tempOrder.begin(), tempOrder.end());
std::vector<size_t>::iterator it = std::unique(tempOrder.begin(), tempOrder.end());
tempOrder.erase( it, tempOrder.end() );

size_t eltNum=0, nonEltNum=0;
for(size_t nn=0; nn<nnode; nn++){
    size_t nodeNumber= nodes[nn].getNodeNumber();
    std::vector<size_t>::iterator it1 = std::find(tempOrder.begin(), tempOrder.end(), nodeNumber);
//    std::vector<size_t>::const_iterator   it2 = std::find(sommets.begin(), sommets.end(), nn);
    if(it1!=tempOrder.end() /*|| it2!=sommets.end()*/){renum[nn]=eltNum++; eltMasq[nn]=1;}
    else{renum[nn]=nonEltNum++;}
   }

}


template<typename T, size_t D>
void MeshPbc<T,D>::order(){
//This function fills num : map<size_t, size_t>; key is node number in connectivity
//and value is the appearing order in  nodes : vector<Node<T,D> > nodes;
   num.clear();
   for(size_t i=0; i<nodes.size(); i++) /*num[nodes[i].getNodeNumber()]=i;*/
   {Node<T,D> nod=nodes[i]; num[nod.getNodeNumber()]=i;}
}
template<typename T, size_t D>
bool MeshPbc<T,D>::setReferenceDofsOK(bool ok){referenceDofsOK = ok;}

template<typename T, size_t D>
bool MeshPbc<T,D>::getReferenceDofsOK(){return referenceDofsOK ;}

//template<typename T, size_t D>
//T measElt(const MeshPbc<T,D>& Mh, size_t e){

//   size_t eltType = Mh[e].getElementType();

//   if(eltType==4 || eltType==50){
//    //4->tetra4, 50->Herman5

//    Node<T,D> node1 = Mh.getNodes()[(Mh.getConnectivity(e))[0]];
//    Node<T,D> node2 = Mh.getNodes()[(Mh.getConnectivity(e))[1]];
//    Node<T,D> node3 = Mh.getNodes()[(Mh.getConnectivity(e))[2]];
//    Node<T,D> node4;
//    if(eltType==4) node4 = Mh.getNodes()[(Mh.getConnectivity(e))[3]]; else node4 = Mh.getNodes()[(Mh.getConnectivity(e))[4]];
//    return std::fabs( 1/6.*(dot( (node2-node1)^(node3-node1), (node4-node1) )) );
//   }
//   assert(0); return 0;
//}



template<typename T, size_t D>
std::vector<size_t> MeshPbc<T,D>::getConnectivity(size_t _e) const{
   //This function returns list of nodes that constitutes _e but renumbered from 0;
   //This eases renumbering of dofs later in fem computation
   vector<size_t> res;
   Element e=(*this)[_e];
   size_t eltSize=e.getElementSize();
   res.resize(eltSize);
   for(size_t i=0; i<eltSize; i++) res[i]= num.at(e[i]);
   return res;
}

template<typename T, size_t D>
T measElt(const MeshPbc<T,D>& Mh, size_t e){

    size_t eltType = Mh[e].getElementType();

    if(eltType==4 || eltType==50){
        //4->tetra4, 50->Herman5

    Node<T,D> node1 = Mh.getNodes()[(Mh.getConnectivity(e))[0]];
    Node<T,D> node2 = Mh.getNodes()[(Mh.getConnectivity(e))[1]];
    Node<T,D> node3 = Mh.getNodes()[(Mh.getConnectivity(e))[2]];
    Node<T,D> node4;
    if(eltType==4) node4 = Mh.getNodes()[(Mh.getConnectivity(e))[3]]; else node4 = Mh.getNodes()[(Mh.getConnectivity(e))[4]];
    return std::fabs( 1/6.*(dot( (node2-node1)^(node3-node1), (node4-node1) )) );
    }
    assert(0); return 0;
}

template<typename T, size_t D>
T measEltD(const MeshPbc<T,D>& Mh, size_t e, size_t ndof, const std::valarray<T>& disp){

    size_t eltType = Mh[e].getElementType();

    if(eltType==4 || eltType==50){
    //4->tetra4, 50->Herman5

    Node<T,D> node1 = Mh.getNodes()[(Mh.getConnectivity(e))[0]];
    Node<T,D> node2 = Mh.getNodes()[(Mh.getConnectivity(e))[1]];
    Node<T,D> node3 = Mh.getNodes()[(Mh.getConnectivity(e))[2]];
    Node<T,D> node4;

    if(eltType==4) node4 = Mh.getNodes()[(Mh.getConnectivity(e))[3]]; else node4 = Mh.getNodes()[(Mh.getConnectivity(e))[4]];

    T xd1[D]; T xd2[D]; T xd3[D]; T xd4[3];

    for(size_t k=0; k<ndof; k++) {
    xd1[k]=node1[k];  xd2[k]=node2[k];  xd3[k]=node3[k];  xd4[k]=node4[k];
    size_t numdof1 = ndof*Mh.getConnectivity(e)[0]+k;
    size_t numdof2 = ndof*Mh.getConnectivity(e)[1]+k;
    size_t numdof3 = ndof*Mh.getConnectivity(e)[2]+k;
    size_t numdof4 = ndof*Mh.getConnectivity(e)[3]+k;
    xd1[k] += disp[numdof1];
    xd2[k] += disp[numdof2];
    xd3[k] += disp[numdof3];
    xd4[k] += disp[numdof4];
    }
    Node<T,D> node1d = Node<T,D>(xd1);
    Node<T,D> node2d = Node<T,D>(xd2);
    Node<T,D> node3d = Node<T,D>(xd3);
    Node<T,D> node4d = Node<T,D>(xd4);
    return std::fabs( 1/6.*(dot( (node2d-node1d)^(node3d-node1d), (node4d-node1d) )) );
    }
}

template<typename T, size_t D>
T meas(const MeshPbc<T,D>& Mh){

    T res = 0;
    size_t nelt=Mh.getNumberOfElements();
    for(size_t e=0; e<nelt; e++){
    res += measElt(Mh,e);
    }
    return res;
}

template<typename T, size_t D>
T meas(const MeshPbc<T,D>& Mh, size_t ndof, const std::valarray<double>& disp){

    T res = 0;
    size_t nelt=Mh.getNumberOfElements();
    for(size_t e=0; e<nelt; e++){
    res += measEltD(Mh,e, ndof, disp);
    }
    return res;

}


#endif
