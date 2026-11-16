#ifndef PHYSICS_H
#define PHYSICS_H

#include<map>
#include<variant>
#include<MeshPbc.h>
#include<elementtype.h>

/***** Material Type ******************
 *
 *
 *
 * */

//typedef double Mat4x3[4][3] ;
//using Mat4x3 = double[4][3]; //same as above
using Mat4x3 = std::array<std::array<double, 3>, 4> ;
//using Mat30x3 = std::array<std::array<double,3>, 30>; // For tet10

using Vec3x1 = std::array<double,3> ;
//using Mat10x3 = std::array<std::array<double(*)(double, double, double),3>, 10>; // For tet10
using Mat10x3 = std::array<std::array<double ,3>, 10>; // For tet10
using Mat3x3 = std::array<std::array<double,3>, 3> ; // To hold determinant in 3d for eg.

Mat3x3 adj3(const Mat3x3& mat3); // compute transpose(com(mat3))
double det3(const Mat3x3& mat3);
Mat3x3 prod3(const Mat3x3& mat, double alpha);
Mat3x3 inv3(const Mat3x3& mat);
Mat3x3 operator+(const Mat3x3& mat1, const Mat3x3& mat2);
Mat3x3 operator*(const Vec3x1& vec1, const Vec3x1& vec2); //Pour le produit tensoriel entre deux vecteurs(produit dyadique en dim=3)
Vec3x1 operator|(const Vec3x1& vec, const Mat3x3& mat) ; //Pour représenter le produit contracté entre un vecteur et un tenseur d'ordre 2 en dim=3

class Material : public std::map<std::string, double> {


    private:
        std::string type = "Hooke";
        std::string name = "";  //LinearElastic3D

    public:
    Material(){}

    Material(std::string _type, std::string _name):type(_type), name(_name){}






    std::string getType() const{return type;}
    void setType(const std::string& _type){type=_type;}

    std::string getName() const {return name;}
    void setName(const std::string& _name) {name=_name;}

    double getProperty(const std::string& propName){
        return this->at(propName);
    }

};



class Physics{

private:
    std::string type = "StaticLinear";
    //std::string name = "";  //

public:
    Physics(std::string _type/*, std::string _name*/):type(_type)/*, name(_name)*/{
    }

    std::string getType() const{return type;}
    void setType(const std::string& _type){type=_type;}

//    std::string getName() const {return name;}
//    void setName(const std::string& _name) {name=_name;}

};

class LinearElasticity3D : public Physics{


private :
    std::string type = "LinearElasticity3D";

public:

    LinearElasticity3D() : Physics( std::string("StaticLinear") ){}
};



class ElementalMatrix{


    public:

       //ElementalMatrix(){}

        ElementalMatrix(const Physics& physics, const std::vector<Material>& _mats, const MeshPbc<double, 3>& mh) : mh(mh), physics(physics), mats(_mats){
        }

        virtual double operator()(size_t e) = 0; // must be defined for class attribute data filling
        virtual double operator()(size_t e, size_t ie, size_t je, size_t kie, size_t kje) const = 0;


    protected:
        //std::string matName = "LinearElasticity3D"/*"Hooke"*/;
        //const Material& mat;
        /*scaletag::fem::eltType*/ size_t type;
       // size_t eltMatType;

        const MeshPbc<double, 3>& mh  ;
        const Physics& physics;
        const std::vector<Material>& mats;
        std::map<std::string, std::variant<double, int, Mat3x3, Mat4x3, Mat10x3, void*> > data;
};


class ElementalMatrixLinearElasticity3D : public ElementalMatrix{

 public :
        ElementalMatrixLinearElasticity3D(const Physics& physics, const std::vector<Material>& mats, const MeshPbc<double, 3>& mh) : ElementalMatrix(physics, mats, mh){

        }

