/*
 *      How to run?
 *      gcc -o main main.c -pthread
 *       ./main -d test.txt -n 2 2 2 2
 *
 *      fseek fuction might be used in project.
 */

#include <stdio.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void *readLine(void *threadid);
void* upper_function(void* args);
int determineLineNumber();
void readFile();
int getFileLineCount();
void* read_function(void* args);
char* getLine(int index);
char *strupr(char *str);

#define MAXLINELENGTH 150
#define LINENUMBER 150


struct thread_data
{
    int thread_id;
};



struct line_data
{
    char *line;
    int  readFlag;
    int  upperFlag;
    int  replaceFlag;
    int  writeFlag;
};


int readCount = 0;
int upperCount = 0;
int replaceCount = 0;
int writeCount = 0;
int totalNumOfLines = 0;
char* fileName;


pthread_mutex_t readCount_mutex;
pthread_mutex_t upperCount_mutex;
pthread_mutex_t replaceCount_mutex;
pthread_mutex_t writeCount_mutex;
pthread_mutex_t array_mutex[LINENUMBER];





struct line_data lines[LINENUMBER];
int number_of_threads;




int main(int argc, char **args) {
    
    
    
    // To print error when usage of the program is wrong
    if((argc != 8) || strstr(args[1], "-d") == NULL || strstr(args[3], "-n") == NULL){
        perror("ERROR: Invalid arguments");
        perror("USAGE: ./a.out -d <fileName> -n <#ofReadThreads> <#ofUpperThreads> <#ofReplaceThreads> <#ofWriteThreads> ");
        exit(1);
    }
    
    
    printf("\n\nFile to read: %s , Thread count, respectively : %s %s %s %s\n\n\n", args[2],args[4],args[5],args[6],args[7]);
    
    printf("file name is %s\n", fileName);
    fileName = malloc(sizeof(args[2]));
    //printLines();
    strcpy(fileName, args[2]);
    totalNumOfLines = getFileLineCount(args[2]);
    printf("TNOL is: %d\n", totalNumOfLines);
    
    int number_of_read_threads = atoi(args[4]);
    int number_of_upper_threads = atoi(args[5]);
    int number_of_replace_threads = atoi(args[6]);
    int number_of_write_threads = atoi(args[7]);
    
    pthread_t read_thread[number_of_read_threads];
    pthread_t upper_thread[number_of_upper_threads];
    pthread_t replace_thread[number_of_replace_threads];
    pthread_t write_thread[number_of_write_threads];
    
    pthread_mutex_init(&readCount_mutex,NULL);
    pthread_mutex_init(&upperCount_mutex,NULL);
    pthread_mutex_init(&replaceCount_mutex,NULL);
    pthread_mutex_init(&writeCount_mutex,NULL);
    
    for (int i = 0; i < LINENUMBER; ++i)
    {
        pthread_mutex_init(&array_mutex[i],NULL);
        
    }
 
    /////CREATE THREADS
    //Read Thread
    for (int i = 0; i < number_of_read_threads; i++) {
        pthread_create (&read_thread[i], NULL, &read_function, (void*)&i);
    }
    for (int i = 0; i < number_of_upper_threads; i++) {
        pthread_create (&upper_thread[i], NULL, &upper_function, (void*)&i);
    }
    /////KILL THREADS
    for (int i = 0; i < number_of_read_threads; i++)
    {
        pthread_join(read_thread[i], NULL);
    }
    /////CREATE THREADS
    //upper Thread

    /////KILL THREADS
    for (int i = 0; i < number_of_upper_threads; i++)
    {
        pthread_join(upper_thread[i], NULL);
    }
    

    printf("\nARRAY IS:\n");
    for (int i = 0; i <= totalNumOfLines; ++i)
    {
        printf("Text: %sReadF: %d, UF: %d, RepF: %d, WF: %d\n", lines[i].line, lines[i].readFlag, lines[i].upperFlag, lines[i].replaceFlag, lines[i].writeFlag);
    }

    
    return 0;
}
void* read_function(void* args){
    int thread_id=*((int*)args);
    
    while(readCount<totalNumOfLines){
        pthread_mutex_lock(&readCount_mutex);
        for (int i = 0; i <= totalNumOfLines; i++)
        {
            pthread_mutex_lock(&array_mutex[i]);
            if (lines[i].readFlag != 1){
                printf("Thread %d is working in index %d\n",thread_id, i);
                //read line
                //printf("get line %s\n", getLine(i));
                lines[i].line = getLine(i);
                
                //end of read line
                readCount = readCount + 1;
                lines[i].readFlag = 1;
            }
            else{
                pthread_mutex_unlock(&array_mutex[i]);
                pthread_mutex_unlock(&readCount_mutex);
                continue;
            }
            pthread_mutex_unlock(&array_mutex[i]);
            pthread_mutex_unlock(&readCount_mutex);
        }
    }
    pthread_exit((void*)0);
}


void* upper_function(void* args){
    int thread_id=*((int*)args);
    
    while(upperCount<totalNumOfLines){
        pthread_mutex_lock(&upperCount_mutex);
        for (int i = 1; i <= totalNumOfLines; i++)
        {
            pthread_mutex_lock(&array_mutex[i]);
            if (lines[i].readFlag == 1 && lines[i].upperFlag != 1){
                printf("Thread %d is working to upper in index %d\n",thread_id, i);
                //upper line
                //strcpy(lines[i].line,toUppercase(lines[i].line));
                //toUppercase(lines[i].line);
                
                char * t; // first copy the pointer to not change the original
                int size = 0;

                unsigned char *p = (unsigned char *)lines[i].line;

                while (*p) {
                    *p = toupper((unsigned char)*p);
                    p++;
                }
                     

                //end of upper line
                upperCount = upperCount + 1;
                lines[i].upperFlag = 1;
            }
            else{
                pthread_mutex_unlock(&array_mutex[i]);
                pthread_mutex_unlock(&upperCount_mutex);
                continue;
            }
            pthread_mutex_unlock(&array_mutex[i]);
            pthread_mutex_unlock(&upperCount_mutex);
        }
    }
    pthread_exit((void*)0);
}

int getFileLineCount(){
    
    FILE *fp;
    int count = 0;  // Line counter (result)
    char c;  // To store a character read from file
    
    // Get file name from user. The file should be
    // either in current folder or complete path should be provided
    
    
    // Open the file
    fp = fopen(fileName, "r");
    
    // Check if file exists
    if (fp == NULL)
    {
        printf("Could not open file %s", fileName);
        return 0;
    }
    
    // Extract characters from file and store in character c
    for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') // Increment count if this character is newline
            count = count + 1;
    
    // Close the file
    fclose(fp);
    printf("The file %s has %d lines\n ", fileName, count);
    return count;
    
}

char* getLine(int index){
    char buf[512];
    int count = 0;
    FILE *file = fopen(fileName, "r");
    char *line = malloc(MAXLINELENGTH);
    while (fgets(line,  MAXLINELENGTH, file) != NULL) {
        count++;
        if (count == index){
            //printf("copy line %s\n", line);
            fclose(file);
            return line;
        }
    }
    return 0;
}


char *strupr(char *str)
{
  unsigned char *p = (unsigned char *)str;

  while (*p) {
     *p = toupper((unsigned char)*p);
      p++;
  }

  return str;
}
