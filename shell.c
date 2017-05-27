void commandRecognition(char*);

int main(){
	char command[80];
	while(1){
		interrupt(0x21, 0, "SHELL>: \0" , 0, 0);
		interrupt(0x21, 1, command , 0, 0);
		//interrupt(0x21, 0, "Bad command !\n\r\0" , 0, 0);
		commandRecognition(command);
		interrupt(0x21, 5, 0 , 0, 0);
	}	

}

void commandRecognition(char* command)
{
	char* ptr;
	char file[7];
	char file2 [7];
	char buffer [13312];
	int i;
	int counter;

	//for dir
	char map[512];
	char directory[512];

	char* line;
	char message[13];

	int j;
	int k;
	int NumOfSec;
	int bufferPointer;

	int processToKill;

	bufferPointer = 0;
	NumOfSec = 0;
	
	ptr = command;
	interrupt(0x21, 2, map, 1, 0);
	interrupt(0x21, 2, directory, 2, 0);
	i = 0 ;
	while(i<6){
			file[i]=0x0;
			i++;
		}
		i=0;

	if(*(ptr) == 'v' && *(ptr+1)=='i' && *(ptr+2) == 'e' && *(ptr+3) == 'w'
		&& *(ptr+4)==' ')
	{
		ptr = ptr+5;
		counter = 0;
		while(*(ptr)!=0xd && counter < 6)
		{
			file[i] = *(ptr);
			ptr++;
			i++;
			counter++;
		}
		file[6] = 0x0;
		interrupt(0x21, 3, file , buffer, 0);
		interrupt(0x21, 0, buffer, 0, 0);
	}
	else if (*(ptr) == 'e' && *(ptr+1)=='x' && *(ptr+2) == 'e' && *(ptr+3) == 'c' && *(ptr+4)=='u'
		&& *(ptr+5) == 't' && *(ptr+6)=='e' && *(ptr+7)==' ')
	{
		ptr = ptr+8;
		while(i < 6 && *(ptr)!=0xd)
		{
			file[i] = *(ptr);
			ptr++;
			i++;
		}
		interrupt(0x21, 4, file , 0x2000, 0);

	}
	else if (*(ptr) == 'd' && *(ptr+1)=='e' && *(ptr+2) == 'l' && *(ptr+3) == 'e' && *(ptr+4)=='t'
		&& *(ptr+5) == 'e' && *(ptr+6)==' ')
	{
		ptr = ptr+7;
		while(*(ptr)!=0xd)
		{
			file[i] = *(ptr);
			ptr++;
			i++;
		}
		interrupt(0x21, 7, file, 0, 0);

	}
	else if(*(ptr) == 'c' && *(ptr+1)=='o' && *(ptr+2) == 'p' && *(ptr+3) == 'y'
		&& *(ptr+4)==' ')
	{
		ptr = ptr+5;
		while(i < 6 && *(ptr)!=' ')
		{
			file[i] = *(ptr);
			ptr++;
			i++;
		}


		interrupt(0x21, 3, file , buffer, 0);

		i=1;
		while(buffer[i] != 0)
		{
			i++;
		}

		NumOfSec = ceil(i, 512);

		i=0;
		while(i<6)
		{
			file2[i]=0x0;
			i++	;
		}

		ptr++;
	    
		i = 0;
		while(*(ptr)!=0xd)
		{
			file2[i] = *(ptr);
			ptr++;
			i++;
		}

		interrupt(0x21, 8, file2, buffer, NumOfSec);
	}
	else if(*(ptr) == 'd' && *(ptr+1)=='i' && *(ptr+2) == 'r')
	{
		for(i = 0; i<16; i++)
		{
			j = i*32;
			if(directory[j]!=0x0)
			{
				for (k = 0; k < 6; ++k)
				{
					if(directory[j+k] == 0x0)
						break;
					buffer[bufferPointer++] = directory[j+k]; 
				}
				for (k = 6; k < 32; ++k)
				{
					if(directory[j+k] == 0x0)
						break;
					NumOfSec++; 
				}
				buffer[bufferPointer++] = ' ';
				buffer[bufferPointer++] = DIV(NumOfSec, 10)+0x30;
				buffer[bufferPointer++] = MOD(NumOfSec, 10)+0x30;//manipulate number into string of characters in case of more than 9 sectors
				buffer[bufferPointer++] = '\n';
				buffer[bufferPointer++] = '\r';
				NumOfSec = 0;

			}
		}
		buffer[bufferPointer] = '\0';
		interrupt(0x21, 0, buffer, 0, 0);
	}
	else if(*(ptr) == 'c' && *(ptr+1)=='r' && *(ptr+2) == 'e' && *(ptr+3) == 'a' && *(ptr+4)=='t'
		&& *(ptr+5) == 'e' && *(ptr+6)==' ')
	{
		ptr+=7;

		while(i < 6 && *(ptr)!=0xd)
		{
			file[i] = *(ptr);
			ptr++;
			i++;
		}	

		message[0] = 'E';
		message[1] = 'n';
		message[2] = 't';
		message[3] = 'e';
		message[4] = 'r';
		message[5] = ' ';
		message[6] = 'l';
		message[7] = 'i';
		message[8] = 'n';
		message[9] = 'e';
		message[10] = '\n';
		message[11] = '\r';
		message[12] = '\0';
		interrupt(0x21, 0, message, 0, 0);
		interrupt(0x21, 1, line, 0, 0);

		i = 0;
		j = 0;
		
		while(*(line) != 0xd)
		{
			while(*(line) != 0xd)
			{
				buffer[i++] = *(line++);
				j++;
			}
			buffer[i++] = '\n';
			buffer[i++] = '\r';
			line-=j;
			j=0;
			interrupt(0x21, 0, message, 0, 0);
			interrupt(0x21, 1, line, 0, 0);
		}

		buffer[i++] = '\0';

		NumOfSec = ceil(i, 512);
		
		interrupt(0x21, 8, file, buffer, NumOfSec);

	}
	else if(*(ptr) == 'k' && *(ptr+1)=='i' && *(ptr+2) == 'l' && *(ptr+3) == 'l'
		&& *(ptr+4)==' ')
	{
		ptr = ptr+5;

		processToKill = *(ptr)-0x30;

		interrupt(0x21, 9, processToKill , 0, 0);
	}
	else{
		interrupt(0x21, 0, "Bad command !\n\r\0" , 0, 0);
	}
	
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
	int c=0;
	c=DIV(x, y);

	if(MOD(x, y))
		c++;
	return c;
}