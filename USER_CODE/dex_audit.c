
	/* ----------------------- GLOBAL CONSTANTS -------------------------- */
	#define 	BUFFER_SIZE 			5500 			// data buffer size
	#define 	FIELD_BUFFER			120
	#define 	MAX_FIELD       		249			// max field buffer
	#define 	SUCCESS			  	   0	  			// function will return 0 if successful
	#define 	FAIL						-1   			// Function will return -1 if unsuccessful
	#define 	MILLI_SECONDS	 		15

	/* ------------------------- DEX Commands --------------------------- */
	#define 	NUL     					0x00
	#define 	SOH						0x01
	#define 	STX						0x02
	#define 	ETX						0x03
	#define 	EOT						0x04
	#define 	ENQ						0x05
	#define 	ACK						0x06
	#define 	DLE						0x10
	#define 	NAK						0x15
	#define 	SYN						0x16
	#define 	ETB						0x17

	/* ----------------------- GLOBAL VARIABLES ------------------------- */
	#define  CHANGER_IS_SLAVE  "1234567890RR01L01"  /* COM_ID R REV_LEVEL */
	#define  CHANGER_IS_MASTER "001234567890R01L01" /*RESP_CODE COM_ID REV_LEVEL */
	#define  SLAVE  0
	#define  MASTER 1
	extern   SetforDEX();
	extern   int endbuf,startbuf,ccbuf[],MenuNum;
	extern   int send(char x);
	extern   int wait4(unsigned char &ch);
	extern   char wait(int time);
	extern   void FlushCom();
	extern   void setvects();		     			// defined in pc_io.cpp
	extern   i_disable(), resvects();      	// defined in pc_io.CPP
	extern   void init_serial();

	int 		Port;
	char		*field[500];
	char     third_handshake_data[250];
	int      lastfield;
	int      outofmem;
	int      dont_ENQ_twice;
	unsigned int 	BCC, BCC_0, BCC_1, BCC_14, DATA_0, X2, X15, X16;
	int spincnt;										// current whirler pointer

	FILE  *fdex;

	////////////////////////////////////////////////////////////////////////
	/* This function saves the dex audit data to disk in exactly the same
	format that it is retieved from the coin mech/VMC
	*/

	void writedex()
	{
		fclose(fdex);
		  if((fdex = fopen("DUMP.DEX","w")) == NULL)
			   {
					printf("\n\nUnable to open file DUMP.DEX\n\n");
				}
			else
				{
				for(int i=0;i<lastfield;i++)
				fprintf(fdex,"%s",field[i]);
			   }
			fclose(fdex);
	}


	////////////////////////////////////////////////////////////////////////
	void GetPort(char p)
	/* sets up the communications port */
	{
		switch(p)
	          {
					case '1':       Port = 1;       break;
					case '2':       Port = 2;       break;
					case '3':       Port = 3;       break;
					case '4':       Port = 4;       break;
					default:			 	Port = 1;				break;
				 }

	}
	////////////////////////////////////////////////////////////////////////
	/* Nice little spinner to show program activity */
	void spin()
	{
		int   x=1;
	   int   y =10;
	   char  spinner[4]={'\\','|','/','-'};
	   if (spincnt>3) spincnt=0;
		gotoxy(x,y);
	   printf("%c",spinner[spincnt++]);

	}
	////////////////////////////////////////////////////////////////////////
	int wait_for_response(unsigned char &ch)
	{
		while (wait4(ch) == 0)       // Wait for a response
		{
		  if (kbhit())
			 if (getch() == ESC)      // User hit the ESC key
				return(FAIL);
		}
		return(SUCCESS);
	}// wait_for_response
	////////////////////////////////////////////////////////////////////////
	void alternate_answer(int j)
	{
	   delay(MILLI_SECONDS);
	   send(DLE);
	   delay(MILLI_SECONDS);
	   send(0x30+(j));
	}
	////////////////////////////////////////////////////////////////////////
	/* CRC check routine  */
	void crc_16(char data)
	{
	   int j;
	   for (j=0;j<8;j++)
	   {
	     DATA_0 = (data >> j) & 0x0001;
	     BCC_0 = (BCC & 0x0001);
	     BCC_1 = (BCC >> 1) & 0x0001;
	     BCC_14 = (BCC >> 14) & 0x0001;
	     X16 = (BCC_0 ^ DATA_0) & 0x0001; 	// bit15 of BCC after shift
	     X15  = (BCC_1 ^ X16) & 0x0001;		// bit0 of BCC after shift
	     X2  = (BCC_14 ^ X16) & 0x0001;		// bit13 of BCC after shift
	     BCC = BCC >> 1;
	     BCC = BCC & 0x5FFE;
	     BCC = BCC | (X15);
	     BCC = BCC | (X2 << 13);
	     BCC = BCC | (X16 << 15);
	   }
	}
	////////////////////////////////////////////////////////////////////////
	int slave_handshake()
	{
		unsigned char response;
		BCC = NUL;
		for (;;)
		{
		  if (wait_for_response(response)==FAIL)  /* Waiting for ENQ */
			 break;
		  delay(MILLI_SECONDS);
			if (response == ENQ)
			 alternate_answer(0);
		  else if (response == ETX)
		  	{
			 	crc_16(response);
			 	if (wait_for_response(response)==FAIL)
					return(FAIL);
			 	crc_16(response);
			 	if (wait_for_response(response)==FAIL)
					return(FAIL);
			 	crc_16(response);
			 	delay(MILLI_SECONDS);
			 	if (BCC == NUL)
					alternate_answer(1);
			 	else
			 		{
						BCC = NUL;
  						send(NAK);
			 		}
		  	}
		  else if (response == EOT)
			 return(SUCCESS);
		  else if ((response != DLE) && (response != SOH) && (response != STX))
			 crc_16(response);

	 	} /* end of forever loop */
		return(FAIL);
	}
	////////////////////////////////////////////////////////////////////////
	int master_handshake(char data_string[])
	{
	   int i;
	   char done = FALSE;
	   unsigned char response1,response2;
	   unsigned char response = NUL;
	   BCC = NUL;


	   if (dont_ENQ_twice == 1)   // True on Master Mode
	   	  send(ENQ);

	   i=0;
	   while (!done)
	  		 {
	     		response = wait(100);
	     		if (dont_ENQ_twice == 1)    // True in Master mode
	     			{
	       			while (response != DLE)
	       					{
									if (kbhit())
	                           if (getch() == ESC)
					 						return(FAIL);

									send(ENQ);
									response = wait(100);
			 					}


			 			if (wait_for_response(response)== FAIL)
							return(FAIL);
			 			if (response != 0x30)
							return(FAIL);
		  			}

		  		dont_ENQ_twice = 0;
		  		delay(MILLI_SECONDS);
		  		send(DLE);
		  		delay(MILLI_SECONDS);
		  		send(SOH);
		  		delay(MILLI_SECONDS);

		  		for (i=0;i<strlen(data_string);i++)
		  			{
			 			crc_16(data_string[i]);
			 			send(data_string[i]);
			 			delay(MILLI_SECONDS);
		  			}
		  		send(DLE);
		  		  delay(MILLI_SECONDS);
		  		send(ETX);
		  		  delay(MILLI_SECONDS);
		  	  	crc_16(ETX);
	         send(BCC&0x00FF);
		  		  delay(MILLI_SECONDS);
		  		send((BCC&0xFF00)>>8);
		  		  delay(MILLI_SECONDS);
		  		  delay(5*MILLI_SECONDS);
		  		if (wait_for_response(response1)==FAIL)
			 		return(FAIL);
		  		if (wait_for_response(response2)==FAIL)
			 		return(FAIL);
		  		if ((response1 == DLE) && ((response2 == 0x31) || (response2 ==0x30)))
		  			{
		 				send(EOT);
			 			return(SUCCESS);
		  			}
		  		done = TRUE;
		  		if (response1 == NAK)
			 		done = FALSE;
		  		else return(FAIL);
		    }
		return(SUCCESS);
	}
	////////////////////////////////////////////////////////////////////////
	int third_handshake(int error)
	{
	unsigned char response,response1,response2;
	int           j, i, block_number, save_i, k, fieldlen, savefield;

	   savefield=0;
	   lastfield=0;
	   save_i=0;
	   k=0;
	   i=0;
	   fieldlen=0;
	   error = 0;
		j=1;
		block_number=1;
		*field[i]=NULL;
		outofmem=FALSE;
		BCC = 0x0000;
		for (;;)
		{
		  if (wait_for_response(response)==FAIL)
	 			 return(FAIL);

		  if (response == ENQ)
		  {
				 alternate_answer(0);
				 i = 0;
		  }

		  else if (response == STX)
		  {
				 save_i = i;
				 savefield=fieldlen;
				 BCC = 0x0000;
		  }

		  else if (response == ETB)
		  {
				 crc_16(response);
				 if (wait_for_response(response1)==FAIL)
					return(FAIL);
				 if (wait_for_response(response2)==FAIL)
					return(FAIL);
				 crc_16(response1);
				 crc_16(response2);


				 if (BCC == 0x0000)
				 {
						if (block_number == error)
				  		{
					  		i = save_i;
					  		send(NAK);
				 		}
				 		else
						{
					  		alternate_answer(int(j%2));
					  		j++;
				 		}
				 		block_number++;
			 	 }
			 	 else
			 	 {
						if(field[save_i]!=NULL)
					  	strcpy(third_handshake_data,field[save_i]);
						for(k=save_i;k<i;k++)
					  		free(field[k]);
						i = save_i;
						fieldlen=savefield;
						send(NAK);
			 	 }
		  }


		  else if (response == ETX)
		  {
				 crc_16(response);
				 if (wait_for_response(response1)==FAIL)
						return(FAIL);

				 if (wait_for_response(response2)==FAIL)
						return(FAIL);
				 crc_16(response1);
				 crc_16(response2);

				 if (BCC == 0x0000)
				 {
						alternate_answer(int(j%2));
						j++;
				 }
				 else
				 {
						if(field[save_i]!=NULL)
							  strcpy(third_handshake_data,field[save_i]);

						for(k=save_i;k<i;k++)
				 	 	   free(field[k]);

						i = save_i;
						fieldlen=savefield;
						send(NAK);
				 }
		  }

		  else if (response == EOT)
		  {
				 lastfield=i;
				 return(SUCCESS);
		  }

		  else if ((response != DLE) && (response != SOH))
		  {
				 crc_16(response);
				 third_handshake_data[fieldlen++] = toupper(response);

				 if(response==LF)
	  	  			{
				 		third_handshake_data[fieldlen]=NULL;
				 		field[i]=(char *)malloc(strlen(third_handshake_data)+1);
				 		fieldlen=0;

				 		if(field[i]!=NULL)
							{
					  			strcpy(field[i],third_handshake_data);
					  			if((i+1)<MAX_FIELD)
	 				  				i++;
					  			*field[i]=NULL;
							}
						else
				  				outofmem=TRUE;
			 		}
		  }
		}//end forever loop
	}
	////////////////////////////////////////////////////////////////////////
	int Do_dex()
	{
		int  	 dex_com;
	   char 	 ch;

		  clrscr();
	     printf("  Program:       Dexread.exe\n");
	     printf("  Description:   Example DEX Audit data collection program \n");
	     printf("                 Released to the public domain by\n");
	     printf("                 Coin Acceptors Europe Ltd. - Engineering Dept\n\n\n");
	     printf("  Usage:         DEXREAD #\n");
	     printf("                 Where # is the COM port to use (1 or 2)\n\n\n\n");
	     printf("  DEXREAD is using COM%d\n",Port);
		  printf("  Hit ESC to exit program.\n\n");

		  FlushCom();
		  dex_com = NAK;


	for(;;)
		  {
				 send(ENQ);

				 if ((ch = wait(200)) == ENQ)
					 {
							dex_com = MASTER;
	             		break;
	             }

				 if (ch == DLE)
				  	 {
							if (wait_for_response((unsigned char)ch) == FAIL)
				 				{
								  ch = NAK;
								  dex_com = NAK;
								  break;
								}
								else
								{
								  dex_com = SLAVE;
								  break;
								}
			 		 }

		 		 if (kbhit())
							if ((ch = getch()) == ESC)
								{
				 				  ch = NAK;
								  dex_com = NAK;
								  break;
								}

		  }  /* forever bracket */


		  if (dex_com == SLAVE)      // GLOBAL COMMUNICATION
		  {
			 	 dont_ENQ_twice = 0;
				 if (master_handshake(CHANGER_IS_SLAVE) == SUCCESS)
					 {
						printf("Master Handshake Successful\n");
						if (slave_handshake() == SUCCESS)
							{
							  printf("Slave Handshake Successful\n");
							  if (third_handshake(0) == SUCCESS)
								  {
									 printf("Third Handshake Successful\n");
								  }
		        			  else
				  				  {
					 				 printf("Third Handshake Failed\n");
					 				 ch=ESC;
				  				  }
							}
						else
							{
				  			  printf("Slave Handshake Failed\n");
				  			  ch=ESC;
							}
			 		 }
			    else
			 		 {
						printf("Master Handshake Failed\n");
						ch=ESC;
			 		 }
		  }
		  else if (dex_com == MASTER)
		  {
			    dont_ENQ_twice = 1;
			    if (slave_handshake() == SUCCESS)
			 		 {
						printf("Slave Handshake Successful\n");
						if (master_handshake(CHANGER_IS_MASTER) == SUCCESS)
							{
				  				printf("Master Handshake Successful\n");
				  				if (third_handshake(0) == SUCCESS)
				  					{
					 					printf("Third Handshake Successful\n");
				  					}
				  				else
				  					{
					 					printf("Slave Handshake Failed\n");
					 					ch=ESC;
				  					}
							}
						else
							{
				  				printf("Third Handshake Failed\n");
				  				ch=ESC;
							}
			 		 }
			 	 else
				 	 {
						printf("Master Handshake Failed\n");
						ch=ESC;
			 		 }
		  }
		  else
		  {
			 printf("DEX Communications Failed\n");
			 ch=ESC;
		  }
		return(SUCCESS);
	}



	////////////////////////////////////////////////////////////////////////////
	/* ------------------------- MAIN PROGRAM --------------------------- */
/*	void main(int argc, char * argv[])
	{

	   clrscr();
		if (argc < 2)
			Port = 1;
		else GetPort(argv[1][0]);
		setvects();

		init_serial();
		SetforDEX();
		Do_dex();
		writedex();
		i_disable();  	// turn off serial int
		resvects();		// putback old int vectors
		fcloseall();
		exit(0);

	}
	*/

	/* main */
  	////////////////////////////////////////////////////////////////////////
