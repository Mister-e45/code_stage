
/** 
 * \file   reconstructionPGE.h
 * \author Intégré depuis un code source C/C++
 * 
 * \date 20 août 2025
 */

#ifndef RECONSTRUCTIONPGE_H
#define	RECONSTRUCTIONPGE_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef WIN32
#ifdef ALOAS_TMT_RECONSTRUCTIONPGE_EXPORTS
#define ALOAS_TMT_RECONSTRUCTIONPGE_API __declspec(dllexport)
#else
#define ALOAS_TMT_RECONSTRUCTIONPGE_API __declspec(dllimport)
#endif
#else
#define ALOAS_TMT_RECONSTRUCTIONPGE_API
#endif


    // ****************************************************************************
    // Dependences
#include "PGE.h"
    // ****************************************************************************
    // Aloas-api
#include <aloas/api.h> 

typedef struct {
	bool stopFire;
} Reconstruction_PGE;

    /**
     * \brief Obtient le modele de traitement
     * \param templatetmt modele de traitement initial, servant notamment obtenir les attributs
     */
    void ALOAS_TMT_RECONSTRUCTIONPGE_API reconstructionPGE_buildTemplate(void* templatetmt);

    /**
     * \brief Pre-initialise le traitement
     * \param treatmentInstance accès à l'instance du traitement
     */
    void ALOAS_TMT_RECONSTRUCTIONPGE_API reconstructionPGE_preinitialize(TAlsTIStruct* treatmentInstance);

    /**
     * \brief Initialise le traitement
     * \param treatmentInstance accès à l'instance du traitement
     */
    void ALOAS_TMT_RECONSTRUCTIONPGE_API reconstructionPGE_initialize(TAlsTIStruct* treatmentInstance);

    /**
     * \brief Pr?pare l'ex?cution du traitement
     * \param treatmentInstance accès à l'instance du traitement
     * \return vrai si le traitement peu s'executer, faux sinon.
     */
    char ALOAS_TMT_RECONSTRUCTIONPGE_API reconstructionPGE_prefire(TAlsTIStruct* treatmentInstance);

    /**
     * \brief Exécute le traitement
     * \param treatmentInstance accès à l'instance du traitement
     */
    void ALOAS_TMT_RECONSTRUCTIONPGE_API reconstructionPGE_fire(TAlsTIStruct* treatmentInstance);

    /**
     * \brief Finalise l'ex?cution du traitement
     * \param treatmentInstance accès à l'instance du traitement
     * \return vrai si le traitement pourra à nouveau s'executer, faux sinon.
     */
    char ALOAS_TMT_RECONSTRUCTIONPGE_API reconstructionPGE_postfire(TAlsTIStruct* treatmentInstance);

    /**
     * \brief Finalise l'ex?cution du traitement
     * \param treatmentInstance accès à l'instance du traitement
     */
    void ALOAS_TMT_RECONSTRUCTIONPGE_API reconstructionPGE_wrapup(TAlsTIStruct* treatmentInstance);

#ifdef	__cplusplus
}
#endif

#endif	/* RECONSTRUCTIONPGE_H */
