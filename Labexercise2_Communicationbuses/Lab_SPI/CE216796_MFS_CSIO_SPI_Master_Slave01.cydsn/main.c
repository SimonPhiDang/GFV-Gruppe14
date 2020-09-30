/*******************************************************************************
* File: main.c
* 
* Version : 1.11
*
* Description: 
*   This application demonstrates master and slave CSIO (SPI) communication
*   using a single device. First, the master sends a string to the slave,
*   then the roles reverse and the slave sends a different string to the master.
*
********************************************************************************
* Copyright 2016, Cypress Semiconductor Corporation. All rights reserved.
* This software is owned by Cypress Semiconductor Corporation and is protected
* by and subject to worldwide patent and copyright laws and treaties.
* Therefore, you may use this software only as provided in the license agreement
* accompanying the software package from which you obtained this software.
* CYPRESS AND ITS SUPPLIERS MAKE NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* WITH REGARD TO THIS SOFTWARE, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT,
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************************/

#include "project.h"
#include "string.h"


/*******************************************************************************
* Defines for LED and CSIO (SPI) messages
*******************************************************************************/

#define LED_ON              (0u)
#define LED_OFF             (1u)

#define MAX_MSG_SIZE        (100)


/*******************************************************************************
* Global variables for messages
*******************************************************************************/

/* Flash-resident strings to transfer */
const char master_msg[] = "Cypress Rocks!";
const char slave_msg[] = "FM0+ Flexible Microcontrollers";

/* Ram-resident buffers to recieve the messages */
char master_recbuf[MAX_MSG_SIZE];
char slave_recbuf[MAX_MSG_SIZE];


/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary:
*  Main function performs following functions:
*   1: Initialize the LED (off)
*   2. Initialize the master (Tx enabled) and slave (Rx enabled) components
*   3: Send a message from master to slave
*   4: Verify sent data == received data
*   5: Switch Tx and Rx functions
*   6: Send a message from slave to master
*   7: Verify sent data == received data
*   8: Indicate success with green LED
*   9: Deinitialize the MFS components
* 
* Parameters:
*  None.
*
* Return:
*  Not used.
*
*******************************************************************************/
int main(void)
{
    char *mbufptr;      /* Pointer to walk through the master message/buffer */
    char *sbufptr;      /* Pointer to walk through the slave message/buffer */
    
    /* Initialize the LED pin as digital output, initially off */
    Green_LED_GpioInitOut( LED_OFF );
    
    /* Set up the pin routing */
    Master_SetPinFunc_SCK();
    Master_SetPinFunc_SOT();
    Master_SetPinFunc_SIN();
    
    Slave_SetPinFunc_SCK();
    Slave_SetPinFunc_SOT();
    Slave_SetPinFunc_SIN();
    
    
    /*
    Initialize the master (Tx enabled) and slave (Rx enabled) components
    */

    while( Mfs_Csio_Init( &Master_HW, &Master_Config ) )
    {
        /* Loop forever - ErrorInvalidParameter */
    }

    while( Mfs_Csio_EnableFunc( &Master_HW, CsioTx ) )
    {
        /* Loop forever - ErrorInvalidParameter */
    }
    
    while( Mfs_Csio_Init( &Slave_HW, &Slave_Config ) )
    {
        /* Loop forever - ErrorInvalidParameter */
    }

    while( Mfs_Csio_EnableFunc( &Slave_HW, CsioRx ) )
    {
        /* Loop forever - ErrorInvalidParameter */
    }
    
    
    /*
    Send a message from master to slave
    */
    
    /* Set the pointers to the start of the buffers */
    mbufptr = (char *)&master_msg[0];
    sbufptr = &slave_recbuf[0];
    
    /* Send and receive the whole string */
    while( *mbufptr != '\0' )
    {
        /* Master waits until it is ready and then sends data */
        while( TRUE != Mfs_Csio_GetStatus( &Master_HW, CsioTxEmpty ) )
        {
            /* Wait until TX buffer empty */
        }
        
        Mfs_Csio_SendData( &Master_HW, *mbufptr, TRUE );  
        
        /* Slave receives data */
        while( TRUE != Mfs_Csio_GetStatus( &Slave_HW, CsioRxFull ) )
        {
            /* Wait until RX buffer full */
        }
        
        *sbufptr = Mfs_Csio_ReceiveData( &Slave_HW );
        
        /* Move onto the next char */
        mbufptr++;
        sbufptr++;
    }
    
    /* Make sure the transaction is complete */
    while( TRUE != Mfs_Csio_GetStatus( &Master_HW, CsioTxIdle ) )
    {
        /* Wait until master TX bus idle */
    }   
    
    /* Verify sent data == received data */
    while( 0 != strcmp( master_msg, slave_recbuf ) )
    {
        /* Error - received data does not match sent data */
    }

    
    /*
    Switch functions - send from slave to master
    */    
    
    /* On the master, leave Tx enabled and enable Rx */
    while( Mfs_Csio_EnableFunc( &Master_HW, CsioRx ) )
    {
        /* Loop forever - ErrorInvalidParameter */
    }
    
    /* On the slave, disable Rx and enable Tx */
    while( Mfs_Csio_DisableFunc( &Slave_HW, CsioRx ) )
    {
        /* Loop forever - ErrorInvalidParameter */
    }
    
    while( Mfs_Csio_EnableFunc( &Slave_HW, CsioTx ) )
    {
        /* Loop forever - ErrorInvalidParameter */
    }
    
    
    /*
    Send a message from slave to master
    */

    /* Set the pointers to the start of the buffers */
    sbufptr = (char *)&slave_msg[0];
    mbufptr = &master_recbuf[0];
    
    /* Send and recieve the whole string */
    while( *sbufptr != '\0' )
    {    
        /* Slave waits until it is ready and then sends data */
        while( TRUE != Mfs_Csio_GetStatus( &Slave_HW, CsioTxEmpty ) )
        {
            /* Wait until TX buffer empty */
        }
        
        Mfs_Csio_SendData( &Slave_HW, *sbufptr, TRUE );  
        
        /* Send dummy data to disable master output (SOE bit in SMR register) */
        Mfs_Csio_SendData( &Master_HW, 0x00u, FALSE );  
        
        /* Master receives data */
        while( TRUE != Mfs_Csio_GetStatus( &Master_HW, CsioRxFull ) )
        {
            /* Wait until RX buffer full */
        }
        
        *mbufptr = Mfs_Csio_ReceiveData( &Master_HW );
        
        /* Move onto the next char */
        mbufptr++;
        sbufptr++;
    }
    
    /* Make sure the transaction is complete */
    while( TRUE != Mfs_Csio_GetStatus( &Master_HW, CsioTxIdle ) )
    {
        /* Wait until master TX bus idle */
    }
    
    while( TRUE != Mfs_Csio_GetStatus( &Slave_HW, CsioTxIdle ) )
    {
        /* Wait until slave TX bus idle */
    }
    
    /* Verify sent data == received data */
    while( 0 != strcmp( slave_msg, master_recbuf ) )
    {
        /* Error - received data does not match sent data */
    }
    
    /* Indicate successful slave -> master and master -> slave transfers */
    Green_LED_GpioPut( LED_ON );
    
    
    /* Turn off both MFS blocks */
    while( Mfs_Csio_DeInit( &Master_HW, TRUE ) )
    {
        /* Loop forever - ErrorInvalidParameter */
    }
    
    while( Mfs_Csio_DeInit( &Slave_HW, TRUE ) )
    {
        /* Loop forever - ErrorInvalidParameter */
    }
    
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
