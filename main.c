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
int getFileLineCount(char *fileName);


#define MAXLINELENGTH 150
#define LINENUMBER 150


struct thread_data
{
    int thread_id;
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


int readCount = 0;
int upperCount = 0;
int replaceCount = 0;
int writeCount = 0;

int totalNumOfLines = 0;

pthread_mutex_t readCount_mutex;
pthread_mutex_t upperCount_mutex;
pthread_mutex_t replaceCount_mutex;
pthread_mutex_t writeCount_mutex;



struct line_data lines[MAXLINELENGTH];
int number_of_threads;




int main(int argc, char **args) {
    
    
    
    // To print error when usage of the program is wrong
    if((argc != 8) || strstr(args[1], "-d") == NULL || strstr(args[3], "-n") == NULL){
        perror("ERROR: Invalid arguments");
        perror("USAGE: ./a.out -d <fileName> -n <#ofReadThreads> <#ofUpperThreads> <#ofReplaceThreads> <#ofWriteThreads> ");
        exit(1);
    }
    
    
    printf("\n\nFile to read: %s , Thread count, respectively : %s %s %s %s\n\n\n", args[2],args[4],args[5],args[6],args[7]);
    number_of_threads = atoi(args[4]);
    //readFile();
    
    //printLines();
    totalNumOfLines = getFileLineCount(args[2]);
    printf("TNOL is: %d\n", totalNumOfLines);
    
    
    
    
    /////CREATE THREADS
    for (int i = 0; i < mod_thread_size; i++) {
        pthread_create (&mod_thread[i],NULL,&read_function,(void*)&i);
    }
    
    
    /////KILL THREADS
    
    
    return 0;
}
void* read_function(void* args){
    char output[200];
    char temp_string[200];
    output[0]='\0';
    temp_string[0]='\0';
    int count=0;
    int thread_id=*((int*)args);
    while(genf_cond==0){ //it waits for queue1 to be created.
        
    }
    if(modf_cond==0){ //it will create queue2 if it is not created.
        //mutex1
        pthread_mutex_lock(&mod_mutex1);
        if(queue2 == NULL){
            queue2=createQueue();
            for (size_t i = 0; i<(SIZE/5)*(SIZE/5); i++)
                enQueue(queue2,i);//it add all elements into queue but their inside
            modf_cond=1;
        }
        pthread_mutex_unlock(&mod_mutex1);
        //close mutex1
    }
    struct QNode* temp;
    struct QNode* temp2;
    int cond_temp=0;
    
    while(count!=(SIZE/5)*(SIZE/5)){
        
        temp=queue1->front;
        temp2=queue2->front;
        count=0;
        
        while(temp !=NULL){
            //mutex2
            pthread_mutex_lock(&mod_mutex2);
            if(temp->gen_status>=2&&temp->mod_status==0){
                temp->mod_status=1;
                cond_temp=1;
            }
            //mutex2
            pthread_mutex_unlock(&mod_mutex2);
            
            
            if(cond_temp==1){
                
                while(temp2!=NULL){
                    if(temp2->id==temp->id)
                        break;
                    
                    temp2=temp2->next;
                }
                if(temp2!=NULL){
                    temp2->node=create_mod_inner_matrix(temp->node->in_matrix);
                    temp->mod_status=2;
                    temp2->mod_status=2;
                    log_into_matrix(temp->id,temp->node->in_matrix);
                    
                    sprintf(output,"Mod_%d genereted following submatrix into bigger matrix\n", thread_id);
                    print_in_matrix(temp_string,temp2->node->in_matrix);
                    strcat(output,temp_string);
                    sprintf(temp_string,"From  [%d,%d] submatrix\n",(temp->id/(SIZE/5)),(temp->id%(SIZE/5))  );
                    strcat(output,temp_string);
                    printf("%s\n",output );
                    
                }
                
                cond_temp=0;
            }
            if(temp->mod_status > 0)
                count ++;
            
            if(count==(SIZE/2)*(SIZE/2)){
                break;
            }
            temp=temp->next;
        }
        
        
    }
    //
    
    pthread_exit((void*)0);
    
    
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
    
    pthread_exit(NULL);
    
}
