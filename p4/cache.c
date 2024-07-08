/*
 * EECS 370, University of Michigan
 * Project 4: LC-2K Cache Simulator
 * Instructions are found in the project spec.
 */

#include <stdio.h>
#include <math.h>

#define MAX_CACHE_SIZE 256
#define MAX_BLOCK_SIZE 256

extern int mem_access(int addr, int write_flag, int write_data);
extern int get_num_mem_accesses(void);

//Use this when calling printAction. Do not modify the enumerated type below.
enum actionType
{
    cacheToProcessor,
    processorToCache,
    memoryToCache,
    cacheToMemory,
    cacheToNowhere
};

typedef struct blockStruct
{
    int data[MAX_BLOCK_SIZE];
    int dirty;
    int valid;
    int lruLabel;
    int set;
    int tag;
} blockStruct;

typedef struct cacheStruct
{
    blockStruct blocks[MAX_CACHE_SIZE];
    int blockSize;
    int numSets;
    int blocksPerSet;
} cacheStruct;

/* Global Cache variable */
cacheStruct cache;
int numBitBlock = 0;
int numBitSet = 0;
int numBitTag = 0;
int sizeOfBlocks = 0;
int hitAt = -1;

void printAction(int, int, enum actionType);
void printCache(void);
int make16Bit(int, int, int, int, int);
int isFull(int);

/*
 * Set up the cache with given command line parameters. This is
 * called once in main(). You must implement this function.
 */
void cache_init(int blockSize, int numSets, int blocksPerSet){
    cache.blockSize = blockSize;
    cache.numSets = numSets;
    cache.blocksPerSet = blocksPerSet;

    numBitBlock = log2(blockSize);
    numBitSet = log2(numSets);
    numBitTag = 32 - numBitBlock - numBitSet;
    sizeOfBlocks = numSets * blocksPerSet;
    
    for(int i = 0; i < cache.numSets; i++){
        for(int j = 0; j < cache.blocksPerSet; j++){
            cache.blocks[i * cache.blocksPerSet + j].dirty = 0;
            cache.blocks[i * cache.blocksPerSet + j].valid = 0;
            cache.blocks[i * cache.blocksPerSet + j].lruLabel = 0;
            cache.blocks[i * cache.blocksPerSet + j].set = i;
            cache.blocks[i * cache.blocksPerSet + j].tag = -1;
        }
    }

}

/*
 * Access the cache. This is the main part of the project,
 * and should call printAction as is appropriate.
 * It should only call mem_access when absolutely necessary.
 * addr is a 16-bit LC2K word address.
 * write_flag is 0 for reads (fetch/lw) and 1 for writes (sw).
 * write_data is a word, and is only valid if write_flag is 1.
 * The return of mem_access is undefined if write_flag is 1.
 * Thus the return of cache_access is undefined if write_flag is 1.
 */
