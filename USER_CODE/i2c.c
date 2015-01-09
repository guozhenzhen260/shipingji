/*****************************************************************************
 *   i2c.c:  I2C C file for NXP LPC17xx Family Microprocessors
 *
 *   Copyright(C) 2009, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.05.26  ver 1.00    Prelimnary version, first Release
 *
*****************************************************************************/
#include "..\driver\LPC17xx.h"
#include "..\driver\lpc_types.h"
#include "global.h"
#include "i2c.h"

volatile uint8_t I2C0_State = I2C_IDLE;

volatile uint8_t I2C0_CmdType;
volatile uint8_t I2C0_Mode;

volatile uint8_t I2C0_Buffer[I2C_BUFSIZE];
volatile uint8_t I2C0_Count = 0;
volatile uint8_t I2C0_RdLength;
volatile uint8_t I2C0_WrLength;

volatile uint8_t I2C0_RdIndex = 0;
volatile uint8_t I2C0_WrIndex = 0;

/* 
From device to device, the I2C communication protocol may vary, 
in the example below, the protocol uses repeated start to read data from or 
write to the device:
For master read: the sequence is: STA,Addr(W),offset,RE-STA,Addr(r),data...STO 
for master write: the sequence is: STA,Addr(W),length,RE-STA,Addr(w),data...STO
Thus, in state 8, the address is always WRITE. in state 10, the address could 
be READ or WRITE depending on the I2C0_CmdType.
*/   

/*****************************************************************************
** Function name:		I2C0_IRQHandler
**
** Descriptions:		中断处理函数I2C0 interrupt handler, deal with master mode
**				only.
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void I2C0_IRQHandler(void)  
{
	uint8_t StatValue;

	OS_ENTER_CRITICAL(); 	 //   ucosii add
	OSIntEnter();

	/* this handler deals with master read and master write only */
	StatValue = LPC_I2C0 -> I2STAT;

	switch ( StatValue )
	{
		case 0x08:			/* A Start condition is issued. */
			LPC_I2C0 -> I2DAT = I2C0_Buffer[0];
			LPC_I2C0 -> I2CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);
			I2C0_State = I2C_STARTED;
			break;

		case 0x10:			/* A repeated started is issued */
			if ( I2C0_CmdType == I2C_READ )
			{
			  LPC_I2C0 -> I2DAT = I2C0_Buffer[2];
			}
			LPC_I2C0 -> I2CONCLR = (I2CONCLR_SIC | I2CONCLR_STAC);
			I2C0_State = I2C_RESTARTED;
			break;

		case 0x18:			/* Regardless, it's a ACK */
			if ( I2C0_State == I2C_STARTED )
			{
			  LPC_I2C0 -> I2DAT = I2C0_Buffer[1+I2C0_WrIndex];
			  I2C0_WrIndex++;
			  I2C0_State = I2C_DATA_ACK;
			}
			LPC_I2C0 -> I2CONCLR = I2CONCLR_SIC;
			break;

		case 0x28:	/* Data byte has been transmitted, regardless ACK or NACK */
		case 0x30:
			if ( I2C0_WrIndex < I2C0_WrLength )
			{   
				  LPC_I2C0 -> I2DAT = I2C0_Buffer[1+I2C0_WrIndex]; /* this should be the last one */
				  I2C0_WrIndex++;  
				  I2C0_State = I2C_DATA_ACK;
			}
			else
			{
				  I2C0_WrIndex = 0;
				  if ( I2C0_RdLength > 0 )
				  {
					LPC_I2C0 -> I2CONSET = I2CONSET_STA;	/* Set Repeated-start flag */
					I2C0_State = I2C_REPEATED_START;
				  }
				  else
				  {
					I2C0_State = I2C_DATA_NACK;
				  }
			}
			LPC_I2C0 -> I2CONCLR = I2CONCLR_SIC;
			break;

		case 0x40:	/* Master Receive, SLA_R has been sent */
			LPC_I2C0 -> I2CONCLR = I2CONCLR_SIC;
			break;

		case 0x50:	/* Data byte has been received, regardless following ACK or NACK */
		case 0x58:
			I2C0_Buffer[3+I2C0_RdIndex] = LPC_I2C0 -> I2DAT;
			I2C0_RdIndex++;
			if ( I2C0_RdIndex < I2C0_RdLength)
			{   
			  LPC_I2C0 -> I2CONSET = I2CONSET_AA; //
			  I2C0_State = I2C_DATA_ACK;
			}
			else
			{
			  I2C0_RdIndex = 0;
			  LPC_I2C0 -> I2CONCLR = I2CONCLR_AAC;	//
			  I2C0_State = I2C_DATA_NACK;
			}
			LPC_I2C0 -> I2CONCLR = I2CONCLR_SIC;
			break;

		case 0x20:		/* regardless, it's a NACK */

		case 0x48:
			LPC_I2C0 -> I2CONCLR = I2CONCLR_SIC;
			I2C0_State = I2C_DATA_NACK;	  
			break;

		case 0x38:		/* Arbitration lost, in this example, we don't
						deal with multiple master situation */
		default:
			LPC_I2C0 -> I2CONCLR = I2CONCLR_SIC;	
			break;
	}

	OSIntExit();
	OS_EXIT_CRITICAL();	//   ucosii add
}

