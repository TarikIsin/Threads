#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
 

pthread_mutex_t mutex; #create mutex
int numberOfThread[4]; #create array for how many thread will be

void* readd(void* arg) {
	int number = *(int*)arg; #give threads number to number 
	pthread_mutex_lock(&mutex); # Start the mutex
	FILE* file;
	
	int lineLength = 255;
	char line[lineLength];
	
	int t;
	char* satir = malloc(25);
	
	file = fopen("Input.txt", "r");
	
	#take lines from file
	for(t = 0; t < number; t++){			
		fgets(line, lineLength, file);
	}
	printf("Thread %d read the : %s", number, line);
	
	//strcpy(satir,line);
	pthread_mutex_unlock(&mutex); # Close the mutex
	fclose(file);	
}

void* upper(void* arg) {
	
	char* str = (char*)arg; #give str that come with thread to str 
	
	char s[50];
	strcpy(s, str);
	int i = 0;
	pthread_mutex_lock(&mutex); # Start the mutex
	#Convert to uppercase
	while (s[i]) { 
        	s[i] = s[i]; 
        	printf("%c", toupper(s[i])); 
        	i++; 
    	}
    	printf("\n");
	pthread_mutex_unlock(&mutex); # Close the mutex
}

void* replace(void* arg) {
	
	char* str = arg;  #give str that come with thread to str 
	char s[50];
	strcpy(s, str);
	int i = 0;
	pthread_mutex_lock(&mutex); # Start the mutex
	#Conver spaces to underscore
	while (s[i]) {
		if(s[i] == ' ') 
        	s[i] = '_'; 
        	printf("%c", s[i]); 
        	i++; 
    	} 
    	printf("\n");
    	pthread_mutex_unlock(&mutex); # Close the mutex
}

void* write(void* arg) {
	pthread_mutex_lock(&mutex); # Start the mutex
	printf("File updated..\n");
	
	char* str = arg;  #give str that come with thread to str 
	
	char s[250];
	#Copy str to s
	strcpy(s, str);
	
	FILE* file = fopen("Output.txt", "w");;
	#Print the s
	fprintf(file, "%s", s);
	
	fclose(file);
	pthread_mutex_unlock(&mutex); # Close the mutex
}

int main(void) {
	
	# Take threads number from user
	scanf("%d %d %d %d", &numberOfThread[0], &numberOfThread[1], &numberOfThread[2], &numberOfThread[3]);
	
	#Create thread by values given
	pthread_t readth[numberOfThread[0]];
	pthread_t upperth[numberOfThread[1]];
	pthread_t replaceth[numberOfThread[2]];
	pthread_t writeth[numberOfThread[3]];
	
	pthread_mutex_init(&mutex, NULL);
	int i = 1;
	#Send read threads
	while(i - 1 < numberOfThread[0]) {		
		pthread_create(&readth[i], NULL, &readd, &i);	
		i = i + 1;
	}
	
	for (i = 0; i < numberOfThread[0]; i++) {
		pthread_join(readth[i], NULL);	
	}
	#Send upper threads
	for (i = 0; i < numberOfThread[1]; i++){
		char* str = "This is an example line.";
		if(pthread_create(&upperth[i], NULL, &upper, str) != 0) {
			perror("Failed to create thread"); #if cannot crete threads than write error message
			return 1;
		}	
	}
	
	#Send replace threads
	for (i = 0; i < numberOfThread[2]; i++){		 
		char* str = "This is an example line.";
		if(pthread_create(&replaceth[i], NULL, &replace, str) != 0) {
			perror("Failed to create thread"); #if cannot crete threads than write error message
			return 1;
		}	
	}
	#Send write threads
	for (i = 0; i < numberOfThread[3]; i++){
		 
		char* str = "This is an example line.\nThis is second line.\nThis is third line.\nThis is fourth line.\nThis is fifth line.\nThis is sixth line.\nThis is seventh line.";
		if(pthread_create(&writeth[i], NULL, &write, str) != 0) {
			perror("Failed to create thread"); #if cannot crete threads than write error message
			return 1;
		}	
	}
	
	for (i = 0; i < numberOfThread[1]; i++){			
		pthread_join(upperth[i], NULL);
	}
	
	for (i = 0; i < numberOfThread[2]; i++){		
		pthread_join(replaceth[i], NULL);
	}
		
	for (i = 0; i < numberOfThread[3]; i++){		
		pthread_join(writeth[i], NULL);
	}
	
	pthread_mutex_destroy(&mutex);
	pthread_exit(NULL);
	return 0;	
}
