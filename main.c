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
void printLines();
int getFileLineCount(char *fileName);


#define MAXLINELENGTH 150
#define LINENUMBER 150

int readCount = 0;
int upperCount = 0;
int replaceCount = 0;
int writeCount = 0;
int totalNumOfLines = 0;




int readedQueue[LINENUMBER];
char lines[LINENUMBER][MAXLINELENGTH];
int number_of_threads;

struct thread_data
{
    int	thread_id;
    int pass;
    
};

struct line_data
{
    char line[MAXLINELENGTH];
    int  readFlag;
    int  upperFlag;
    int  replaceFlag;
    int  writeFlag;
};



int main(int argc, char **args) {
    
    
    
    // To print error when usage of the program is wrong
    if((argc != 8) || strstr(args[1], "-d") == NULL || strstr(args[3], "-n") == NULL){
        perror("ERROR: Invalid arguments");
        perror("USAGE: ./a.out -d <fileName> -n <#ofReadThreads> <#ofUpperThreads> <#ofReplaceThreads> <#ofWriteThreads> ");
        exit(1);
    }
    
    
    printf("\n\nFile to read: %s , Thread count, respectively : %s %s %s %s \n\n\n", args[2],args[4],args[5],args[6],args[7]);
    number_of_threads = atoi(args[4]);
    //readFile();
    
    //printLines();
    int a = getFileLineCount(args[2]);
    printf("A is: %d", a);

    
    
    return 0;
}

int getFileLineCount(char *fileName){

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



void readFile(){
    struct thread_data thread_data_array[number_of_threads];
    pthread_t threads[number_of_threads];
    int *taskids[number_of_threads];
    int rc, t;
    int pass = 2;
    
    for(t=0;t<number_of_threads;t++) {
        thread_data_array[t].thread_id = t;
        thread_data_array[t].pass = pass;
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
    
    my_data = (struct thread_data *) threadarg;
    taskid = my_data->thread_id;
    lineNumber = my_data->pass;
    printf("Thread id:%d, Pass=%d\n", taskid, lineNumber);
    
    char buf[512];
    int count = 0;
    FILE *file = fopen("test.txt", "r");
    char line[256]; /* or other suitable maximum line size */
    while (fgets(line, sizeof line, file) != NULL) /* read a line */
    {
        count++;
        if (count == lineNumber)
        {
            for (int i = 0; i < LINENUMBER; i++)
            {
                if (lines[i][0] == '\0')
                {
                    printf("copy line %s, id is %d \n", line , taskid );
                    strcpy(lines[i], line);
                    break;
                }
            }
        }
    }
    fclose(file);
    pthread_exit(NULL);
}

void printLines(){
    for (int i = 0; i < LINENUMBER; ++i)
    {
        printf("%s\n", lines[i]);
    }
}
