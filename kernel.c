void printString(char*);
void readString(char*);
int MOD(int,int);
int DIV(int,int);
void readSector(char*, int);
void handleInterrupt21(int,int,int,int);
int readFile(char*,char*);
void executeProgram(char*, int);
void terminate();
void writeSector(char*, int);
void deleteFile(char*);
void writeFile(char*, char*, int);
void handleTimerInterrupt(int,int);
void killProcess(int);

int activeProcesses[8];
int processesStackPointer[8];
int currentProcess;
int timerInterruptCounter;

int main(){

	//Test for Task 1,2,3 Milestone 4
	/*int i ;
	char shell[6];
	shell[0]='s';
	shell[1]='h';
	shell[2]='e';
	shell[3]='l';
	shell[4]='l';
	shell[5]= 0x0;

	for(i = 0 ; i < 8 ; i++)
	{
		activeProcesses[i]=0;
		processesStackPointer[i] = 0xFF00;
	}
	currentProcess = 0;
	timerInterruptCounter = 0;

	makeInterrupt21();
	makeTimerInterrupt();
	interrupt(0x21, 4, shell, 0x2000, 0);
	while(1);*/

    //Test for Task 4 Milestone 4
	/*int i ;

	for(i = 0 ; i < 8 ; i++)
	{
		activeProcesses[i]=0;
		processesStackPointer[i] = 0xFF00;
	}
	currentProcess = 0;
	timerInterruptCounter = 0;

	makeInterrupt21();
	makeTimerInterrupt();
	interrupt(0x21, 4, "hello1\0", 0, 0);
	interrupt(0x21, 4, "hello2\0", 0, 0);
	while(1);*/


	char shell[6];
	int i ;
	
	shell[0]='s';
	shell[1]='h';
	shell[2]='e';
	shell[3]='l';
	shell[4]='l';
	shell[5]= 0x0;

	for(i = 0 ; i < 8 ; i++)
	{
		activeProcesses[i]=0;
		processesStackPointer[i] = 0xFF00;
	}
	currentProcess = 0;
	timerInterruptCounter = 0;

	makeInterrupt21();
	makeTimerInterrupt();
	interrupt(0x21, 4, shell, 0, 0);
	while(1);
}

void handleInterrupt21(int ax, int bx, int cx, int dx){
	//Task 4
	/*char* x = "Task 4 Test\0";
	printString(x);
	while(1){}*/

	//Task 5
	if(ax==0){
		printString(bx);
	}
	if(ax==1){
		readString(bx);
	}
	if(ax==2){
		readSector(bx, cx);
	}
	if(ax==3){
		readFile(bx, cx);
	}
	if(ax==4){
		executeProgram(bx);
	}
	if(ax==5){
		terminate();
	}
	if(ax==6){
		writeSector(bx,cx);
	}
	if(ax==7){
		//interrupt(0x21, 0, "command Found\0", 0, 0);
		deleteFile(bx);
	}
	if(ax==8){
		writeFile(bx,cx,dx);
		}
	if(ax==9){
		killProcess(bx);
	}	
	if(ax>9){
		printString("Error!, ax allowed values are: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}\0");
	}
}


void handleTimerInterrupt(int segment, int sp)
{
	//Task1 Milestone 4
	//printString("Tic\0");

	//Task4, Milestone4
	int nextProcess;
	int nextSP;
	int nextSegment;
	int i;

	timerInterruptCounter++;

	if(timerInterruptCounter==100){

		setKernelDataSegment();
		if(segment != 0x1000){
			processesStackPointer[currentProcess] = sp;
		}
		restoreDataSegment();

		//circular loop
		setKernelDataSegment();
		i=MOD(currentProcess+1,8);
		while(i!=currentProcess){
			if(activeProcesses[i]==1){
				nextProcess = i;
				break;
			}
			i=MOD(i+1,8);
		}
		if(i!=currentProcess){
			currentProcess = nextProcess;
		}
		restoreDataSegment();

		setKernelDataSegment();
		nextSP = processesStackPointer[currentProcess];
		nextSegment = (currentProcess+2) * (0x1000);
		timerInterruptCounter = 0;
		restoreDataSegment();

		returnFromTimer(nextSegment,nextSP);
	}
	else{
		returnFromTimer(segment,sp);
	}
}