        double operator()(size_t e) override{

            /* This operator initializes the attribute member data */

            // Put lambda, mu, detTau, tabGradN in data

            double E, nu, lambda, mu, detTau;
            //E=/*0.5*/608.; nu=0.4;

            E = mats[mh.getMatType(e)].at("E");
            nu = mats[mh.getMatType(e)].at("nu");

            lambda=(nu*E)/((1+nu)*(1-2*nu));
            mu=(E)/(2*(1+nu));

            size_t eltType = mh[e].getElementType();

            if(/*eltType==50*/eltType==scaletag::fem::TET4 || eltType==scaletag::fem::HERM5){

            double xA,yA,zA, xB,yB,zB, xC,yC,zC, xD,yD,zD;  // detTau;
            double A11,A22,A33, A12,A21, A13,A31, A23,A32; // commatrice coefficients
            //i.e To invert the transformation matrix, one need to transpose the matrix A.

            xA=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[0]]).getXi(0);
            yA=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[0]]).getXi(1);
            zA=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[0]]).getXi(2);

            xB=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[1]]).getXi(0);
            yB=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[1]]).getXi(1);
            zB=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[1]]).getXi(2);

            xC=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[2]]).getXi(0);
            yC=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[2]]).getXi(1);
            zC=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[2]]).getXi(2);

            xD=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[3] ]).getXi(0);
            yD=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[3]]).getXi(1);
            zD=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[3]]).getXi(2);


            detTau= (-xA+xB)*(-yA+yC)*(-zA+zD)-(-xA+xB)*(-zA+zC)*(-yA+yD)
                     -(-yA+yB)*(-xA+xC)*(-zA+zD)+(-yA+yB)*(-zA+zC)*(-xA+xD)
                     +(-zA+zB)*(-xA+xC)*(-yA+yD)-(-zA+zB)*(-yA+yC)*(-xA+xD);

            if(detTau<=1.e-7) std::cout<< "volume of element "<<e<<" is less than 1.e-7. vol : "<<detTau<<std::endl;
            //Aij for commatrix of transformation
            //Tau-1 is then the transpose of A divided by detTau
            A11=( (-yA+yC)*(-zA+zD) - (-zA+zC)*(-yA+yD) );
            A22=( (-xA+xB)*(-zA+zD) - (-zA+zB)*(-xA+xD)  );
            A33=( (-xA+xB)*(-yA+yC) - (-yA+yB)*(-xA+xC) );

            A12=-1*( (-yA+yB)*(-zA+zD) - (-zA+zB)*(-yA+yD) );
            A21=-1*( (-xA+xC)*(-zA+zD) - (-zA+zC)*(-xA+xD) );

            A13=( (-yA+yB)*(-zA+zC) - (-zA+zB)*(-yA+yC) );
            A31=( (-xA+xC)*(-yA+yD) - (-yA+yC)*(-xA+xD) );

            A23=-1*( (-xA+xB)*(-zA+zC) - (-zA+zB)*(-xA+xC) );
            A32=-1*( (-xA+xB)*(-yA+yD) - (-yA+yB)*(-xA+xD) );

            double GradN1[]{(-A11-A12-A13)/detTau, (-A21-A22-A23)/detTau, (-A31-A32-A33)/detTau};
            double GradN2[]{A11/detTau, A21/detTau, A31/detTau};
            double GradN3[]{A12/detTau, A22/detTau, A32/detTau};
            double GradN4[]{A13/detTau, A23/detTau, A33/detTau};  // The real values must be gotten by multiplying every line by Grad xi/x.

            double /*tabGradN*/B[4][3] = { {GradN1[0],GradN1[1],GradN1[2]}, {GradN2[0],GradN2[1],GradN2[2]},
                                     {GradN3[0],GradN3[1],GradN3[2]}, {GradN4[0],GradN4[1],GradN4[2]} };

            std::array<std::array<double, 3>, 4> tabGradN;

            for(size_t i=0; i<4; i++) for(size_t j=0; j<3; j++) tabGradN[i][j]=B[i][j];

            data["lambda"] = lambda;
            data["mu"] = mu;
            data["detTau"] = detTau;
            data["tabGradN"] = tabGradN;
        }

        if(eltType==scaletag::fem::TET10){

            double xA,yA,zA, xB,yB,zB, xC,yC,zC, xD,yD,zD;  // detTau;
            double xAB, yAB, zAB,    xBC, yBC, zBC,    xCA, yCA, zCA,   xAD, yAD, zAD,    xBD, yBD, zBD,    xCD, yCD, zCD;
            double A11,A22,A33, A12,A21, A13,A31, A23,A32;

            double detTauG1, detTauG2, detTauG3, detTauG4;
            Mat10x3 tabGradNG1, tabGradNG2, tabGradNG3, tabGradNG4 ;
            {/* The gradient here are computed respectively to the reference space */
double alpha = (5.+3.*std::sqrt(5))/20., beta = (5.-1.*std::sqrt(5))/20., w = 1./24.;

double G1[3] = {alpha, beta, beta};
double G2[3] = {beta, alpha, beta};
double G3[3] = {beta, beta, alpha};
double G4[3] = {beta, beta, beta};  //Gauss points

double n11(double xi, double eta, double zeta);
double n12(double xi, double eta, double zeta);
double n13(double xi, double eta, double zeta);

double n21(double xi, double eta, double zeta);
double n22(double xi, double eta, double zeta);
double n23(double xi, double eta, double zeta);

double n31(double xi, double eta, double zeta);
double n32(double xi, double eta, double zeta);
double n33(double xi, double eta, double zeta);

double n41(double xi, double eta, double zeta);
double n42(double xi, double eta, double zeta);
double n43(double xi, double eta, double zeta);

double n51(double xi, double eta, double zeta);
double n52(double xi, double eta, double zeta);
double n53(double xi, double eta, double zeta);
double n61(double xi, double eta, double zeta);
double n62(double xi, double eta, double zeta);
double n63(double xi, double eta, double zeta);

double n71(double xi, double eta, double zeta);
double n72(double xi, double eta, double zeta);
double n73(double xi, double eta, double zeta);

double n81(double xi, double eta, double zeta);
double n82(double xi, double eta, double zeta);
double n83(double xi, double eta, double zeta);

double n91(double xi, double eta, double zeta);
double n92(double xi, double eta, double zeta);
double n93(double xi, double eta, double zeta);

double n101(double xi, double eta, double zeta);
double n102(double xi, double eta, double zeta);
double n103(double xi, double eta, double zeta);

tabGradNG1 = {{
    {n11( G1[0], G1[1], G1[2] ), n12( G1[0], G1[1], G1[2] ), n13( G1[0], G1[1], G1[2] ) },
    {n21( G1[0], G1[1], G1[2] ), n22( G1[0], G1[1], G1[2] ), n23( G1[0], G1[1], G1[2] ) },
    {n31( G1[0], G1[1], G1[2] ), n32( G1[0], G1[1], G1[2] ), n33( G1[0], G1[1], G1[2] ) },
    {n41( G1[0], G1[1], G1[2] ), n42( G1[0], G1[1], G1[2] ), n43( G1[0], G1[1], G1[2] ) },
    {n51( G1[0], G1[1], G1[2] ), n52( G1[0], G1[1], G1[2] ), n53( G1[0], G1[1], G1[2] ) },
    {n61( G1[0], G1[1], G1[2] ), n62( G1[0], G1[1], G1[2] ), n63( G1[0], G1[1], G1[2] ) },
    {n71( G1[0], G1[1], G1[2] ), n72( G1[0], G1[1], G1[2] ), n73( G1[0], G1[1], G1[2] ) },
    {n81( G1[0], G1[1], G1[2] ), n82( G1[0], G1[1], G1[2] ), n83( G1[0], G1[1], G1[2] ) },
    {n91( G1[0], G1[1], G1[2] ), n92( G1[0], G1[1], G1[2] ), n93( G1[0], G1[1], G1[2] ) },
    {n101( G1[0], G1[1], G1[2] ), n102( G1[0], G1[1], G1[2] ), n103( G1[0], G1[1], G1[2] ) }

}} ;

tabGradNG2= {{ {n11( G2[0], G2[1], G2[2] ), n12( G2[0], G2[1], G2[2] ), n13( G2[0], G2[1], G2[2] ) },
    {n21( G2[0], G2[1], G2[2] ), n22( G2[0], G2[1], G2[2] ), n23( G2[0], G2[1], G2[2] ) },
    {n31( G2[0], G2[1], G2[2] ), n32( G2[0], G2[1], G2[2] ), n33( G2[0], G2[1], G2[2] ) },
    {n41( G2[0], G2[1], G2[2] ), n42( G2[0], G2[1], G2[2] ), n43( G2[0], G2[1], G2[2] ) },
    {n51( G2[0], G2[1], G2[2] ), n52( G2[0], G2[1], G2[2] ), n53( G2[0], G2[1], G2[2] ) },
    {n61( G2[0], G2[1], G2[2] ), n62( G2[0], G2[1], G2[2] ), n63( G2[0], G2[1], G2[2] ) },
    {n71( G2[0], G2[1], G2[2] ), n72( G2[0], G2[1], G2[2] ), n73( G2[0], G2[1], G2[2] ) },
    {n81( G2[0], G2[1], G2[2] ), n82( G2[0], G2[1], G2[2] ), n83( G2[0], G2[1], G2[2] ) },
    {n91( G2[0], G2[1], G2[2] ), n92( G2[0], G2[1], G2[2] ), n93( G2[0], G2[1], G2[2] ) },
    {n101( G2[0], G2[1], G2[2] ), n102( G2[0], G2[1], G2[2] ), n103( G2[0], G2[1], G2[2] ) }

}} ;

tabGradNG3= {{ {n11( G3[0], G3[1], G3[2] ), n12( G3[0], G3[1], G3[2] ), n13( G3[0], G3[1], G3[2] ) },
    {n21( G3[0], G3[1], G3[2] ), n22( G3[0], G3[1], G3[2] ), n23( G3[0], G3[1], G3[2] ) },
    {n31( G3[0], G3[1], G3[2] ), n32( G3[0], G3[1], G3[2] ), n33( G3[0], G3[1], G3[2] ) },
    {n41( G3[0], G3[1], G3[2] ), n42( G3[0], G3[1], G3[2] ), n43( G3[0], G3[1], G3[2] ) },
    {n51( G3[0], G3[1], G3[2] ), n52( G3[0], G3[1], G3[2] ), n53( G3[0], G3[1], G3[2] ) },
    {n61( G3[0], G3[1], G3[2] ), n62( G3[0], G3[1], G3[2] ), n63( G3[0], G3[1], G3[2] ) },
    {n71( G3[0], G3[1], G3[2] ), n72( G3[0], G3[1], G3[2] ), n73( G3[0], G3[1], G3[2] ) },
    {n81( G3[0], G3[1], G3[2] ), n82( G3[0], G3[1], G3[2] ), n83( G3[0], G3[1], G3[2] ) },
    {n91( G3[0], G3[1], G3[2] ), n92( G3[0], G3[1], G3[2] ), n93( G3[0], G3[1], G3[2] ) },
    {n101( G3[0], G3[1], G3[2] ), n102( G3[0], G3[1], G3[2] ), n103( G3[0], G3[1], G3[2] ) }

}} ;

tabGradNG4 = {{ {n11( G4[0], G4[1], G4[2] ), n12( G4[0], G4[1], G4[2] ), n13( G4[0], G4[1], G4[2] ) },
               {n21( G4[0], G4[1], G4[2] ), n22( G4[0], G4[1], G4[2] ), n23( G4[0], G4[1], G4[2] ) },
               {n31( G4[0], G4[1], G4[2] ), n32( G4[0], G4[1], G4[2] ), n33( G4[0], G4[1], G4[2] ) },
               {n41( G4[0], G4[1], G4[2] ), n42( G4[0], G4[1], G4[2] ), n43( G4[0], G4[1], G4[2] ) },
               {n51( G4[0], G4[1], G4[2] ), n52( G4[0], G4[1], G4[2] ), n53( G4[0], G4[1], G4[2] ) },
               {n61( G4[0], G4[1], G4[2] ), n62( G4[0], G4[1], G4[2] ), n63( G4[0], G4[1], G4[2] ) },
               {n71( G4[0], G4[1], G4[2] ), n72( G4[0], G4[1], G4[2] ), n73( G4[0], G4[1], G4[2] ) },
               {n81( G4[0], G4[1], G4[2] ), n82( G4[0], G4[1], G4[2] ), n83( G4[0], G4[1], G4[2] ) },
               {n91( G4[0], G4[1], G4[2] ), n92( G4[0], G4[1], G4[2] ), n93( G4[0], G4[1], G4[2] ) },
               {n101( G4[0], G4[1], G4[2] ), n102( G4[0], G4[1], G4[2] ), n103( G4[0], G4[1], G4[2] ) }

}} ;}

            xA=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[0]]).getXi(0);
            yA=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[0]]).getXi(1);
            zA=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[0]]).getXi(2);

            xB=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[1]]).getXi(0);
            yB=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[1]]).getXi(1);
            zB=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[1]]).getXi(2);

            xC=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[2]]).getXi(0);
            yC=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[2]]).getXi(1);
            zC=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[2]]).getXi(2);

            xD=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[3] ]).getXi(0);
            yD=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[3]]).getXi(1);
            zD=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[3]]).getXi(2);

            xAB=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[4]]).getXi(0);
            yAB=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[4]]).getXi(1);
            zAB=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[4]]).getXi(2);
            xBC=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[5]]).getXi(0);
            yBC=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[5]]).getXi(1);
            zBC=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[5]]).getXi(2);
            xCA=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[6]]).getXi(0);
            yCA=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[6]]).getXi(1);
            zCA=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[6]]).getXi(2);
            xAD=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[7]]).getXi(0);
            yAD=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[7]]).getXi(1);
            zAD=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[7]]).getXi(2);
            xBD=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[9]]).getXi(0);
            yBD=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[9]]).getXi(1);
            zBD=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[9]]).getXi(2);
            xCD=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[8]]).getXi(0);
            yCD=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[8]]).getXi(1);
            zCD=Node<double,3>(mh.getNodes()[ mh.getConnectivity(e)[8]]).getXi(2);


            Mat10x3 XNods{{{xA, yA, zA}, {xB, yB, zB}, {xC, yC, zC}, {xD, yD, zD}, {xAB, yAB, zAB}, {xBC, yBC, zBC}
                       ,{xCA, yCA, zCA}, {xAD, yAD, zAD}, {xCD, yCD, zCD}, {xBD, yBD, zBD}
            }};

            for(int k=4;k<10;k++){
double best=1e300; int ba=-1, bb=-1;
for(int a=0;a<4;a++) for(int b=a+1;b<4;b++){
        double d=0; for(int c=0;c<3;c++) d+=std::abs(XNods[k][c]-0.5*(XNods[a][c]+XNods[b][c]));
        if(d<best){best=d;ba=a;bb=b;}
    }
//fprintf(stderr,"elt %zu noeud %d : milieu de (%d,%d), ecart %g\n",e,k,ba,bb,best);
            }  // To be deleted

            Mat3x3 GradXG1{{{0.,0.,0.},{0.,0.,0.},{0.,0.,0.}}}, GradXG2{{{0.,0.,0.},{0.,0.,0.},{0.,0.,0.}}},
                GradXG3{{{0.,0.,0.},{0.,0.,0.},{0.,0.,0.}}}, GradXG4{{{0.,0.,0.},{0.,0.,0.},{0.,0.,0.}}};

            for(int k=0; k<10; k++){
                GradXG1 = GradXG1 + Vec3x1(XNods[k])*Vec3x1(tabGradNG1[k]) ;
                GradXG2 = GradXG2 + Vec3x1(XNods[k])*Vec3x1(tabGradNG2[k]) ;
                GradXG3 = GradXG3 + Vec3x1(XNods[k])*Vec3x1(tabGradNG3[k]) ;
                GradXG4 = GradXG4 + Vec3x1(XNods[k])*Vec3x1(tabGradNG4[k]) ;
                //Les GradXGi ici sont par rapport aux variables de référence

            }

            detTauG1 = det3(GradXG1);   detTauG2 = det3(GradXG2);
            detTauG3 = det3(GradXG3); detTauG4 = det3(GradXG4);

