//shell.c
//Sangeetha Jayasankar, Fall 2019

char* fileNaming(char*,char*,int);

void main()
{
enableInterrupts();
while(1)
{
	char c[512], filename[7],newfile[7], buffer[15512];
	interrupt(0x21,0,"SHELL> ",0,0);
	interrupt(0x21,1,c,0,0);
	

	//Execute when type command is entered in command prompt
	if(c[0]==0x74 && c[1]==0x79 && c[2]==0x70 && c[3]==0x65 && c[4]==0x20)
	{
		fileNaming(filename,c,5);
		interrupt(0x21,3,filename,buffer,0);
		interrupt(0x21,0,buffer,0,0);	
	}
	//Execute when execute command is entered in command prompt
	else if(c[0]==0x65 && c[1]==0x78 && c[2]==0x65 && c[3]==0x63 && c[4]==0x75 && c[5]==0x74 && c[6]==0x65 && c[7]==0x20)
	{
		fileNaming(filename,c,8);
		interrupt(0x21,4,filename,0x2000,0);	
		
	}
	//Execute for delete command
	else if(c[0]==0x64 && c[1]==0x65 && c[2]==0x6C && c[3]==0x65 && c[4]==0x74 && c[5]==0x65 && c[6]==0x20)
	{
		//interrupt(0x21,0,c,0,0);
		fileNaming(filename,c,7);
		interrupt(0x21,7,filename,0,0);	
	}

	//Execute when copy command is entered in shell
	else if(c[0]==0x63 && c[1]==0x6F && c[2]==0x70 && c[3]==0x79 && c[4]==0x20)
	{
		int i=12,j=0;
		fileNaming(filename,c,5);
		interrupt(0x21,3,filename,buffer,0);
		//interrupt(0x21,0,buffer,0,0);	

		while(c[i]!='\0')
		{
			newfile[j]=c[i];
			i++;
			j++;
		}
		newfile[j]='\0';
		interrupt(0x21,8,newfile,buffer,26);
		//interrupt(0x21,3,newfile,buffer,0);
		//interrupt(0x21,0,buffer,0,0);
		
	}
	
	//Execute for dir command
	else if(c[0]==0x64 && c[1]==0x69 && c[2]==0x72)
	{
		int i=0, j=0, sec=0;
		char dir[512],fileName[7];
		interrupt(0x21,2,dir,2,0);
		

		while(i<512)
		{
			if(dir[i]!=0x00)
			{
					fileName[0]=dir[i];
					fileName[1]=dir[i+1];
					fileName[2]=dir[i+2];
					fileName[3]=dir[i+3];
					fileName[4]=dir[i+4];
					fileName[5]=dir[i+5];
					fileName[6]='\0';
					interrupt(0x21,0,fileName,0,0);
					interrupt(0x21,0,"\t",0,0);
			}
			i+=32;		
		}
	}
	
	//Execute for create filename command
	else if(c[0]==0x63 && c[1]==0x72 && c[2]==0x65 && c[3]==0x61 && c[4]==0x74 && c[5]==0x65 && c[6]==0x20)
	{
		interrupt(0x21,0,">",0,0);
		interrupt(0x21,1,buffer,0,0);
		//interrupt(0x21,0,buffer,0,0);
		fileNaming(filename,c,7);
		interrupt(0x21,8,filename,buffer,26);
	}

	//Execute for Kill command
	else if(c[0]==0x6B && c[1]==0x69 && c[2]==0x6C && c[3]==0x6C && c[4]==0x20)
	{
		interrupt(0x21,9,c[5]-0x30,0,0);
	}
	
	else
	{
		interrupt(0x21,0,"Bad Command\n",0,0);
	}
}
}

char* fileNaming(char* filename,char* c,int j)
{
	int i=0;
	while(i<6)
	{
		filename[i]=c[j];
		i++;
		j++;
	}
	filename[i]='\0';
	return filename;
}	
