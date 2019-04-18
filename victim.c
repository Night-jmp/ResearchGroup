/*
				Victim version 1.0

		Simple program to read in NMEA formatted GPS data from 
		a file and print the fields into a human readable format.	
			
		To do: need to implement a state machine to continuously
		monitor tty for updates every 1 second and if retrieve a 
		new sentence display it to the console. 


*/

#include <stdio.h>
#include <unistd.h>
#include <nmea.h>
#include <nmea/gpgll.h>

//Function prototypes
nmea_s* getSentence(FILE*);
void printData(nmea_s*);

typedef enum {READ_SENTENCE, PRINT} states;

int main()
{
	FILE* fptr;				
	fptr = fopen("tty", "r");		//Open tty (just a text file for now)
	nmea_s* nmeaSentence;			//Create nmea struct object
	states state = READ_SENTENCE;
	while(1)
	{
		switch(state)
		{
			case READ_SENTENCE:
			nmeaSentence = getSentence(fptr);	//Get the nmea sentence from the file
			if(nmeaSentence != NULL)
				state = PRINT;
			break;
			case PRINT:
			printData(nmeaSentence);		//Print the data from the sentence
			state = READ_SENTENCE;
			sleep(1);
			break;
		}
	}

	return 0;
}

//This function retrieves a line from the file and parses it to an nmea structure object
nmea_s* getSentence(FILE* file)
{
	char* sentence;
	//allocate space on the heap for the sentence (getline requires this)
	sentence = (char *)malloc(80);
	size_t len;
	nmea_s* nmeaSentence;

	getline(&sentence, &len, file); //gets the line from the file
	
	sentence = strcat(sentence, "\r\n");	//nmea struct requires return and newline characters
	nmeaSentence = nmea_parse(sentence, strlen(sentence), 0);	//Parse it!
	
	free(sentence);			//deallocate sentence
	
	//This is a check to make sure the sentence was parsed correctly
	if(nmeaSentence == NULL)
	{
		return NULL;
	}
	return nmeaSentence;		//return the nmea struct
}

//Prints certain fields of the sentence
void printData(nmea_s* data)
{
	nmea_gpgll_s* GLL = (nmea_gpgll_s*)data;	//Cast the struct as a GLL type
							//We can assume this since we arent
							//going to use any other sentence type

	//Print the minutes and degrees of the latitude and longitude
	//This could be nicer looking
	printf("%d degrees ", GLL->longitude.degrees);
	printf("%f minutes ", GLL->longitude.minutes);
	printf("Longitude\n");
	printf("%d degrees ", GLL->latitude.degrees);
	printf("%f minutes ", GLL->latitude.minutes);
	printf("Latitude\n");
}
