#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int len;
int *nums ;
// 90 81 78 95 79 72 85
// = {90, 81, 78, 95, 79, 72, 85};
int r_min, r_max;
float r_avg, r_med, r_stddev;

void *myMin (void *param)
{
    int i;
    
    r_min= nums[0];
    
    for ( i = 0; i < len; ++i ) {
        if ( r_min > nums[i] ) 
            r_min = nums[i];
    }
    pthread_exit(0);
}

void *myMax (void *param)
{
    int i;
    
    r_max = nums[0];
    
    for ( i = 0; i < len; ++i ) {
        if (r_max < nums[i] ) 
            r_max = nums[i];
    }
    pthread_exit(0);
}

void *myAvg (void *param)
{
    int i;
    
    r_avg = 0;
    
    for ( i = 0; i < len; ++i ) {
        r_avg = r_avg + nums[i];
    }

    r_avg = r_avg / len;

    pthread_exit(0);
}

void *myMed (void *param)
{
    int i, j, temp;
    
    r_med = 0;
    
    for ( i = 0; i < len; ++i ) {
        for (j = 0; j<len-i-1; ++j){
            if (nums[j+1] < nums[j]){
                temp = nums[j];
                nums[j] = nums[j+1];
                nums[j+1] = temp;
            }
        }
    }

    if (len % 2 == 0){
        r_med = ( nums[len/2] + nums[(len/2) - 1] ) / 2.0;
    } else 
        r_med = nums[len/2];


    pthread_exit(0);
}

void *myStdDev (void *param)
{
    float mean = 0;
    float mean_diff = 0;
    
    for ( int i = 0; i < len; ++i ) {
        mean += nums[i];
    }
    mean = mean/len;


    for ( int i = 0; i < len; ++i ) {
        mean_diff +=( (nums[i] - mean) * (nums[i] - mean) );
    }

    mean_diff = mean_diff / len;

    r_stddev = sqrt(mean_diff);

    pthread_exit(0);
}



int main (int argc, char** argv)
{
    if(argc <= 1){
        printf("Give arguments! \n");
        exit(0);
    }

    len = argc-1;
    nums = (int*) malloc(sizeof(int)*len);

    for (int i = 1; i< argc; ++i){
        nums[i-1] = atoi(argv[i]);
    }
    pthread_t tid;
    pthread_t tid2;
    pthread_t tid3;
    pthread_t tid4;
    pthread_t tid5;

    pthread_create ( &tid, 0, myMin, NULL );
    pthread_create ( &tid2, 0, myMax, NULL );
    pthread_create ( &tid3, 0, myAvg, NULL );
    pthread_create ( &tid4, 0, myMed, NULL );
    pthread_create ( &tid5, 0, myStdDev, NULL );
    
    pthread_join ( tid, NULL );
    pthread_join ( tid2, NULL );
    pthread_join ( tid3, NULL );
    pthread_join ( tid4, NULL );
    pthread_join ( tid5, NULL );

    
    printf ( "Min = %d\n", r_min);    
    printf ("Max = %d\n", r_max);
    printf ("Average = %f\n", r_avg);
    printf ("Median = %f\n", r_med);
    printf ("Standard Deviation = %f\n", r_stddev);


    return 0;
}