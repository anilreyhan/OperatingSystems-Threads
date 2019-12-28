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

int number_of_threads;
int maxLineLenght = 150;

struct thread_data
{
    int	thread_id;
    int  pass;
};


int main(int argc, char **args) {
    
    
    
     // To print error when usage of the program is wrong
    if((argc != 8) || strstr(args[1], "-d") == NULL || strstr(args[3], "-n") == NULL){
        perror("ERROR: Invalid arguments");
        perror("USAGE: ./a.out -d <directoryName> -n <#ofReadThreads> <#ofUpperThreads> <#ofReplaceThreads> <#ofWriteThreads> ");
        exit(1);
    }
    
    
    printf("File to read: %s , Thread count, respectively : %s %s %s %s \n", args[2],args[4],args[5],args[6],args[7]);
    int lineNumber = determineLineNumber();
    



    return 0;
}
void readFile(){
    struct thread_data thread_data_array[number_of_threads];
    pthread_t threads[number_of_threads];
    int *taskids[number_of_threads];
    int rc, t;
    int pass = 0;

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
    pthread_exit(NULL);
}

void *readLine(void *threadarg){
    //Empty function for now...
    int taskid, sum;
    struct thread_data *my_data;

    my_data = (struct thread_data *) threadarg;
    taskid = my_data->thread_id;
    sum = my_data->pass;
    printf("Thread id:%d, Pass=%d\n", taskid, sum);
    pthread_exit(NULL);
}

int determineLineNumber(){
    FILE *fp;
    int count = 0;  // Line counter (result)
    int countMaxLine = 0;
    char ch;  // To store a character read from file

    fp = fopen("test.txt", "r");
    if (fp == NULL){
    printf("Could not open file %s", "test.txtg");
    return 0;
    }
    for (ch = getc(fp); ch != EOF; ch = getc(fp)){
        countMaxLine++;
        maxLineLenght = countMaxLine;
        if (ch == '\n') { // Increment count if this character is newline
            count = count + 1;
            countMaxLine = 0;
        }
    }

    fclose(fp);
    return count;
    }
