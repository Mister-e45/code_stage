/**
 * \file methode_PGE.cpp
 * \author 
 * \date 13 août 2025
 *
 * \brief Intégré depuis un code source C/C++
 * 
 * <div class="tabs">\n\n</div>
 * 
 * TODO : compléter la documentation
 * 
 * \section StructureDuTraitement Structure du traitement
 *    \subsection Paramètres Paramètres
 *      \li <Nom du paramètre> : <description> / Type : <type du paramètre>
 * 
 *    \subsection Entrées Entrées
 *      \li <Nom du paramètre> : <description> / Type : <type du paramètre>
 * 
 *    \subsection Sorties Sorties
 *      \li <Nom du paramètre> : <description> / Type : <type du paramètre>
 * 
 * \section Descriptiondutraitement Description du traitement
 * 
 *\n
 * <div class="tabs">\n\n</div>
 */

/*
 * ==========================================================================
 * NOTE : Pour plus de détails sur l'implémentation de traitements, cf. NTC01
 * ==========================================================================
 */

/*
 * NOTE : Tout au long du template, on prendra comme un exemple un traitement
 * avec :
 *  - 2 ports de paramètres de type :
 *    - double ;
 *    - chaine de caractère ;
 *  - 2 entrées de type :
 *    - entier ;
 *    - une union entre les types entier, vecteur d'entier et matrice d'entier ;
 *  - un nombre variable de sortie (suivant un attribut N), toutes de type 
 *    complexe.
 */

#include <methode_PGE.h>
#define STOPFIRE ((Methode_PGE*) treatmentInstance->userdata)->stopFire
/*
 * TODO : ajouter les includes vers les fichiers dont vous souhaitez utiliser
 * les fonctions.
 */

/*
 * FIXME : si vous souhaitez faire persister des données d'une itération sur 
 * l'autre vous pouvez déclarer une structure avec des champs personnalisés. En 
 * l'initialisant dans la fonction preinit ou init, et en la finalisant dans la
 * fonction wrapup, vous pourrez y accéder dans la fonction fire en faisant 
 * persister les données d'une itération à l'autre (mais pas d'une exécution 
 * de scénario à l'autre).
 * 
 * Pour accéder plus facilement aux différents champs de la structure, vous 
 * pouvez définir des macros d'accès de la manière suivante :
 */
//#define METHODE_PGE_DATAS_FIELD1 ((TmtMethode_PGEData*)treatmentInstance->userdata)->field1
//#define METHODE_PGE_DATAS_FIELD2 ((TmtMethode_PGEData*)treatmentInstance->userdata)->field2
//#define METHODE_PGE_DATAS_FIELD3 ((TmtMethode_PGEData*)treatmentInstance->userdata)->field3
//#define METHODE_PGE_DATAS_FIELD4 ((TmtMethode_PGEData*)treatmentInstance->userdata)->field4
////etc...
/* 
 * Exemple de structure:
 */
//typedef struct {
//  int field1; /**< Champ 1 de type entier */
//  void * field2; /**< Champ 2 de type void* */
//  DoubleBlock field3; /**< Champ 3 de type DoubleBlock, permettant par exemple de garder la valeur d'un paramètre
//  TAlsToken field4; /**< Champ 4 de type TAlsToken
//  // etc...
//} TmtMethode_PGEData;

/**
 * \brief Obtient le modèle de traitement
 * \param templatetmt modèle de traitement initial, servant notamment à
 * obtenir les attributs
 */