/*****************************************************************************
** Function name:		I2C0_Start
**
** Descriptions:		Create I2C start condition, a timeout
**				value is set if the I2C never gets started,
**				and timed out. It's a fatal error. 
**
** parameters:			None
** Returned value:		true or false, return false if timed out
** 
*****************************************************************************/
uint8_t I2C0_Start( void )
{
  uint32_t timeout = 0;
  uint8_t retVal = FALSE;
  
 /*--- Issue a start condition ---*/
  LPC_I2C0 -> I2CONSET = I2CONSET_STA;	/* Set Start flag */
    
  /*--- Wait until START transmitted ---*/
  while( 1 )
  {
	if ( I2C0_State == I2C_STARTED )
	{
	  retVal = TRUE;
	  break;	
	}
	if ( timeout >= I2C_MAX_TIMEOUT )
	{
	  retVal = FALSE;
	  break;
	}
	timeout++;
  }
  return( retVal );
}

/*****************************************************************************
** Function name:		I2C0_Stop
**
** Descriptions:		Set the I2C stop condition, if the routine
**				never exit, it's a fatal bus error.
**
** parameters:			None
** Returned value:		true or false, return false if timed out
** 
*****************************************************************************/
uint8_t I2C0_Stop( void )
{
  uint32_t timeout = 0;
  
  LPC_I2C0 -> I2CONSET = I2CONSET_STO;      /* Set Stop flag */ 
  LPC_I2C0 -> I2CONCLR = I2CONCLR_SIC;  /* Clear SI flag */ 
            
  /*--- Wait for STOP detected ---*/
  while( LPC_I2C0 -> I2CONSET & I2CONSET_STO )
  {
      if ( timeout >= I2C_MAX_TIMEOUT )
	  {
	      return FALSE;;
	  }
	  timeout++;
  }
  return TRUE; 
}

/*****************************************************************************
** Function name:		I2C0_Init
**
** Descriptions:		Initialize I2C controller
**
** parameters:			I2c mode is either MASTER or SLAVE
** Returned value:		true or false, return false if the I2C
**				interrupt handler was not installed correctly
** 
*****************************************************************************/
uint8_t I2C0_Init( uint8_t I2cMode ) 
{

  LPC_SC -> PCONP |= (1 << 19);
  LPC_PINCON -> PINSEL1 &= ~0x03C00000;
  LPC_PINCON -> PINSEL1 |= 0x01400000;	/* set PIO0.27 and PIO0.28 to I2C0 SDA and SCK */
							/* function to 01 on both SDA and SCK. */
  /*--- Clear flags ---*/
  LPC_I2C0 -> I2CONCLR = I2CONCLR_AAC | I2CONCLR_SIC | I2CONCLR_STAC | I2CONCLR_I2ENC;    

  /*--- Reset registers ---*/
  LPC_I2C0 -> I2SCLL   = I2SCLL_SCLL;
  LPC_I2C0 -> I2SCLH   = I2SCLH_SCLH;

  /* Install interrupt handler */	

  //NVIC_EnableIRQ(I2C0_IRQn);
  zyIsrSet((I2C0_IRQn +16), (unsigned long)I2C0_IRQHandler, PRIO_ELEVEN);
  zyIrqEnable();

  LPC_I2C0 -> I2CONSET = I2CONSET_I2EN;
  return( TRUE );
}