void killProcess(int process){

	char notFound[12];

	if(process>7){

		notFound[0] = 'N';
		notFound[1] = 'o';
		notFound[2] = 't';
		notFound[3] = ' ';
		notFound[4] = 'F';
		notFound[5] = 'o';
		notFound[6] = 'u';
		notFound[7] = 'n';
		notFound[8] = 'd';
		notFound[9] = '\n';
		notFound[10] = '\r';
		notFound[11] = '\0';
		interrupt(0x21, 0, notFound, 0, 0);


	}else{

		if(process!=0){

			setKernelDataSegment();
			activeProcesses[process] = 0;
			processesStackPointer[process] = 0xFF00;
			restoreDataSegment();
			
		}

	}

}

int readFile(char* fileName,char* buffer){
	//Task 1
	char sector[512];
	char name[6];
	char fileSector[26];
	char notFound[12];
	int i;
	int j;
	int k;
	int flag;
	char erorrHandler[3];
	erorrHandler[0] ='N';
	erorrHandler[1]='O';
	erorrHandler[2]=0x0;
	readSector(sector,2);
	for(i = 0; i<16; i++)
	{
		flag = 1;
		j = i*32;
		for (k = 0; k < 6; ++k)
		{
			name[k] = sector[j+k];
		}
		for(k = 0; k < 6 ; k++)
		{
			if(name[k] != *(fileName+k))
			{
				flag =0;
				break;
			}
		}
		if(flag == 1)
		{
			break;
		}
	}
	if(i == 16)
	{
		*(buffer) = 'N';
		*(buffer+1) = 'o';
		*(buffer+2) = 't';
		*(buffer+3) = ' ';
		*(buffer+4) = 'F';
		*(buffer+5) = 'o';
		*(buffer+6) = 'u';
		*(buffer+7) = 'n';
		*(buffer+8) = 'd';
		*(buffer+9) = '\n';
		*(buffer+10) = '\r';
		*(buffer+11) = '\0';
		return 0;
	}
	j= 0;
	k=6;
	while(j<26)
	{
		fileSector[j]=sector[i*32+k];
		k = k + 1;
		j = j + 1 ;
	}
	for (j= 0; j<26; j++)
	{
		if(fileSector[j] != 0 )
		{
			readSector(buffer,fileSector[j]);
			buffer = buffer +512;
		}else{
			break;
		}
		
	}
	return 1;

}

void deleteFile(char* name)
{

	char map[512];
	char directory[512];
	char fileSector[26];
	char filename[6];
	char notFound[12];
	int i;
	int j;
	int k;
	int found;
	readSector(map,1);
	readSector(directory,2);
	for(i=0; i < 16 ; i++)
	{
		j= i*32;
		for(k = 0 ; k < 6 ; k++)
		{
			filename[k] = directory[j+k];
		}
		j = 0;
		found = 1;
		while(j < 6)
		{
			if(filename[j] != *(name+j))
			{
				found = 0;
				break;
			}
			j++;
		}
		if(found == 1)
		{
			break;
		}
	}
	if( i == 16 )
	{
		notFound[0] = 'N';
		notFound[1] = 'o';
		notFound[2] = 't';
		notFound[3] = ' ';
		notFound[4] = 'F';
		notFound[5] = 'o';
		notFound[6] = 'u';
		notFound[7] = 'n';
		notFound[8] = 'd';
		notFound[9] = '\n';
		notFound[10] = '\r';
		notFound[11] = '\0';
		interrupt(0x21, 0, notFound, 0, 0);
		return;
	}
	directory[i*32] = 0x00;
	j= 0;
	k=6;
	while(j<26)
	{
		fileSector[j]=directory[i*32+k];
		k = k + 1;
		j = j + 1 ;
	}
	for (j= 0; j<26; j++)
	{
		if(fileSector[j] != 0 )
		{
			map[fileSector[j]] = 0x00;
			
		}else{
			break;
		}
	}
	writeSector(map,1);
	writeSector(directory,2);

}

