/** 
 * \file   methode_SVR.h
 * \author Intégré depuis un code source C/C++
 * 
 * \date 13 août 2025
 */

#ifndef METHODE_SVR_H
#define	METHODE_SVR_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef WIN32
#ifdef ALOAS_TMT_METHODE_SVR_EXPORTS
#define ALOAS_TMT_METHODE_SVR_API __declspec(dllexport)
#else
#define ALOAS_TMT_METHODE_SVR_API __declspec(dllimport)
#endif
#else
#define ALOAS_TMT_METHODE_SVR_API
#endif


    // ****************************************************************************
    // Dependences
    // ****************************************************************************
    // Aloas-api
#include "SVR.h"
#include <aloas/api.h> 

typedef struct {
	bool stopFire;
} Methode_SVR;

    /**
     * \brief Obtient le modele de traitement
     * \param templatetmt modele de traitement initial, servant notamment obtenir les attributs
     */
    void ALOAS_TMT_METHODE_SVR_API methode_SVR_buildTemplate(void* templatetmt);

    /**
     * \brief Pre-initialise le traitement
     * \param treatmentInstance accès à l'instance du traitement
     */
    void ALOAS_TMT_METHODE_SVR_API methode_SVR_preinitialize(TAlsTIStruct* treatmentInstance);

    /**
     * \brief Initialise le traitement
     * \param treatmentInstance accès à l'instance du traitement
     */
    void ALOAS_TMT_METHODE_SVR_API methode_SVR_initialize(TAlsTIStruct* treatmentInstance);

    /**
     * \brief Pr?pare l'ex?cution du traitement
     * \param treatmentInstance accès à l'instance du traitement
     * \return vrai si le traitement peu s'executer, faux sinon.
     */
    char ALOAS_TMT_METHODE_SVR_API methode_SVR_prefire(TAlsTIStruct* treatmentInstance);

    /**
     * \brief Exécute le traitement
     * \param treatmentInstance accès à l'instance du traitement
     */
    void ALOAS_TMT_METHODE_SVR_API methode_SVR_fire(TAlsTIStruct* treatmentInstance);

    /**
     * \brief Finalise l'ex?cution du traitement
     * \param treatmentInstance accès à l'instance du traitement
     * \return vrai si le traitement pourra à nouveau s'executer, faux sinon.
     */
    char ALOAS_TMT_METHODE_SVR_API methode_SVR_postfire(TAlsTIStruct* treatmentInstance);

    /**
     * \brief Finalise l'ex?cution du traitement
     * \param treatmentInstance accès à l'instance du traitement
     */
    void ALOAS_TMT_METHODE_SVR_API methode_SVR_wrapup(TAlsTIStruct* treatmentInstance);

#ifdef	__cplusplus
}
#endif

#endif	/* METHODE_SVR_H */
