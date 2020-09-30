/*******************************************************************************
* File Name: isrspim.h
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
#if !defined(CY_ISR_isrspim_H)
#define CY_ISR_isrspim_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void isrspim_Start(void);
void isrspim_StartEx(cyisraddress address);
void isrspim_Stop(void);

CY_ISR_PROTO(isrspim_Interrupt);

void isrspim_SetVector(cyisraddress address);
cyisraddress isrspim_GetVector(void);

void isrspim_SetPriority(uint8 priority);
uint8 isrspim_GetPriority(void);

void isrspim_Enable(void);
uint8 isrspim_GetState(void);
void isrspim_Disable(void);

void isrspim_SetPending(void);
void isrspim_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the isrspim ISR. */
#define isrspim_INTC_VECTOR            ((reg32 *) isrspim__INTC_VECT)

/* Address of the isrspim ISR priority. */
#define isrspim_INTC_PRIOR             ((reg8 *) isrspim__INTC_PRIOR_REG)

/* Priority of the isrspim interrupt. */
#define isrspim_INTC_PRIOR_NUMBER      isrspim__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable isrspim interrupt. */
#define isrspim_INTC_SET_EN            ((reg32 *) isrspim__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the isrspim interrupt. */
#define isrspim_INTC_CLR_EN            ((reg32 *) isrspim__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the isrspim interrupt state to pending. */
#define isrspim_INTC_SET_PD            ((reg32 *) isrspim__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the isrspim interrupt. */
#define isrspim_INTC_CLR_PD            ((reg32 *) isrspim__INTC_CLR_PD_REG)


#endif /* CY_ISR_isrspim_H */


/* [] END OF FILE */
