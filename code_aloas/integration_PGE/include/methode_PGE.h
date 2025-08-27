/** 
 * \file   methode_PGE.h
 * \author Intégré depuis un code source C/C++
 * 
 * \date 13 août 2025
 */

#ifndef METHODE_PGE_H
#define	METHODE_PGE_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef WIN32
#ifdef ALOAS_TMT_METHODE_PGE_EXPORTS
#define ALOAS_TMT_METHODE_PGE_API __declspec(dllexport)
#else
#define ALOAS_TMT_METHODE_PGE_API __declspec(dllimport)
#endif
#else
#define ALOAS_TMT_METHODE_PGE_API
#endif


    // ****************************************************************************
    // Dependences
#include "PGE.h"
    // ****************************************************************************
    // Aloas-api
#include <aloas/api.h> 

typedef struct {
	bool stopFire;
} Methode_PGE;
    /**
     * \brief Obtient le modele de traitement
     * \param templatetmt modele de traitement initial, servant notamment obtenir les attributs
     */
    void ALOAS_TMT_METHODE_PGE_API methode_PGE_buildTemplate(void* templatetmt);

    /**
     * \brief Pre-initialise le traitement
     * \param treatmentInstance accès à l'instance du traitement
     */
    void ALOAS_TMT_METHODE_PGE_API methode_PGE_preinitialize(TAlsTIStruct* treatmentInstance);

    /**
     * \brief Initialise le traitement
     * \param treatmentInstance accès à l'instance du traitement
     */
    void ALOAS_TMT_METHODE_PGE_API methode_PGE_initialize(TAlsTIStruct* treatmentInstance);

    /**
     * \brief Pr?pare l'ex?cution du traitement
     * \param treatmentInstance accès à l'instance du traitement
     * \return vrai si le traitement peu s'executer, faux sinon.
     */
    char ALOAS_TMT_METHODE_PGE_API methode_PGE_prefire(TAlsTIStruct* treatmentInstance);

    /**
     * \brief Exécute le traitement
     * \param treatmentInstance accès à l'instance du traitement
     */
    void ALOAS_TMT_METHODE_PGE_API methode_PGE_fire(TAlsTIStruct* treatmentInstance);

    /**
     * \brief Finalise l'ex?cution du traitement
     * \param treatmentInstance accès à l'instance du traitement
     * \return vrai si le traitement pourra à nouveau s'executer, faux sinon.
     */
    char ALOAS_TMT_METHODE_PGE_API methode_PGE_postfire(TAlsTIStruct* treatmentInstance);

    /**
     * \brief Finalise l'ex?cution du traitement
     * \param treatmentInstance accès à l'instance du traitement
     */
    void ALOAS_TMT_METHODE_PGE_API methode_PGE_wrapup(TAlsTIStruct* treatmentInstance);

#ifdef	__cplusplus
}
#endif

#endif	/* METHODE_PGE_H */
