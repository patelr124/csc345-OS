/** Riya Patel
CSC 345-01
Lab 9 Exercise 1
**/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int tot_frames;
int * FRAME;
int tot_refs;
int* page_refs;

voic frame_init(){
    for(int i = 0; i< tot_frames; ++i){
        FRAME[i]=-1;
    }
}

void pr_fifo(){
    pf_fifo = 0;
    int dx;
    int fptr;

    frame_init();
    for(int i = 0; i< tot_refs; ++i){
        idx = -1;
        for(int j = 0; j< tot_frames; ++j){
            if(page_refs[i]==FRAME[j]){ 
                /* if no page fault > continue to next request */
                idx = j;
                break;
            }
        }
        /* yes page fault > insert page */
        if(idx == -1){
            ++pf_fifo;
            FRAME[fptr] = page_refs[i];
            fptr = (fptr + 1) % tot_frames;
        }
    }
    return;
}

void pr_lru(){
    pf_lru = 0;
    int idx;

    frame_init();

    for(int i = 0; i< tot_refs; ++i){
        idx = -1;
        for(int j = 0; j< tot_frames; ++j){
            if(page_refs[i]==FRAME[j]){ 
                /* if no page fault > continue to next request */
                idx = j;
                break;
            }
        }
        /* yes page fault > insert page */
        if(idx == -1 || (tot_frames - 1) == idx){
            ++pf_lru;
            for(int j = tot_frames - 1; j > 0; --j){
                FRAME[j] = FRAME[j-1];
            }
            FRAME[0] = page_refs[i];
        } else {
            for (int j = idx; j > 0; --j){
                FRAME[j] = FRAME[j-1];
            }
            FRAME[0] = page_refs[i];
        }
    }
    return;
}



int main(int argc, char** argv){
    srand(time(NULL));
    int numRefs = atoi(argv[1]);
    FILE* fp = fopen("input.txt","wt");
    fprintf(fp, "%d\n", numRefs);

    for(int i = 0; i< numRefs; ++i){
        fprintf(fp, "%d ", rand()%MAX_PAGE);
    }

    fprintf(fp, "\n");
    fclose(fp);
    return 0;


}