void ALOAS_TMT_METHODE_PGE_API methode_PGE_buildTemplate(void* templatetmt) {


    TAlsTemplateType BitType,IntType, DoubleType;
    als_tmttpt_initializeType(&BitType);
    als_tmttpt_initializeType(&IntType);
    als_tmttpt_initializeType(&DoubleType);

    als_tmttpt_resetPort(templatetmt);

    als_tmttpt_createBasicType(&BitType, ATBT_BIT);
    als_tmttpt_createBasicType(&IntType, ATBT_INT);
    als_tmttpt_createBasicType(&DoubleType, ATBT_DOUBLE);

    TAlsTemplatePort* portLongueurCode = als_tmttpt_createParameterPort(templatetmt, L"Longueur Du Code");
    TAlsTemplatePort* portNombreMotsRecus = als_tmttpt_createParameterPort(templatetmt, L"Nombre de mots recus");
    TAlsTemplatePort* portNombreMotsUtilises = als_tmttpt_createParameterPort(templatetmt, L"Nombre de mots a utiliser dans PGE");
    TAlsTemplatePort* portTauxErreur = als_tmttpt_createParameterPort(templatetmt, L"Taux d'erreur");
    TAlsTemplatePort* port_t = als_tmttpt_createParameterPort(templatetmt, L"Nombre de Ligne a echelonner (t)");
    TAlsTemplatePort* port_p = als_tmttpt_createParameterPort(templatetmt, L"Nombre de colonnes a collisionner (p)");
    TAlsTemplatePort* portNombreLimiteIterationRechercheVecteur = als_tmttpt_createParameterPort(templatetmt, L"Nombre maximal d'iterations de recherche de vecteur");


    TAlsTemplatePort* inputPortMotsRecus  = als_tmttpt_createInputDataPort(templatetmt,L"U1");
    TAlsTemplatePort* outputPort = als_tmttpt_createOutputPort(templatetmt, L"Y1");


    als_tmttpt_setTypeEquals(portLongueurCode, &IntType);
    als_tmttpt_setTypeEquals(portNombreMotsRecus, &IntType);
    als_tmttpt_setTypeEquals(portNombreMotsUtilises, &IntType);
    als_tmttpt_setTypeEquals(portTauxErreur, &DoubleType);
    als_tmttpt_setTypeEquals(port_t, &IntType);
    als_tmttpt_setTypeEquals(port_p, &IntType);
    als_tmttpt_setTypeEquals(portNombreLimiteIterationRechercheVecteur, &IntType);

    als_tmttpt_setTypeAtMost(inputPortMotsRecus, &BitType);
    als_tmttpt_setTypeAtLeast(outputPort, &BitType);

    als_tmttpt_releaseType(&BitType);
    als_tmttpt_releaseType(&IntType);
    als_tmttpt_releaseType(&DoubleType);
 
}

/**
 * \brief Pre-initialise le traitement
 * \param treatmentInstance accès à l'instance du traitement
 */
void ALOAS_TMT_METHODE_PGE_API methode_PGE_preinitialize(TAlsTIStruct* treatmentInstance) {
    treatmentInstance->userdata = malloc(sizeof (Methode_PGE));
    STOPFIRE = false;
    /** \n <b> Structure de la fonction </b> : \li Début de la fonction */

    /*
     * FIXME : si vous avez décidé de conserver des données d'une itération sur 
     * l'autre, c'est ici que la structure doit être allouée et placée dans 
     * l'instance de traitement.
     * 
     * Exemple :
     */
    // treatmentInstance->userdata = new TmtMethode_PGEData;
    // // Initialisation des champs de la structure
    // METHODE_PGE_DATAS_FIELD1 = 0;
    // METHODE_PGE_DATAS_FIELD2 = 0;
    // als_data_initializeDoubleBloc(&METHODE_PGE_DATAS_FIELD3);
    // als_data_initializeToken(&METHODE_PGE_DATAS_FIELD4, ADT_UNKNOWN);

    /*
     * NOTE : toutes les fonctions de préinitialisation sont appelées avant de 
     * démarrer l'exécution d'un scénario. Cette phase peut servir à générer des 
     * tokens d'initialisation à destination de la phase "initialize" de 
     * traitements connexes.
     */

    /** \li Fin de la fonction \n*/
}

/**
 * \brief Initialise le traitement
 * \param treatmentInstance accès à l'instance du traitement
 */