//            static int np = 0;
//            if(np < 5){ np++;
//                fprintf(stderr,"elt %zu detTauG: %g %g %g %g\n", e, detTauG1, detTauG2, detTauG3, detTauG4);
//            }//To be deleted

            Mat3x3 InvGradXG1 = inv3(GradXG1), InvGradXG2= inv3(GradXG2), InvGradXG3= inv3(GradXG3), InvGradXG4= inv3(GradXG4);
            for(int k=0; k<10; k++) {
                tabGradNG1[k] = tabGradNG1[k]|InvGradXG1;
                tabGradNG2[k] = tabGradNG2[k]|InvGradXG2;
                tabGradNG3[k] = tabGradNG3[k]|InvGradXG3;
                tabGradNG4[k] = tabGradNG4[k]|InvGradXG4;


            }

            data["lambda"] = lambda;
            data["mu"] = mu;
            data["tabGradNG1"] = tabGradNG1;
            data["tabGradNG2"] = tabGradNG2;
            data["tabGradNG3"] = tabGradNG3;
            data["tabGradNG4"] = tabGradNG4;
            data["detTauG1"] = detTauG1; data["detTauG2"] = detTauG2; data["detTauG3"] = detTauG3; data["detTauG4"] = detTauG4;

            //Just for fun, gradient of form functions evaluated at the 4 Gauss points are stored
            //detTau here is a function

        }


        }

        double operator()(size_t e, size_t ie, size_t je, size_t kie, size_t kje) const override {

    size_t eltType = mh[e].getElementType();
    std::string matType = mats[ mh.getMatType(e)].getType(); //
    std::string phys = physics.getType();

    // Recover lambda, mu, detTau, tabGradN in data for computing the following parameters
    double lambda = std::get<double>(data.at("lambda"));
    double mu = std::get<double>(data.at("mu"));

    if((eltType==scaletag::fem::TET4 || eltType==scaletag::fem::HERM5) && phys=="StaticLinear" && matType=="Hooke" ){


            Mat4x3 tabGradN = std::get<Mat4x3>(data.at("tabGradN"));
            double detTau = std::get<double>(data.at("detTau"));

        return (1./6.)*detTau*( lambda*(tabGradN[ie][kie])*(tabGradN[je][kje]) + mu*( (tabGradN[ie][kje])*(tabGradN[je][kie])
            +(kie==kje)*( (tabGradN[ie][0])*(tabGradN[je][0])+(tabGradN[ie][1])*(tabGradN[je][1])+(tabGradN[ie][2])*(tabGradN[je][2]) ) )  ) ;
    }

    if(eltType==scaletag::fem::TET10  && phys=="StaticLinear" && matType=="Hooke" ){

        Mat10x3 tabGradNG1 = std::get<Mat10x3>(data.at("tabGradNG1"));
        double detTauG1 = std::get<double>(data.at("detTauG1"));
        Mat10x3 tabGradNG2 = std::get<Mat10x3>(data.at("tabGradNG2"));
        double detTauG2 = std::get<double>(data.at("detTauG2"));
        Mat10x3 tabGradNG3 = std::get<Mat10x3>(data.at("tabGradNG3"));
        double detTauG3 = std::get<double>(data.at("detTauG3"));
        Mat10x3 tabGradNG4 = std::get<Mat10x3>(data.at("tabGradNG4"));
        double detTauG4 = std::get<double>(data.at("detTauG4"));

//        if(detTauG1<=0 || detTauG2<=0 || detTauG3<=0 || detTauG4<=0)
//        fprintf(stderr, "TET10 elt %zu detJ<=0 : %g %g\n",e, detTauG1,detTauG4);


        double aij=0; //To be recomputed
        double aijG1 = detTauG1*( lambda*(tabGradNG1[ie][kie])*(tabGradNG1[je][kje]) + mu*( (tabGradNG1[ie][kje])*(tabGradNG1[je][kie])
                +(kie==kje)*( (tabGradNG1[ie][0])*(tabGradNG1[je][0])+(tabGradNG1[ie][1])*(tabGradNG1[je][1])+(tabGradNG1[ie][2])*(tabGradNG1[je][2]) ) )  );
        double aijG2 = detTauG2*( lambda*(tabGradNG2[ie][kie])*(tabGradNG2[je][kje]) + mu*( (tabGradNG2[ie][kje])*(tabGradNG2[je][kie])
                 +(kie==kje)*( (tabGradNG2[ie][0])*(tabGradNG2[je][0])+(tabGradNG2[ie][1])*(tabGradNG2[je][1])+(tabGradNG2[ie][2])*(tabGradNG2[je][2]) ) )  );
        double aijG3 = detTauG3*( lambda*(tabGradNG3[ie][kie])*(tabGradNG3[je][kje]) + mu*( (tabGradNG3[ie][kje])*(tabGradNG3[je][kie])
                +(kie==kje)*( (tabGradNG3[ie][0])*(tabGradNG3[je][0])+(tabGradNG3[ie][1])*(tabGradNG3[je][1])+(tabGradNG3[ie][2])*(tabGradNG3[je][2]) ) )  );
        double aijG4 = detTauG4*( lambda*(tabGradNG4[ie][kie])*(tabGradNG4[je][kje]) + mu*( (tabGradNG4[ie][kje])*(tabGradNG4[je][kie])
            +(kie==kje)*( (tabGradNG4[ie][0])*(tabGradNG4[je][0])+(tabGradNG4[ie][1])*(tabGradNG4[je][1])+(tabGradNG4[ie][2])*(tabGradNG4[je][2]) ) )  );
        double weight = 1./24.;
        aij = weight*(aijG1+aijG2+aijG3+aijG4) ;
        return aij;
    }

    }

};


#endif
