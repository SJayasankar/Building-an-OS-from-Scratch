//kernel.c
//Sangeetha Jayasankar, Fall 2019

int processActive[8], processStackPointer[8];
int currentProcess;
int dataseg=0;

void printString(char*);

void printChar(char c);

char* readString(char*);

char* readSector(char*,int);

int divCalc(int,int);

int modCalc(int,int);

char* handleInterrupt21(int, int, int, int);

char* readFile(char*, char*);

void executeProgram(char*, int);

void terminate();

void writeSector(char*,int);
	
void deleteFile(char*);

void writeFile(char*,char*,int);

void handleTimerInterrupt(int, int);

void killProcess(int);


void main()
{
	int i;
	
	char* char_array="Hello World\0";
	char buffer[13312];
	
	//char line[80], buffer[512];
	
	/*printString("Enter a line:\0");
	
	readString(line);
	printString(line);
	
	readSector(buffer,30);
	printString(buffer);*/

	for(i=0;i<8;i++)
	{
		processActive[i]=0;
		processStackPointer[i]=0xff00;
		
	}
	currentProcess=-1;	
	
	makeInterrupt21();	

	interrupt(0x21,4,"shell\0",0x2000,0);
	//interrupt(0x21,5,0,0,0);
	
	makeTimerInterrupt();

	/*interrupt(0x21,0,"Enter a line:",0,0);
	interrupt(0x21,1,line,0,0);
	interrupt(0x21,0,line,0,0);
	interrupt(0x21,2,buffer,30,0);
	interrupt(0x21,0,buffer,0,0);

	interrupt(0x21,3,"messag\0",buffer,0);
	interrupt(0x21,0,buffer,0,0);
	interrupt(0x21,4,"tstprg\0",0x2000,0);
	interrupt(0x21,4,"tstpr2\0",0x2000,0);
	interrupt(0x21,5,0,0,0);*/

	while(1);			
}

void printString(char* array)
{
	int i;
	for(i=0;array[i]!=0x0;i++)
	{
		interrupt(0x10, 0xe*256+array[i], 0, 0, 0);
		//printChar(array[i]);
	}
		
}

void printChar(char c)
{
	interrupt(0x10, 0xe*256+c, 0, 0, 0);	
}

char* readString(char* c)
{
	int i=0;
	char keypressed;

	while(keypressed!=0xd)
	{
		keypressed = interrupt(0x16, 0, 0, 0, 0);
		
		if(keypressed==0x8)
		{
			if(i!=0)
			{
				printChar(keypressed);
				i--;	
				printChar(0x20);
				printChar(keypressed);
			}
		}
		else
		{
			printChar(keypressed);
			c[i]=keypressed;
			i++;
		}
	}
	c[i]= 0xa;
	printChar(c[i]);
	i++;
	c[i]= 0x0;
	printChar(c[i]);

	return c;
}

char* readSector(char* buffer, int sector)
{
	int track, relSector, head;
	track = divCalc(sector,36);
	relSector = modCalc(sector,18) + 1;
	head = modCalc(divCalc(sector, 18), 2);
	interrupt(0x13,2*256+1,	buffer, track*256+relSector, head*256+0);
}

int divCalc(int a, int b)
{
	int q=0;
	while((q*b)<=a)
	{
		q = q+1;
	}
	return q-1;
}

int modCalc(int a,int b)
{
	while(a>=b)
	{
		a =a-b;
	}
	return a;
}

char* readFile(char* c, char* b)
{
	int i = 0;
	char arr[512];
	readSector(arr,2);
	while(arr[i]!=0 || i<512)
	{
		if(arr[i]==c[0] && arr[i+1]==c[1] && arr[i+2]==c[2] && arr[i+3]==c[3] && arr[i+4]==c[4] && arr[i+5]==c[5])
		{
			int j;
			j=i+32;
			
			for(i=i+6;i<j;i++)
			{
				if(arr[i]!=0x00)
				{
					readSector(b,arr[i]);
					b+=512;
				}
			}
		}
		i+=32;
	}
	return b;	
}

void executeProgram(char* name, int segment)
{
	char buffer[15512];
	int pos=0x0000, i=0, j=0;	
	readFile(name, buffer);
	dataseg=setKernelDataSegment();
	for(j=0;j<8;j++)
	{
		
		if(processActive[j]==0)
		{
			segment = (j+2)*0x1000;
			break;
		}
		
	}
	restoreDataSegment(dataseg);
	//putInMemory(0x20000,pos,buffer);
	while(i<15512)
	{
		putInMemory(segment,pos,buffer[i]);
		i+=1;
		pos+=1;
	}
	//launchProgram(segment);
	initializeProgram(segment);
	dataseg=setKernelDataSegment();
	processActive[j]=1;
	processStackPointer[j]=0xff00;
	restoreDataSegment(dataseg);
}

