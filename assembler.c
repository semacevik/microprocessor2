#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int hex2int( char* hex)  
{
	int result=0;

	while ((*hex)!='\0')
	{
		if (('0'<=(*hex))&&((*hex)<='9'))
			result = result*16 + (*hex) -'0';
		else if (('a'<=(*hex))&&((*hex)<='f'))
			result = result*16 + (*hex) -'a'+10;
		else if (('A'<=(*hex))&&((*hex)<='F'))
			result = result*16 + (*hex) -'A'+10; 
		hex++;
	}
	return(result);
}


main()
{ 	
	FILE *fp;
        char line[100];
        char *token = NULL;   
	char *op1, *op2, *op3, *label; //where do I allocate memory for these?
	char ch;
	int  chch;

	int program[1000];
	int counter=0;

	struct label
	{
		int location;
		char *label;
	};
	struct label labeltable[50];
	int nooflabels = 0;

	struct jumpinstruction   //jump instructions are modified by labels
	{
		int location;
		char *label;
	};
	struct jumpinstruction jumptable[100];
	int noofjumps=0;	

	struct variable
	{
		int location;
		char *name;
	};
	struct variable variabletable[50];
	int noofvariables = 0;

	struct ldiinstruction    //ldi instructions are modified by variables
	{
		int location;
		char *name;
	};
	struct ldiinstruction lditable[100];
	int noofldis=0;


			
	
	fp = fopen("polling.txt","r");
	
	if (fp != NULL)
	{
		while(fgets(line,sizeof line,fp)!= NULL)  //skip till .code
		{
			token=strtok(line,"\n\t\r ");
			if (strcmp(token,".code")==0 )
				break;
		} 
		while(fgets(line,sizeof line,fp)!= NULL)
		{
			token=strtok(line,"\n\t\r ");
			while (token)
			{
				if (strcmp(token,"ldi")==0)
				{
					op1 = strtok(NULL,"\n\t\r ");
					op2 = strtok(NULL,"\n\t\r ");
					program[counter]=0x1000+hex2int(op1);
					counter++;
					if ((op2[0]=='0')&&(op2[1]=='x')) 
						program[counter]=hex2int(op2+2)&0xffff;
					else if ((  (op2[0])=='-') || ((op2[0]>='0')&&(op2[0]<='9'))  )
						program[counter]=atoi(op2)&0xffff;  
					else
					{
						lditable[noofldis].location = counter;
						op1=(char*)malloc(sizeof(op2));
						strcpy(op1,op2);
						lditable[noofldis].name = op1;
						noofldis++;
					}		
					counter++;
				}
				else if (strcmp(token,"ld")==0) 
				{
					op1 = strtok(NULL,"\n\t\r ");
					op2 = strtok(NULL,"\n\t\r ");
					ch = (op1[0]-48)| ((op2[0]-48)<<3);
					program[counter]=0x2000+((ch)&0x00ff); 
					counter++;
				}
				
				else if (strcmp(token,"st")==0) 
				{
					op1 = strtok(NULL,"\n\t\r ");
					op2 = strtok(NULL,"\n\t\r ");
					ch = ((op1[0]-48) <<3)|((op2[0]-48)<<6);
					program[counter]=0x3000+((ch)&0x00ff); 
					counter++;
                                          
				}
				
				else if (strcmp(token,"jz")==0) 
				{
					op1 = strtok(NULL,"\n\t\r ");
					jumptable[noofjumps].location = counter;
					op2=(char*)malloc(sizeof(op1));
					strcpy(op2,op1);
					jumptable[noofjumps].label=op2;
					noofjumps++;
					program[counter]=0x4000;
					counter++;
                                         
				}
				else if (strcmp(token,"jmp")==0) 
				{
					op1 = strtok(NULL,"\n\t\r ");
					jumptable[noofjumps].location = counter;
					op2=(char*)malloc(sizeof(op1));
					strcpy(op2,op1);
					jumptable[noofjumps].label=op2;
					noofjumps++;
					program[counter]=0x5000;
					counter++;
				}				
				else if (strcmp(token,"add")==0)
				{
				
					op1 = strtok(NULL,"\n\t\r ");
					op2 = strtok(NULL,"\n\t\r ");
					op3 = strtok(NULL,"\n\t\r ");
					chch = ((op2[0]-48)<<6)| ((op3[0]-48)<<3)|((op1[0]-48));  //4 0 5
					program[counter]=0x7000+((chch)&0x01ff); 
					counter++; 
				}
				else if (strcmp(token,"sub")==0)
				{
					op1 = strtok(NULL,"\n\t\r ");
					op2 = strtok(NULL,"\n\t\r ");
					op3 = strtok(NULL,"\n\t\r ");
					chch = (op1[0]-48)| ((op3[0]-48)<<3)|((op3[0]-48)<<6);  //4 0 5
					program[counter]=0x7200+((chch)&0x01ff); 
					counter++;                                           
				}
				else if (strcmp(token,"and")==0)
				{
					op1 = strtok(NULL,"\n\t\r ");
					op2 = strtok(NULL,"\n\t\r ");
					op3 = strtok(NULL,"\n\t\r ");
					chch = (op1[0]-48)| ((op3[0]-48)<<3)|((op2[0]-48)<<6);  //4 0 5
					program[counter]=0x7400+((chch)&0x01ff); 
					counter++; 
                                       //to be added
				}
				else if (strcmp(token,"or")==0)
				{
					op1 = strtok(NULL,"\n\t\r ");
					op2 = strtok(NULL,"\n\t\r ");
					op3 = strtok(NULL,"\n\t\r ");
					chch = (op1[0]-48)| ((op3[0]-48)<<3)|((op2[0]-48)<<6);  //4 0 5
					program[counter]=0x7600+((chch)&0x01ff); 
					counter++;
                                        
				}
					else if (strcmp(token,"xor")==0)
				{
					op1 = strtok(NULL,"\n\t\r ");
					op2 = strtok(NULL,"\n\t\r ");
					op3 = strtok(NULL,"\n\t\r ");
					chch = (op1[0]-48)| ((op3[0]-48)<<3)|((op2[0]-48)<<6);  //4 0 5
					program[counter]=0x7800+((chch)&0x01ff); 
					counter++;
                                        
				}			
				else if (strcmp(token,"not")==0)
				{
					op1 = strtok(NULL,"\n\t\r ");
					op2 = strtok(NULL,"\n\t\r ");
					ch = (op1[0]-48)| ((op2[0]-48)<<3);
					program[counter]=0x7e00+((ch)&0x00ff);  
					counter++;
				}
				else if (strcmp(token,"mov")==0)
				{	
					op1 = strtok(NULL,"\n\t\r ");
					op2 = strtok(NULL,"\n\t\r ");
					ch = (op1[0]-48)| ((op2[0]-48)<<3);
					program[counter]=0x7e40+((ch)&0x00ff);  
					counter++;                                    
				}
				else if (strcmp(token,"inc")==0)
				{
					op1 = strtok(NULL,"\n\t\r ");
					ch = (op1[0]-48)| ((op1[0]-48)<<3);
					program[counter]=0x7e80+((ch)&0x00ff);  
					counter++;
				}
			else if (strcmp(token,"dec")==0)
				{
					op1 = strtok(NULL,"\n\t\r ");
					ch = (op1[0]-48)| ((op1[0]-48)<<3);
					program[counter]=0x7ec0+((ch)&0x00ff);  
					counter++;
                            
				}
			else if (strcmp(token,"pop")==0)
				{
					op1 = strtok(NULL,"\n\t\r ");
					ch = (op1[0]-48);
					program[counter]=0x9000+((ch)&0x00ff);  
					counter++;
                            
				}
			else if (strcmp(token,"push")==0)
				{
					op1 = strtok(NULL,"\n\t\r ");
					ch = (op1[0]-48)<<6;
					program[counter]=0x8000+((ch)&0x00ff);  
					counter++;
                            
				}
			else if (strcmp(token,"call")==0)
				{
					op1 = strtok(NULL,"\n\t\r ");
					jumptable[noofjumps].location = counter;
					op2=(char*)malloc(sizeof(op1));
					strcpy(op2,op1);
					jumptable[noofjumps].label=op2;
					noofjumps++;
					program[counter]=0xa000+((ch)&0x00ff);  
					counter++;
                            
				}
			else if (strcmp(token,"ret")==0)
				{
					op1 = strtok(NULL,"\n\t\r ");
		
					program[counter]=0xb000;  
					counter++;
                            
				}
				else
				{
					labeltable[nooflabels].location = counter;  
					op1=(char*)malloc(sizeof(token));
					strcpy(op1,token);
					labeltable[nooflabels].label=op1;
					nooflabels++;
				} 
				token = strtok(NULL,",\n\t\r ");
			}
		}

	
// supply the address fields of the jump and jz instructions
		int i,j;         
		for (i=0; i<noofjumps;i++)
		{
			j=0;
			while ( strcmp(jumptable[i].label , labeltable[j].label) != 0 )
				j++;
			program[jumptable[i].location] +=(labeltable[j].location-jumptable[i].location-1)&0x0fff;
		}

	
// search for the start of the .data segment
		rewind(fp);  
		while(fgets(line,sizeof line,fp)!= NULL)  //skip till .data, if no .data, also ok.
		{
			token=strtok(line,"\n\t\r ");
			if (strcmp(token,".data")==0 )
				break;

		}
// process the data segment
		int dataarea=0;
 		while(fgets(line,sizeof line,fp)!= NULL)
		{
			token=strtok(line,"\n\t\r ");
			if (strcmp(token,".code")==0 )  //go till the .code segment
				break;
			else if (token[strlen(token)-1]==':')
			{				
				token[strlen(token)-1]='\0';  //will not cause memory leak, as we do not do malloc
				variabletable[noofvariables].location=counter+dataarea;
				op1=(char*)malloc(sizeof(token));
				strcpy(op1,token);
				variabletable[noofvariables].name=op1;
				token = strtok(NULL,",\n\t\r ");
				if (token==NULL)
					program[counter+dataarea]=0;
				else if (strcmp(token, ".space")==0)
				{
					token=strtok(NULL,"\n\t\r ");
					dataarea+=atoi(token);
				}
				else if((token[0]=='0')&&(token[1]=='x')) 
					program[counter+dataarea]=hex2int(token+2)&0xffff; 
				else if ((  (token[0])=='-') || ((token[0]>='0')&&(token[0]<='9'))  )
					program[counter+dataarea]=atoi(token)&0xffff;  
				noofvariables++;
				dataarea++;
			}
		}

// supply the address fields for the ldi instructions from the variable table
		for( i=0; i<noofldis;i++)
		{
			j=0;
			while ((j<noofvariables)&&( strcmp( lditable[i].name , variabletable[j].name)!=0 ))
				j++;
			if (j<noofvariables)
				program[lditable[i].location] = variabletable[j].location;				
		} 

// supply the address fields for the ldi instructions from the label table
		for( i=0; i<noofldis;i++)
		{
			j=0;
			while ((j<nooflabels)&&( strcmp( lditable[i].name , labeltable[j].label)!=0 ))
				j++;
			if (j<nooflabels){
				program[lditable[i].location] = (labeltable[j].location)&0x0fff;
				printf("%d %d %d\n", i, j, (labeltable[j].location));	
			}			
		} 

//display the resulting tables
		printf("LABEL TABLE\n");
		for (i=0;i<nooflabels;i++)
			printf("%d %s\n", labeltable[i].location, labeltable[i].label);	
		printf("\n");
		printf("JUMP TABLE\n");
		for (i=0;i<noofjumps;i++)
			printf("%d %s\n", jumptable[i].location, jumptable[i].label);	
		printf("\n");
		printf("VARIABLE TABLE\n");
		for (i=0;i<noofvariables;i++)
			printf("%d %s\n", variabletable[i].location, variabletable[i].name);	
		printf("\n");
		printf("LDI INSTRUCTIONS\n");
		for (i=0;i<noofldis;i++)
			printf("%d %s\n", lditable[i].location, lditable[i].name);	
		printf("\n");
		fclose(fp);
		fp = fopen("RAM.txt","w");  //saves the program
		fprintf(fp,"v2.0 raw\n");
		for (i=0;i<counter+dataarea;i++)
			fprintf(fp,"%04x\n",program[i]);
	}		
}

