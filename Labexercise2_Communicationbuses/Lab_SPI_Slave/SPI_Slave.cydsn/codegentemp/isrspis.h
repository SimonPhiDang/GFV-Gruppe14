/*******************************************************************************
* File Name: isrspis.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_isrspis_H)
#define CY_ISR_isrspis_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void isrspis_Start(void);
void isrspis_StartEx(cyisraddress address);
void isrspis_Stop(void);

CY_ISR_PROTO(isrspis_Interrupt);

void isrspis_SetVector(cyisraddress address);
cyisraddress isrspis_GetVector(void);

void isrspis_SetPriority(uint8 priority);
uint8 isrspis_GetPriority(void);

void isrspis_Enable(void);
uint8 isrspis_GetState(void);
void isrspis_Disable(void);

void isrspis_SetPending(void);
void isrspis_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the isrspis ISR. */
#define isrspis_INTC_VECTOR            ((reg32 *) isrspis__INTC_VECT)

/* Address of the isrspis ISR priority. */
#define isrspis_INTC_PRIOR             ((reg8 *) isrspis__INTC_PRIOR_REG)

/* Priority of the isrspis interrupt. */
#define isrspis_INTC_PRIOR_NUMBER      isrspis__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable isrspis interrupt. */
#define isrspis_INTC_SET_EN            ((reg32 *) isrspis__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the isrspis interrupt. */
#define isrspis_INTC_CLR_EN            ((reg32 *) isrspis__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the isrspis interrupt state to pending. */
#define isrspis_INTC_SET_PD            ((reg32 *) isrspis__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the isrspis interrupt. */
#define isrspis_INTC_CLR_PD            ((reg32 *) isrspis__INTC_CLR_PD_REG)


#endif /* CY_ISR_isrspis_H */


/* [] END OF FILE */
