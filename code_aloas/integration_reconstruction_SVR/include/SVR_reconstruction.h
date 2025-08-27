/** 
 * \file   SVR_recontruction.h
 * \author Intégré depuis un code source C/C++
 * 
 * \date 13 août 2025
 */

#ifndef SVR_RECONSTRUCTION_H
#define	SVR_RECONSTRUCTION_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef WIN32
#ifdef ALOAS_TMT_SVR_RECONSTRUCTION_EXPORTS
#define ALOAS_TMT_SVR_RECONSTRUCTION_API __declspec(dllexport)
#else
#define ALOAS_TMT_SVR_RECONSTRUCTION_API __declspec(dllimport)
#endif
#else
#define ALOAS_TMT_SVR_RECONSTRUCTION_API
#endif


    // ****************************************************************************
    // Dependences
    // ****************************************************************************
    // Aloas-api
#include "SVR.h"
#include <aloas/api.h> 

typedef struct {
	bool stopFire;
} ReconstructionSVR;

    /**
     * \brief Obtient le modele de traitement
     * \param templatetmt modele de traitement initial, servant notamment obtenir les attributs
     */
    void ALOAS_TMT_SVR_RECONSTRUCTION_API SVR_reconstruction_buildTemplate(void* templatetmt);

    /**
     * \brief Pre-initialise le traitement
     * \param treatmentInstance accès à l'instance du traitement
     */
    void ALOAS_TMT_SVR_RECONSTRUCTION_API SVR_reconstruction_preinitialize(TAlsTIStruct* treatmentInstance);

    /**
     * \brief Initialise le traitement
     * \param treatmentInstance accès à l'instance du traitement
     */
    void ALOAS_TMT_SVR_RECONSTRUCTION_API SVR_reconstruction_initialize(TAlsTIStruct* treatmentInstance);

    /**
     * \brief Pr?pare l'ex?cution du traitement
     * \param treatmentInstance accès à l'instance du traitement
     * \return vrai si le traitement peu s'executer, faux sinon.
     */
    char ALOAS_TMT_SVR_RECONSTRUCTION_API SVR_reconstruction_prefire(TAlsTIStruct* treatmentInstance);

    /**
     * \brief Exécute le traitement
     * \param treatmentInstance accès à l'instance du traitement
     */
    void ALOAS_TMT_SVR_RECONSTRUCTION_API SVR_reconstruction_fire(TAlsTIStruct* treatmentInstance);

    /**
     * \brief Finalise l'ex?cution du traitement
     * \param treatmentInstance accès à l'instance du traitement
     * \return vrai si le traitement pourra à nouveau s'executer, faux sinon.
     */
    char ALOAS_TMT_SVR_RECONSTRUCTION_API SVR_reconstruction_postfire(TAlsTIStruct* treatmentInstance);

    /**
     * \brief Finalise l'ex?cution du traitement
     * \param treatmentInstance accès à l'instance du traitement
     */
    void ALOAS_TMT_SVR_RECONSTRUCTION_API SVR_reconstruction_wrapup(TAlsTIStruct* treatmentInstance);

#ifdef	__cplusplus
}
#endif

#endif	/* SVR_RECONTRUCTION_H */