int cache_access(int addr, int write_flag, int write_data){

    int setB = (((1 << numBitSet) - 1) & (addr >> numBitBlock));
    int tagB = (((1 << numBitTag) - 1) & (addr >> (numBitBlock + numBitSet)));
    if(numBitBlock == 0 && numBitSet == 0){
        tagB = addr;
    }
    int blockB = (((1 << numBitBlock) - 1) & (addr >> 0));

    int isHit = 0;
    for(int i = 0; i < sizeOfBlocks; i++){
        if(cache.blocks[i].set == setB && cache.blocks[i].tag == tagB && cache.blocks[i].valid == 1){
            hitAt = i;
            isHit = 1;
        }
    }

    if(isHit){ // if Hit
        // Update LRU
        for(int i = 0; i < sizeOfBlocks; i++){
            if (cache.blocks[i].set == setB && cache.blocks[hitAt].lruLabel > cache.blocks[i].lruLabel){
                cache.blocks[i].lruLabel++;
            }
        }
        cache.blocks[hitAt].lruLabel = 0;

        if(write_flag == 1){ // sw
            // Make it dirty
            cache.blocks[hitAt].dirty = 1;
            cache.blocks[hitAt].data[blockB] = write_data;
            
            printAction(addr, 1, processorToCache);
        }
        else if(write_flag == 0){ // fetch, lw
            printAction(addr, 1, cacheToProcessor);

            return cache.blocks[hitAt].data[blockB];
        }
    }
    else if(isHit == 0 && write_flag == 0){ // if Miss & fetch, lw
        int missAt = -1;
        
        if(isFull(setB) == 1){ // Set is empty
            int maxLRU = -1;
            
            for(int i = 0; i < sizeOfBlocks; i++){
                if(cache.blocks[i].set == setB && cache.blocks[i].lruLabel > maxLRU){
                    maxLRU = cache.blocks[i].lruLabel;
                    missAt = i;
                }
            }

            // Update LRU
            for(int i = 0; i < sizeOfBlocks; i++){
                if(cache.blocks[i].set == setB){
                    cache.blocks[i].lruLabel++;
                }
            }
            cache.blocks[missAt].lruLabel = 0;

            int address = make16Bit(numBitTag, cache.blocks[missAt].tag, numBitSet, setB, numBitBlock);

            if(cache.blocks[missAt].dirty == 0){
                // Don't need to write back
                printAction(address, cache.blockSize, cacheToNowhere);
            }
            else{
                // Update Memory
                cache.blocks[missAt].dirty = 0;
                printAction(address, cache.blockSize, cacheToMemory);

                for(int i = 0; i < cache.blockSize; i++){
                    mem_access(address, 1, cache.blocks[missAt].data[i]);
                    address++;
                }
            }

            address = make16Bit(numBitTag, tagB, numBitSet, setB, numBitBlock);

            // Load
            printAction(address, cache.blockSize, memoryToCache);
            printAction(addr, 1, cacheToProcessor);
            
            for(int i = 0; i < cache.blockSize; i++){
                cache.blocks[missAt].data[i] = mem_access(address, 0, -1);
                cache.blocks[missAt].tag = tagB;
                address++;
            }       
        }
        else { // Set is full
            // Update cache
            
            for(int i = 0; i < sizeOfBlocks; i++){
                if(cache.blocks[i].set == setB && cache.blocks[i].valid == 0){
                    cache.blocks[i].tag = tagB;
                    cache.blocks[i].valid = 1;
                    missAt = i;
                    break;
                }
            }
            // Update LRU
            for(int i = 0; i < sizeOfBlocks; i++){
                if(cache.blocks[i].set == setB){
                    cache.blocks[i].lruLabel++;
                }
            }
            cache.blocks[missAt].lruLabel = 0;

            int address = make16Bit(numBitTag, cache.blocks[missAt].tag, numBitSet, setB, numBitBlock);

            printAction(address, cache.blockSize, memoryToCache);
            printAction(addr, 1, cacheToProcessor);
            

            for(int i = 0; i < cache.blockSize; i++){
                 cache.blocks[missAt].data[i] = mem_access(address, 0, -1);
                 address++;
            }
        }
        return cache.blocks[missAt].data[blockB];
    }
    else if(isHit == 0 && write_flag == 1){ // Miss, sw
        int missAt = -1;

        if(isFull(setB) == 1){ 
            int missAt = -1;
            int maxLRU = -1;

            // Update LRU
            for(int i = 0; i < sizeOfBlocks; i++){
                if(cache.blocks[i].set == setB && cache.blocks[i].lruLabel > maxLRU){
                    maxLRU = cache.blocks[i].lruLabel;
                    missAt = i;
                }
            }
            for(int i = 0; i < sizeOfBlocks; i++){
                if(cache.blocks[i].set == setB){
                    cache.blocks[i].lruLabel++;
                }
            }
            cache.blocks[missAt].lruLabel = 0;

            int address = make16Bit(numBitTag, cache.blocks[missAt].tag, numBitSet, setB, numBitBlock);

            if(cache.blocks[missAt].dirty == 0){
                printAction(address, cache.blockSize, cacheToNowhere);
            }
            else if(cache.blocks[missAt].dirty == 1){
                cache.blocks[missAt].dirty = 0;
                printAction(address, cache.blockSize, cacheToMemory);
                for(int i = 0; i < cache.blockSize; i++){
                    mem_access(address, 1, cache.blocks[missAt].data[i]);
                    address++;
                }
            }

            address = make16Bit(numBitTag, tagB, numBitSet, setB, numBitBlock);

            printAction(address, cache.blockSize, memoryToCache);
            printAction(addr, 1, processorToCache);

            for(int i = 0; i < cache.blockSize; i++){
                cache.blocks[missAt].data[i] = mem_access(address, 0, -1);
                cache.blocks[missAt].tag = tagB;
                address++;
            }
            cache.blocks[missAt].data[blockB] = write_data;
            cache.blocks[missAt].dirty = 1;
        }
        else{ 
            int missAt = 0;

            for(int i = 0; i < sizeOfBlocks; i++){
                if(cache.blocks[i].set == setB && cache.blocks[i].valid == 0){
                    cache.blocks[i].dirty = 1;
                    cache.blocks[i].tag = tagB;
                    cache.blocks[i].valid = 1;
                    missAt = i;
                    break;
                }
            }

            for(int i = 0; i < sizeOfBlocks; i++){
                if(cache.blocks[i].set == setB){
                    cache.blocks[i].lruLabel++;
                }
            }
            cache.blocks[missAt].lruLabel = 0;

            int address = make16Bit(numBitTag, cache.blocks[missAt].tag, numBitSet, setB, numBitBlock);

            printAction(address, cache.blockSize, memoryToCache);
            printAction(addr, 1, processorToCache);
            
            for(int i = 0; i < cache.blockSize; i++){
                cache.blocks[missAt].data[i] = mem_access(address, 0, -1);
                address++;
            }
            cache.blocks[missAt].data[blockB] = write_data;
        }
    }
    return -1;
}

