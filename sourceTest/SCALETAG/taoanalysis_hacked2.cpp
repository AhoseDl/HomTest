   // 1. Définition du PC global en FieldSplit
    ierr = KSPGetPC(ksp, &pc); CHKERRQ(ierr);
    ierr = PCSetType(pc, PCFIELDSPLIT); CHKERRQ(ierr);

    // Définition manuelle des deux champs
    IS is_row[2];
    MatNestGetISs(A, is_row, NULL);
    PCFieldSplitSetIS(pc, "champ_0", is_row[0]);
    PCFieldSplitSetIS(pc, "champ_1", is_row[1]);

    // 2. Activation du mode Schur
    ierr = PCFieldSplitSetType(pc, PC_COMPOSITE_SCHUR); CHKERRQ(ierr);
    ierr = PCFieldSplitSetSchurFactType(pc, PC_FIELDSPLIT_SCHUR_FACT_FULL); CHKERRQ(ierr);
    ierr = PCFieldSplitSetSchurPre(pc, PC_FIELDSPLIT_SCHUR_PRE_A11, NULL); CHKERRQ(ierr);

    // =========================================================================
    // INJECTION PROGRAMMATIQUE DES OPTIONS (Évite le besoin de PCFieldSplitGetSubKSP)
    // =========================================================================
    // Récupération de la base d'options locale attachée au PC ou globale
    PetscOptions options;
    PetscObjectGetOptions((PetscObject)ksp, &options);

    // CONFIGURATION DU CHAMP 0 (Rigidité -> MUMPS)
    // Équivalent à la ligne de commande : -fieldsplit_champ_0_ksp_type preonly etc.
    PetscOptionsSetValue(options, "-fieldsplit_champ_0_ksp_type", "preonly");
    PetscOptionsSetValue(options, "-fieldsplit_champ_0_pc_type", "lu");
    PetscOptionsSetValue(options, "-fieldsplit_champ_0_pc_factor_mat_solver_type", "mumps");

    // CONFIGURATION DU CHAMP 1 (Schur / Lagrange -> GMRES + PCNONE)
    // C'est ce "-fieldsplit_champ_1_pc_type none" qui supprime définitivement le NANORINF !
    PetscOptionsSetValue(options, "-fieldsplit_champ_1_ksp_type", "fgmres"); //gmres
    PetscOptionsSetValue(options, "-fieldsplit_champ_1_pc_type", "none");//none
    PetscOptionsSetValue(options, "-fieldsplit_champ_1_ksp_rtol", "1e-14");
    PetscOptionsSetValue(options, "-fieldsplit_champ_1_ksp_max_it", "1000");

    // 3. Application des options et configuration globale
    ierr = KSPSetType(ksp, KSPGMRES); CHKERRQ(ierr);
    KSPSetTolerances(ksp, 0, 1.e-10, PETSC_DEFAULT, 1000);

    ierr = KSPSetFromOptions(ksp); CHKERRQ(ierr); // Applique les options injectées ci-dessus

    // 4. Initialisation sécurisée
    // PETSc va lire les options forcées (mumps et none), le PCSetUp ne crashera pas !
    ierr = PCSetUp(pc); CHKERRQ(ierr);
