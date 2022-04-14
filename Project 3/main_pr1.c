/*
 * Project 3: Virtual Memory Manager
 * Shwetha Raju & Riya Patel
 * CSC 345 01
 * Due Date: 04/15/2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>

/* Declaring CONST variables */
#define PAGE_SIZE 256                   /* page size (bytes) */
#define PT_ENTRY_MAX 256              /* max val for table entries */

#define FRAME_SIZE 256                  /* frame size (bytes) */
#define MAX_FRAMES 128                  /* max frames in physical memory */

#define INPUT_BYTE 256             /* count bytes read from BACKING_STORE.bin */
#define TLB_ENTRY_MAX 16           /* max TLB entries */

/* Output Files Created */
FILE *virtual;                          /* output file for virtual address */
FILE *physical;                         /* output file for physical address */
FILE *val;     
FILE *backing_store;                    /* mirror of everything in logical address space */

typedef struct
{
    int page_num;
    int frame_num;
} page_frame;

int phy_memory[MAX_FRAMES][FRAME_SIZE];         /* physical memory holds 256 bytes per frame */
page_frame TLB[TLB_ENTRY_MAX];             /* TLB holds a page number and a corresponding frame number */
page_frame page_table[PT_ENTRY_MAX];          /* page table holds a page number and a corresponding frame number */

int address_c = 0;                              /* address counter */
int page_faults = 0;                            /* page fault counter */
int hits = 0;                                   /* TLB hit counter */
int next_pos = 0;                               /* next available frame position in physical memory */
int next_table_pos = 0;                         /* next available position in page table */
int next_tlb_pos = 0;                           /* next available position in TLB */
int tempFIFO = 0;                               

/*instantiate functions */
void TLBInsert(int page_num, int frame_num);
void getPage(int address); 
void readStore(int page_num);

void TLBInsert(int page_num, int frame_num){
    if(next_tlb_pos < TLB_ENTRY_MAX){                  /* while there is space in TLB */
        TLB[next_tlb_pos].page_num = page_num;
        TLB[next_tlb_pos].frame_num = frame_num;
        next_tlb_pos++;
    } else {                                                /* if full - use FIFO for replacement */ 
        TLB[tempFIFO].page_num = page_num;
        TLB[tempFIFO].frame_num = frame_num; 
        tempFIFO = (tempFIFO+1) % TLB_ENTRY_MAX;       /* tracks next frame to be replaced */
    }
}
void getPage(int logical_address){
    /* Initializing page and offset */
    uint8_t page = logical_address >> 8 & 0xFF;     /* 0x0000FF00 */ 
    uint8_t offset = logical_address & 0xFF;        /* 0x000000FF */ 
    int frame = -1;                                 /* > 0 when frame is found */

	/* Iterative search through TLB for page */
    for(int i = 0; i < next_tlb_pos; i++){
        if(TLB[i].page_num == page){
            frame = TLB[i].frame_num;
            hits++;
            break; /* break loop once frame found */
        }
    }

    /* Iterative search through page table for page */
    if(frame == -1){
        for(int i = 0;i < next_table_pos; i++){
            if(page_table[i].page_num == page){
                frame = page_table[i].frame_num;
                break; /* break loop once frame found */

            }
        }
    }
    /* When page not found in tlb or page table: get frame from backing_store file */
    if(frame == -1){
        readStore(page);
        page_faults++;
        frame = next_pos - 1;
    }

    /* Insert page num and frame into TLB if not there already */
    TLBInsert(page, frame);

    int physical_address = frame * PAGE_SIZE + offset;
    int8_t value = phy_memory[frame][offset];
    fprintf(virtual,"%d\n",logical_address);        /* translated logical_address */
    fprintf(physical,"%d\n",physical_address);      /* corresponding physical address */
    fprintf(val,"%d\n",value);                      /* signed byte value stored in physical memory at the translated physical address */

}

void readStore(int page_num){
    int8_t buffer[256];           
    backing_store = fopen("BACKING_STORE.bin","rb");

    if(fseek(backing_store, page_num*INPUT_BYTE, SEEK_SET) != 0){
        fprintf(stderr,"Error: Unable to iterate through backing_store\n");
    }

    if(fread(buffer, sizeof(int8_t), INPUT_BYTE, backing_store) == 0){
        fprintf(stderr, "Error: Unable to read through file\n");
    }

    fclose(backing_store); 

    /* while there is space in physical memory */
    if(next_pos < MAX_FRAMES){
        for(int i = 0; i < INPUT_BYTE; ++i){
            phy_memory[next_pos][i] = buffer[i];
        }

        /* update page table information */
        page_table[next_table_pos].page_num = page_num;
        page_table[next_table_pos].frame_num = next_pos;

        next_pos++;
        next_table_pos++;
    } else { /* physical memory full - use FIFO for replacement */
        int j;
        for(j = 0; j < MAX_FRAMES-1; j++) {
            for(int i = 0; i < INPUT_BYTE; ++i) {
                phy_memory[j][i] = phy_memory[j+1][i];  
            }
            page_table[j] = page_table[j+1];            
        }

        /* append buffer to physical memory */
        for(int i = 0; i < INPUT_BYTE; ++i){
            phy_memory[j][i] = buffer[i];
        }

        page_table[j].page_num = page_num;             
        page_table[j].frame_num = next_pos-1;
    }
}

int main(int argc, char** argv){
    FILE *input;
    if(argc != 2) {
        fprintf(stderr,"Input address file not specified.\n"); 
        return -1;
    } else{
        char const* const fileName = argv[1]; 
        input = fopen(fileName, "rt");

        /* create output files */
        virtual = fopen("out1.txt","w");
        physical = fopen("out2.txt","w");
        val = fopen("out3.txt","w");
    }

    int32_t log_add;
    fscanf(input, "%d", &log_add); 

    /* pull logical addresses from the address file -- get physical address and value stored at that address */
    while(!feof(input)){    
        address_c++;
        getPage(log_add);
        fscanf(input, "%d", &log_add);
    }
    double pf_rate = page_faults / (double)address_c;
    double tlb_rate = hits / (double)address_c;

    printf("(with FIFO page replacement, using 128 frames)\n");
    printf("Page Faults = %d / %d, %.2f\n", page_faults, address_c, pf_rate);
    printf("TLB Hits =  %d / %d, %.3f\n", hits, address_c, tlb_rate);

    fclose(input);
    fclose(virtual);
    fclose(physical);
    fclose(val);
    return 0;
}