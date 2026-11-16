#include<physics.h>


/* form functions */
double n11(double xi, double eta, double zeta){ return 4.*(xi+eta+zeta)-3.; }  //n11=n12=n13
double n12(double xi, double eta, double zeta){ return 4.*(xi+eta+zeta)-3.; }
double n13(double xi, double eta, double zeta){ return 4.*(xi+eta+zeta)-3.; }

double n21(double xi, double eta, double zeta){ return 4.*xi-1. ; }
double n22(double xi, double eta, double zeta){ return 0. ; }
double n23(double xi, double eta, double zeta){ return 0.; }

double n31(double xi, double eta, double zeta){ return 0.; }
double n32(double xi, double eta, double zeta){ return 4.*eta-1. ; }
double n33(double xi, double eta, double zeta){ return 0.; }

double n41(double xi, double eta, double zeta){ return 0.; }
double n42(double xi, double eta, double zeta){ return 0.; }
double n43(double xi, double eta, double zeta){ return 4.*zeta-1. ; }

double n51(double xi, double eta, double zeta){ return -8.*xi -4.*eta -4.*zeta +4 ; }
double n52(double xi, double eta, double zeta){ return -4.*xi ; }
double n53(double xi, double eta, double zeta){ return -4.*xi ; }

double n61(double xi, double eta, double zeta){ return 4.*eta ; }
double n62(double xi, double eta, double zeta){ return 4.*xi ; }
double n63(double xi, double eta, double zeta){ return 0. ; }

double n71(double xi, double eta, double zeta){ return -4.*eta ; }
double n72(double xi, double eta, double zeta){ return -4.*xi -8.*eta -4.*zeta +4 ; }
double n73(double xi, double eta, double zeta){ return -4.*eta ; }

double n81(double xi, double eta, double zeta){ return -4.*zeta ; }
double n82(double xi, double eta, double zeta){ return -4.*zeta ; }
double n83(double xi, double eta, double zeta){ return -4.*xi -4.*eta -8.*zeta +4 ; }

double n91(double xi, double eta, double zeta){ return 0. ; }
double n92(double xi, double eta, double zeta){ return 4.*zeta ; }
double n93(double xi, double eta, double zeta){ return 4.*eta ; }

double n101(double xi, double eta, double zeta){ return 4.*zeta ; }
double n102(double xi, double eta, double zeta){ return 0. ; }
double n103(double xi, double eta, double zeta){ return 4.*xi ; }



//double n101(double xi, double eta, double zeta){ return 4.*zeta ; }
//double n102(double xi, double eta, double zeta){ return 0. ; }
//double n103(double xi, double eta, double zeta){ return 4.*xi ; }

//double n91(double xi, double eta, double zeta){ return 0. ; }
//double n92(double xi, double eta, double zeta){ return 4.*zeta ; }
//double n93(double xi, double eta, double zeta){ return 4.*eta ; }

/* End of form functions */

Mat3x3 adj3(const Mat3x3& mat3){
    double a11 = mat3[0][0], a12  = mat3[0][1], a13 = mat3[0][2],
        a21 = mat3[1][0], a22  = mat3[1][1], a23 = mat3[1][2],
        a31 = mat3[2][0], a32  = mat3[2][1], a33 = mat3[2][2] ;

    double r11, r12, r13, r21, r22, r23, r31, r32, r33; // comatrice components

    r11 = a22*a33-a23*a32; r22 = a11*a33-a13*a31 ; r33 =a11*a22-a12*a21;
    r12 = -1.*(a21*a33-a31*a23); r21 = -1.*(a12*a33-a32*a13);
    r13 = a21*a32-a31*a22; r31 = a12*a23-a22*a13;
    r23 = -1.*(a11*a32-a31*a12) ; r32 = -1.*(a11*a23-a21*a13) ;

    return { {{r11, r21, r31}, {r12, r22, r32}, {r13, r23, r33}}};
} // compute transpose(com(mat3))


double det3(const Mat3x3& mat3){
    double a11 = mat3[0][0], a12  = mat3[0][1], a13 = mat3[0][2],
    a21 = mat3[1][0], a22  = mat3[1][1], a23 = mat3[1][2],
        a31 = mat3[2][0], a32  = mat3[2][1], a33 = mat3[2][2] ;

    double J = a11*a22*a33+ a12*a23*a31 + a13*a21*a32 -a13*a22*a31 -a11*a23*a32 -a12*a21*a33 ;
    return J ;
}

Mat3x3 prod3(const Mat3x3& mat, double alpha){
    Mat3x3 res;
    for(int i=0; i<3; i++) for(int j=0; j<3; j++) res[i][j] = mat[i][j]* alpha;

    return res;
}

Mat3x3 inv3(const Mat3x3& mat){ return prod3(adj3(mat), 1./det3(mat));}


Mat3x3 operator+(const Mat3x3& mat1, const Mat3x3& mat2){
    Mat3x3 res;
    res[0][0] = mat1[0][0] + mat2[0][0];    res[0][1] = mat1[0][1] + mat2[0][1];    res[0][2] = mat1[0][2] + mat2[0][2];
    res[1][0] = mat1[1][0] + mat2[1][0];    res[1][1] = mat1[1][1] + mat2[1][1];    res[1][2] = mat1[1][2] + mat2[1][2];
    res[2][0] = mat1[2][0] + mat2[2][0];    res[2][1] = mat1[2][1] + mat2[2][1];    res[2][2] = mat1[2][2] + mat2[2][2];
    return res;
}

Mat3x3 operator*(const Vec3x1& vec1, const Vec3x1& vec2){
    /* Cette fonction retourne le produit dyadique de deux vecteurs en dim=3 */
    Mat3x3 res;
    res[0][0] = vec1[0]*vec2[0];    res[0][1] = vec1[0]*vec2[1];    res[0][2] = vec1[0]*vec2[2];
    res[1][0] = vec1[1]*vec2[0];    res[1][1] = vec1[1]*vec2[1];    res[1][2] = vec1[1]*vec2[2];
    res[2][0] = vec1[2]*vec2[0];    res[2][1] = vec1[2]*vec2[1];    res[2][2] = vec1[2]*vec2[2];
    return res;
}

Vec3x1 operator|(const Vec3x1& vec, const Mat3x3& mat) {
    /* Cette fonction retourne retourne le produit contracté simple entre un vecteur et une matrice
     * carrée en dim=3*/

    Vec3x1 res{{0., 0., 0.}};
    for(int i=0; i<3; i++) for(int k=0; k<3; k++) res[i] += vec[k]*mat[k][i];

    return res;

}
