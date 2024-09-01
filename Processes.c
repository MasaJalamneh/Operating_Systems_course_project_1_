// Name: Masa Ahmad Jalamneh
// ID: 1212145
// Section: 1
///////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <math.h>
#include <errno.h>


#define max_length_lines 100
#define max_records 1000

///////////////////////////////////////////////////////////

typedef struct // struct to store data from file
{
    float h; // Height
    float w; // Weight
} p;

p one_person[max_records];
int lines = 0;   // global conuter to count the number of lines


//////////////////////////////////////////////////////////
float bmi_calculator(float h, float w) // the naive function to do the calculations (bmi for each line)
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

        one_person[lines].w = atof(W_string); // storing Weight in the struct for each line
        one_person[lines].h = atof(H_string); // storing Height in the struct for each line

        lines++;
    }

    fclose(File);// file closing
}

//////////////////////////////////////////////////////////
float child_process(int num_c ,double lines_for_c)
{

    int x = lines_for_c*(num_c-1) ;
    float total =0;
    for(int j=x+1; j<=lines_for_c+x; j++)
    {
        if (j>=lines)
        break;


        total += (bmi_calculator(one_person[j].h,  one_person[j].w));

    }
    return total;
}

///////////////////////////////////////////////////////////
int main()
{


    int num_c = 5;  // number of processes
    double lines_for_c =0.0;  // number of lines for each process

    float total=0; // total of bmi for each line



    reading_file();  // call file reading




    lines_for_c= lines/num_c;  // calculate the number of line for each process
    ceil(lines_for_c);


    int fd[num_c][2];

    for (int k =0; k<num_c; k++) // loop to create pipes for processes (for communication)
    {
        if (pipe(fd[k]) == -1)
        {
            printf("error\n");
        }

    }



    struct  timespec begin;
    timespec_get(&begin,TIME_UTC);

    for (int x = 1; x <= num_c; x++) // loop to create processes
    {
        int pid = fork();
        if (pid == -1)
        {
            printf("Fork failed\n");
        }
        else if (pid == 0)// child process
        {

            total += child_process(x,lines_for_c );
            write(fd[x-1][1], &total, sizeof(float));//write (pipe)
            close(fd[x-1][1]);
            exit(0);
        }
        else// parent process
        {
            close(fd[x-1][1]);
            read(fd[x-1][0], &total, sizeof(float)); //read (pipe)
            close(fd[x-1][0]);
        }
    }

    float BMI_AVG = total / (500); // calculate the BMI average
    int j =0;
    //printf("total=%0.2f\n", total);
    if( j==0)
    {
        printf("avg bmi = %.2f\n",BMI_AVG);
        j=1;
    }

    struct  timespec end;
     timespec_get(&end,TIME_UTC);

    double time_spent=(end.tv_sec - begin.tv_sec)+(end.tv_nsec - begin.tv_nsec)/1000000000.0;
     printf("execution time= %lf\n",time_spent);

    return 0;
}