/*****************************************************************************
** Function name:		I2C0_Engine
**
** Descriptions:		The routine to complete a I2C transaction
**				from start to stop. All the intermitten
**				steps are handled in the interrupt handler.
**				Before this routine is called, the read
**				length, write length, I2C master buffer,
**				and I2C command fields need to be filled.
**				see i2cmst.c for more details. 
**
** parameters:			None
** Returned value:		true or false, return false only if the
**				start condition can never be generated and
**				timed out. 
** 
*****************************************************************************/
uint8_t I2C0_Engine( void ) 
{
  I2C0_State = I2C_IDLE;
  I2C0_RdIndex = 0;
  I2C0_WrIndex = 0;
  if ( I2C0_Start() != TRUE )
  {
	I2C0_Stop();
	return ( FALSE );
  }

  while ( 1 )
  {
	if ( I2C0_State == I2C_DATA_NACK )
	{
	  if(I2C0_Stop() == TRUE)
	  {
	      return ( TRUE );
	  }
	  else
	  {
	      return ( FALSE );
	  }
	}
  }         
}

/*****************************************************************************
** Function name:		I2C0_Write
**
** Descriptions:		Write I2C controller
**
** parameters:			DevAddr：地址 SubAddr：子地址 dat：写数据指针 wrlen：写数据长度，不超过I2C_BUFSIZE-2
** Returned value:	    true or false, return false if timed out
**				
** 
*****************************************************************************/
uint8_t I2C0_Write(uint8_t DevAddr, uint8_t SubAddr, uint8_t *dat, uint8_t wrlen) 
{
 uint8_t i, uclen;

  for ( i = 0; i < I2C_BUFSIZE; i++ )	/* clear buffer */
  {
	I2C0_Buffer[i] = 0;
  }

  if(wrlen > I2C_BUFSIZE - 2)
  {
      uclen = I2C_BUFSIZE - 1;
  }
  else 
  {
      uclen = wrlen + 1;
  }

  I2C0_WrLength = uclen;
  I2C0_RdLength = 0;

  I2C0_Buffer[0] = DevAddr;
  I2C0_Buffer[1] = SubAddr;

  for(i = 0; i < uclen - 1; i++)
  { 
     I2C0_Buffer[2 + i] = *dat++;
  }

  I2C0_CmdType = I2C_WRITE;

  return I2C0_Engine();  
}

/*****************************************************************************
** Function name:		I2C0_Read
**
** Descriptions:		Read I2C controller
**						
** parameters:			DevAddr：地址 SubAddr：子地址 dat：写数据指针 rdlen：读数据长度，不超过I2C_BUFSIZE-3
** Returned value:		true or false, return false if timed out
**				
** 
*****************************************************************************/
uint8_t I2C0_Read(uint8_t DevAddr, uint8_t SubAddr, uint8_t *dat, uint8_t rdlen)  
{
  uint8_t i;
  uint8_t ucstate, uclen;

  for ( i = 0; i < I2C_BUFSIZE; i++ )	/* clear buffer */
  {
	I2C0_Buffer[i] = 0;
  }

  if(rdlen > I2C_BUFSIZE - 3)
  {
      uclen = I2C_BUFSIZE - 3;
  }
  else 
  {
      uclen = rdlen;
  }

  I2C0_WrLength = 1;
  I2C0_RdLength = uclen;
  I2C0_Buffer[0] = DevAddr;
  I2C0_Buffer[1] = SubAddr;
  I2C0_Buffer[2] = DevAddr | I2C_RD_BIT;
  I2C0_CmdType = I2C_READ;
  
  ucstate = I2C0_Engine();

  for(i = 0; i < uclen; i++)
  {
      *dat++ = I2C0_Buffer[3+i];
  }
  return ucstate;
}
/******************************************************************************
**                            End Of File
******************************************************************************/

