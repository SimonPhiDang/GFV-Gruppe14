/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <I2C_handler.h>

#include <project.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


void readTemp()
{
    //Variabler 
    uint8 b1,b2;
    uint8 slaveAddress = 36;
    uint8 readCommand = 1;
    uint8 I2C_MSTR_NO_ERROR;
    int16_t tempData;
    float temp;
    
    //1001 A2 A1 A0  Alle A'er er modsat ON = 0 
    uint8 status = I2C_1_MasterSendStart(slaveAddress,readCommand);
    
    if (status == I2C_MSTR_NO_ERROR)
    {
        // Læser første byte i register MSB første 8 bit
            b1 = I2C_1_MasterReadByte(I2C_1_ACK_DATA);  
            UART_1_PutString("b1 success");
        // Læser anden byte i register, sidste byte er NAKed  // LSB sidste 8 bit
            b2 = I2C_1_MasterReadByte(I2C_1_NAK_DATA);
            UART_1_PutString("b2 success");
        //    MasterSendStop
            I2C_1_MasterSendStop();        
        //    Combine bytes to a temperature (2 Komplement konvert)
            tempData = b1 | b2 ;
        // Data skal aflæses som 2 komplements int så fortegn er korrekt
            temp = tempData / 256;
        //    Send temperature to UART
            writeTempToUART(temp);
    }
    else {
//    report error on UART
      UART_1_PutString("Error I2C");  
//    MasterSendStop
      I2C_1_MasterSendStop();
    }  
}
// Udprintning til UART temperaturen
void writeTempToUART(float temp)
{
    char outputBuffer[256];
    snprintf(outputBuffer, sizeof(outputBuffer), "Temp: %f \r\n", temp);
    UART_1_PutString(outputBuffer);
}


/* [] END OF FILE */