void ALOAS_TMT_METHODE_PGE_API methode_PGE_initialize(TAlsTIStruct* treatmentInstance) {
    /** \n <b> Structure de la fonction </b> : \li Début de la fonction */

    /*
     * NOTE : toutes les fonctions d'initialisation sont appelées après les 
     * fonctions de préinitialisation, mais avant l'exécution du scénario. Cette 
     * phase peut servir à utiliser des tokens d'initialisation générés dans la
     * phase "preinitialize" de traitements connexes.
     */

    /*
     * NOTE : cette phase peut également être utilisée pour modifier le taux de 
     * consommation des données pour les entrées. La fonction à utiliser est 
     * als_ifc_setConsumptionRate(TAlsReceiver* receiver, int value), et les 
     * différentes valeurs possibles sont :
     *  -> -1 pour ne pas ralentir l'exécution de traitement en amont s'il n'est 
     *    pas nécessaire d'avoir des données sur le port ;
     *  -> 0 s'il n'est pas nécessaire d'avoir des données sur le port ;
     *  -> X s'il faut de toutes façons au moins X échantillon sur le port pour 
     *    s'exécuter.
     * 
     * Exemple, passage du CS rate du port d'entrée d'indice 1 à 0 :
     */
    // TAlsReceiver * inputPort = als_ifc_getInputDataReceiver(treatmentInstance, 1);
    // als_ifc_setConsumptionRate(inputPort, 0);

    /** \li Fin de la fonction \n*/
}

/**
 * \brief Prépare l'exécution du traitement
 * \param treatmentInstance accès à l'instance du traitement
 * \return vrai si le traitement peut s'executer, faux sinon.
 */
char ALOAS_TMT_METHODE_PGE_API methode_PGE_prefire(TAlsTIStruct* treatmentInstance) {
    /** \n <b> Structure de la fonction </b> : \li Début de la fonction */

    /*
     * NOTE : à chaque itération, si le traitement est éligible, la fonction
     * prefire est appelée afin de savoir si la fonction fire pourra être appelée.
     * La fonction prefire doit rendre vrai si le traitement peut s'exécuter, 
     * faux sinon.
     * 
     * En général, on vérifie que suffisamment de données sont présentes sur les
     * ports de paramètres et d'entrées pour pouvoir lancer une exécution.
     */

    int result = 1;

    /** \li On regarde si suffisamment de données sont présents sur tous les ports
     * d'entrée \n*/

    /*
     * FIXME : vérifier que le code générique ci-dessous correspond à vos besoin.
     * Ce dernier vérifie que le nombre d'échantillons sur chaque port d'entrée du 
     * traitement (paramètre ou donnée) est supérieur à 0.
     */
    int count = als_ifc_getInputPortsCount(treatmentInstance);
    for (int i = 0; i < count; ++i) {
        result &= 0 < als_ifc_getSampleCount(als_ifc_getInputReceiver(treatmentInstance, i));
    }

    /** \li Fin de la fonction \n*/
    return result > 0;
}

/**
 * \brief Exécute le traitement
 * \param treatmentInstance accès à l'instance du traitement
 */
