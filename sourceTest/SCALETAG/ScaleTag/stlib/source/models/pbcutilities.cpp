#include "pbcutilities.h"

//using namespace std;
namespace scaletag {
namespace models {

PbcUtilities::PbcUtilities(QObject *parent)
    : QObject{parent}/*, pbcEqs(), sommets()*/
{

}
void PbcUtilities::structBox(const MeshPbc<double,3>& meshPbc, double& Xmin, double& Xmax, double& Ymin, double& Ymax, double& Zmin, double& Zmax){
// This function computes the bounding box of the mesh

    size_t nPts=meshPbc.getNumberOfNodes();
    vector<double> X, Y, Z;
    for (size_t i=0; i<nPts; i++){X.push_back(meshPbc.getNodes()[i].getXi(0));}
    for (size_t i=0; i<nPts; i++){Y.push_back(meshPbc.getNodes()[i].getXi(1));}
    for (size_t i=0; i<nPts; i++){Z.push_back(meshPbc.getNodes()[i].getXi(2));}

    sort(X.begin(), X.end());
    sort(Y.begin(), Y.end());
    sort(Z.begin(), Z.end());

    Xmin=X[0]; Xmax=X[X.size()-1]; Ymin=Y[0]; Ymax=Y[Y.size()-1]; Zmin=Z[0]; Zmax=Z[Z.size()-1];
}

void PbcUtilities::nodesSelect(const MeshPbc<double,3>& meshPbc, /*string box,*/ bitset<6>bs, double epsi, vector<size_t>& Output) {

    //This function identifies nodes that match bitset<6>bs
    /*
    box="Xmin Xmax Ymin Ymax Zmin Zmax"
    */
    //Output is the list of nodes that match bs

    double Xmin, Xmax, Ymin, Ymax, Zmin, Zmax;
    structBox(meshPbc, Xmin, Xmax, Ymin, Ymax, Zmin, Zmax);
    size_t nPts=meshPbc.getNumberOfNodes();

    string bPtString, bsString;

    for(size_t i=0; i<nPts; i++){
    bool bxmin, bxmax, bymin, bymax, bzmin, bzmax;

    bxmin=(fabs(meshPbc.getNodes()[i].getXi(0)-Xmin)<=epsi);
    bxmax=(fabs(meshPbc.getNodes()[i].getXi(0)-Xmax)<=epsi);
    bymin=(fabs(meshPbc.getNodes()[i].getXi(1)-Ymin)<=epsi);
    bymax=(fabs(meshPbc.getNodes()[i].getXi(1)-Ymax)<=epsi);
    bzmin=(fabs(meshPbc.getNodes()[i].getXi(2)-Zmin)<=epsi);
    bzmax=(fabs(meshPbc.getNodes()[i].getXi(2)-Zmax)<=epsi);

    bPtString=to_string(bxmin)+to_string(bxmax)+to_string(bymin)+to_string(bymax)+to_string(bzmin)+to_string(bzmax);
    bsString=bs.to_string();

    if(bPtString==bsString) Output.push_back(i);
}
}

void PbcUtilities::edgesLinks(const MeshPbc<double,3>& meshPbc, const vector<size_t>& Sommets, int ind, bool trans, const vector<size_t>& v1, const vector<size_t>& v2,
                              double epsi,std::vector<pbcEquation>& sortie) {
//Given two node lists on edges this function matches nodes two by two

// The parameter ind traduces whether the edges are oriented in x direction(ind=0), y direction(ind=1) or in z direction(ind=2)
// The function assumes that vectA, vectB, vectC, vectD, vectAp, vectBp, etc ... have been prior declared

size_t tn, rt1, rt2;
    size_t rt3; /*bool trans=0;*///for transverse correspondances
    size_t sizev1=v1.size(), sizev2=v2.size();  //v1 for tied and v2 for retained

    assert(sizev1==sizev2);
    assert(ind==0 || ind==1 || ind==2);
    double coordAv1, coordBv1, coordAv2, coordBv2;
    double condA, condB;
    sortie.clear();
    for (size_t i=0; i<sizev1; i++){
        for (size_t j=0; j<sizev1; j++){
            {
                if(ind==0){
                coordAv1=meshPbc.getNodes()[ v1[i] ].getXi(0);
                coordAv2= meshPbc.getNodes()[ v2[j] ].getXi(0);
                coordBv1= meshPbc.getNodes()[ v1[i]  ].getXi(2);
                coordBv2= meshPbc.getNodes()[ v2[j]  ].getXi(2);}
                else if(ind==1){
                coordAv1=meshPbc.getNodes()[ v1[i] ].getXi(1);
                coordAv2=meshPbc.getNodes()[ v2[j] ].getXi(1);
                coordBv1=meshPbc.getNodes()[ v1[i] ].getXi(2);
                coordBv2=meshPbc.getNodes()[ v2[j] ].getXi(2);}
                else if(ind==2){
                coordAv1=meshPbc.getNodes()[ v1[i] ].getXi(2);
                coordAv2=meshPbc.getNodes()[ v2[j] ].getXi(2);

                coordBv1=meshPbc.getNodes()[ v1[i] ].getXi(1);
                coordBv2=meshPbc.getNodes()[ v2[j] ].getXi(1);}

                condA=std::fabs(coordAv1-coordAv2);

            }
            if(condA<=epsi){
            tn=v1[i];
            rt1=v2[j];
            condB=std::fabs(coordBv1-coordBv2);
                if(condB<=epsi){
                    if(ind==0) {rt2=Sommets[3];/*vectD[0] ;*/}
                    else if (ind==1 || ind==2){rt2=Sommets[1];/*vectB[0] ;*/}//else if(ind==2){rt2=Sommets[1];/* vectB[0];*/}
                }
                else
                {
                if(ind==0) {rt2=Sommets[4];/*vectAp[0] ;*/rt3=Sommets[3];/*vectD[0] ;*/}
                else if (ind==1){rt2=Sommets[4];/*vectAp[0] ;*/rt3=Sommets[1];/*vectB[0] ;*/}
                else if(ind==2){rt2=Sommets[3];/*vectD[0] ;*/rt3=Sommets[1];/*vectB[0] ;*/}
                //rt3 will be used only if trans==1
                }
                size_t rtA = Sommets[0];
                vector<size_t> ptIds; vector<double> coeffs;
                if(trans==0) {ptIds.push_back(tn);   ptIds.push_back(rt1); ptIds.push_back(rt2); ptIds.push_back(rtA);
                coeffs.push_back(1);  coeffs.push_back(+1); coeffs.push_back(-1);}
                if(trans==1) {ptIds.push_back(tn); ptIds.push_back(rt1); ptIds.push_back(rt2); ptIds.push_back(rt3); ptIds.push_back(rtA);
                coeffs.push_back(+1);  coeffs.push_back(+1); coeffs.push_back(+1); coeffs.push_back(-2);}
                sortie.push_back( pbcEquation(coeffs.size(), ptIds,coeffs) );
                break;
            }

            }
//    size_t rtA = Sommets[0];
//    vector<size_t> ptIds; vector<double> coeffs;
//    if(trans==0) {ptIds.push_back(tn);   ptIds.push_back(rt1); ptIds.push_back(rt2); ptIds.push_back(rtA);
//                                         coeffs.push_back(1);  coeffs.push_back(+1); coeffs.push_back(-1);}
//    if(trans==1) {ptIds.push_back(tn); ptIds.push_back(rt1); ptIds.push_back(rt2); ptIds.push_back(rt3); ptIds.push_back(rtA);
//                                        coeffs.push_back(+1);  coeffs.push_back(+1); coeffs.push_back(+1); coeffs.push_back(-2);}
//    sortie.push_back( pbcEquation(coeffs.size(), ptIds,coeffs) );
    }

}
void PbcUtilities::facesLinks(const MeshPbc<double,3>& meshPbc, const vector<size_t>& Sommets, int ind, const vector<size_t>& v1, const vector<size_t>& v2,
                              double epsi, vector<pbcEquation>& sortie){

    //Given two node lists on faces this function matches nodes two by two

    //The parameter ind indicates wether the faces normals are oriented in te x  (ind=0), y(ind=1), z(ind=2) direction

   // array<size_t,3>PointEqui;
    size_t tn, rt1, rt2;
    size_t sizev1=v1.size(), sizev2=v2.size();

    assert(sizev1==sizev2);
    assert(ind==0 || ind==1 || ind==2);
    double coordAv1, coordBv1, coordAv2, coordBv2;
    double condA, condB;
    sortie.clear();

    for (size_t i=0; i<sizev1; i++){
        for (size_t j=0; j<sizev1; j++){
                if(ind==0){
                coordAv1=meshPbc.getNodes()[ v1[i] ].getXi(1);
                coordAv2=meshPbc.getNodes()[ v2[j] ].getXi(1);
                coordBv1=meshPbc.getNodes()[ v1[i] ].getXi(2);
                coordBv2=meshPbc.getNodes()[ v2[j] ].getXi(2);}
                else if(ind==1){
                coordAv1=meshPbc.getNodes()[ v1[i] ].getXi(0);
                coordAv2=meshPbc.getNodes()[ v2[j] ].getXi(0);
                coordBv1=meshPbc.getNodes()[ v1[i] ].getXi(2);
                coordBv2=meshPbc.getNodes()[ v2[j] ].getXi(2);}
                else if(ind==2){
                coordAv1=meshPbc.getNodes()[ v1[i] ].getXi(0);
                coordAv2=meshPbc.getNodes()[ v2[j] ].getXi(0);
                coordBv1=meshPbc.getNodes()[ v1[i] ].getXi(1);
                coordBv2=meshPbc.getNodes()[ v2[j] ].getXi(1);}


                condA=std::fabs(coordAv1-coordAv2);
                condB=std::fabs(coordBv1-coordBv2);
            if(condA<=epsi && condB<=epsi){
            tn=v1[i];
            rt1=v2[j];
            if(ind==0){rt2=Sommets[1];} else if(ind==1){rt2=Sommets[3];} else if(ind==2){rt2=Sommets[4];}

            size_t rtA= Sommets[0];
            vector<size_t> ptIds; vector<double> coeffs;
            ptIds.push_back(tn);   ptIds.push_back(rt1); ptIds.push_back(rt2); ptIds.push_back(rtA);
            coeffs.push_back(1); coeffs.push_back(1); coeffs.push_back(-1);
            sortie.push_back( pbcEquation(coeffs.size(), ptIds, coeffs) );
            break;

            }
        }
//        size_t rtA= Sommets[0];
//        vector<size_t> ptIds; vector<double> coeffs;
//        ptIds.push_back(tn);   ptIds.push_back(rt1); ptIds.push_back(rt2); ptIds.push_back(rtA);
//        coeffs.push_back(1); coeffs.push_back(1); coeffs.push_back(-1);
//        sortie.push_back( pbcEquation(coeffs.size(), ptIds,coeffs) );
    }

}

void PbcUtilities::createPbcLinks(MeshPbc<double, 3>& meshPbc, std::vector<pbcEquation>& pbcLinks, size_t& NbLinks, std::vector<size_t>& sommets
                                  , std::vector<size_t>& notMeshNodes){

    /*Numbering of nodes used is the one provided by the order of acquisition but not the one used in connectivity
    ie the natural ordering in MeshPbc<T, D>::nodes attribute*/

    //double epsi=0.0;
    //double epsi=1.0e-80;
    //double epsi=1.0e-14;
    //NbLinks --> Number of dofs constraints

    double epsi = std::numeric_limits<double>::epsilon()*10.;

    vector<size_t> vectA, vectB, vectC, vectD, vectAp, vectBp, vectCp, vectDp;
    bitset<6> bA("101010"), bB("011010"), bC("010110"), bD("100110"), bAp("101001"), bBp("011001"), bCp("010101"), bDp("100101");

    pbcLinks.clear();
    //Definition of corners vertices indices
    double Xmin, Xmax, Ymin, Ymax, Zmin, Zmax;
    structBox(meshPbc, Xmin, Xmax, Ymin, Ymax, Zmin, Zmax);
    size_t vexist=0;
    nodesSelect(meshPbc, bA, epsi, vectA);
    nodesSelect(meshPbc, bB, epsi, vectB);
    nodesSelect(meshPbc, bC, epsi, vectC);
    nodesSelect(meshPbc, bD, epsi, vectD);
    nodesSelect(meshPbc, bAp, epsi, vectAp);
    nodesSelect(meshPbc, bBp, epsi, vectBp);
    nodesSelect(meshPbc, bCp, epsi, vectCp);
    nodesSelect(meshPbc, bDp, epsi, vectDp);

    if(vectA.size()==1) ; else {vectA.push_back(meshPbc.getNumberOfNodes());  vexist++; meshPbc.addNode(vectA[0],Xmin, Ymin, Zmin); }
    if(vectB.size()==1) ; else {vectB.push_back(meshPbc.getNumberOfNodes());  vexist++; meshPbc.addNode(vectB[0],Xmax, Ymin, Zmin);}
    if(vectC.size()==1) ; else {vectC.push_back(meshPbc.getNumberOfNodes()); vexist++; meshPbc.addNode(vectC[0],Xmax, Ymax, Zmin);}
    if(vectD.size()==1) ; else {vectD.push_back(meshPbc.getNumberOfNodes()) ; vexist++; meshPbc.addNode(vectD[0],Xmin, Ymax, Zmin);}
    if(vectAp.size()==1) ; else {vectAp.push_back(meshPbc.getNumberOfNodes());  vexist++; meshPbc.addNode(vectAp[0],Xmin, Ymin, Zmax);}
    if(vectBp.size()==1) ; else {vectBp.push_back(meshPbc.getNumberOfNodes()); vexist++; meshPbc.addNode(vectBp[0],Xmax, Ymin, Zmax);}
    if(vectCp.size()==1) ; else {vectCp.push_back(meshPbc.getNumberOfNodes()) ; vexist++; meshPbc.addNode(vectCp[0],Xmax, Ymax, Zmax);}
    if(vectDp.size()==1) ; else {vectDp.push_back(meshPbc.getNumberOfNodes()); vexist++;  meshPbc.addNode(vectDp[0],Xmin, Ymax, Zmax);}

    notMeshNodes.resize(vexist);
    size_t mNodes=meshPbc.getNumberOfNodes()-vexist;
    for(size_t i=0; i<vexist; i++){notMeshNodes[i]=mNodes+i;
    }

    sommets.push_back(vectA[0]); sommets.push_back(vectB[0]); sommets.push_back(vectC[0]); sommets.push_back(vectD[0]);
    sommets.push_back(vectAp[0]); sommets.push_back(vectBp[0]); sommets.push_back(vectCp[0]); sommets.push_back(vectDp[0]);


/* Selection of edge nodes. From the definition of nodesSelect, corners vertices are not selected
 corners vertices cannot satisfy bitset<6> bYmaxZmin("000110"), bYminZmin("001010"), bYmaxZmax("000101"),
bYminZmax("001001"), bXmaxZmin("010010"), bXminZmin("100010"), bXmaxZmax("010001"), bXminZmax("100001"),
bXmaxYmin("011000"), bXminYmin("101000"), bXmaxYmax("010100"), bXminYmax("100100");*/

    vector<size_t> YmaxZmin, YminZmin, YmaxZmax, YminZmax, XmaxZmin, XminZmin, XmaxZmax, XminZmax, XmaxYmin, XminYmin, XmaxYmax, XminYmax;
    bitset<6>bYmaxZmin("000110"), bYminZmin("001010"), bYmaxZmax("000101"), bYminZmax("001001"), bXmaxZmin("010010"), bXminZmin("100010"), bXmaxZmax("010001"), bXminZmax("100001"), bXmaxYmin("011000"), bXminYmin("101000"), bXmaxYmax("010100"), bXminYmax("100100");

    //Rotation autour de x
    nodesSelect(meshPbc, bYmaxZmin, epsi, YmaxZmin);
    nodesSelect(meshPbc, bYminZmin, epsi, YminZmin);
    nodesSelect(meshPbc, bYmaxZmax, epsi, YmaxZmax);
    nodesSelect(meshPbc, bYminZmax, epsi, YminZmax);

    //Rotation autour de y
    nodesSelect(meshPbc, bXmaxZmin, epsi, XmaxZmin);
    nodesSelect(meshPbc, bXminZmin, epsi, XminZmin);
    nodesSelect(meshPbc, bXmaxZmax, epsi, XmaxZmax);
    nodesSelect(meshPbc, bXminZmax, epsi, XminZmax);

    //Rotation autour de z
    nodesSelect(meshPbc, bXmaxYmin, epsi, XmaxYmin);
    nodesSelect(meshPbc, bXminYmin, epsi, XminYmin);
    nodesSelect(meshPbc, bXmaxYmax, epsi, XmaxYmax);
    nodesSelect(meshPbc, bXminYmax, epsi, XminYmax);

    //Selection of face nodes. From the definition of nodesSelect, corners vertices are not selected
    // As the corner vertices cannot satisfy bitset<6> bXminF("100000"), bXmaxF("010000"), bYminF("001000"),
    //bYmaxF("000100"), bZminF("000010"), bZmaxF("000001");

    vector<size_t> XminF, XmaxF, YminF, YmaxF, ZminF, ZmaxF;
    bitset<6> bXminF("100000"), bXmaxF("010000"), bYminF("001000"), bYmaxF("000100"), bZminF("000010"), bZmaxF("000001");

    nodesSelect(meshPbc, bXminF, epsi, XminF);
    nodesSelect(meshPbc, bXmaxF, epsi, XmaxF);
    nodesSelect(meshPbc, bYminF, epsi, YminF);
    nodesSelect(meshPbc, bYmaxF, epsi, YmaxF);
    nodesSelect(meshPbc, bZminF, epsi, ZminF);
    nodesSelect(meshPbc, bZmaxF, epsi, ZmaxF);

    //Partial verification of mesh symetry
    if( !(XminF.size()==XmaxF.size() && YminF.size()==YmaxF.size() && ZminF.size()==ZmaxF.size() && XminZmin.size()==YminZmin.size() && YmaxZmax.size()==YminZmax.size() && XmaxZmin.size()==XminZmin.size() && XmaxZmax.size()==XminZmax.size() && XmaxYmin.size()==XminYmin.size() && XmaxYmax.size()==XminYmax.size()) )
{qDebug()<<"The mesh is not symmetric"; abort();}

//Generation of pbcLinks
array<size_t,4> cornerLink1, cornerLink2, cornerLink4;
array<size_t,5> cornerLink3;
//cornerLink1={vectC[0], vectD[0], vectB[0]};
//cornerLink2={vectCp[0], vectDp[0], vectB[0]};
//cornerLink3={vectBp[0], vectAp[0], vectB[0]};
//cornerLink4={vectDp[0], vectAp[0], vectD[0]};

cornerLink1={vectC[0],  vectB[0], vectD[0], vectA[0]}; //+1 +1 -1
cornerLink2={vectBp[0], vectB[0], vectAp[0], vectA[0]}; //+1  +1 -1
cornerLink3={vectCp[0], vectB[0], vectD[0], vectAp[0], vectA[0]}; //+1  +1 +1 -2
cornerLink4={vectDp[0], vectD[0], vectAp[0], vectA[0]}; //+1 +1 -1


vector<size_t> ptIds1{cornerLink1.begin(),cornerLink1.end()},
    ptIds2{cornerLink2.begin(), cornerLink2.end()}, ptIds3{cornerLink3.begin(), cornerLink3.end()},
    ptIds4{cornerLink4.begin(), cornerLink4.end()};
vector<double> coeffs1{1., 1., -1.}, coeffs2{1., 1., -1}, coeffs3{1., 1., 1., -2.}, coeffs4{1., 1., -1};

std::vector<pbcEquation> cornersLinks;
cornersLinks.push_back( pbcEquation(coeffs1.size(), ptIds1, coeffs1) );
cornersLinks.push_back( pbcEquation(coeffs2.size(), ptIds2, coeffs2) );
cornersLinks.push_back( pbcEquation(coeffs3.size(), ptIds3, coeffs3) );
cornersLinks.push_back( pbcEquation(coeffs4.size(), ptIds4, coeffs4) );

vector<pbcEquation> edgesLinks1, edgesLinks2, edgesLinks3, edgesLinks4, edgesLinks5, edgesLinks6, edgesLinks7, edgesLinks8, edgesLinks9;
vector<size_t> cornersNodesList; //cornersNodesList contains corners nodes numerotation in the order A, B, C, D, Ap, Bp, Cp, Dp
cornersNodesList.push_back(vectA[0]); cornersNodesList.push_back(vectB[0]); cornersNodesList.push_back(vectC[0]); cornersNodesList.push_back(vectD[0]);
cornersNodesList.push_back(vectAp[0]); cornersNodesList.push_back(vectBp[0]); cornersNodesList.push_back(vectCp[0]); cornersNodesList.push_back(vectDp[0]);

//Pour les deux premieres lignes, tied-rt1 =rt2; numerotation <tn, rt1, rt2>
//pour la dernière ligne,

//rotation autour de x
edgesLinks(meshPbc, cornersNodesList, 0, 0, YmaxZmax, YminZmax, epsi, edgesLinks1); //<tn, rt1, rt2, A>  //+1  +1 -1
edgesLinks(meshPbc, cornersNodesList, 0, 1, YmaxZmax, YminZmin, epsi, edgesLinks2); //<tn, rt1, rt2, rt3, A>  // +1 +1 +1 -2
edgesLinks(meshPbc, cornersNodesList, 0,0, YmaxZmax, YmaxZmin, epsi, edgesLinks3); //<tn, rt1, rt2, A>  //+1  +1 -1

//rotation autour de y
edgesLinks(meshPbc, cornersNodesList, 1,0, XmaxZmax, XminZmax, epsi, edgesLinks4); //   <tn, rt1, rt2, A>  //+1  +1 -1
edgesLinks(meshPbc, cornersNodesList, 1,1, XmaxZmax, XminZmin, epsi, edgesLinks5); //   <tn, rt1, rt2, rt3, A>  // +1 +1 +1 -2
edgesLinks(meshPbc, cornersNodesList, 1,0, XmaxZmax, XmaxZmin, epsi, edgesLinks6); //   <tn, rt1, rt2, A>  //+1  +1 -1

//rotation autour de z
edgesLinks(meshPbc, cornersNodesList, 2,0, XmaxYmax, XminYmax, epsi, edgesLinks7); //   <tn, rt1, rt2, A>  //+1  +1 -1
edgesLinks(meshPbc, cornersNodesList, 2,1, XmaxYmax, XminYmin,  epsi, edgesLinks8); //   <tn, rt1, rt2, rt3, A>  // +1 +1 +1 -2
edgesLinks(meshPbc, cornersNodesList, 2,0, XmaxYmax, XmaxYmin, epsi, edgesLinks9); //   <tn, rt1, rt2, A>  //+1  +1 -1

vector<pbcEquation> facesLinksX, facesLinksY, facesLinksZ;

facesLinks(meshPbc, cornersNodesList, 0, XmaxF, XminF, epsi, facesLinksX); //tn-rt1=rt2 <tn, rt1, rt2>  //+1  +1 -1
facesLinks(meshPbc, cornersNodesList, 1, YmaxF, YminF, epsi, facesLinksY); //tn-rt1=rt2 <tn, rt1, rt2>  //+1  +1 -1
facesLinks(meshPbc, cornersNodesList, 2, ZmaxF, ZminF, epsi, facesLinksZ); //tn-rt1=rt2 <tn, rt1, rt2>  //+1  +1 -1

//vector<array<size_t,3> > pbcLinks(cornersLinks);


NbLinks=3*( 4+3*( YminZmin.size()+XminZmin.size()+XminYmin.size() )+ XminF.size()+YminF.size()+ZminF.size() );
pbcLinks.reserve(NbLinks);

pbcLinks.insert(pbcLinks.end(), cornersLinks.begin(), cornersLinks.end()  );

pbcLinks.insert(pbcLinks.end(),  edgesLinks1.begin(), edgesLinks1.end() );
pbcLinks.insert(pbcLinks.end(),  edgesLinks2.begin(), edgesLinks2.end() );
pbcLinks.insert(pbcLinks.end(),  edgesLinks3.begin(), edgesLinks3.end() );
pbcLinks.insert(pbcLinks.end(),  edgesLinks4.begin(), edgesLinks4.end() );
pbcLinks.insert(pbcLinks.end(),  edgesLinks5.begin(), edgesLinks5.end() );
pbcLinks.insert(pbcLinks.end(),  edgesLinks6.begin(), edgesLinks6.end() );
pbcLinks.insert(pbcLinks.end(),  edgesLinks7.begin(), edgesLinks7.end() );
pbcLinks.insert(pbcLinks.end(),  edgesLinks8.begin(), edgesLinks8.end() );
pbcLinks.insert(pbcLinks.end(),  edgesLinks9.begin(), edgesLinks9.end() );

pbcLinks.insert(pbcLinks.end(),  facesLinksX.begin(), facesLinksX.end() );
pbcLinks.insert(pbcLinks.end(),  facesLinksY.begin(), facesLinksY.end() );
pbcLinks.insert(pbcLinks.end(),  facesLinksZ.begin(), facesLinksZ.end() );

//for (const auto& p : cornersLinks) pbcLinks.push_back(p);
//for (const auto& p: edgesLinks1) pbcLinks.push_back(p);
//for (const auto& p : edgesLinks2) pbcLinks.push_back(p);
//for (const auto& p : edgesLinks3) pbcLinks.push_back(p);
//for (const auto& p : edgesLinks4) pbcLinks.push_back(p);
//for (const auto& p: edgesLinks5) pbcLinks.push_back(p);
//for (const auto& p : edgesLinks6) pbcLinks.push_back(p);
//for (const auto& p : edgesLinks7) pbcLinks.push_back(p);
//for (const auto& p : edgesLinks8) pbcLinks.push_back(p);
//for (const auto& p : edgesLinks9) pbcLinks.push_back(p);
//for (const auto& p : facesLinksX) pbcLinks.push_back(p);
//for (const auto& p : facesLinksY) pbcLinks.push_back(p);
//for (const auto& p : facesLinksZ) pbcLinks.push_back(p);


}


void PbcUtilities::createPbcLinks(MeshPbc<double,3>& meshPbc, size_t& NbLinks, vector<size_t>& sommets, std::vector<size_t>& notMeshNodes,
        std::vector<pbcEquation>& pbcLinks) {

//NbLinks --> Number of dofs constraints
//tilingDirs.reserve(9);
auto meshData = meshPbc.getMeshData();
std::vector<std::vector<double>> mappingDirs = meshData.getMappingDirs();
double tx, ty, tz; // Components of a mapping direction.

double xA, yA, zA,  V1x, V1y, V1z,    V2x, V2y, V2z,   V3x, V3y, V3z;
//We suppose here that the first 3 surfaces picked correspond to the tiling directions
V1x = mappingDirs[0][0]; V1y = mappingDirs[0][1]; V1z = mappingDirs[0][2];
V2x = mappingDirs[1][0]; V2y = mappingDirs[1][1]; V2z = mappingDirs[1][2];
V3x = mappingDirs[2][0]; V3y = mappingDirs[2][1]; V3z = mappingDirs[2][2];
this->setTilingDirs({{V1x, V1y, V1z}, {V2x, V2y, V2z}, {V3x, V3y, V3z}});


//The lambda function takes a mapping direction trVec and return its compenents in the tiling basis {a1, a2, a3}
auto dirConstants = [&V1x, &V1y, &V1z, &V2x, &V2y, &V2z, &V3x, &V3y, &V3z](std::vector<double> trVec)->std::vector<int>{

    std::vector<int> theDirCsts(3);
    if(trVec.size() != 3 ) return {} ;
    double tx, ty, tz; tx=trVec[0]; ty=trVec[1]; tz=trVec[2];
//    double a1x, a1y, a1z,   a2x, a2y, a2z,   a3x, a3y, a3z; // tiling directions components
//We suppose here that the first 3 surfaces picked correspond to the tiling directions
//    a1x = mappingDirs[0][0]; a1y = mappingDirs[0][1]; a1z = mappingDirs[0][2];
//    a2x = mappingDirs[1][0]; a2y = mappingDirs[1][1]; a2z = mappingDirs[1][2];
//    a3x = mappingDirs[2][0]; a3y = mappingDirs[2][1]; a3z = mappingDirs[2][2];


    boost::qvm::mat<double, 3,3> m = {V1x, V1y, V1z,    V2x, V2y, V2z,    V3x, V3y, V3z};
    float detM = boost::qvm::determinant(m);
    auto m_inv = boost::qvm::inverse(m);
    m_inv = boost::qvm::transposed(m_inv);
    //int n1, n2, n3;
    double n1, n2, n3;
    n1 = /*std::lround*/((m_inv.a[0][0])*tx + (m_inv.a[0][1])*ty + (m_inv.a[0][2])*tz);
    n2 = /*std::lround*/(m_inv.a[1][0]*tx + m_inv.a[1][1]*ty + m_inv.a[1][2]*tz);
    n3 = /*std::lround*/(m_inv.a[2][0]*tx + m_inv.a[2][1]*ty + m_inv.a[2][2]*tz);

    theDirCsts[0] = n1; theDirCsts[1]=n2; theDirCsts[2]=n3;

    return theDirCsts;

};

//std::vector<double> V1{V1x, V1y, V1z}, V2{V2x, V2y, V2z}, V3{V3x, V3y, V3z}; //Tiling directions

//boost::qvm::mat<double, 3,3> m = {V1[0], V2[0], V3[0],  V1[1], V2[1], V3[1],     V1[2], V2[2], V3[2]};
//float detV = boost::qvm::determinant(m);
//if(std::fabs(detV) < std::numeric_limits<double>::epsilon()*10) {std::cout<<"The tiling directions must be non coplanar."; return;}
//auto m_inv = boost::qvm::inverse(m);   //to acces m_inv(r,c);

//int n1, n2, n3;
//n1 = m_inv(0,0)*tx + m_inv(0,1)*ty + m_inv(0,2)*tz;
//n2 = m_inv(1,0)*tx + m_inv(1,1)*ty + m_inv(1,2)*tz;
//n3 = m_inv(2,0)*tx + m_inv(2,1)*ty + m_inv(2,2)*tz;

int nA, nB, nC, nD, nAp, nBp, nCp, nDp;

size_t mNodes = meshPbc.getNumberOfNodes();
size_t maxLab = 0;
for (auto& nd : meshPbc.getNodes()) maxLab = std::max(maxLab, (size_t)nd.getNodeNumber());
//nA = mNodes; nB = mNodes+1; nC = mNodes+2; nD = mNodes+3;
//nAp = mNodes+4; nBp = mNodes+5; nCp = mNodes+6; nDp = mNodes+7;
bool numOk = meshPbc.getReferenceDofsOK();
if(!numOk){
nA = mNodes; nB = mNodes+1;/* nC = mNodes+2;*/ nD = mNodes+2;
nAp = mNodes+3; /*nBp = mNodes+5; nCp = mNodes+6; nDp = mNodes+7;*/

xA=yA=zA=0. ;
std::vector<double> ptA{xA, yA, zA}, ptB{V1x+xA, V1y+yA, V1z+zA}/*, ptC{V1x+V2x+xA, V1y+V2y+yA, V1z+V2z+zA}*/,
    ptD{V2x+xA, V2y+yA, V2z+zA}, ptAp{V3x+xA, V3y+yA, V3z+zA}/*, ptBp{V1x+V3x+xA, V1y+V3y+yA, V1z+V3z+zA},
    ptCp{V1x+V2x+V3x+xA, V1y+V2y+V3y+yA, V1z+V2z+V3z+zA}, ptDp{V2x+V3x+xA, V2y+V3y+yA, V2z+V3z+zA}*/;

meshPbc.addNode(/*nA*/maxLab+1, ptA[0], ptA[1], ptA[2]); meshPbc.addNode(/*nB*/maxLab+2, ptB[0], ptB[1], ptB[2]);
/*meshPbc.addNode(nC, ptC[0], ptC[1], ptC[2]);*/ meshPbc.addNode(/*nD*/maxLab+3, ptD[0], ptD[1], ptD[2]);
meshPbc.addNode(/*nAp*/maxLab+4, ptAp[0], ptAp[1], ptAp[2]); /*meshPbc.addNode(nBp, ptBp[0], ptBp[1], ptBp[2]);
meshPbc.addNode(nCp, ptCp[0], ptCp[1], ptCp[2]); meshPbc.addNode(nDp, ptDp[0], ptDp[1], ptDp[2]);*/
meshPbc.setReferenceDofsOK(true);
}
else{
nAp=mNodes-1; nD=nAp-1; nB=nD-1; nA=nB-1;
}

//meshPbc.order(); //So that num in meshPbc can be populated
std::vector<std::unordered_map<int, int>> nodesConstraints = meshData.getNodesPeriodicity();

//nB=meshPbc.getNum(nB); nD=meshPbc.getNum(nD); nAp=meshPbc.getNum(nAp); //For numbering reasons
//nA=meshPbc.getNum(nA); /*nC=meshPbc.getNum(nC); nBp=meshPbc.getNum(nBp); nCp=meshPbc.getNum(nCp);*/

//array<size_t,4> controlLink1, controlLink2, controlLink4;
//array<size_t,5> controlLink3;
//controlLink1={nC,  nB, nD, nA}; //+1 +1 -1
//controlLink2={nBp, nB, nAp, nA}; //+1  +1 -1
//controlLink3={nCp, nB, nD, nAp, nA}; //+1  +1 +1 -2
//controlLink4={nDp, nD, nAp, nA}; //+1 +1 -1
////The above constraints may be unnecessary. To be reanalized

//vector<size_t> ptIds1{controlLink1.begin(),controlLink1.end()},
//    ptIds2{controlLink2.begin(), controlLink2.end()}, ptIds3{controlLink3.begin(), controlLink3.end()},
//    ptIds4{controlLink4.begin(), controlLink4.end()};
//vector<double> coeffs1{1., 1., -1.}, coeffs2{1., 1., -1}, coeffs3{1., 1., 1., -2.}, coeffs4{1., 1., -1};

//std::vector<pbcEquation> controlLinks;
//controlLinks.push_back( pbcEquation(coeffs1.size(), ptIds1, coeffs1) );
//controlLinks.push_back( pbcEquation(coeffs2.size(), ptIds2, coeffs2) );
//controlLinks.push_back( pbcEquation(coeffs3.size(), ptIds3, coeffs3) );
//controlLinks.push_back( pbcEquation(coeffs4.size(), ptIds4, coeffs4) );


//std::vector<std::vector<double>> mappingDirs = meshData.getMappingDirs();
if(mappingDirs.empty()) return;
std::vector<pbcEquation> meshLinks;

std::ofstream ofs("/home/ahose/Desktop/pbc.dat", std::ios::app);

int nDir = nodesConstraints.size();
for(int i=0; i<nDir; i++){
        int n1, n2, n3;
        std::vector<double> aDir = mappingDirs[i];
        std::unordered_map<int, int> aDirNodes = nodesConstraints[i];
        std::vector<int> aDirCsts = dirConstants(aDir);  //containts the components n1, n2, n3 of aDir on the tiling directions.
        n1=aDirCsts[0]; n2=aDirCsts[1]; n3=aDirCsts[2];

//        ofs << "dir " << i << " | nPairs=" << aDirNodes.size()
//                  << " | aDir=(" << aDir[0] << "," << aDir[1] << "," << aDir[2] << ")"
//                  << " | n=(" << n1 << "," << n2 << "," << n3 << ")\n";

        int nDirNodesPair = aDirNodes.size();

        std::unordered_map<int, int>::iterator pIt = aDirNodes.begin();
        size_t tiedN, retN;
        while(pIt != aDirNodes.end()){
            tiedN = meshPbc.getNum(pIt->first);
            retN = meshPbc.getNum(pIt->second);
            //meshLinks.push_back(pbcEquation(4, {tiedN, retN, nB, nD, nAp}, {1, -1.*n1, -1.*n2, -1.*n3}));
            meshLinks.push_back(pbcEquation(5, {tiedN, retN, nB, nD, nAp, nA}, {1, -1.*n1, -1.*n2, -1.*n3, (n1+n2+n3)*1.} ) );
            pIt++;
        }

    }


    auto& N = meshPbc.getNodes();
    for(int i=0;i<nDir;i++){
        double maxDev=0, maxDevFlip=0; size_t bad=0;
        for(auto& p : nodesConstraints[i]){
            size_t a=meshPbc.getNum(p.first), b=meshPbc.getNum(p.second);
            double dev=0, devFlip=0;
            for(int k=0;k<3;k++){
            double d = N[b].getXi(k)-N[a].getXi(k);
            dev     = std::max(dev,     std::abs(d - mappingDirs[i][k]));
            devFlip = std::max(devFlip, std::abs(d + mappingDirs[i][k]));
            }
            if(dev>1e-6) ++bad;
            maxDev=std::max(maxDev,dev); maxDevFlip=std::max(maxDevFlip,devFlip);
        }
        ofs<<"dir "<<i<<" bad="<<bad<<"/"<<nodesConstraints[i].size()
                  <<" maxDev="<<maxDev<<" maxDevIfFlipped="<<maxDevFlip<<"\n";
    }

std::ofstream ofs2("/home/ahose/Desktop/pbc2.dat", std::ios::app);

   // auto& N = meshPbc.getNodes();
    for(int i=0;i<nDir;i++){
        double s[3]={0,0,0};
        for(auto& p : nodesConstraints[i]){
            size_t a=meshPbc.getNum(p.first), b=meshPbc.getNum(p.second);
            for(int k=0;k<3;k++) s[k]+=N[b].getXi(k)-N[a].getXi(k);
        }
        double n=nodesConstraints[i].size();
        ofs2<<"map "<<i<<" mean d=("<<s[0]/n<<","<<s[1]/n<<","<<s[2]/n<<")\n";
    }



pbcLinks.reserve(meshLinks.size()/*+controlLinks.size()*/);
pbcLinks.insert(pbcLinks.end(), meshLinks.begin(), meshLinks.end());
//pbcLinks.insert(pbcLinks.end(), controlLinks.begin(), controlLinks.end());
NbLinks =  3*pbcLinks.size(); // three directionnal constraints per link

sommets.resize(4/*8*/);
sommets[0] = nA;
sommets[1] = nB;
//sommets[2] = nC;
/*sommets[3] = nD;
sommets[4] = nAp;*/ //These node numbers are the nodes' orders in the vector of nodes.
//sommets[5] = nBp;
//sommets[6] = nCp;
//sommets[7] = nDp;

sommets[2] = nD;
sommets[3] = nAp;
notMeshNodes=sommets;

}


std::vector<std::vector<double>> PbcUtilities::getTilingDirs() const {return tilingDirs;}

void PbcUtilities::setTilingDirs(const std::vector<std::vector<double>>& _tilingDirs){tilingDirs = _tilingDirs;}

//*************************** For computing pbc constraints
//Relative
std::vector<std::list<size_t> > PbcUtilities::getMatrixStructure(const MeshPbc<double,3>& Mh, const vector<pbcEquation>& pbcLinks,
const std::valarray<size_t>& numPbc,const fem::BCs& bc, size_t ndof) const{

//tdofs = constrMat*rtdofs+constrVec

std::vector<std::list<size_t> > vec(bc.getTiedNumber());

size_t tiedOffset=ndof*Mh.getNumberOfNodes()-bc.getTiedNumber(); /*-bc.getDirNumber()*/
size_t retainedOffset=ndof*Mh.getNumberOfNodes()-bc.getTiedNumber()-bc.getRetainedNumber(); /*-bc.getDirNumber()*/
size_t tdof, rtdof;
for(size_t i=0; i<pbcLinks.size()/*bc.getTiedNumber()*/; i++)
        for(size_t kndof=0; kndof<ndof; kndof++){
            size_t tId=pbcLinks[i].getTiedId();
            tdof=ndof*( tId ) + kndof;

            for(size_t iRetPbc=0; iRetPbc<(pbcLinks[i].nRet); iRetPbc++){
            size_t rtId=pbcLinks[i].getRetId(iRetPbc);
            rtdof=ndof*( rtId ) + kndof;
            if( !bc.isDirichlet(rtdof) /*true*/) vec[numPbc[tdof]-tiedOffset].insert(vec[numPbc[tdof]-tiedOffset].end(), numPbc[rtdof]-retainedOffset);}
        }
return vec;
}


void PbcUtilities::createConstraintMatrix(const MeshPbc<double,3>& Mh, const vector<pbcEquation>& pbcLinks,
const std::valarray<size_t>& numPbc,const fem::BCs& bc, size_t ndof,
fem::matrix<double>& mat) const{

//tdofs = constrMat*rtdofs+constrVec

mat.resize(getMatrixStructure(Mh, pbcLinks, numPbc, bc, ndof), bc.getRetainedNumber());

size_t tiedOffset=ndof*Mh.getNumberOfNodes()-bc.getTiedNumber();/*-bc.getDirNumber()*/
size_t retainedOffset=ndof*Mh.getNumberOfNodes()-bc.getTiedNumber()-bc.getRetainedNumber();/*-bc.getDirNumber()*/
size_t tdof, rtdof;
for(size_t i=0; i<pbcLinks.size(); i++)
        for(size_t kndof=0; kndof<ndof; kndof++){
            size_t tId=pbcLinks[i].getTiedId();
            tdof=ndof*( tId ) + kndof;

            for(size_t iRetPbc=0; iRetPbc<(pbcLinks[i].nRet); iRetPbc++){
            size_t rtId=pbcLinks[i].getRetId(iRetPbc);
            rtdof=ndof*( rtId ) + kndof;
            if( !bc.isDirichlet(rtdof) /*true*/)
            mat.at(numPbc[tdof]-tiedOffset, numPbc[rtdof]-retainedOffset) = pbcLinks[i].getCoeff(iRetPbc);
            //Ut-Urt1+Urt2=0
            }
        }
}

void PbcUtilities::createConstraintVector(const MeshPbc<double,3>& Mh, const vector<pbcEquation>& pbcLinks,
                            const std::valarray<size_t>& numPbc,const fem::BCs& bc, size_t ndof,
                                          std::valarray<double>& vec) const {

//tdofs = constrMat*rtdofs+constrVec

size_t tiedOffset=ndof*Mh.getNumberOfNodes()-bc.getTiedNumber(); //-bc.getDirNumber()
size_t tdof, rtdof;
vec.resize(bc.getTiedNumber(),0);

for(size_t i=0; i<pbcLinks.size(); i++) for(size_t kndof=0; kndof<ndof; kndof++){

    for(size_t iRetPbc=0; iRetPbc<(pbcLinks[i].nRet); iRetPbc++){
            size_t tId=pbcLinks[i].getTiedId();
            tdof=ndof*( tId ) + kndof;
            size_t rtId=pbcLinks[i].getRetId(iRetPbc);
            rtdof=ndof*( rtId ) + kndof;

            if( bc.isDirichlet(rtdof) )  vec[numPbc[tdof]-tiedOffset] += (pbcLinks[i].getCoeff(iRetPbc))*bc.pbcDirichletValue(rtdof, ndof);
    }
}
}

std::vector<std::list<size_t> > PbcUtilities::getGlobMatrixStructure(const MeshPbc<double,3>& Mh, const vector<pbcEquation>& pbcLinks,
                                                                     const std::valarray<size_t>& numPbc,const fem::BCs& bc, size_t ndof) const{

//Global numerotation is used here.
//The purpose is to derive constraint equation in the form ConstraintMat*U=ConstraintVec
// constrMat*U-constrVec = 0

std::vector<std::list<size_t> > vec(ndof*Mh.getNumberOfNodes());

size_t tdof, rtdof;
for(size_t i=0; i<pbcLinks.size(); i++){
    size_t tId=pbcLinks[i].getTiedId();
    for(size_t kndof=0; kndof<ndof; kndof++){
    tdof=ndof*( tId ) + kndof;
    vec[ numPbc[tdof] ].insert(vec[ numPbc[tdof] ].end(), numPbc[tdof]);

    for(size_t iRetPbc=0; iRetPbc<(pbcLinks[i].nRet); iRetPbc++){
            size_t rtId=pbcLinks[i].getRetId(iRetPbc);
            rtdof=ndof*( rtId ) + kndof;
            if( !bc.isDirichlet(rtdof) ) vec[ numPbc[tdof] ].insert(vec[ numPbc[tdof] ].end(), numPbc[rtdof]);}
}}

std::vector<size_t> dirdofs = bc.getDirichletDofs();
for(size_t i=0; i<dirdofs.size(); i++) vec[ numPbc[dirdofs[i]] ].insert(vec[ numPbc[dirdofs[i]]].end(),  numPbc[dirdofs[i]]);
return vec;
}


void PbcUtilities::createConstraintGlobMatrix(const MeshPbc<double,3>& Mh, const vector<pbcEquation>& pbcLinks,
                                          const std::valarray<size_t>& numPbc,const fem::BCs& bc, size_t ndof,
                                          fem::matrix<double>& mat) const{
//Global numerotation is used here.
//The purpose is to derive constraint equation in the form ConstraintMat*U=ConstraintVec
// constrMat*U-constrVec = 0

mat.resize(getGlobMatrixStructure(Mh, pbcLinks, numPbc, bc, ndof), ndof*Mh.getNumberOfNodes());

size_t tdof, rtdof;
for(size_t i=0; i<pbcLinks.size(); i++){
    size_t tId=pbcLinks[i].getTiedId();
for(size_t kndof=0; kndof<ndof; kndof++){
    tdof=ndof*( tId ) + kndof;
    mat.at(numPbc[tdof], numPbc[tdof]) = +1.0;//-1.0 ;

    for(size_t iRetPbc=0; iRetPbc<(pbcLinks[i].nRet); iRetPbc++){
            size_t rtId=pbcLinks[i].getRetId(iRetPbc);
            rtdof=ndof*( rtId ) + kndof;
            if( !bc.isDirichlet(rtdof) )
            mat.at(numPbc[tdof], numPbc[rtdof]) = -1*pbcLinks[i].getCoeff(iRetPbc);
    }
}}

std::vector<size_t> dirdofs = bc.getDirichletDofs();
for(size_t i=0; i<dirdofs.size(); i++) mat.at( numPbc[dirdofs[i]],  numPbc[dirdofs[i]]) = 1.0;
}

void PbcUtilities::createConstraintGlobVector(const MeshPbc<double,3>& Mh, const vector<pbcEquation>& pbcLinks,
                                          const std::valarray<size_t>& numPbc,const fem::BCs& bc, size_t ndof,
                                          std::valarray<double>& vec) const {

//Global numerotation is used here.
//The purpose is to derive constraint equation in the form ConstraintMat*U=ConstraintVec
// constrMat*U-constrVec = 0
size_t tdof, rtdof;
vec.resize(ndof*Mh.getNumberOfNodes(),0);

for(size_t i=0; i<pbcLinks.size(); i++) for(size_t kndof=0; kndof<ndof; kndof++){

    for(size_t iRetPbc=0; iRetPbc<(pbcLinks[i].nRet); iRetPbc++){
            size_t tId=pbcLinks[i].getTiedId();
            tdof=ndof*( tId ) + kndof;
            size_t rtId=pbcLinks[i].getRetId(iRetPbc);
            rtdof=ndof*( rtId ) + kndof;

            if( bc.isDirichlet(rtdof) )  vec[numPbc[tdof]] += +1*(pbcLinks[i].getCoeff(iRetPbc))*bc.pbcDirichletValue(rtdof, ndof);
    }}
//vec=0.;
std::vector<size_t> dirdofs = bc.getDirichletDofs();
for(size_t i=0; i<dirdofs.size(); i++) vec[ numPbc[dirdofs[i]] ] = +1*bc.pbcDirichletValue(dirdofs[i], ndof);
}


//void PbcUtilities:: createPbcConstraintsMatrix(const MeshPbc<double,3>& Mh, const vector</*array<size_t,3> */pbcEquation>& pbcLinks,
//                               const std::valarray<size_t>& numPbc,const fem::BCs& bc, size_t ndof,
//                                             fem::matrix<double>& mat)const{

////Implementing the structure of the matrix ---> vec
////std::vector<std::list<size_t> > vec(bc.getTiedNumber()+bc.getDirNumber());
//size_t nEqs = ndof*pbcLinks.size(); // nEqs stands for only periodic boundary constraints
//std::vector<std::list<size_t> > vec(nEqs+bc.getDirNumber());

//size_t tiedOffset=/*ndof*Mh.getNumberOfNodes()-*/bc.getNotConstraindedDofs();//To be deleted later



//size_t tdof, rtdof;
//for(size_t i=0; i< pbcLinks.size(); i++){
//size_t tId=pbcLinks[i].getTiedId();
//for(size_t kndof=0; kndof<ndof; kndof++){
//    //size_t tId=pbcLinks[i].getTiedId();
//    tdof=ndof*( tId ) + kndof;
//    vec[numPbc[tdof]-tiedOffset].insert(vec[numPbc[tdof]-tiedOffset].end(), numPbc[tdof]);

//    for(size_t iRetPbc=0; iRetPbc<(pbcLinks[i].nRet); iRetPbc++){
//            size_t rtId=pbcLinks[i].getRetId(iRetPbc);
//            rtdof=ndof*( rtId ) + kndof;
//            vec[numPbc[tdof]-tiedOffset].insert(vec[numPbc[tdof]-tiedOffset].end(), numPbc[rtdof]);}
//}}

//std::vector<size_t> dirdofs = bc.getDirichletDofs();
////for(size_t i=0; i<dirdofs.size(); i++) vec[ bc.getTiedNumber()+i ].insert(vec[ bc.getTiedNumber()+i  ].end(),  numPbc[dirdofs[i]]);
//for(size_t i=0; i<dirdofs.size(); i++) vec[ nEqs+i ].insert(vec[ nEqs+i  ].end(),  numPbc[dirdofs[i]]);

////Implementing the matrix filling --> mat
//mat.resize(vec, ndof*Mh.getNumberOfNodes());

//for(size_t i=0; i<pbcLinks.size(); i++){
//size_t tId=pbcLinks[i].getTiedId();
//for(size_t kndof=0; kndof<ndof; kndof++){
//    tdof=ndof*( tId ) + kndof;
//    mat.at(numPbc[tdof]-tiedOffset, numPbc[tdof]) = +1.0;//-1.0 ;

//    for(size_t iRetPbc=0; iRetPbc<(pbcLinks[i].nRet); iRetPbc++){
//            size_t rtId=pbcLinks[i].getRetId(iRetPbc);
//            rtdof=ndof*( rtId ) + kndof;
//            //if( !bc.isDirichlet(rtdof) )
//            mat.at(numPbc[tdof]-tiedOffset, numPbc[rtdof]) = -1*pbcLinks[i].getCoeff(iRetPbc);
//    }
//}}

////std::vector<size_t> dirdofs = bc.getDirichletDofs();
//for(size_t i=0; i<dirdofs.size(); i++) mat.at(nEqs+i,  numPbc[dirdofs[i]]) = 1.0;
////return vec;
//}

void PbcUtilities:: createPbcConstraintsMatrix(const MeshPbc<double,3>& Mh, const std::vector<pbcEquation>& pbcLinks,
                                              const std::valarray<size_t>& numPbc,const fem::BCs& bc, size_t ndof,
                                              fem::matrix<double>& mat)const{

//Implementing the structure of the matrix ---> vec
size_t nEqs = ndof*pbcLinks.size(); // nEqs stands for only periodic boundary constraints
std::vector<std::list<size_t> > vec(nEqs+bc.getDirNumber());

for(size_t i=0; i< pbcLinks.size(); i++){
for(size_t kndof=0; kndof<ndof; kndof++){
    vec[ndof*i+kndof].insert(vec[ndof*i+kndof].end(), numPbc[ ndof*(pbcLinks[i].getTiedId())+kndof ]);
    for(size_t kret=0; kret<pbcLinks[i].nRet; kret++){
    vec[ndof*i+kndof].insert(vec[ndof*i+kndof].end(), numPbc[ ndof*(pbcLinks[i].getRetId(kret))+kndof ]);
    }
}}
std::vector<size_t> dirdofs = bc.getDirichletDofs();
for(size_t i=0; i<dirdofs.size(); i++) vec[ nEqs+i ].insert(vec[ nEqs+i  ].end(),  numPbc[dirdofs[i]]);

//Implementing the matrix filling --> mat
mat.resize(vec, ndof*Mh.getNumberOfNodes());
for(size_t i=0; i<pbcLinks.size(); i++){
for(size_t kndof=0; kndof<ndof; kndof++){
    mat.at(ndof*i+kndof, numPbc[ ndof*(pbcLinks[i].getTiedId())+kndof ]) = +1;
    for(size_t kret=0; kret<pbcLinks[i].nRet; kret++){
    mat.at(ndof*i+kndof, numPbc[ ndof*(pbcLinks[i].getRetId(kret))+kndof ]) = -1*pbcLinks[i].getCoeff(kret);}
}}

for(size_t i=0; i<dirdofs.size(); i++) mat.at(nEqs+i,  numPbc[dirdofs[i]]) = 1.0;
}



void PbcUtilities::createPbcConstraintsVector(const MeshPbc<double,3>& Th, const std::vector<pbcEquation>& pbcLinks,
                                              const std::valarray<size_t>& numPbc,const fem::BCs& bc, size_t ndof,
                                              std::valarray<double>& vec) const{

std::vector<size_t> sommets = bc.getSommets();
 ; //, orderedSommets;

//int nA = controlNodes[0]; int nB = controlNodes[1];
//int nD = controlNodes[3]; int nAp = controlNodes[4];
 int nA = sommets[0]; int nB = sommets[1];
 int nD = sommets[2]; int nAp = sommets[3];

 std::vector<int>controlNodes = {nA, nB, nD, nAp};

size_t nEqs = ndof*pbcLinks.size(); // nEqs stands for only periodic boundary constraints
vec.resize(nEqs+bc.getDirNumber(), 0.0);
std::vector<size_t> dirdofs = bc.getDirichletDofs();
for(size_t i=0; i<dirdofs.size(); i++) vec[nEqs+i]=bc.pbcDirichletValue(Th, controlNodes, bc.getLoadingType(), dirdofs[i], 0.01  )/*bc.pbcDirichletValue(dirdofs[i], ndof)*/; //A revoir

}
} // namespace models
} // namespace scaletag
