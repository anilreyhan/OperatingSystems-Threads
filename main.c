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
void *readLine(void *threadid);
int determineLineNumber();
void readFile();
int getFileLineCount();
void* read_function(void* args);
void* replace_function(void* line);
char* getLine(int index);
char *replaceChars(char *line);



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
    
    
    //hghgf
    
    
    /////CREATE THREADS
    //Read Thread
    for (int i = 0; i < number_of_read_threads; i++) {
        pthread_create (&read_thread[i], NULL, &read_function, (void*)&i);
    }
    for (int i = 0; i < number_of_replace_threads; i++) {
        pthread_create (&replace_thread[i], NULL, &replace_function, (void*)&i);
    }
    
    
    /////KILL THREADS
    
    pthread_join(read_thread[0], NULL);
    pthread_join(read_thread[1], NULL);
    
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
                printf("Read_%d is working in index %d\n",thread_id, i);
                //read line
                printf("get line %s\n", getLine(i));
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

void* replace_function(void* args){
    int thread_id=*((int*)args);
    
    while(replaceCount<totalNumOfLines){
        pthread_mutex_lock(&replaceCount_mutex);
        for (int i = 1; i <= totalNumOfLines; i++)
        {
            pthread_mutex_lock(&array_mutex[i]);
            if (lines[i].readFlag == 1 && lines[i].replaceFlag != 1){
                printf("Replace_%d is working in index %d\n",thread_id, i);
                //read line
                //printf("get line %s\n", getLine(i));
                printf("To replace: %s\n", lines[i].line);
               // printf("To replaced maybe??: %s\n", replaceChars(lines[i].line));

                
                char * t; // first copy the pointer to not change the original
                int size = 0;

                unsigned char *p = (unsigned char *)lines[i].line;

                while (*p) {
                    if(*p == ' '){
                        *p = '_';
                    }
                    p++;
                }

                
               //printf("Replaced: %s\n", replaceChars(lines[i].line));
                //end of read line
                replaceCount = replaceCount + 1;
                lines[i].replaceFlag = 1;
            }
            else{
                pthread_mutex_unlock(&array_mutex[i]);
                pthread_mutex_unlock(&replaceCount_mutex);
                continue;
            }
            pthread_mutex_unlock(&array_mutex[i]);
            pthread_mutex_unlock(&replaceCount_mutex);
        }
    }
    pthread_exit((void*)0);
}


char *replaceChars(char *line){
    line = malloc(MAXLINELENGTH);
    printf("Beginning of replace: \n%s", line);

    while(*line!='\0')
    {
        if(*line==" ")
        {
            line="_";
        }
        line++;
    }
    printf("End of replace: \n%s",line);

    return line;
    
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






void readFile(){
    struct thread_data thread_data_array[number_of_threads];
    pthread_t threads[number_of_threads];
    int *taskids[number_of_threads];
    int rc, t;
    
    for(t=0;t<number_of_threads;t++) {
        thread_data_array[t].thread_id = t;
        printf("Creating thread %d\n", t);
        rc = pthread_create(&threads[t], NULL, readLine, (void *)
                            &thread_data_array[t]);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    
    pthread_exit(NULL);
}

void *readLine(void *threadarg){
    //Empty function for now...
    int taskid, lineNumber;
    struct thread_data *my_data;
    
    pthread_exit(NULL);
    
}