void ALOAS_TMT_METHODE_PGE_API methode_PGE_fire(TAlsTIStruct* treatmentInstance) {
    TAlsReceiver *port_n=als_ifc_getParamReceiver(treatmentInstance,0);
    TAlsReceiver *port_numRecus=als_ifc_getParamReceiver(treatmentInstance,1);
    TAlsReceiver *port_numUtilise=als_ifc_getParamReceiver(treatmentInstance,2);
    TAlsReceiver *port_tauxErreur=als_ifc_getParamReceiver(treatmentInstance,3);
    TAlsReceiver *port_t=als_ifc_getParamReceiver(treatmentInstance,4);
    TAlsReceiver *port_p=als_ifc_getParamReceiver(treatmentInstance,5);
    TAlsReceiver *port_maxIterationProcedure=als_ifc_getParamReceiver(treatmentInstance,6);

    TAlsReceiver *port_entree=als_ifc_getInputDataReceiver(treatmentInstance, 0);
    TAlsReceiver *port_sortie=als_ifc_getOutputReceiver(treatmentInstance, 0);

    IntBlock n_param, numRecus_param, numUtilises_param, t_param, p_param, maxIterProcedure_param;
    DoubleBlock tauxErreur_param;

    als_data_initializeIntBlock(&n_param);
    als_data_initializeIntBlock(&numRecus_param);
    als_data_initializeIntBlock(&numUtilises_param);
    als_data_initializeDoubleBlock(&tauxErreur_param);
    als_data_initializeIntBlock(&t_param);
    als_data_initializeIntBlock(&p_param);
    als_data_initializeIntBlock(&maxIterProcedure_param);

    if (als_ifc_getSampleCount(port_n) <= 0){
      STOPFIRE = true;
      als_ifc_error(treatmentInstance, ALS_USER_ERROR, L"Le parametre <Longueur Du Code> n'a pas été récupéré.");
    }
    else if (als_ifc_getSampleCount(port_numRecus) <= 0){
      STOPFIRE = true;
      als_ifc_error(treatmentInstance, ALS_USER_ERROR, L"Le parametre <Nombre de mots recus> n'a pas été récupéré.");
    }
    else if (als_ifc_getSampleCount(port_numUtilise) <= 0){
      STOPFIRE = true;
      als_ifc_error(treatmentInstance, ALS_USER_ERROR, L"Le parametre <Nombre de mots a utiliser dans PGE> n'a pas été récupéré.");
    }
    else if (als_ifc_getSampleCount(port_tauxErreur) <= 0){
      STOPFIRE = true;
      als_ifc_error(treatmentInstance, ALS_USER_ERROR, L"Le parametre <Taux d'erreur> n'a pas été récupéré.");
    }
    else if (als_ifc_getSampleCount(port_t) <= 0){
      STOPFIRE = true;
      als_ifc_error(treatmentInstance, ALS_USER_ERROR, L"Le parametre <Nombre de Ligne a echelonner (t)> n'a pas été récupéré.");
    }
    else if (als_ifc_getSampleCount(port_p) <= 0){
      STOPFIRE = true;
      als_ifc_error(treatmentInstance, ALS_USER_ERROR, L"Le parametre <Nombre de colonnes a collisionner (p)> n'a pas été récupéré.");
    }
    else if (als_ifc_getSampleCount(port_maxIterationProcedure) <= 0){
      STOPFIRE = true;
      als_ifc_error(treatmentInstance, ALS_USER_ERROR, L"Le parametre <Nombre maximal d'iterations de recherche de vecteur> n'a pas été récupéré.");
    }
    else{
        if (!als_ifc_getParameterIntBlock(treatmentInstance, 0, &n_param)){
            STOPFIRE = true ; 
            als_ifc_error(treatmentInstance, ALS_USER_ERROR, L"Le parametre <Longueur Du Code> n'a pas été récupéré.");
        }
        if (!als_ifc_getParameterIntBlock(treatmentInstance, 1, &numRecus_param)){
            STOPFIRE = true ; 
            als_ifc_error(treatmentInstance, ALS_USER_ERROR, L"Le parametre <Nombre de mots recus> n'a pas été récupéré.");
        }
        if (!als_ifc_getParameterIntBlock(treatmentInstance, 2, &numUtilises_param)){
            STOPFIRE = true ; 
            als_ifc_error(treatmentInstance, ALS_USER_ERROR, L"Le parametre <Nombre de mots a utiliser dans PGE> n'a pas été récupéré.");
        }
        if (!als_ifc_getParameterDoubleBlock(treatmentInstance, 3, &tauxErreur_param)){
            STOPFIRE = true ; 
            als_ifc_error(treatmentInstance, ALS_USER_ERROR, L"Le parametre <Taux d'erreur> n'a pas été récupéré.");
        }
        if (!als_ifc_getParameterIntBlock(treatmentInstance, 4, &t_param)){
            STOPFIRE = true ; 
            als_ifc_error(treatmentInstance, ALS_USER_ERROR, L"Le parametre <Nombre de Ligne a echelonner (t)> n'a pas été récupéré.");
        }
        if (!als_ifc_getParameterIntBlock(treatmentInstance, 5, &p_param)){
            STOPFIRE = true ; 
            als_ifc_error(treatmentInstance, ALS_USER_ERROR, L"Le parametre <Nombre de colonnes a collisionner (p)> n'a pas été récupéré.");
        }
        if (!als_ifc_getParameterIntBlock(treatmentInstance, 6, &maxIterProcedure_param)){
            STOPFIRE = true ; 
            als_ifc_error(treatmentInstance, ALS_USER_ERROR, L"Le parametre <Nombre maximal d'iterations de recherche de vecteur> n'a pas été récupéré.");
        }
    }

    int n,numRecus,numUtilises,t,p,maxIterPGE, maxIterProcedure;
    double tauxErreur;
    n=n_param.ptr[0];
    numRecus=numRecus_param.ptr[0];
    numUtilises=numUtilises_param.ptr[0];
    tauxErreur=tauxErreur_param.ptr[0];
    t=t_param.ptr[0];
    p=p_param.ptr[0];
    maxIterProcedure=maxIterProcedure_param.ptr[0];

    if(numUtilises>numRecus || t>numUtilises || p>n-t){
        STOPFIRE = true ; 
        als_ifc_error(treatmentInstance, ALS_USER_ERROR, L"Parametres non cohérents.");
    }


    if (!STOPFIRE){
        TAlsToken bitEntree;
        BitBlock out;

        als_ifc_setConsumptionRate(port_entree,n*numRecus);  

        if (als_ifc_getSampleCount(port_entree) >= n*numRecus  && !als_ifc_cancelled(treatmentInstance) ){
            als_data_initializeToken(&bitEntree,ADT_UNKNOWN);
            als_ifc_readTokenBySamples(port_entree,&bitEntree, n*numRecus );

            if (als_data_upgradeTokenType(&bitEntree, ADT_BIT) == ACR_SUCCESS && bitEntree.bitBlock.size>=(n*numRecus)){


                RawMatrix motsRecus=create_empty_rows_matrix(numRecus);
                motsRecus.ncols=n;

                for(unsigned int i=0;i<motsRecus.nrows;++i){ // on recupere l'entrée sous forme de structure "rawmatrix"
                    Vector ligne=create_vector(n);
                    for(unsigned int j=0;j<motsRecus.ncols;++j){
                        if(als_data_getBit( &(bitEntree.bitBlock) , i*motsRecus.ncols+j )){
                            put_coordinate(ligne,j,1);
                        }
                    }
                    rawmatrix_replace_row(motsRecus,i,ligne);
                }

                Vector resultat=find_parity_vector_PGE(motsRecus.rows,numRecus,numUtilises,&tauxErreur,n,t,p,maxIterProcedure);
                

                als_data_initializeBitBlock(&out);
                als_data_createBitBlock(&out,n);

                for(unsigned int i=0;i<n;++i){
                    als_data_setBit(&out,i,get_coordinate(resultat,i));
                }

                als_ifc_writeBitBlock(als_ifc_getOutputReceiver(treatmentInstance, 0), &out);
                als_data_releaseBitBlock(&out);

                destroy_rawmatrix(motsRecus);
                destroy_vector(resultat);

            }
            else{
	            ((Methode_PGE*) treatmentInstance->userdata)->stopFire = true;
	            als_ifc_error(treatmentInstance, ALS_USER_ERROR, L"La conversion de l'entrée en bits a échoué.");	
	        }

            als_data_releaseToken(&bitEntree);
        }

    }

    als_data_releaseIntBlock(&n_param);
    als_data_releaseIntBlock(&numRecus_param);
    als_data_releaseIntBlock(&numUtilises_param);
    als_data_releaseDoubleBlock(&tauxErreur_param);
    als_data_releaseIntBlock(&t_param);
    als_data_releaseIntBlock(&p_param);
    als_data_releaseIntBlock(&maxIterProcedure_param);
    /** \n <b> Structure de la fonction </b> : \li Début de la fonction */

    /*
     * NOTE : le fire est la fonction contenant la logique du traitment, c'est 
     * cette fonction qui doit consommer les données en entrées et produire des
     * données sur les ports de sorties.
     */

    /*
     * NOTE : récupération des paramètres et des données d'entrées.
     * 
     * Après avoir initialisé une variable, il est possible de récupérer les 
     * données d'un port de paramètre ou d'entrée en connaissant son numéro.
     * Si les données doivent être modifiées avant d'être recopiées sur la sortie,
     * il est nécessaire de "détacher" le bloc récupérer.
     * 
     * /!\ Attention, pour que la conversion implicite puisse être effectuée, il
     * est nécessaire d'utiliser les fonctions als_ifc_getParameter<TYPE>Block() 
     * et als_ifc_read<TYPE>Block()
     */

    /*
     * NOTE : si vous avez décidé de conserver des données d'une itération sur
     * l'autre, vous pouvez récupérer la structure que vous avez définis dans le
     * champ userdata de l'instance de traitement. Vous pouvez alternativement 
     * utiliser des macros d'accès aux différents champs (voir en début de 
     * fichier pour des exemples de macros).
     */
    // TmtMethode_PGEData userdata = *((TmtMethode_PGEData*) treatmentInstance->userdata);

    /*
     * FIXME : Exemple de récupération du paramètre d'indice 0, en le stockant 
     * dans la structure du traitement. Cette méthode permet par exemple de ne 
     * faire certaines opérations seulement quand le paramètre est modifié
     */
    //  /** \li Obtention des paramètres */
    //  /* Obtention des paramètres */
    //  /** <ul> <li type="circle" style="margin-left: 20px"><i>parametre d'indice 0</i> </li> </ul> */
    //  /* Obtention du parametre d'indice 0 */
    //  if (0 < als_ifc_getParameterBoolBlock(treatmentInstance, 0, &METHODE_PGE_DATAS_FIELD3)) {
    //    assert(METHODE_PGE_DATAS_FIELD3.size > 0);
    //    // On ne passe à l'intérieur du if que si le paramètre a été modifié.
    //  }


    /*
     * FIXME : Exemple de récupération d'un paramètre (si on a bien vérifier dans
     * le préfire qu'il y avait au moins une donnée sur le port de paramètre, on
     * peut directement récupérer la donnée à l'indice 0 du block récupéré).
     */
    //  /** <ul> <li type="circle" style="margin-left: 20px"><i>parametre d'indice 1</i> </li> </ul> */
    //  /* Obtention du parametre d'indice 1 */
    //  AlsStringBlock paramStringBlock
    //  als_data_initializeAlsStringBlock(&paramStringBlock);
    //  als_ifc_getParameterAlsStringBlock(treatmentInstance, 1, &paramStringBlock);
    //  TAlsConstString paramString = paramStringBlock.ptr[0].ptr;

    /*
     * FIXME : Exemple de récupération d'un bloc en entrée.
     * Si vous souhaitez modifier le contenu du block pour par exemple le recopier
     * directement sur la sortie, il faut d'abord le "détacher" (cette opération 
     * n'est pas nécessaire si vous ne faites que lire les données).
     */
    //  /** \li Obtention des données de l'entrée d'indice 0 */
    //  // Récupération du port d'entrée d'indice 0
    //  void * inputPort = als_ifc_getInputDataReceiver(treatmentInstance, 0);
    //  // Initialisation du bloc lu
    //  IntBlock inputIntBlock;
    //  als_data_initializeIntBlock(&inputIntBlock);
    //  // Lecture du bloc
    //  als_ifc_readIntBlock(inputPort, &inputIntBlock);
    //  // Détachement du block pour modification
    //  als_data_detachIntBlock(&inputIntBlock);

    /*
     * FIXME : Exemple de récupération d'un bloc en entrée d'un port de type 
     * union.
     */
    //  /** \li Obtention des données de l'entrée d'indice 1 */
    //  // Récupération du port d'entrée d'indice 1
    //  inputPort = als_ifc_getInputDataReceiver(treatmentInstance, 1);
    //  // Initialisation du bloc lu, comme c'est un type union, il faut le lire dans 
    //  // un token
    //  TAlsToken inputTokenBlock;
    //  als_data_initializeToken(&inputTokenBlock, ADT_UNKNOWN);
    //  // Lecture du bloc
    //  als_ifc_readToken(inputPort, &inputTokenBlock);

    /*
     * NOTE : attention au type des données lors de la lecture des données sur
     * un token
     */
    //  switch (inputTokenBlock.type) {
    //    case ADT_INT:
    //      // Accès aux données via inputTokenBlock.intBlock
    //      break;
    //    case ADT_INT_VECTOR:
    //      // Accès aux données via inputTokenBlock.intArrayBlock
    //      break;
    //    case ADT_INT_MATRIX:
    //      // Accès aux données via inputTokenBlock.intArrayBlock
    //      break;
    //    default:
    //      // Erreur ?
    //  }

    /*
     * NOTE pour la manipulation des matrices
     * --------------------------------------
     * 
     * Attention à bien utiliser les fonctions de manipulation des matrices 
     * fournies par l'API ALOAS pour être sûr d'utiliser la bonne représentation :
     * 
     * Accès aux éléments :
     *  - als_data_getXXXMatrixLineCount
     *  - als_data_getXXXMatrixColumnCount
     *  - als_data_getXXXMatrixElement
     *  - als_data_setXXXMatrixElement
     * 
     * Parcours d'une matrice, utilisation des macros ALOAS_DATA_FORMATRIX et 
     * ALOAS_DATA_FORMATRIX_END. Par exemple pour une matrice "matrix" avec 
     * "sizeLine" lignes et "sizeColumn" colonnes :
     * ALOAS_DATA_FORMATRIX(line, column, sizeLine, sizeColumn);
     *    // Les deux premiers arguments sont les noms de variables d'indice à 
     *    // utiliser (elles sont déclarées par la macro).
     *    // Traitement sur les éléments de la matrice, par exemple
     *    als_data_getXXXMatrixElement(&matrix, column, line);
     * ALOAS_DATA_FORMATRIX_END();
     */

    /*
     * FIXME : initialisation du bloc contenant les données de sorties
     */
    //  // Déclaration et initialisation
    //  ComplexBlock outputComplexBlock;
    //  als_data_initializeComplexBlock(&outputComplexBlock);
    //  // Création du bloc, le deuxième paramètre correspond à la taille désirée. 
    //  // Pour continuer avec l'exemple présenté plus haut, on va créer un bloc de 
    //  // la même taille que l'entrée contenant les entiers
    //  als_data_createDoubleBlock(&outputComplexBlock, inputIntBlock.size);


    /** \li Pour chaque donnée d'entrée \n*/
    /*
     * FIXME : parcours des données sur les entrées
     * 
     * La taille d'un bloc se trouve dans le champ "size" de la structure, tandis
     * que les données sont dans le champ "ptr" sous forme de tableau (les 
     * complexes ont en général ptr_r et ptr_i). Il est donc possible de parcourir
     * les données grâce à ces deux informations
     */
    //  for (int i = 0 ; i < inputIntBlock.size ; ++i) {
    //    // Récupération de la donnée à l'indice i  via inputIntBlock.ptr[i]
    //
    //    /** <ul> <li type="circle" style="margin-left: 20px"> Logique du traitement
    //     *  </li> </ul> */
    //    /*
    //     * TODO : logique du traitement
    //     * Ici on va par exemple remplir le bloc de sortie afin d'avoir un complexe
    //     * ayant comme partie entière le paramètre d'indice 0, et comme partie 
    //     * imaginaire la donnée sur l'entrée d'indice 0.
    //     */
    //    outputComplexBlock.ptr_r[i] = METHODE_PGE_DATAS_FIELD3.ptr[0];
    //    outputComplexBlock.ptr_i[i] = inputIntBlock.ptr[i];
    //  }

    /** \li Recopie du résultat du tmt sur le port de sortie \n*/
    /*
     * FIXME : recopie des résultats sur un port de sortie.
     * 
     * Comme signalé plus haut, si les données d'entrées ont été détachées puis
     * modifiées, il  est possible de directement les recopier sur un port de 
     * sortie :
     * void * outputPort = als_ifc_getOutputReceiver(treatmentInstance, 0);
     * als_ifc_writeIntBlock(outputPort, &inputIntBlock);
     * 
     * Sinon, si un nouveau bloc a été alloué, il peut être recopié sur les ports
     * de sortie grâce aux fonctions als_ifc_writeXXXBlock()
     * 
     * Pour l'exemple, on va recopier le même bloc sur tous les ports de sorties
     * déclarés :
     */
    //  /** \li Lecture de l'attribut qui permet de specifier le nombre de sorties */
    //  /* Lecture de l'attribut qui permet de récupérer le nombre de sorties 
    //   * Cet attribut aurait pu être récupéré dans la fonction preinitialize et 
    //   * stocké dans un champ de la structure de donnée du traitement.
    //   */
    //  int attrN = 0;
    //  {
    //    TAlsToken token;
    //    char isOk = 0;
    //
    //    /*  Initialisation des tokens */
    //    als_data_initializeToken(&token, ADT_UNKNOWN);
    //
    //    /* On récupère l'attribut */
    //    if (als_ifc_getAttribute(treatmentInstance, L"N", &token) != 0) {
    //      /* Si l'attribut existe, on vérifie qu'il est valide. */
    //      TAlsConvertResult result = als_data_upgradeTokenType(&token, ADT_INT);
    //      if (result == ACR_SUCCESS && als_data_getSizeToken(&token) > 0
    //              && token.intBlock.ptr[0] > 0) {
    //        /* Si la conversion réussie, c'est que l'attribut est valide */
    //        isOk = 1;
    //        attrN = token.intBlock.ptr[0];
    //      }
    //    }
    //    als_data_releaseToken(&token);
    //
    //    /* Si la conversion a échouée, on signal l'erreur */
    //    if (!isOk) {
    //      als_ifc_error(treatmentInstance, ALS_USER_ERROR, L"Paramètre \"Nombre d'entrees (int, >0)\" erroné. Impossible d'exécuter le traitement.");
    //    }
    //  }
    //  /* Recopie du bloc outputComplexBlock */
    //  for (int outputIdx = 0 ; outputIdx < attrN ; ++outputIdx) {
    //    // récupération du port de sortie
    //    void * outputPort = als_ifc_getOutputPort(treatmentInstance, outputIdx);
    //    als_ifc_writeComplexBlock(outputPort, &outputComplexBlock);
    //  }


    /** \li Libération des blocs \n*/
    /*
     * NOTE : libération de la mémoire.
     * 
     * Une fois le traitement terminé, les différents blocs utilisés (paramètres,
     * entrées et sorties) doivent être libérés grâce aux fonctions 
     * als_data_release<TYPE>Block();
     * 
     * Les paramètres qui ont été stockés dans la structure de données du 
     * traitement n'ont pas besoin d'être libérés, cela devra néanmoins être fait
     * dans la fonction wrapup.
     */
    //   // Libération du bloc de paramètre d'indice 1
    //   als_data_releaseAlsStringBlock(&paramStringBlock);
    //   // Libération du bloc d'entrée d'indice 0
    //   als_data_releaseIntBlock(&inputIntBlock);
    //   // Libération du bloc d'entrée d'indice 1
    //   als_data_releaseToken(&inputTokenBlock);
    //   // Libération du bloc de sortie
    //   als_data_releaseComplexBlock(&outputComplexBlock);

    /** \li Fin de la fonction \n*/
}

