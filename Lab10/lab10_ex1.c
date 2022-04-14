/** Riya Patel
CSC 345-01
Lab 10 Exercise 1
**/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

int max_sec;
int num_req;
int cur_loc;

int* req;

int fcfs() {
    int total = 0;
    int cur = cur_loc;

    for(int i = 0; i < num_req; i++){
        total += abs(req[i] - cur);
        cur = req[i];
    }
    return total;
}

int scan(){
    int total = 0;
    int cur = cur_loc;
    int* req_tmp = (int*) malloc(sizeof(int) * num_req);
    int i, j;

    for(i = 0; i < num_req; ++i)
        req_tmp[i] = req[i];
    
    // sort req
    for(i = 0; i < (num_req - 1); ++i){
        for (j = i; j < num_req; ++j){
            if (req_tmp[j] < req_tmp[i]){
                int t = req_tmp[j];
                req_tmp[j] = req_tmp[i];
                req_tmp[i] = t;
            }
        }
    }
    // decide direction
    for(i = 0; i < num_req; ++i){
        if (cur < req_tmp[i])
            break;
    }

    if (i == 0){
        for(j = 0; j < num_req; ++j){
            total += abs(req_tmp[j] - cur);
            cur = req_tmp[j];
        }
    } else if (i == num_req){
        for(j = (num_req - 1); j >= 0; --j){
            total += abs(req_tmp[j] - cur);
            cur = req_tmp[j];
        }
    } else {
        if(abs(cur - req_tmp[i-1]) < abs(cur - req_tmp[i])){
            for(j = (i - 1); j >= 0 ; --j){
                total += abs(req_tmp[j] - cur);
                cur = req_tmp[j];
            }

            cur = 0;
            for (int j = i; j < num_req; ++j){
                total += abs(req_tmp[j] - cur);
                cur = req_tmp[j];
            }
        } else {
            for(j = i; j < num_req; ++j){
                total += abs(req_tmp[j] - cur);
                cur = req_tmp[j];
            }
            cur = max_sec;
            for(j = (i - 1); j >= 0 ; --j){
                total += abs(req_tmp[j] - cur);
                cur = req_tmp[j];
            }
        }
    }
    return total;
}

int cscan(){
    int total = 0;
	int min = req[0];
	int max = req[0];
	int max_b_cur = req[0];

	for(int i = 1; i < num_req; ++i)
	{
        if(req[i] > max_b_cur && req[i] < cur_loc)
			max_b_cur = req[i];
        if(req[i] > max)
			max = req[i];
		if(req[i] < min)
			min = req[i];
	}

	if(cur_loc < max && cur_loc > min)
		total = (max_sec - cur_loc) + max_b_cur;
	else if(cur_loc <= min)
		total = (max - cur_loc);
	else if(cur_loc >= max)
		total = (max_sec - cur_loc) + max;	

	return total;
}

int main(int argc, char** argv)
{
    FILE* fp = fopen("input.txt", "rt");

    fscanf(fp, "%d", &max_sec);
    fscanf(fp, "%d", &num_req);

    req = (int*)malloc(sizeof(int) * num_req);

    for(int i = 0; i < num_req; i++)
    {
        fscanf(fp, "%d", &(req[i]));
    }

    fscanf(fp, "%d", &cur_loc);
    fclose(fp);

    fp = fopen("output.txt", "wt");
    fprintf(fp, "%d\n", fcfs());
    fprintf(fp, "%d\n", scan());
    fprintf(fp, "%d\n", cscan());

    free(req);
    return 0;
}