/*
 * print end of run statistics like in the spec. **This is not required**,
 * but is very helpful in debugging.
 * This should be called once a halt is reached.
 * DO NOT delete this function, or else it won't compile.
 * DO NOT print $$$ in this function
 */
void printStats(void){
    printf("\n");
}

/*
 * Log the specifics of each cache action.
 *
 *DO NOT modify the content below.
 * address is the starting word address of the range of data being transferred.
 * size is the size of the range of data being transferred.
 * type specifies the source and destination of the data being transferred.
 *  -    cacheToProcessor: reading data from the cache to the processor
 *  -    processorToCache: writing data from the processor to the cache
 *  -    memoryToCache: reading data from the memory to the cache
 *  -    cacheToMemory: evicting cache data and writing it to the memory
 *  -    cacheToNowhere: evicting cache data and throwing it away
 */
void printAction(int address, int size, enum actionType type)
{
    printf("$$$ transferring word [%d-%d] ", address, address + size - 1);

    if (type == cacheToProcessor) {
        printf("from the cache to the processor\n");
    }
    else if (type == processorToCache) {
        printf("from the processor to the cache\n");
    }
    else if (type == memoryToCache) {
        printf("from the memory to the cache\n");
    }
    else if (type == cacheToMemory) {
        printf("from the cache to the memory\n");
    }
    else if (type == cacheToNowhere) {
        printf("from the cache to nowhere\n");
    }
}

/*
 * Prints the cache based on the configurations of the struct
 * This is for debugging only and is not graded, so you may
 * modify it, but that is not recommended.
 */
void printCache(void)
{
    printf("\ncache:\n");
    for (int set = 0; set < cache.numSets; ++set) {
        printf("\tset %i:\n", set);
        for (int block = 0; block < cache.blocksPerSet; ++block) {
            printf("\t\t[ %i ]: {", block);
            for (int index = 0; index < cache.blockSize; ++index) {
                printf(" %i", cache.blocks[set * cache.blocksPerSet + block].data[index]);
            }
            printf(" }\n");
        }
    }
    printf("end cache\n");
}

int isFull(int set){
    for(int i = 0; i < sizeOfBlocks; i++){
        if(cache.blocks[i].set == set && cache.blocks[i].valid == 0){
            return 0;
        }
    }
    return 1;
}

int make16Bit(int numBitTag, int tagB, int numBitSet, int setB, int numBitBlock){
    int result = 0;
    result = result << numBitTag;
    result = result | tagB;
    result = result << numBitSet;
    result = result | setB;
    result = result << numBitBlock;
    result = result | 0;

    return result;
}