void terminate()
{
	/*char shell[6];
	shell[0]='s';
	shell[1]='h';
	shell[2]='e';
	shell[3]='l';
	shell[4]='l';
	shell[5]='\0';
	interrupt(0x21,4,shell,0x2000,0);*/

	dataseg=setKernelDataSegment();
	processActive[currentProcess]=0;
	restoreDataSegment(dataseg);
	while(1);
}

void writeSector(char* buffer, int sector)
{
	int track, relSector, head;
	track = divCalc(sector,36);
	relSector = modCalc(sector,18) + 1;
	head = modCalc(divCalc(sector, 18), 2);
	interrupt(0x13,3*256+1,	buffer, track*256+relSector, head*256+0);
}

void deleteFile(char* name)
{
	int i=0;
	char dir[512],map[512];
	readSector(dir,2);
	readSector(map,1);
	
	while(dir[i]!=0x00 || i<512)
	{
		if(dir[i+0]==name[0] && dir[i+1]==name[1] && dir[i+2]==name[2] && dir[i+3]==name[3] && dir[i+4]==name[4] && dir[i+5]==name[5])
		{
			int k;
			//interrupt(0x21,0,dir[i],0,0);
			dir[i]=0x00;
			/*map[0x0E]=0x00;
			dir[i+6]=0x00;*/
			for(k=i+6;k<i+32;k++)
			{
				//interrupt(0x21,0,dir[k],0,0);
				if(dir[k]!=0x00)
				{
					map[dir[k]]=0x00;
				}
			}
		}
		i+=32;
	}
	interrupt(0x21,6,dir,2,0);
	interrupt(0x21,6,map,1,0);
}

void writeFile(char* name, char* buffer, int numberOfSectors)
{
	int i;
	char dir[512],map[512];
	readSector(dir,2);
	readSector(map,1);
	
	for(i=0;i<512;i=i+32)
	{
		if(dir[i]==0x00)
		{
			int j,k;
			for(j=0;name[j]!=0 && j<6;j++,i++)
			{
				dir[i]=name[j];				
			}
			while(modCalc(i,5)!=0)
			{
				dir[i]=0x00;
				i++;
			}
			for(k=5;map[k]!=0xFF && numberOfSectors!=0;k++)
			{
				map[k]=0xFF;
				dir[i]=k;
				interrupt(0x21,6,buffer,dir[i],0);
				buffer+=512;
				numberOfSectors--;
				i++;
			}
			while(modCalc(i,31)!=0)
			{
				dir[i]=0x00;
				i++;
			}
			
		}
		else return;
			//interrupt(0x21,5,0,0,0);
		

	}
	interrupt(0x21,6,dir,2,0);
	interrupt(0x21,6,map,1,0);
}

void handleTimerInterrupt(int segment, int sp)
{
	/*printChar('t');
	printChar('i');
	printChar('c');*/
	int i;
	dataseg=setKernelDataSegment();
	if(currentProcess!=-1)
	{
		processStackPointer[currentProcess] = sp;
	}
	do
	{	
		if(currentProcess==8)
		{
			currentProcess=0;	
		}
		else
		{			
			currentProcess+=1;
		}
	}while(processActive[currentProcess]==0);
	segment = (currentProcess+2)*0x1000;
	sp = processStackPointer[currentProcess];
	restoreDataSegment(dataseg);
	returnFromTimer(segment,sp);
}

void killProcess(int processID)
{
	dataseg=setKernelDataSegment();
	processActive[processID]=0;
	restoreDataSegment(dataseg);
}

char* handleInterrupt21(int ax, int bx, int cx, int dx)
{
	switch(ax)
	{
		case 0: printString(bx);
			break;
		case 1: return readString(bx);	
		case 2: return readSector(bx,cx);
		case 3: return readFile(bx,cx);
		case 4: executeProgram(bx,cx);
			break;
		case 5: terminate();
			break;
		case 6: writeSector(bx,cx);
			break;
		case 7: deleteFile(bx);
			break;
		case 8: writeFile(bx,cx,dx);
			break;
		case 9: killProcess(bx);
			break;
		
		default: printString("Invalid Interrupt Call\0");
			break;

	}
	return 0;
	while(1);
}

