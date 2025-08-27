/** 
 * \file   echelonnage.h
 * \author Intégré depuis un code source C/C++
 * 
 * \date 16 juil. 2025
 */

#ifndef ECHELONNAGE_H
#define	ECHELONNAGE_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef WIN32
#ifdef ALOAS_TMT_ECHELONNAGE_EXPORTS
#define ALOAS_TMT_ECHELONNAGE_API __declspec(dllexport)
#else
#define ALOAS_TMT_ECHELONNAGE_API __declspec(dllimport)
#endif
#else
#define ALOAS_TMT_ECHELONNAGE_API
#endif


    // ****************************************************************************
    // Dependences
    // ****************************************************************************
    // Aloas-api
#include "rawmatrix.h"
#include <aloas/api.h> 


typedef struct {
	bool stopFire;
} EchelonnageData;

    /**
     * \brief Obtient le modele de traitement
     * \param templatetmt modele de traitement initial, servant notamment obtenir les attributs
     */
    void ALOAS_TMT_ECHELONNAGE_API echelonnage_buildTemplate(void* templatetmt);

    /**
     * \brief Pre-initialise le traitement
     * \param treatmentInstance accès à l'instance du traitement
     */
    void ALOAS_TMT_ECHELONNAGE_API echelonnage_preinitialize(TAlsTIStruct* treatmentInstance);

    /**
     * \brief Initialise le traitement
     * \param treatmentInstance accès à l'instance du traitement
     */
    void ALOAS_TMT_ECHELONNAGE_API echelonnage_initialize(TAlsTIStruct* treatmentInstance);

    /**
     * \brief Pr?pare l'ex?cution du traitement
     * \param treatmentInstance accès à l'instance du traitement
     * \return vrai si le traitement peu s'executer, faux sinon.
     */
    char ALOAS_TMT_ECHELONNAGE_API echelonnage_prefire(TAlsTIStruct* treatmentInstance);

    /**
     * \brief Exécute le traitement
     * \param treatmentInstance accès à l'instance du traitement
     */
    void ALOAS_TMT_ECHELONNAGE_API echelonnage_fire(TAlsTIStruct* treatmentInstance);

    /**
     * \brief Finalise l'ex?cution du traitement
     * \param treatmentInstance accès à l'instance du traitement
     * \return vrai si le traitement pourra à nouveau s'executer, faux sinon.
     */
    char ALOAS_TMT_ECHELONNAGE_API echelonnage_postfire(TAlsTIStruct* treatmentInstance);

    /**
     * \brief Finalise l'ex?cution du traitement
     * \param treatmentInstance accès à l'instance du traitement
     */
    void ALOAS_TMT_ECHELONNAGE_API echelonnage_wrapup(TAlsTIStruct* treatmentInstance);

#ifdef	__cplusplus
}
#endif

#endif	/* ECHELONNAGE_H */
