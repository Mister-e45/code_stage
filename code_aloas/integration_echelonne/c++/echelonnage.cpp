/**
 * \file echelonnage.cpp
 * \author 
 * \date 16 juil. 2025
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

#include <echelonnage.h>

#define STOPFIRE ((EchelonnageData*) treatmentInstance->userdata)->stopFire
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
//#define ECHELONNAGE_DATAS_FIELD1 ((TmtEchelonnageData*)treatmentInstance->userdata)->field1
//#define ECHELONNAGE_DATAS_FIELD2 ((TmtEchelonnageData*)treatmentInstance->userdata)->field2
//#define ECHELONNAGE_DATAS_FIELD3 ((TmtEchelonnageData*)treatmentInstance->userdata)->field3
//#define ECHELONNAGE_DATAS_FIELD4 ((TmtEchelonnageData*)treatmentInstance->userdata)->field4
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
//} TmtEchelonnageData;

/**
 * \brief Obtient le modèle de traitement
 * \param templatetmt modèle de traitement initial, servant notamment à
 * obtenir les attributs
 */
void ALOAS_TMT_ECHELONNAGE_API echelonnage_buildTemplate(void* templatetmt) {
    /** \n <b> Structure de la fonction </b> : \li Début de la fonction */
    TAlsTemplateType BitType,IntType;
    als_tmttpt_initializeType(&BitType);
    als_tmttpt_initializeType(&IntType);

    als_tmttpt_resetPort(templatetmt);

    als_tmttpt_createBasicType(&BitType, ATBT_BIT);
    als_tmttpt_createBasicType(&IntType, ATBT_INT);

    TAlsTemplatePort* portNombreColonnes = als_tmttpt_createParameterPort(templatetmt, L"Nombre de Colonnes");
    TAlsTemplatePort* portNombreLignes = als_tmttpt_createParameterPort(templatetmt, L"Nombre de Lignes");

    TAlsTemplatePort* inputPort  = als_tmttpt_createInputDataPort(templatetmt,L"U1");
    TAlsTemplatePort* outputPort = als_tmttpt_createOutputPort(templatetmt, L"Y1");


    als_tmttpt_setTypeEquals(portNombreColonnes, &IntType);
    als_tmttpt_setTypeEquals(portNombreLignes, &IntType);
    als_tmttpt_setTypeAtMost(inputPort, &BitType);
    als_tmttpt_setTypeAtLeast(outputPort, &BitType);

    als_tmttpt_releaseType(&BitType);
    als_tmttpt_releaseType(&IntType);
    /*
     * NOTE : cette fonction permet d'initialiser les ports de paramètres, 
     * d'entrées et de sorties du traitement. Pour créer un port, il faut :
     *  - le déclarer au template passé en paramètre
     *  - initialiser son type
     *  - associer son type au port
     */

    /** \li Activation/désactivation de l'ICS */
    /* 
     * FIXME : décommentez la ligne suivante si vous souhaitez désactiver l'ICS 
     * pour ce traitement.
     */
    // als_tmttpt_desactivateICS(templatetmt);

    /** \li Récupération des attributs */
    /*
     * NOTE : les attributs doivent être récupéré grâce à leur nom dans un token
     * ALOAS. Une fois le token récupéré, il faut vérifier leur type avant de 
     * chercher à récupérer la valeur de l'attribut. Enfin, si l'attribut n'existe
     * pas encore (la fonction de récupération aura échouée, il sera nécessaire de
     * définir une valeur par défaut pour l'attribut.
     * 
     * Exemple (attribut N permettant de spécifier le nombre de sorties) :
     */
    //  // Valeur par défaut
    //  int attrN = 1;
    //  {
    //    TAlsToken token;
    //    char isOk = 0;
    //
    //    // Initialisation des tokens
    //    als_data_initializeToken(&token, ADT_UNKNOWN);
    //
    //    // On récupère l'attribut
    //    if (als_tmttpt_getAttribute(templatetmt, L"N", &token) != 0) {
    //      // Si l'attribut existe, on vérifie qu'il est valide.
    //      TAlsConvertResult result = als_data_upgradeTokenType(&token, ADT_INT);
    //      if (result == ACR_SUCCESS && als_data_getSizeToken(&token) > 0) {
    //        // Si la conversion réussie, c'est que l'attribut est valide
    //        isOk = token.intBlock.ptr[0] > 0;
    //        if (isOk) {
    //          attrN = token.intBlock.ptr[0];
    //        }
    //      }
    //    }
    //    als_data_releaseToken(&token);
    //
    //    // Si la conversion a échouée, on définit une valeur par défaut
    //    if (!isOk) {
    //      als_data_initializeToken(&token, ADT_INT);
    //      als_data_createToken(&token, 1);
    //      token.intBlock.ptr[0] = attrN;
    //      als_tmttpt_setAttribute(templatetmt, L"N", &token);
    //      als_data_releaseToken(&token);
    //    }
    //  }

    /** \li Création des ports du traitement */
    /*
     * NOTE : Création des ports du traitements.
     * -----------------------------------------
     * 
     * Vous pouvez déclarer un port en appelant les fonctions suivantes : 
     * - als_tmttpt_createParameterPort() pour déclarer un port de paramètre
     * - als_tmttpt_createInputDataPort() pour déclarer un port d'entrée
     * - als_tmttpt_createOutputPort() pour déclarer un port de sortie
     * 
     * Ces fonctions prennent en paramètre le template de traitement, et le nom
     * du port (en wchar*), et renvoient un pointeur vers un template de port.
     * De plus, l'ordre dans lequel sont déclarés les ports déterminera 
     * leur ordre dans l'IHM ALOAS. L'ordre est :
     * - paramètres (dans l'ordre de déclaration)
     * - entrées (dans l'ordre de déclaration)
     * - sorties (dans l'ordre de déclaration)
     * 
     * Il faut au maximum éviter les caractères non ASCII (accents...) dans les 
     * noms de ports.
     * 
     * /!\ ALOAS utilise des chaines de caractères basées sur wchar plutôt que 
     * char, les chaines doivent donc être déclarées en utilisant L"" (avec un L 
     * devant les guillemets). Alternativement, la macro ALS_STR peut être 
     * utilisée.
     * TAlsString alsStr1 = L"chaine de caractere pour ALOAS";
     * TAlsString alsStr2 = ALS_STR("chaine de caractere pour ALOAS");
     * 
     * Exemple :
     * TAlsTemplatePort *paramPort = als_tmttpt_createParameterPort(templatetmt, L"parametre d'indice 0");
     * TAlsTemplatePort *inputPort = als_tmttpt_createInputDataPort(templatetmt, L"entree d'indice 0");
     * TAlsTemplatePort *outputPort = als_tmttpt_createOutputPort(templatetmt, L"sortie d'indice 0");
     * 
     * NOTE : Création des types qui seront utilisés par les ports
     * -----------------------------------------------------------
     * 
     * Pour pouvoir assigner un type à un port, il faut d'abord créer le type en 
     * question. Pour cela, il faut utiliser la fonction als_tmttpt_initializeType() 
     * pour initialiser la variable de type, puis utiliser la fonction 
     * als_tmttpt_createBasicType() pour effectivement créer le type. Cette 
     * fonction prend en paramètre un pointeur vers un TAlsTemplateType, et un 
     * TAlsTemplateBasicType. Le TAlsTemplateBasicType correspond au type de 
     * donnée que l'on veut utilisé. Ces types sont préfixés par ATBT, on peut les 
     * trouver dans le fichier basictype.h.
     * 
     * /!\ Les types ATBT_UNKNOWN et ATBT_UNION ne doivent pas être utilisés 
     * directement.
     * 
     * Exemple :
     * TAlsTemplateType intType;
     * als_tmttpt_initializeType(&intType);
     * als_tmttpt_createBasicType(&intType, ATBT_INT);
     * 
     * NOTE : assignation d'un type à un port
     * --------------------------------------
     * 
     * Pour assigner un type à un port, il faut utiliser l'une des fonctions 
     * suivantes :
     * - als_tmttpt_setTypeAtMost()
     * - als_tmttpt_setTypeEquals()
     * - als_tmttpt_setTypeAtLeast()
     * 
     * Ces fonctions prennent en paramètre un pointeur vers un port et un pointeur
     * vers un type. Chaque fonction permet d'avoir une contrainte d'inégalité 
     * différente sur le port :
     * - setTypeAtMost permet au port d'accepter au plus le type passé en 
     *   paramètre. Par exemple, initialiser un port pour qu'il accepte 
     *   "au plus un double" lui permettra d'accepter également un entier ou un
     *   booléen (mais pas un complexe). Cette fonction est à utiliser en priorité 
     *   pour les ports de paramètres ou d'entrée.
     * - setTypeEquals permet au port d'accepter seulement le type passer en 
     *   paramètre. Par exemple, initialiser un port pour qu'il accepte 
     *   "exactement un booléen" ne lui permettra d'accepter ni le type bit, ni 
     *   le type entier. Cette fonction est à utiliser en priorité pour les ports
     *   de paramètres ou de sortie.
     * - setTypeAtLeast permet au port d'accepter au moins le type passé en 
     *   paramètre. Par exemple, initialiser un port pour qu'il accepte "au moins
     *   un entier" lui permettra d'accepter également un double ou un complexe
     *   (mais pas un booléen). Cette fonction est à utiliser en priorité pour les 
     *   ports de sortie lorsque l'on ne peut pas être sûr du type renvoyé.
     * 
     * En général il faut utiliser :
     * - pour les ports de paramètres : setTypeEquals()
     * - pour les ports d'entrée : setTypeAtMost()
     * - pour les ports de sorties : setTypeEquals() si on connait d'avance le 
     *   type qui sera renvoyé, setTypeAtLeast() sinon.
     * 
     * /!\ Attention, en utilisant setTypeAtMost() sur un port d'entrée, il faut
     * quand même s'assurer de récupérer la donnée correctement pour qu'elle soit
     * convertie dans le bon type (voir les commentaires dans la fonction fire)
     * 
     * Exemple :
     * als_tmttpt_setTypeEquals(paramPort, &intType);
     * als_tmttpt_setTypeAtMost(inputPort, &doubleType);
     *
     * NOTE : libération des types de ports
     * ------------------------------------
     * 
     * Une fois tous les ports typés, les types utilisés peuvent être libérés 
     * grâce à la fonction als_tmttpt_releaseType().
     * 
     * Exemple :
     * als_tmttpt_releaseType(&intType);
     */

    /*
     * TODO : Voici la déclaration des ports pour l'exemple décrit en entête de 
     * fichier. A remplacer par la déclaration de vos propres ports.
     */
    //  /** \li Création des ports de paramètres */
    //  /**   <ul> <li type="circle" style="margin-left: 20px">Création du port de paramètre <i>parametre d'indice 0</i> </li> </ul> */
    //  /* Création du port de paramètre d'indice 0 */
    //  {
    //    TAlsTemplateType type;
    //    TAlsTemplatePort * port;
    //    TAlsToken token;
    //
    //    // Création du port
    //    port = als_tmttpt_createParameterPort(templatetmt, L"parametre d'indice 0");
    //    
    //    // Attribution du type au paramètre
    //    als_tmttpt_initializeType(&type);
    //    als_tmttpt_createBasicType(&type, ATBT_DOUBLE);
    //    als_tmttpt_setTypeAtMost(port, &type);
    //    als_tmttpt_releaseType(&type);
    //    
    //    // Ajout d'une valeur par défaut (optionnel)
    //    als_data_initializeToken(&token, ADT_DOUBLE);
    //    als_data_createToken(&token, 1);
    //    token.doubleBlock.ptr[0] = 2.5;
    //    als_tmttpt_writeDefaultToken(port, &token);
    //    als_data_releaseToken(&token);
    //  }
    //  /**   <ul> <li type="circle" style="margin-left: 20px">Création du port de paramètre <i>parametre d'indice 1</i> </li> </ul> */
    //  /* Création du port de paramètre d'indice 1 */
    //  {
    //    TAlsTemplateType type;
    //    TAlsTemplatePort * port;
    //    TAlsToken token;
    //
    //    // Création du port
    //    port = als_tmttpt_createParameterPort(templatetmt, L"parametre d'indice 1");
    //    
    //    // Attribution du type au paramètre
    //    als_tmttpt_initializeType(&type);
    //    als_tmttpt_createBasicType(&type, ATBT_STRING);
    //    als_tmttpt_setTypeAtMost(port, &type);
    //    als_tmttpt_releaseType(&type);
    //    
    //    // Ajout d'une valeur par défaut (optionnel)
    //    als_data_initializeToken(&token, ADT_STRING);
    //    als_data_createToken(&token, 1);
    //    als_data_createAlsStringFromString(token.alsStringBlock.ptr, L"valeur par defaut");
    //    als_tmttpt_writeDefaultToken(port, &token);
    //    als_data_releaseToken(&token);
    //  }
    //
    //  /** \li Création des ports d'entrée du traitement */
    //  /**   <ul> <li type="circle" style="margin-left: 20px">Création du port <i>entree d'indice 0</i> </li> </ul> */
    //  /* Création du port d'entrée d'indice 0 */
    //  {
    //    TAlsTemplateType type;
    //    TAlsTemplatePort * port;
    //
    //    port = als_tmttpt_createInputDataPort(templatetmt, L"entree d'indice 0");
    //    als_tmttpt_initializeType(&type);
    //    als_tmttpt_createBasicType(&type, ATBT_INT);
    //    als_tmttpt_setTypeAtMost(port, &type);
    //    als_tmttpt_releaseType(&type);
    //  }
    //  /**   <ul> <li type="circle" style="margin-left: 20px">Création du port <i>entree d'indice 1</i> </li> </ul> */
    //  /* Création du port d'entrée d'indice 1 */
    //  {
    //    TAlsTemplateType type;
    //    TAlsTemplatePort * port;
    //
    //    port = als_tmttpt_createInputDataPort(templatetmt, L"entree d'indice 1");
    //    als_tmttpt_initializeType(&type);
    //    als_tmttpt_createUnionTypeFromBasic(&type, 3, ATBT_INT, ATBT_INT_VECTOR, ATBT_INT_MATRIX);
    //    als_tmttpt_setTypeAtMost(port, &type);
    //    als_tmttpt_releaseType(&type);
    //  }
    //
    //  /** \li Creation des ports en sortie d'après l'attribut N */
    //  /* Creation des ports en sortie */
    //  {
    //    int i = 0;
    //    TAlsTemplateType type;
    //    TAlsTemplatePort * port;
    //    while (i < attrN) {
    //
    //      TAlsChar name[64];
    //
    //      als_sprintf(name, L"sortie d'indice %d", i);
    //      port = als_tmttpt_createOutputPort(templatetmt, name);
    //      als_tmttpt_initializeType(&type);
    //      als_tmttpt_createBasicType(&type, ATBT_COMPLEX);
    //      als_tmttpt_setTypeEquals(port, &type);
    //      als_tmttpt_releaseType(&type);
    //
    //      ++i;
    //    }
    //  }

    /** \li Fin de la fonction \n*/
}

