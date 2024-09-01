// Name: Masa Ahmad Jalamneh
// ID: 1212145
// Section: 1
////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>

#define max_length_lines 100
#define max_records 1000
////////////////////////////////////////////////////////
typedef struct // struct to store data
{
    float h; // Height
    float w; // Weight
} p;

p one_person[max_records];

// global variables
float total =0.0;  // total bmi for each line
int thread_f =0;
int numOfThds =5; // number of threads
float th_lines = 0; // number of lines for each thread to execute
int num_p = 0; // number of people (lines)



pthread_mutex_t mutex1;

////////////////////////////////////////////////////////
float bmi_calculator(float h, float w) // the bmi calculating function to do the calculations (bmi for each line)
{
    float bmi;
    float height_in_meter= h / 100.0;

    bmi = w / (height_in_meter * height_in_meter);

    return bmi;
}

///////////////////////////////////////////////////////////
void reading_file() // reading function
{

    char *W_string;
    char *H_string;
    char *g;

    FILE *File = fopen("/home/taleen-bayatneh/Desktop/masa/bmi.csv", "r");// file opening
    if (File == NULL)
    {
        printf("Error opening file");
        exit(0);
    }

    char line_[max_length_lines];
    while (fgets(line_, max_length_lines, File) != NULL)// file reading
    {
        g = strtok(line_, ",");    // spliting line by ","
        H_string = strtok(NULL, ",");
        W_string = strtok(NULL, ",");

        one_person[num_p].w = atof(W_string); // storing Weight in the struct for each line
        one_person[num_p].h = atof(H_string); // storing Height in the struct for each line

        num_p++;
    }

    fclose(File);// file closing
}


////////////////////////////////////////////////////////
void* thread_process (void* num_of_th) // Threads function
{

    int n= *(int*)num_of_th; // number of thread (sequential- passed from the for loop in main function)
    int y= th_lines*(n);
    float sub_total=0;
    pthread_mutex_lock(&mutex1);

    for(int j=y+1; j<=th_lines+y ; j++)
    {
        if (j>=num_p)
            break;

        sub_total += (bmi_calculator(one_person[j].h, one_person[j].w));

    }
    thread_f ++;
    total+=sub_total;
    pthread_mutex_unlock(&mutex1);

    if (thread_f==numOfThds)// calculate the BMI average - final result
        printf("bmi avg = %.2lf\n", total/ (num_p-1));

}

////////////////////////////////////////////////////////
int main()
{
    struct  timespec begin;
    timespec_get(&begin,TIME_UTC);

    reading_file();



    pthread_t threads [numOfThds];
    th_lines= num_p/numOfThds;
    //float final_total=0;

    if (pthread_mutex_init(&mutex1, NULL) != 0)
    {
        printf("Mutex init has failed\n");
        return 1;
    }



    for (int  i=0; i < numOfThds; i++) // loop to create threads
    {
        int* thNUM = malloc(sizeof(int));
        *thNUM = i;
        pthread_create(&threads[i],NULL,&thread_process,(void *)thNUM);

    }

    for (int m=0; m < numOfThds; m++)
    {
        pthread_join(threads[m],NULL);
    }


    pthread_mutex_destroy(&mutex1);

    struct  timespec end;
    timespec_get(&end,TIME_UTC);

    double time_spent=(end.tv_sec - begin.tv_sec)+(end.tv_nsec - begin.tv_nsec)/1000000000.0;
    printf("execution time= %lf\n",time_spent);


     return 0;
}