//issue
/*
	ensure no duplicate of names
*/
void writeFile(char* name, char* buffer, int secNum)
{
	char map[512];
	char directory[512];
	char contents[512];
	char availableSector[26];
	char filename[6];
	char notFound[11];
	int s;
	int i;
	int k;
	int j;
	int m;
	int emptyPlace;
	notFound[0] = 'N';
	notFound[1] = 'o';
	notFound[2] = ' ';
	notFound[3] = 's';
	notFound[4] = 'p';
	notFound[5] = 'a';
	notFound[6] = 'c';
	notFound[7] = 'e';
	notFound[8] = '\n';
	notFound[9] = '\r';
	notFound[10] = '\0';
	readSector(map,1);
	readSector(directory,2);
	s = secNum;
	emptyPlace = 0;
	// Check first if there is enough sectors
	for(i=0; i < 512 ; i++)
	{
		if(s == 0)
		{
			break;
		}
		if(map[i] == 0x00)
		{
			s--;
		}
	}
	if(s>0)
	{
		interrupt(0x21, 0, notFound, 0, 0);
		return;
	}
	// The check for sectors is ended
	for(i = 0; i< 6; i++)
	{
		if(*(name+i) != 0x0)
		{
			filename[i] = *(name+i);
		}else{
			filename[i] = 0x00;
		}
	}
	//Now I load the file
	m = 0;
	for(i = 0 ; i< 16 ; i++)
	{
		if(directory[i*32]==0x00)
		{
			for(s=0;s<6;s++)
			{
				directory[i*32+s]=filename[s];
			}
			for(k=4;k<512 && secNum > 0;k++)
			{
				if(map[k] == 0x00){
					secNum--;
					directory[i*32+s]=k;
					map[k] = 0xFF;
					s++;
					for(j = 0; j < 512 ; j++)
					{
						contents[j] = *(buffer+m);
						m++;
					}
					writeSector(contents,k);
				}
			}
			while(s<32)
			{
				directory[i*32+s]=0x00;
				s++;
			}
			break;
		}
	}
	if(i == 16)
	{
		notFound[0] = 'N';
		notFound[1] = 'o';
		notFound[2] = ' ';
		notFound[3] = 's';
		notFound[4] = 'p';
		notFound[5] = 'a';
		notFound[6] = 'c';
		notFound[7] = 'e';
		notFound[8] = '\n';
		notFound[9] = '\r';
		notFound[10] = '\0';
		interrupt(0x21, 0, notFound, 0, 0);
	}
	else{
		writeSector(map,1);
		writeSector(directory,2);
	}

}

void executeProgram(char* name){

	//Task 2 Milestone 2
	char buffer[13312];
	int i;
	int j;
	int segment;
	int fileExists;
	char noSpace[10];

	setKernelDataSegment();
	for(i = 0 ; i < 8 ; i++)
	{
		if(activeProcesses[i] == 0)
		{
			processesStackPointer[i] = 0xff00;
			break;
		}
	}
	restoreDataSegment();

	if(i == 8)
	{
		noSpace[0] = 'N';
		noSpace[1] = 'o';
		noSpace[2] = ' ';
		noSpace[3] = 's';
		noSpace[4] = 'p';
		noSpace[5] = 'a';
		noSpace[6] = 'c';
		noSpace[7] = 'e';
		noSpace[8] = '\n';
		noSpace[9] = '\r';
		noSpace[10] = '\0';
		interrupt(0x21, 0, noSpace, 0, 0);
		return;
	}

	fileExists = readFile(name,buffer);
	if(fileExists == 0)
	{
		interrupt(0x21, 0, buffer, 0, 0);
		return;
	}

	segment = (i+2) * (0x1000);

	j = 0;
	if(buffer[j] != 0x0){
		while(j < 13312)
		{
			putInMemory(segment,j,buffer[j]);
			buffer[j] = 0x0;
			j= j+1;
		}

		if(i!=8){
			initializeProgram(segment);	
		}

		setKernelDataSegment();
		activeProcesses[i] = 1;
		restoreDataSegment();
	}
	else{
		noSpace[0] = 'N';
		noSpace[1] = 'o';
		noSpace[2] = 't';
		noSpace[3] = 'F';
		noSpace[4] = 'o';
		noSpace[5] = 'u';
		noSpace[6] = 'n';
		noSpace[7] = 'd';
		noSpace[8] = '\n';
		noSpace[9] = '\r';
		noSpace[10] = '\0';
		interrupt(0x21, 0, noSpace, 0, 0);
	}
}