/**
 * \brief Pre-initialise le traitement
 * \param treatmentInstance accès à l'instance du traitement
 */
void ALOAS_TMT_ECHELONNAGE_API echelonnage_preinitialize(TAlsTIStruct* treatmentInstance) {
  treatmentInstance->userdata = malloc(sizeof (EchelonnageData));

	/** <ul> <li type="circle" style="margin-left: 20px">Initialisation de la variable pour la gestion des erreurs</li> </ul> */
	STOPFIRE = false;
    /** \n <b> Structure de la fonction </b> : \li Début de la fonction */

    /*
     * FIXME : si vous avez décidé de conserver des données d'une itération sur 
     * l'autre, c'est ici que la structure doit être allouée et placée dans 
     * l'instance de traitement.
     * 
     * Exemple :
     */
    // treatmentInstance->userdata = new TmtEchelonnageData;
    // // Initialisation des champs de la structure
    // ECHELONNAGE_DATAS_FIELD1 = 0;
    // ECHELONNAGE_DATAS_FIELD2 = 0;
    // als_data_initializeDoubleBloc(&ECHELONNAGE_DATAS_FIELD3);
    // als_data_initializeToken(&ECHELONNAGE_DATAS_FIELD4, ADT_UNKNOWN);

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
void ALOAS_TMT_ECHELONNAGE_API echelonnage_initialize(TAlsTIStruct* treatmentInstance) {
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
char ALOAS_TMT_ECHELONNAGE_API echelonnage_prefire(TAlsTIStruct* treatmentInstance) {
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
void ALOAS_TMT_ECHELONNAGE_API echelonnage_fire(TAlsTIStruct* treatmentInstance) {
  IntBlock n_param;
  IntBlock k_param;
  als_data_initializeIntBlock(&k_param);
  als_data_initializeIntBlock(&n_param);
  
  if (als_ifc_getSampleCount(als_ifc_getParamReceiver(treatmentInstance,0)) <= 0){
      STOPFIRE = true;
      als_ifc_error(treatmentInstance, ALS_USER_ERROR, L"Le parametre <Nombre de Colonnes> n'a pas été récupéré.");
  }
  else if (als_ifc_getSampleCount(als_ifc_getParamReceiver(treatmentInstance,1)) <= 0){
      STOPFIRE = true;
      als_ifc_error(treatmentInstance, ALS_USER_ERROR, L"Le parametre <Nombre de Lignes> n'a pas été récupéré.");
  }
  else{
    if (!als_ifc_getParameterIntBlock(treatmentInstance, 0, &n_param)){
      STOPFIRE = true ; 
      als_ifc_error(treatmentInstance, ALS_USER_ERROR, L"Le parametre <Nombre de Colonnes> n'a pas été récupéré.");
    }
    if (!als_ifc_getParameterIntBlock(treatmentInstance, 1, &k_param)){
      STOPFIRE = true ; 
      als_ifc_error(treatmentInstance, ALS_USER_ERROR, L"Le parametre <Nombre de Lignes> n'a pas été récupéré.");
    }
  }

  

  if (!STOPFIRE)
  {    
  
    RawMatrix matrice=create_rawmatrix(k_param.ptr[0],n_param.ptr[0]);
    TAlsToken bitEntree;
    BitBlock out;
     
     
     /* Taux de consommation de l'entree */
     als_ifc_setConsumptionRate(als_ifc_getInputDataReceiver(treatmentInstance, 0),matrice.ncols*matrice.nrows);  
      
     if (als_ifc_getSampleCount(als_ifc_getInputDataReceiver(treatmentInstance, 0)) >= matrice.ncols*matrice.nrows  && !als_ifc_cancelled(treatmentInstance) ){

      als_data_initializeToken(&bitEntree,ADT_UNKNOWN);
      als_ifc_readTokenBySamples(als_ifc_getInputDataReceiver(treatmentInstance, 0),&bitEntree, matrice.ncols*matrice.nrows );	

	   if (als_data_upgradeTokenType(&bitEntree, ADT_BIT) == ACR_SUCCESS && bitEntree.bitBlock.size>=(matrice.ncols*matrice.nrows)){                                         	   
	       /* Allocation de la mémoire pour la sortie */
         als_data_initializeBitBlock(&out);

         als_data_createBitBlock(&out,matrice.ncols*matrice.nrows);

        for(unsigned int i=0;i<matrice.nrows;++i){
          for(unsigned int j=0;j<matrice.ncols;++j){
            Bit val= (als_data_getBit( &(bitEntree.bitBlock) , i*matrice.ncols+j )? 1 : 0);
            rawmatrix_write(matrice,i,j,val);
          }
        }

        row_echelonize_rawmatrix(matrice);

        for(unsigned int i=0;i<matrice.nrows;++i){
          for(unsigned int j=0;j<matrice.ncols;++j){
            if (!als_ifc_cancelled(treatmentInstance)){
              als_data_setBit(&out, j + matrice.ncols*i ,rawmatrix_get_value(matrice,i,j));
            }
          }
        }

        als_ifc_writeBitBlock(als_ifc_getOutputReceiver(treatmentInstance, 0), &out);
        als_data_releaseBitBlock(&out);
      }
	   else{
	      ((EchelonnageData*) treatmentInstance->userdata)->stopFire = true;
	      als_ifc_error(treatmentInstance, ALS_USER_ERROR, L"La conversion de l'entrée en bits a échoué.");	
	    }

     destroy_rawmatrix(matrice); 
     als_data_releaseToken(&bitEntree);       				  
	   }
     als_data_releaseIntBlock(&n_param);
     als_data_releaseIntBlock(&k_param);
  }
}


/**
 * \brief Finalise l'exécution du traitement
 * \param treatmentInstance accès à l'instance du traitement
 */
char ALOAS_TMT_ECHELONNAGE_API echelonnage_postfire(TAlsTIStruct * treatmentInstance) {
	return 1;
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
    // TmtEchelonnageData userdata = *((TmtEchelonnageData*) treatmentInstance->userdata);

    /*
     * FIXME : Exemple de récupération du paramètre d'indice 0, en le stockant 
     * dans la structure du traitement. Cette méthode permet par exemple de ne 
     * faire certaines opérations seulement quand le paramètre est modifié
     */
    //  /** \li Obtention des paramètres */
    //  /* Obtention des paramètres */
    //  /** <ul> <li type="circle" style="margin-left: 20px"><i>parametre d'indice 0</i> </li> </ul> */
    //  /* Obtention du parametre d'indice 0 */
    //  if (0 < als_ifc_getParameterBoolBlock(treatmentInstance, 0, &ECHELONNAGE_DATAS_FIELD3)) {
    //    assert(ECHELONNAGE_DATAS_FIELD3.size > 0);
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
    //    outputComplexBlock.ptr_r[i] = ECHELONNAGE_DATAS_FIELD3.ptr[0];
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

/**
 * \brief Finalise l'exécution du traitement
 * \param treatmentInstance accès à l'instance du traitement
 */
void ALOAS_TMT_ECHELONNAGE_API echelonnage_wrapup(TAlsTIStruct * treatmentInstance) {
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
    //  if (ECHELONNAGE_DATAS_FIELD2 != 0) {
    //    free(ECHELONNAGE_DATAS_FIELD2);
    //  }
    //  // Libération des blocs
    //  als_data_releaseDoubleBlock(&ECHELONNAGE_DATAS_FIELD3);
    //  als_data_releaseToken(&ECHELONNAGE_DATAS_FIELD4);
    //  // Libération de la mémoire pour la structure
    //  delete treatmentInstance->userdata;

    /** \li Fin de la fonction \n*/
}