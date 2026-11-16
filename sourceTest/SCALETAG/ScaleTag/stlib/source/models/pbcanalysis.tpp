namespace scaletag{
namespace fem{


template <typename T, size_t D>
PetscErrorCode ElasXAnalysisGen(/*int* argc, char*** argv, */MeshPbc<T,D>& Mh, size_t ndof, BCs& bc,
                                ElementalMatrix& elementalMatrix,
                                std::valarray<T>& disp, std::valarray<T>& force, T& E11, T& nu12, T& nu13){

    std::vector<pbcEquation> pbcLinks; /*=std::vector<pbcEquation>();*/
        //bc.initialize();
    PetscFunctionBeginUser;
    /*PetscCall(*/scaletag::fem::pbcTaoDispForceGen(Mh, ndof, bc, pbcLinks, elementalMatrix, 0, disp, force)/*)*/;

    MPI_Barrier(MPI_COMM_WORLD);

    int wr;
    MPI_Comm_rank(MPI_COMM_WORLD, &wr);
    if(wr==0){

        size_t iA = bc.getSommets()[0];
        size_t iB = bc.getSommets()[1];
        size_t iD = bc.getSommets()[2];
        size_t iAp = bc.getSommets()[3];


        double Lx = Mh.getNodes()[iB][0]-Mh.getNodes()[iA][0];
        double Ly = Mh.getNodes()[iD][1]-Mh.getNodes()[iA][1];
        double Lz = Mh.getNodes()[iAp][2]-Mh.getNodes()[iA][2];

        double V = Lx*Ly*Lz;
        double sigma11, eps11, eps22, eps33;
        double dxx=disp[ndof*iB];

//        eps11=dxx/(Lx);
//        sigma11=((Lx)/V)*force[ndof*iB];
//        eps22=disp[ndof*iD+1]/(Ly);
//        eps33=disp[ndof*iAp+2]/(Lz);

//        E11=sigma11/eps11; nu12=-1*eps22/eps11; nu13=-1*eps33/eps11;
//        cout<<"E11 : "<< E11 <<" nu12 : "<< nu12 <<" nu13 : "<<nu13<<std::endl;
        std::cout<<"disp[ndof*iB] : "<<disp[ndof*iB]<<std::endl;
        std::cout<<"uA("<<disp[ndof*iA]<<" , "<<disp[ndof*iA+1]<<" , "<<disp[ndof*iA+2]<<")"<<std::endl;
        std::cout<<"uB("<<disp[ndof*iB]<<" , "<<disp[ndof*iB+1]<<" , "<<disp[ndof*iB+2]<<")"<<std::endl;
        std::cout<<"uD("<<disp[ndof*iD]<<" , "<<disp[ndof*iD+1]<<" , "<<disp[ndof*iD+2]<<")"<<std::endl;
        std::cout<<"uAp("<<disp[ndof*iAp]<<" , "<<disp[ndof*iAp+1]<<" , "<<disp[ndof*iAp+2]<<")"<<std::endl;
        cout<<"fA("<<force[ndof*iA]<<", "<<force[ndof*iA+1]<<", "<<force[ndof*iA+2]<<")"<<std::endl;
        cout<<"fB("<<force[ndof*iB]<<", "<<force[ndof*iB+1]<<", "<<force[ndof*iB+2]<<")"<<std::endl;
        cout<<"fD("<<force[ndof*iD]<<", "<<force[ndof*iD+1]<<", "<<force[ndof*iD+2]<<")"<<std::endl;
        cout<<"fAp("<<force[ndof*iAp]<<", "<<force[ndof*iAp+1]<<", "<<force[ndof*iAp+2]<<")"<<std::endl;
        cout<<"force[ndof*iB] : "<<force[ndof*iB]<<std::endl;

        /*std::cout<<"sigma11 at ndof*iB : "<<sigma11<<std::endl;*/
        std::cout<<"nA : "<<iA<<" getNum(nA) : "<<Mh.getNum(iA)<<std::endl;
        std::cout<<"nB : "<<iB<<" getNum(nB) : "<<Mh.getNum(iB)<<std::endl;
        std::cout<<"nD : "<<iD<<" getNum(nD) : "<<Mh.getNum(iD)<<std::endl;
        std::cout<<"nAp : "<<iAp<<" getNum(nAp) : "<<Mh.getNum(iAp)<<std::endl;}

    PetscFunctionReturn(PETSC_SUCCESS);

}
template <typename T, size_t D>
PetscErrorCode ElasYAnalysisGen(MeshPbc<T,D>& Mh, size_t ndof, BCs& bc,
                                ElementalMatrix& elementalMatrix,
                                std::valarray<T>& disp, std::valarray<T>& force){

    std::vector<pbcEquation> pbcLinks;
        //bc.initialize();
    PetscFunctionBeginUser;
    /*PetscCall(*/scaletag::fem::pbcTaoDispForceGen(Mh, ndof, bc, pbcLinks, elementalMatrix, 1, disp, force)/*)*/;

    MPI_Barrier(MPI_COMM_WORLD);

    int wr;
    MPI_Comm_rank(MPI_COMM_WORLD, &wr);
    if(wr==0){
        size_t iA = bc.getSommets()[0];
        size_t iB = bc.getSommets()[1];
        size_t iD = bc.getSommets()[2];
        size_t iAp = bc.getSommets()[3];

        std::cout<<"disp[ndof*iD+1] : "<<disp[ndof*iD+1]<<std::endl;
        std::cout<<"uA("<<disp[ndof*iA]<<" , "<<disp[ndof*iA+1]<<" , "<<disp[ndof*iA+2]<<")"<<std::endl;
        std::cout<<"uB("<<disp[ndof*iB]<<" , "<<disp[ndof*iB+1]<<" , "<<disp[ndof*iB+2]<<")"<<std::endl;
        std::cout<<"uD("<<disp[ndof*iD]<<" , "<<disp[ndof*iD+1]<<" , "<<disp[ndof*iD+2]<<")"<<std::endl;
        std::cout<<"uAp("<<disp[ndof*iAp]<<" , "<<disp[ndof*iAp+1]<<" , "<<disp[ndof*iAp+2]<<")"<<std::endl;
        cout<<"fA("<<force[ndof*iA]<<", "<<force[ndof*iA+1]<<", "<<force[ndof*iA+2]<<")"<<std::endl;
        cout<<"fB("<<force[ndof*iB]<<", "<<force[ndof*iB+1]<<", "<<force[ndof*iB+2]<<")"<<std::endl;
        cout<<"fD("<<force[ndof*iD]<<", "<<force[ndof*iD+1]<<", "<<force[ndof*iD+2]<<")"<<std::endl;
        cout<<"fAp("<<force[ndof*iAp]<<", "<<force[ndof*iAp+1]<<", "<<force[ndof*iAp+2]<<")"<<std::endl;
        cout<<"force[ndof*iD+1] : "<<force[ndof*iD+1]<<std::endl;

        std::cout<<"nA : "<<iA<<" getNum(nA) : "<<Mh.getNum(iA)<<std::endl;
        std::cout<<"nB : "<<iB<<" getNum(nB) : "<<Mh.getNum(iB)<<std::endl;
        std::cout<<"nD : "<<iD<<" getNum(nD) : "<<Mh.getNum(iD)<<std::endl;
        std::cout<<"nAp : "<<iAp<<" getNum(nAp) : "<<Mh.getNum(iAp)<<std::endl;
    }

    PetscFunctionReturn(PETSC_SUCCESS);
}

template <typename T, size_t D>
PetscErrorCode ElasZAnalysisGen(MeshPbc<T,D>& Mh, size_t ndof, BCs& bc,
                                ElementalMatrix& elementalMatrix,
                                std::valarray<T>& disp, std::valarray<T>& force){
    std::vector<pbcEquation> pbcLinks;
        //bc.initialize();
    PetscFunctionBeginUser;
    /*PetscCall(*/scaletag::fem::pbcTaoDispForceGen(Mh, ndof, bc, pbcLinks, elementalMatrix, 2, disp, force)/*)*/;

    MPI_Barrier(MPI_COMM_WORLD);

    int wr;
    MPI_Comm_rank(MPI_COMM_WORLD, &wr);
    if(wr==0){
        size_t iA = bc.getSommets()[0];
        size_t iB = bc.getSommets()[1];
        size_t iD = bc.getSommets()[2];
        size_t iAp = bc.getSommets()[3];

        std::cout<<"disp[ndof*iAp+2] : "<<disp[ndof*iAp+2]<<std::endl;
        std::cout<<"uA("<<disp[ndof*iA]<<" , "<<disp[ndof*iA+1]<<" , "<<disp[ndof*iA+2]<<")"<<std::endl;
        std::cout<<"uB("<<disp[ndof*iB]<<" , "<<disp[ndof*iB+1]<<" , "<<disp[ndof*iB+2]<<")"<<std::endl;
        std::cout<<"uD("<<disp[ndof*iD]<<" , "<<disp[ndof*iD+1]<<" , "<<disp[ndof*iD+2]<<")"<<std::endl;
        std::cout<<"uAp("<<disp[ndof*iAp]<<" , "<<disp[ndof*iAp+1]<<" , "<<disp[ndof*iAp+2]<<")"<<std::endl;
        cout<<"fA("<<force[ndof*iA]<<", "<<force[ndof*iA+1]<<", "<<force[ndof*iA+2]<<")"<<std::endl;
        cout<<"fB("<<force[ndof*iB]<<", "<<force[ndof*iB+1]<<", "<<force[ndof*iB+2]<<")"<<std::endl;
        cout<<"fD("<<force[ndof*iD]<<", "<<force[ndof*iD+1]<<", "<<force[ndof*iD+2]<<")"<<std::endl;
        cout<<"fAp("<<force[ndof*iAp]<<", "<<force[ndof*iAp+1]<<", "<<force[ndof*iAp+2]<<")"<<std::endl;
        cout<<"force[ndof*iAp+2] : "<<force[ndof*iAp+2]<<std::endl;

        std::cout<<"nA : "<<iA<<" getNum(nA) : "<<Mh.getNum(iA)<<std::endl;
        std::cout<<"nB : "<<iB<<" getNum(nB) : "<<Mh.getNum(iB)<<std::endl;
        std::cout<<"nD : "<<iD<<" getNum(nD) : "<<Mh.getNum(iD)<<std::endl;
        std::cout<<"nAp : "<<iAp<<" getNum(nAp) : "<<Mh.getNum(iAp)<<std::endl;
    }

    PetscFunctionReturn(PETSC_SUCCESS);
}
template <typename T, size_t D>
PetscErrorCode ElasXYAnalysisGen(MeshPbc<T,D>& Mh, size_t ndof, BCs& bc,
                                 ElementalMatrix& elementalMatrix,
                                 std::valarray<T>& disp, std::valarray<T>& force){
    std::vector<pbcEquation> pbcLinks;
        //bc.initialize();
    PetscFunctionBeginUser;
    /*PetscCall(*/scaletag::fem::pbcTaoDispForceGen(Mh, ndof, bc, pbcLinks, elementalMatrix, 3, disp, force)/*)*/;

    MPI_Barrier(MPI_COMM_WORLD);

    int wr;
    MPI_Comm_rank(MPI_COMM_WORLD, &wr);
    if(wr==0){
        size_t iA = bc.getSommets()[0];
        size_t iB = bc.getSommets()[1];
        size_t iD = bc.getSommets()[2];
        size_t iAp = bc.getSommets()[3];

        std::cout<<"disp[ndof*iB+1] : "<<disp[ndof*iB+1]<<std::endl;
        std::cout<<"uA("<<disp[ndof*iA]<<" , "<<disp[ndof*iA+1]<<" , "<<disp[ndof*iA+2]<<")"<<std::endl;
        std::cout<<"uB("<<disp[ndof*iB]<<" , "<<disp[ndof*iB+1]<<" , "<<disp[ndof*iB+2]<<")"<<std::endl;
        std::cout<<"uD("<<disp[ndof*iD]<<" , "<<disp[ndof*iD+1]<<" , "<<disp[ndof*iD+2]<<")"<<std::endl;
        std::cout<<"uAp("<<disp[ndof*iAp]<<" , "<<disp[ndof*iAp+1]<<" , "<<disp[ndof*iAp+2]<<")"<<std::endl;
        cout<<"fA("<<force[ndof*iA]<<", "<<force[ndof*iA+1]<<", "<<force[ndof*iA+2]<<")"<<std::endl;
        cout<<"force[ndof*iB+1] : "<<force[ndof*iB+1]<<std::endl;
    }

    PetscFunctionReturn(PETSC_SUCCESS);

}
template <typename T, size_t D>
PetscErrorCode ElasXZAnalysisGen(MeshPbc<T,D>& Mh, size_t ndof, BCs& bc,
                                 ElementalMatrix& elementalMatrix,
                                 std::valarray<T>& disp, std::valarray<T>& force){
    std::vector<pbcEquation> pbcLinks;
        //bc.initialize();
    PetscFunctionBeginUser;
    /*PetscCall(*/scaletag::fem::pbcTaoDispForceGen(Mh, ndof, bc, pbcLinks, elementalMatrix, 4, disp, force)/*)*/;

    MPI_Barrier(MPI_COMM_WORLD);

    int wr;
    MPI_Comm_rank(MPI_COMM_WORLD, &wr);
    if(wr==0){
        size_t iA = bc.getSommets()[0];
        size_t iB = bc.getSommets()[1];
        size_t iD = bc.getSommets()[2];
        size_t iAp = bc.getSommets()[3];

        std::cout<<"disp[ndof*iB+2] : "<<disp[ndof*iB+2]<<std::endl;
        std::cout<<"uA("<<disp[ndof*iA]<<" , "<<disp[ndof*iA+1]<<" , "<<disp[ndof*iA+2]<<")"<<std::endl;
        std::cout<<"uB("<<disp[ndof*iB]<<" , "<<disp[ndof*iB+1]<<" , "<<disp[ndof*iB+2]<<")"<<std::endl;
        std::cout<<"uD("<<disp[ndof*iD]<<" , "<<disp[ndof*iD+1]<<" , "<<disp[ndof*iD+2]<<")"<<std::endl;
        std::cout<<"uAp("<<disp[ndof*iAp]<<" , "<<disp[ndof*iAp+1]<<" , "<<disp[ndof*iAp+2]<<")"<<std::endl;
        cout<<"fA("<<force[ndof*iA]<<", "<<force[ndof*iA+1]<<", "<<force[ndof*iA+2]<<")"<<std::endl;
        cout<<"force[ndof*iB+2] : "<<force[ndof*iB+2]<<std::endl;
    }

    PetscFunctionReturn(PETSC_SUCCESS);
}
template <typename T, size_t D>
PetscErrorCode ElasYZAnalysisGen(MeshPbc<T,D>& Mh, size_t ndof, BCs& bc,
                                 ElementalMatrix& elementalMatrix,
                                 std::valarray<T>& disp, std::valarray<T>& force){
    std::vector<pbcEquation> pbcLinks;
        //bc.initialize();
    PetscFunctionBeginUser;
    /*PetscCall(*/scaletag::fem::pbcTaoDispForceGen(Mh, ndof, bc, pbcLinks, elementalMatrix, 5, disp, force)/*)*/;

    MPI_Barrier(MPI_COMM_WORLD);

    int wr;
    MPI_Comm_rank(MPI_COMM_WORLD, &wr);
    if(wr==0){
        size_t iA = bc.getSommets()[0];
        size_t iB = bc.getSommets()[1];
        size_t iD = bc.getSommets()[2];
        size_t iAp = bc.getSommets()[3];

        std::cout<<"disp[ndof*iD+2] : "<<disp[ndof*iD+2]<<std::endl;
        std::cout<<"uA("<<disp[ndof*iA]<<" , "<<disp[ndof*iA+1]<<" , "<<disp[ndof*iA+2]<<")"<<std::endl;
        std::cout<<"uB("<<disp[ndof*iB]<<" , "<<disp[ndof*iB+1]<<" , "<<disp[ndof*iB+2]<<")"<<std::endl;
        std::cout<<"uD("<<disp[ndof*iD]<<" , "<<disp[ndof*iD+1]<<" , "<<disp[ndof*iD+2]<<")"<<std::endl;
        std::cout<<"uAp("<<disp[ndof*iAp]<<" , "<<disp[ndof*iAp+1]<<" , "<<disp[ndof*iAp+2]<<")"<<std::endl;
        cout<<"fA("<<force[ndof*iA]<<", "<<force[ndof*iA+1]<<", "<<force[ndof*iA+2]<<")"<<std::endl;
        cout<<"force[ndof*iD+2] : "<<force[ndof*iD+2]<<std::endl;
    }

    PetscFunctionReturn(PETSC_SUCCESS);
}

template <typename T, size_t D>
PetscErrorCode ElasFullAnalysisGen(MeshPbc<T,D>& Mh, size_t ndof, BCs& bc,
                                   ElementalMatrix& elementalMatrix,
                                   std::vector<std::valarray<T>>& disps, std::vector<std::valarray<T>>& forces, T& E11, T& E22, T& E33, T& nu12, T& nu13,
                                   T& nu23, T&G12, T& G13, T& G23){

    std::vector<pbcEquation> pbcLinks;


    disps.resize(6);
    forces.resize(6);

    std::cout<<"Starting full PBC analysis "<<std::endl;

    std::cout<<"Applying exx"<<std::endl;
    //bc.initialize();

    PetscFunctionBeginUser;
    ElasXAnalysisGen(Mh, ndof, bc, elementalMatrix, disps[0], forces[0], E11, nu12, nu13);

    MPI_Barrier(MPI_COMM_WORLD);
    std::cout<<"Applying eyy"<<std::endl;
    //bc.initialize();
    ElasYAnalysisGen(Mh, ndof, bc, elementalMatrix, disps[1], forces[1]);

    MPI_Barrier(MPI_COMM_WORLD);
    std::cout<<"Applying ezz"<<std::endl;
    //bc.initialize();
    ElasZAnalysisGen(Mh, ndof, bc, elementalMatrix, disps[2], forces[2]);

    MPI_Barrier(MPI_COMM_WORLD);
    std::cout<<"Applying exy"<<std::endl;
    //bc.initialize();
    ElasXYAnalysisGen(Mh, ndof, bc, elementalMatrix, disps[3], forces[3]);

    MPI_Barrier(MPI_COMM_WORLD);
    std::cout<<"Applying exz"<<std::endl;
    //bc.initialize();
    ElasXZAnalysisGen(Mh, ndof, bc, elementalMatrix, disps[4], forces[4]);

    MPI_Barrier(MPI_COMM_WORLD);
    std::cout<<"Applying eyz"<<std::endl;
    //bc.initialize();
    ElasYZAnalysisGen(Mh, ndof, bc, elementalMatrix, disps[5], forces[5]);

    std::cout<<"End of all 6 loadings."<<std::endl;

    MPI_Barrier(MPI_COMM_WORLD);

    double delta =  bc.getLoadingType();
    //Retrieving the tiling directions


    int wr;
    MPI_Comm_rank(MPI_COMM_WORLD, &wr);
    if(wr==0){
        try{
            if(bc.getPbcUtilities()==nullptr) {std::cout<<"BCs class must have a pbcUtilities object as member"<<std::endl; throw 0; }
            //if(bc.getPbcUtilities()->getTilingDirs().size() != 3) {std::cout<<"Tiling directions are not correctly set in pbcUtilities"<<std::endl; throw 1;}
            if(bc.getTilingDirs().size() != 3) {std::cout<<"Tiling directions are not correctly set in pbcUtilities"<<std::endl; throw 1;}
        }
        catch(int err){
            PetscFunctionReturn(PETSC_ERR_ARG_WRONG);
        }
//    std::vector<double> a1 = bc.getPbcUtilities()->getTilingDirs()[0];
//    std::vector<double> a2 = bc.getPbcUtilities()->getTilingDirs()[1];
//    std::vector<double> a3 = bc.getPbcUtilities()->getTilingDirs()[2];
        std::vector<double> a1 = bc.getTilingDirs()[0];
        std::vector<double> a2 = bc.getTilingDirs()[1];
        std::vector<double> a3 = bc.getTilingDirs()[2];

    boost::qvm::mat<double, 3, 3> A =  {a1[0], a1[1], a1[2],   a2[0], a2[1], a2[2],   a3[0], a3[1], a3[2]};
    double V = std::fabs(boost::qvm::determinant(A));

    auto A_Inv = boost::qvm::inverse(A);
    boost::qvm::mat<double,3,3> A_Tr = boost::qvm::transposed(A);

//UXI/RXI --> I loading case
    double UBx, UBy, UBz, UDx, UDy, UDz, UApx, UApy, UApz;
    double RBx, RBy, RBz, RDx, RDy, RDz, RApx, RApy, RApz;

    size_t iA = bc.getSommets()[0];
    size_t iB = bc.getSommets()[1];
    size_t iD = bc.getSommets()[2];
    size_t iAp = bc.getSommets()[3];

    //Deformation exx;
    UBx = disps[0][ndof*iB]; UBy = disps[0][ndof*iB+1]; UBz = disps[0][ndof*iB+2];
    UDx = disps[0][ndof*iD]; UDy = disps[0][ndof*iD+1]; UDz = disps[0][ndof*iD+2];
    UApx = disps[0][ndof*iAp]; UApy = disps[0][ndof*iAp+1]; UApz = disps[0][ndof*iAp+2];

    RBx = forces[0][ndof*iB]; RBy = forces[0][ndof*iB+1]; RBz = forces[0][ndof*iB+2];
    RDx = forces[0][ndof*iD]; RDy = forces[0][ndof*iD+1]; RDz = forces[0][ndof*iD+2];
    RApx = forces[0][ndof*iAp]; RApy = forces[0][ndof*iAp+1]; RApz = forces[0][ndof*iAp+2];

    boost::qvm::mat<double, 3,3> U0 = {UBx, UBy, UBz,    UDx, UDy, UDz,     UApx, UApy, UApz} ,
        R0 = {RBx, RBy, RBz,   RDx, RDy, RDz,   RApx, RApy, RApz};

    boost::qvm::mat<double, 3,3> epsbar0 = A_Inv*U0, sigbar0 = (1./V)*(A_Tr*R0) ;
    epsbar0=0.5*(epsbar0+boost::qvm::transposed(epsbar0));

    //Deformation eyy;
    UBx = disps[1][ndof*iB]; UBy = disps[1][ndof*iB+1]; UBz = disps[1][ndof*iB+2];
    UDx = disps[1][ndof*iD]; UDy = disps[1][ndof*iD+1]; UDz = disps[1][ndof*iD+2];
    UApx = disps[1][ndof*iAp]; UApy = disps[1][ndof*iAp+1]; UApz = disps[1][ndof*iAp+2];

    RBx = forces[1][ndof*iB]; RBy = forces[1][ndof*iB+1]; RBz = forces[1][ndof*iB+2];
    RDx = forces[1][ndof*iD]; RDy = forces[1][ndof*iD+1]; RDz = forces[1][ndof*iD+2];
    RApx = forces[1][ndof*iAp]; RApy = forces[1][ndof*iAp+1]; RApz = forces[1][ndof*iAp+2];

    boost::qvm::mat<double, 3,3> U1 = {UBx, UBy, UBz,  UDx, UDy, UDz,   UApx, UApy, UApz} ,
        R1 = {RBx, RBy, RBz,   RDx, RDy, RDz,   RApx, RApy, RApz};

    boost::qvm::mat<double, 3,3> epsbar1 = A_Inv*U1, sigbar1 = (1./V)*(A_Tr*R1) ;
    epsbar1 = 0.5*(epsbar1+ boost::qvm::transposed(epsbar1));

    //Deformation ezz;
    UBx = disps[2][ndof*iB]; UBy = disps[2][ndof*iB+1]; UBz = disps[2][ndof*iB+2];
    UDx = disps[2][ndof*iD]; UDy = disps[2][ndof*iD+1]; UDz = disps[2][ndof*iD+2];
    UApx = disps[2][ndof*iAp]; UApy = disps[2][ndof*iAp+1]; UApz = disps[2][ndof*iAp+2];

    RBx = forces[2][ndof*iB]; RBy = forces[2][ndof*iB+1]; RBz = forces[2][ndof*iB+2];
    RDx = forces[2][ndof*iD]; RDy = forces[2][ndof*iD+1]; RDz = forces[2][ndof*iD+2];
    RApx = forces[2][ndof*iAp]; RApy = forces[2][ndof*iAp+1]; RApz = forces[2][ndof*iAp+2];

    boost::qvm::mat<double, 3,3> U2 = {UBx, UBy, UBz,   UDx, UDy, UDz,  UApx, UApy, UApz} ,
        R2 = {RBx, RBy, RBz,  RDx, RDy, RDz,   RApx, RApy, RApz};

    boost::qvm::mat<double, 3,3> epsbar2 = A_Inv*U2, sigbar2 = (1./V)*(A_Tr*R2) ;
    epsbar2 = 0.5*(epsbar2+ boost::qvm::transposed(epsbar2));

    //Deformation exy;
    UBx = disps[3][ndof*iB]; UBy = disps[3][ndof*iB+1]; UBz = disps[3][ndof*iB+2];
    UDx = disps[3][ndof*iD]; UDy = disps[3][ndof*iD+1]; UDz = disps[3][ndof*iD+2];
    UApx = disps[3][ndof*iAp]; UApy = disps[3][ndof*iAp+1]; UApz = disps[3][ndof*iAp+2];

    RBx = forces[3][ndof*iB]; RBy = forces[3][ndof*iB+1]; RBz = forces[3][ndof*iB+2];
    RDx = forces[3][ndof*iD]; RDy = forces[3][ndof*iD+1]; RDz = forces[3][ndof*iD+2];
    RApx = forces[3][ndof*iAp]; RApy = forces[3][ndof*iAp+1]; RApz = forces[3][ndof*iAp+2];

    boost::qvm::mat<double, 3,3> U3 = {UBx, UBy, UBz,   UDx, UDy, UDz,   UApx, UApy, UApz} ,
        R3 = {RBx, RBy, RBz,   RDx, RDy, RDz,   RApx, RApy, RApz};

    boost::qvm::mat<double, 3,3> epsbar3 = A_Inv*U3, sigbar3 = (1./V)*(A_Tr*R3) ;
    epsbar3 = 0.5*(epsbar3+ boost::qvm::transposed(epsbar3));

    //Deformation exz;
    UBx = disps[4][ndof*iB]; UBy = disps[4][ndof*iB+1]; UBz = disps[4][ndof*iB+2];
    UDx = disps[4][ndof*iD]; UDy = disps[4][ndof*iD+1]; UDz = disps[4][ndof*iD+2];
    UApx = disps[4][ndof*iAp]; UApy = disps[4][ndof*iAp+1]; UApz = disps[4][ndof*iAp+2];

    RBx = forces[4][ndof*iB]; RBy = forces[4][ndof*iB+1]; RBz = forces[4][ndof*iB+2];
    RDx = forces[4][ndof*iD]; RDy = forces[4][ndof*iD+1]; RDz = forces[4][ndof*iD+2];
    RApx = forces[4][ndof*iAp]; RApy = forces[4][ndof*iAp+1]; RApz = forces[4][ndof*iAp+2];

    boost::qvm::mat<double, 3,3> U4 = {UBx, UBy, UBz,   UDx, UDy, UDz,   UApx, UApy, UApz} ,
        R4 = {RBx, RBy, RBz,   RDx, RDy, RDz,   RApx, RApy, RApz};

    boost::qvm::mat<double, 3,3> epsbar4 = A_Inv*U4, sigbar4 = (1./V)*(A_Tr*R4) ;
    epsbar4 = 0.5*(epsbar4+ boost::qvm::transposed(epsbar4));

    //Deformation eyz;
    UBx = disps[5][ndof*iB]; UBy = disps[5][ndof*iB+1]; UBz = disps[5][ndof*iB+2];
    UDx = disps[5][ndof*iD]; UDy = disps[5][ndof*iD+1]; UDz = disps[5][ndof*iD+2];
    UApx = disps[5][ndof*iAp]; UApy = disps[5][ndof*iAp+1]; UApz = disps[5][ndof*iAp+2];

    RBx = forces[5][ndof*iB]; RBy = forces[5][ndof*iB+1]; RBz = forces[5][ndof*iB+2];
    RDx = forces[5][ndof*iD]; RDy = forces[5][ndof*iD+1]; RDz = forces[5][ndof*iD+2];
    RApx = forces[5][ndof*iAp]; RApy = forces[5][ndof*iAp+1]; RApz = forces[5][ndof*iAp+2];

    boost::qvm::mat<double, 3,3> U5 = {UBx, UBy, UBz,   UDx, UDy, UDz,   UApx, UApy, UApz} ,
        R5 = {RBx, RBy, RBz,   RDx, RDy, RDz,   RApx, RApy, RApz};

    boost::qvm::mat<double, 3,3> epsbar5 = A_Inv*U5, sigbar5 = (1./V)*(A_Tr*R5) ;
    epsbar5 = 0.5*(epsbar5+ boost::qvm::transposed(epsbar5));


    boost::qvm::mat<double, 6, 6> epsbar = { epsbar0.a[0][0], epsbar0.a[1][1], epsbar0.a[2][2], 2.*epsbar0.a[0][1], 2.*epsbar0.a[0][2], 2.*epsbar0.a[1][2],
                                      epsbar1.a[0][0], epsbar1.a[1][1], epsbar1.a[2][2], 2.*epsbar1.a[0][1], 2.*epsbar1.a[0][2], 2.*epsbar1.a[1][2],
                                      epsbar2.a[0][0], epsbar2.a[1][1], epsbar2.a[2][2], 2.*epsbar2.a[0][1], 2.*epsbar2.a[0][2], 2.*epsbar2.a[1][2],
                                      epsbar3.a[0][0], epsbar3.a[1][1], epsbar3.a[2][2], 2.*epsbar3.a[0][1], 2.*epsbar3.a[0][2], 2.*epsbar3.a[1][2],
                                      epsbar4.a[0][0], epsbar4.a[1][1], epsbar4.a[2][2], 2.*epsbar4.a[0][1], 2.*epsbar4.a[0][2], 2.*epsbar4.a[1][2],
                                      epsbar5.a[0][0], epsbar5.a[1][1], epsbar5.a[2][2], 2.*epsbar5.a[0][1], 2.*epsbar5.a[0][2], 2.*epsbar5.a[1][2]
                                  },

        sigbar = { sigbar0.a[0][0], sigbar0.a[1][1], sigbar0.a[2][2], sigbar0.a[0][1], sigbar0.a[0][2], sigbar0.a[1][2],
            sigbar1.a[0][0], sigbar1.a[1][1], sigbar1.a[2][2], sigbar1.a[0][1], sigbar1.a[0][2], sigbar1.a[1][2],
            sigbar2.a[0][0], sigbar2.a[1][1], sigbar2.a[2][2], sigbar2.a[0][1], sigbar2.a[0][2], sigbar2.a[1][2],
            sigbar3.a[0][0], sigbar3.a[1][1], sigbar3.a[2][2], sigbar3.a[0][1], sigbar3.a[0][2], sigbar3.a[1][2],
            sigbar4.a[0][0], sigbar4.a[1][1], sigbar4.a[2][2], sigbar4.a[0][1], sigbar4.a[0][2], sigbar4.a[1][2],
            sigbar5.a[0][0], sigbar5.a[1][1], sigbar5.a[2][2], sigbar5.a[0][1], sigbar5.a[0][2], sigbar5.a[1][2]
        } ;



    boost::qvm::mat<double, 6,6> C =(boost::qvm::transposed(sigbar)) * (boost::qvm::transposed(boost::qvm::inverse(epsbar)));
    boost::qvm::mat<double, 6,6> S = boost::qvm::inverse(C);


    E11 = 1./S.a[0][0]; E22 = 1./S.a[1][1]; E33 = 1./S.a[2][2];
    nu12 = -1*S.a[0][1]*E11 ;  nu13 = -1.*S.a[0][2]*E11 ;  nu23 = -1.*S.a[1][2]*E22 ;
    G12 = 1./S.a[3][3]; G13 = 1./S.a[4][4];  G23 = 1./S.a[5][5];



    std::cout<<"E11 : "<<E11<<" E22 : "<<E22<<" E33 : "<<E33<<"\n"
    <<" nu12 : "<<nu12<<" nu13 : "<<nu13<<" nu23 : "<<nu23<<"\n"
              <<" G12 : "<<G12<<" G13 : "<<G13<<" G23 : "<<G23<<std::endl;

/*    std::cout<<"nA : "<<iA<<" getNum(nA) : "<<Mh.getNum(iA)<<std::endl;
    std::cout<<"nB : "<<iB<<" getNum(nB) : "<<Mh.getNum(iB)<<std::endl;
    std::cout<<"nD : "<<iD<<" getNum(nD) : "<<Mh.getNum(iD)<<std::endl;
    std::cout<<"nAp : "<<iAp<<" getNum(nAp) : "<<Mh.getNum(iAp)<<std::endl;*/}






PetscFunctionReturn(PETSC_SUCCESS);

}

}}
