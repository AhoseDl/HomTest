    // Récupération du préconditionneur pour appliquer le découpage de champs
    ierr = KSPGetPC(ksp, &pc); CHKERRQ(ierr);
    ierr = PCSetType(pc, PCFIELDSPLIT/*PCLU*/); CHKERRQ(ierr);
    ierr = PCFieldSplitSetType(pc, PC_COMPOSITE_SCHUR); CHKERRQ(ierr);


    //    Mat SPre;
    //    MatDuplicate(LL, MAT_COPY_VALUES, &SPre);
    //    MatShift(SPre, 1);
    // 2. Indiquer à PETSc comment construire l'opérateur du complément de Schur.
    // En Lagrange pur, on utilise généralement le mode "USER" ou "DIAG" pour former le préconditionneur
    // PCFieldSplitSetSchurPre(pc, PC_FIELDSPLIT_SCHUR_PRE_USER, NULL);
    // CORRECTION CRUCIALE : On utilise PC_FIELDSPLIT_SCHUR_PRE_A00 (qui correspond à K_uu)
    // Au lieu de USER ou DIAG, cela empêche PETSc de toucher à la matrice nulle K_cc.
    PCFieldSplitSetSchurPre(pc, PC_FIELDSPLIT_SCHUR_PRE_A11, NULL/*SPre*/);
    // IMPORTANT : Définir le type de factorisation globale de Schur sur FULL
    // Cela force un couplage fort et stable pour les points-selle
    PCFieldSplitSetSchurFactType(pc, PC_FIELDSPLIT_SCHUR_FACT_FULL);
    //ierr = PCSetType(pc, PCLU); CHKERRQ(ierr);
    ierr=KSPSetType(ksp, KSPGMRES); CHKERRQ(ierr);
    //ierr=KSPSetType(ksp, KSPPREONLY); CHKERRQ(ierr);
    // ierr = KSPSetFromOptions(ksp); CHKERRQ(ierr);
    //    KSPSetTolerances(ksp, 1e-5, PETSC_DEFAULT, PETSC_DEFAULT, 1000);

    KSPSetTolerances(ksp, 0, 1.e-4, PETSC_DEFAULT, 1000);

    IS is_row[2];
    MatNestGetISs(A, is_row, NULL);
    PCFieldSplitSetIS(pc, "champ_0", is_row[0]); //
    PCFieldSplitSetIS(pc, "champ_1", is_row[1]); //
    ierr = KSPSetFromOptions(ksp); CHKERRQ(ierr);
    //    ierr = PCSetUp(pc); CHKERRQ(ierr);

    PetscBarrier((PetscObject)petscKG);

    ierr = PCFieldSplitGetSubKSP(pc, &nf, &subksp); CHKERRQ(ierr); // It is important to invoke PCFieldSplitSetIS() before PCSetup()
    //ierr = PCFieldSplitSchurGetSubKSP(pc, &nf, &subksp); CHKERRQ(ierr);
    std::cout<<"Number of fields : "<< nf<<std::endl;

    PetscBarrier((PetscObject)petscKG);
    //Subsolvers configuration
    for (PetscInt i = 0; i < nf; i++) {
   PC sub_pc;

   if (i == 1) {
            // Configuration du Bloc 0 (ex: Vitesse / Pression principal)
            KSPSetType(subksp[i], KSPGMRES);
            KSPGetPC(subksp[i], &sub_pc);
            PCSetType(sub_pc, /*PCILU*//*PCJACOBI*/PCNONE);
            KSPSetTolerances(subksp[i], /*1e-4*/0, 1e-4, PETSC_DEFAULT, /*PETSC_DEFAULT*/1000);
   }
   else if (i == 0) {
            // Configuration du Bloc 1 (ex: Complément de Schur ou second champ)
            KSPSetType(subksp[i], /*KSPCG*/KSPPREONLY);
            KSPGetPC(subksp[i], &sub_pc);
            PCSetType(sub_pc, /*PCJACOBI*/PCLU);
            // 4. Spécifier le package externe MUMPS
            PCFactorSetMatSolverType(sub_pc, MATSOLVERMUMPS);
            KSPSetTolerances(subksp[i], /*1e-4*/0, 1e-4, PETSC_DEFAULT, /*PETSC_DEFAULT*/1000); }

   // Forcer la mise à jour des paramètres du sous-solveur
   KSPSetUp(subksp[i]);
    }

    // 4. Libérer le tableau alloué par PETSc (les objets KSP sous-jacents restent intacts)
    PetscFree(subksp);

    ierr = PCSetUp(pc); CHKERRQ(ierr);
