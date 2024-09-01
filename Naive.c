// Name: Masa Ahmmad Jalamneh
// ID: 1212145
// Section: 1
/////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define max_length_lines 1000

//////////////////////////////////////////////////////////
float naive(float h, float w)
{
    float bmi;
    float height_in_meter= h / 100.0;

    bmi = w /(height_in_meter*height_in_meter);

    return bmi;
}

/////////////////////////////////////////////////////////
int main()
{
    struct  timespec begin;
    timespec_get(&begin,TIME_UTC);


    char *W_string;
    char *g;
    char *H_string;
    float BMI;
    float h;
    float w;

    float total=0;
    float avg_bmi;

    FILE *file;
    file= fopen("/home/taleen-bayatneh/Desktop/masa/bmi.csv","r");

    if (file == NULL) {
        printf("The file is not opened. The program will "
               "now exit.\n");
        exit(0);
        }




    int c1=0;
    char l[max_length_lines];


     while (fgets(l, sizeof(l), file) != NULL) // read file and calculate bmi for each line
     {



        g = strtok(l, ",");
        H_string = strtok(NULL, ",");
        W_string = strtok(NULL, ",");

        h = atof(H_string);
        w = atof(W_string);

        // Calculate BMI
        if (h != 0 && w != 0){
         BMI= naive(h, w); // function call bmi calculation
         total+= BMI;
         c1++;
        }


    }



    avg_bmi= total / c1;

    printf("BMI average is = %.2lf \n", avg_bmi); // find and print the final result (avg)



        fclose(file);

    struct  timespec end;
     timespec_get(&end,TIME_UTC);

     double time_spent=(end.tv_sec - begin.tv_sec)+(end.tv_nsec - begin.tv_nsec)/1000000000.0;
     printf("it took time = %lf \n",time_spent);


    return 0;
}
//////////////////////////////////////////////////////////