void terminate(){
	int i;
	
	setKernelDataSegment();
	i = currentProcess;
	restoreDataSegment();
	
	if(i!= 0){
		
		setKernelDataSegment();
		activeProcesses[currentProcess] = 0;
		processesStackPointer[currentProcess] = 0xFF00;
		restoreDataSegment();
		while(1);
	
	}

}

void readSector(char* buffer, int sector){
	
	int AH, AL, BX, CH,CL,DH,DL;
	int AX,CX,DX;
	AH =0x2;
	AL=0x1;
	BX = buffer;
	CH=DIV(sector,36);
	CL=MOD(sector, 18)+1;
	DH=MOD(DIV(sector,18),2);
	DL=0x0;
	
	AX = AH*256+AL;
	CX = CH*256+CL;
	DX = DH*256+DL;
	interrupt(0x13, AX, BX, CX, DX);
	
}
void writeSector(char* buffer, int sector){
	
	int AH, AL, BX, CH,CL,DH,DL;
	int AX,CX,DX;
	AH =0x3;
	AL=0x1;
	BX = buffer;
	CH=DIV(sector,36);
	CL=MOD(sector, 18)+1;
	DH=MOD(DIV(sector,18),2);
	DL=0x0;
	
	AX = AH*256+AL;
	CX = CH*256+CL;
	DX = DH*256+DL;
	interrupt(0x13, AX, BX, CX, DX);
	
}

int MOD(int x, int y){
	while(x>=y)
	{
		x = x-y;
	}
	return x;

}

int DIV(int x, int y)
{
	int c;
	c=0;
	while(x>=y)
	{
		c=c+1;
		x=x-y;
	}
	return c;
}

int ceil(int x, int y)
{
	int c;
	c=DIV(x, y);

	if(MOD(x, y))
		c++;
	return c;
}

void readString(char* x)
{
	//Task 2
	char* y;
	char temp;
	int size;
	int writtenSoFar;
	writtenSoFar=0;
	y= x;
	size=90;
	while(temp != 0xD &&  size > 2)
	{
		temp = interrupt(0x16,0x0,0,0,0);
		if(temp == 0x8 && writtenSoFar > 0)
		{
			interrupt(0x10, 0xE*256+0x8, 0, 0, 0);
			interrupt(0x10, 0xE*256+0x20, 0, 0, 0);
			interrupt(0x10, 0xE*256+0x8, 0, 0, 0);
			y=y-1;
			size=size+1;
			writtenSoFar = writtenSoFar - 1;
		}
		else if(temp != 0x8){
			interrupt(0x10, 0xE*256+temp, 0, 0, 0);
			*y = temp;
			y=y+1;
			size = size-1;
			writtenSoFar = writtenSoFar +1;
		}
	}
	if(temp==0xD)
	{
		*y=0xA;
		interrupt(0x10, 0xE*256+0xA, 0, 0, 0);//This line prints the enter in the screen to prevent overwriting the (Enter line) message 
		y=y+1;
		*y=0x0;

	}else{
		printString("You exceeded the full capacity of the command \0");
	}	


}

void printString(char* x)
{
	// Task 1
	int i;
	for(i=0; *(x+i) != '\0'; i=i+1)
	{
		interrupt(0x10, 0xE*256+(*(x+i)), 0, 0, 0);
	}

}
