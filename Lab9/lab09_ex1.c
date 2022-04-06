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

void pr_optimal()
{
	pf_opt = 0;
	int idx, ref;
	int* ref_idx;

	frame_init();

	
	ref_idx = (int*)malloc(sizeof(int) * tot_frames);
	for (int i = 0; i < tot_frames; ++i) {
		ref_idx[i] = -1;
	}

	for (int i = 0; i < tot_refs; ++i) {
		idx = -1; // assume page fault

		for (int j = 0; j < tot_frames; ++j) {
			if (page_refs[i] == FRAME[j]) {
				idx = j;
			}
		}

		if (idx == -1) {
			++pf_opt;

			int empty_idx = -1; 
			for (int k = 0; k < tot_frames; ++k) {
				if (FRAME[k] == -1) {	
					empty_idx = k;
					break;
				}	
			}
			if (empty_idx >= 0) {
				FRAME[empty_idx] = page_refs[i];
			} else {
				for (int k = 0; k < tot_frames; ++k) {
					ref = -1; // assume entry is not referenced again
					for (int l = i; l < tot_refs; ++l) {
						if (FRAME[k] == page_refs[l]) {
							ref = l;
							break;
						}
					}
					ref_idx[k] = ref;
				}

				int nr_idx = -1; 
				for (int k = 0; k < tot_frames; ++k) {
					if (ref_idx[k] == -1) {
						nr_idx = k;
						break;
					}
				}
				if (nr_idx >= 0) {
					FRAME[nr_idx] = page_refs[i];
				} else {	
					int max_idx = 0;
					for (int k = 1; k < tot_frames; ++k) {
						if (ref_idx[k] > ref_idx[max_idx]) {
							max_idx = k;
						}
					}	
					FRAME[max_idx] = page_refs[i];
				}
			}

		}	
	} 
	free(ref_idx);
} 



int main (int argc, char** argv)
{
	total_frames = atoi(argv[1]);
	if (total_frames < 1 || total_frames > 7) {
		printf("Total frames must be in range [1, 7]\n");
		return -1;
	}

	/* Create FRAME array and initialize values to -1 */
	FRAME = (int*)malloc(sizeof(int) * total_frames);


	FILE* fp = fopen("input.txt", "rt");
	if (fp == NULL) return -1;

	fscanf(fp, "%d", &total_refs);

	page_refs = (int*)malloc(sizeof(int) * total_refs);

	for (int i = 0; i < total_refs; ++i) {
		fscanf(fp, "%d", &(page_refs[i]));
	}

	fclose(fp);

	// Run FIFO
	pr_fifo();

	// Run LRU
	pr_lru();

	// Run optimal
	pr_optimal();

	// generate report
	fp = fopen("output.txt", "wt");

	fprintf(fp, "FIFO: %d\n", pf_fifo);
	fprintf(fp, "LRU: %d\n", pf_lru);
	fprintf(fp, "Optimal: %d\n", pf_opt);
	

	free(page_refs);
	free(FRAME);

	return 0;
}