/**
 * \brief Finalise l'exécution du traitement
 * \param treatmentInstance accès à l'instance du traitement
 * \return vrai si le traitement pourra à nouveau s'executer, faux sinon.
 */
char ALOAS_TMT_METHODE_PGE_API methode_PGE_postfire(TAlsTIStruct * treatmentInstance) {
    /** \n <b> Structure de la fonction </b> : \li Début de la fonction */

    /*
     * NOTE : cette fonction peut renvoyer 0 si l'on souhaite que le traitement ne
     * soit plus exécuté dans le reste du scénario (si par exemple on est à la 
     * fin de la lecture d'un fichier, ou la fin d'une génération de données...)
     */

    /** \li Fin de la fonction \n*/
    return 1;
}

/**
 * \brief Finalise l'exécution du traitement
 * \param treatmentInstance accès à l'instance du traitement
 */
void ALOAS_TMT_METHODE_PGE_API methode_PGE_wrapup(TAlsTIStruct * treatmentInstance) {
    free(treatmentInstance->userdata);
    /** \n <b> Structure de la fonction </b> : \li Début de la fonction */

    /** \li Libération des données persistantes d'une itération à l'autre \n*/
    /*
     * FIXME : si vous avez décidé de conserver des données d'une itération sur 
     * l'autre, c'est ici que la structure doit être libérée.
     * 
     * Exemple :
     */
    //  // Libération des pointeurs, par exemple
    //  if (METHODE_PGE_DATAS_FIELD2 != 0) {
    //    free(METHODE_PGE_DATAS_FIELD2);
    //  }
    //  // Libération des blocs
    //  als_data_releaseDoubleBlock(&METHODE_PGE_DATAS_FIELD3);
    //  als_data_releaseToken(&METHODE_PGE_DATAS_FIELD4);
    //  // Libération de la mémoire pour la structure
    //  delete treatmentInstance->userdata;

    /** \li Fin de la fonction \n*/
}