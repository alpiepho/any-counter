#include <stdio.h>
#include <stdlib.h>


// gcc any_base_engine.c -o runme ; ./runme

// Given integers representing the max value for each digit, run a counter with for those digits.

// Example: [1,1]
// digits of place 0: 0, 1
// digits of place 1: 0, 1
// count up:
// 00
// 01
// 10
// 11

// Example: [9,9]
// digits of place 0: 0-9
// digits of place 1: 0-9
// count up:
// 00
// 01
// ...
// 99

// Example: [1,2]
// digits of place 0: 0, 1, 2
// digits of place 1: 0, 1
// count up:
// 00
// 01
// 02
// 10
// 11
// 12

// Why?  Image a problem where you want the permutations of the leters of a phone.  Some have 3 letters,
// others have 4 letters possible.  With some simple mapping of numbers to letters, we can use this
// counter to get the permutations.  Besides, its kind of fun.


//////////////////////////////////////////////////////
// Support Functions
//////////////////////////////////////////////////////

void dump(int **returnArrays, int returnSize, int* returnColumnSizes, int show_as_hex){
    int i, j, n;

    printf("returnColumnSizes: [ ");
    for (i=0; i<returnSize; i++) {
        printf("%d", returnColumnSizes[i]);
        if (i < (returnSize-1))
            printf(", ");
    }
    printf(" ]\n");

    printf("returnArrays: [\n");
    for (j=0; j<returnSize; j++) {
        n = returnColumnSizes[j];
        printf("  %2d: [ ", j);
        for (i=0; i<n; i++) {
            if (show_as_hex)
                printf("%x", returnArrays[j][i]);
            else
                printf("%d", returnArrays[j][i]);
            if (i < (n-1))
                printf(", ");
        }
        printf(" ]\n");
    }
    printf(" ]\n");

}

// NOTE: does not null base pointers (just couldnt send int ***)
void cleanup(int **returnArrays, int returnSize, int* returnColumnSizes){
    int i;
    int *p;

    free(returnColumnSizes);

    if (returnArrays) {
        for (i=0; i<returnSize; i++) {
            p = returnArrays[i];
            if (p) free(p);
        }

        free(returnArrays);
    }
}

//////////////////////////////////////////////////////
// Engine Functions
//////////////////////////////////////////////////////


// These functions are intended for use as WebAssembly.  I don't
// think there is memory allocation in that environment so we will
// use a fixed number of digits.  Also, the WASM <-> JS interface
// gets tricky defining the JS signature, so i am starting simple
// by generating seperate parameters.
#define ENGINE_SIZE 10
int engine_mins[ENGINE_SIZE];
int engine_maxs[ENGINE_SIZE];
int engine_nums[ENGINE_SIZE];
int engine_minhex = 0;
int engine_maxhex = 0;

void engine_show(char *label, int *returnArray, int returnSize) {
    int i;

    printf("%s[ ", label);
    for (i=0; i<returnSize; i++) {
        printf("%d", returnArray[i]);
        if (i < (returnSize-1))
            printf(", ");
    }
    printf(" ]\n");
}

void engine_min(
    int digit09,
    int digit08,
    int digit07,
    int digit06,
    int digit05,
    int digit04,
    int digit03,
    int digit02,
    int digit01,
    int digit00
) {
    int i = 9;
    engine_mins[i--] = digit09;
    engine_mins[i--] = digit08;
    engine_mins[i--] = digit07;
    engine_mins[i--] = digit06;
    engine_mins[i--] = digit05;
    engine_mins[i--] = digit04;
    engine_mins[i--] = digit03;
    engine_mins[i--] = digit02;
    engine_mins[i--] = digit01;
    engine_mins[i--] = digit00;
    engine_minhex = 0;
    for (i=0; i<ENGINE_SIZE && !engine_minhex; i++) {
        if (engine_mins[i] > 9) engine_minhex = 1;
    }
    //engine_show("engine_mins", engine_mins, ENGINE_SIZE);
}

void engine_max(
    int digit09,
    int digit08,
    int digit07,
    int digit06,
    int digit05,
    int digit04,
    int digit03,
    int digit02,
    int digit01,
    int digit00
) {
    int i = 9;
    engine_maxs[i--] = digit09;
    engine_maxs[i--] = digit08;
    engine_maxs[i--] = digit07;
    engine_maxs[i--] = digit06;
    engine_maxs[i--] = digit05;
    engine_maxs[i--] = digit04;
    engine_maxs[i--] = digit03;
    engine_maxs[i--] = digit02;
    engine_maxs[i--] = digit01;
    engine_maxs[i--] = digit00;
    engine_maxhex = 0;
    for (i=0; i<ENGINE_SIZE && !engine_maxhex; i++) {
        if (engine_maxs[i] > 9) engine_maxhex = 1;
    }
}

void engine_get(void) {
    // print string, let js decode? convert to hex
    int i;
    for (i=0; i<ENGINE_SIZE; i++) {
        if ((engine_minhex || engine_maxhex) && engine_nums[i]>9)
            printf("%x ", engine_nums[i]);
        else
            printf("%d ", engine_nums[i]);
    }
    printf("\n");
}

void engine_next(void) {
    int place = ENGINE_SIZE-1;

    engine_nums[place] += 1;
    // work backwards, add 1 to buffer[col][n] and carry if needed to n-1
    while (engine_nums[place] > engine_maxs[place]) {
        engine_nums[place] = engine_mins[place]; // reset place
        if (place == 0)
            break;         // have we maxed out
        if (place > 0) engine_nums[place-1] += 1; // carry to next place
        place -= 1;
    }
}

void engine_reset(void) {
    int i;
    for (i=0; i<ENGINE_SIZE; i++) {
        engine_nums[i] = engine_mins[i];
    }
}



//////////////////////////////////////////////////////
// Question Functions
//////////////////////////////////////////////////////

int** counter(int *mins, int* maxs, int numsSize, int* returnSize, int** returnColumnSizes){
    int i, j, n, col = 0;
    int total = 1;
    int *array;
    int **buffer;
    int *accum;

    for (i = 0; i < numsSize; i++) {
        total *= ( (maxs[i]-mins[i]) +1);     
    } 
    *returnSize = total;

    // all columns are same size, n
    array = (int *)malloc(sizeof(int) * total);
    for (i=0; i < total; i++) array[i] = numsSize;
    *returnColumnSizes = array;

    buffer = (int **)malloc(sizeof(int *) * total);
    for (j=0; j < total; j++) {
        n = array[j];
        buffer[j] = (int *)malloc(sizeof(int) * n);
    }

    // engine setup

    // engine_mins and engine_maxs need N parameters rather
    // than array (until I can figure out wasm interface)
    int temp[ENGINE_SIZE];

    // clear temp
    for (i=0; i<ENGINE_SIZE; i++) {
        temp[i] = 0;
    }
    // fill as many least significant digits as possible
    i = ENGINE_SIZE;
    j = numsSize;
    while (i && j) {
        temp[--i] = mins[--j];
    }
    // set mins
    engine_min(
        temp[9],
        temp[8],
        temp[7],
        temp[6],
        temp[5],
        temp[4],
        temp[3],
        temp[2],
        temp[1],
        temp[0]
    );

    // clear temp
    for (i=0; i<ENGINE_SIZE; i++) {
        temp[i] = 0;
    }
    // fill as many least significant digits as possible
    i = ENGINE_SIZE;
    j = numsSize;
    while (i && j) {
        temp[--i] = maxs[--j];
    }
    // set maxs
    engine_max(
        temp[9],
        temp[8],
        temp[7],
        temp[6],
        temp[5],
        temp[4],
        temp[3],
        temp[2],
        temp[1],
        temp[0]
    );

    engine_reset();

    // run counter
    // copy accum
    n = array[0];
    for (i = ENGINE_SIZE-1, j = n-1; j>=0; i--, j--) {
        buffer[0][j] = engine_nums[i];
    }
    for (col=1; col<total; col++) {
        engine_next(); 
        // copy accum
        n = array[col];
        for (i = ENGINE_SIZE-1, j = n-1; j>=0; i--, j--) {
            buffer[col][j] = engine_nums[i];
        }
    }

    return buffer;
}



//////////////////////////////////////////////////////
// Main
//////////////////////////////////////////////////////
int main(void) {
    int returnSize;
    int **returnArrays;
    int *returnColumnSizes;

    printf("any_base_engine.c:\n");
    {
        int show_as_hex = 0;
        int mins[] = { 0,0 };
        int maxs[] = { 1,1 };
        int numsSize = sizeof(maxs)/sizeof(int);
        engine_show("given mins: ", mins, numsSize);
        engine_show("given maxs: ", maxs, numsSize);
        returnArrays = counter(mins, maxs, numsSize, &returnSize, &returnColumnSizes);
        dump(returnArrays, returnSize, returnColumnSizes, show_as_hex);
        cleanup(returnArrays, returnSize, returnColumnSizes);
    }
    {
        int show_as_hex = 0;
        int mins[] = { 0 };
        int maxs[] = { 9 };
        int numsSize = sizeof(maxs)/sizeof(int);
        engine_show("given mins: ", mins, numsSize);
        engine_show("given maxs: ", maxs, numsSize);
        returnArrays = counter(mins, maxs, numsSize, &returnSize, &returnColumnSizes);
        dump(returnArrays, returnSize, returnColumnSizes, show_as_hex);
        cleanup(returnArrays, returnSize, returnColumnSizes);
    }
    {
        int show_as_hex = 1;
        int mins[] = { 0 };
        int maxs[] = { 15 };
        int numsSize = sizeof(maxs)/sizeof(int);
        engine_show("given mins: ", mins, numsSize);
        engine_show("given maxs: ", maxs, numsSize);
        returnArrays = counter(mins, maxs, numsSize, &returnSize, &returnColumnSizes);
        dump(returnArrays, returnSize, returnColumnSizes, show_as_hex);
        cleanup(returnArrays, returnSize, returnColumnSizes);
    }
    {
        int show_as_hex = 0;
        int mins[] = { 0,0,0 };
        int maxs[] = { 3,2,1 };
        int numsSize = sizeof(maxs)/sizeof(int);
        engine_show("given mins: ", mins, numsSize);
        engine_show("given maxs: ", maxs, numsSize);
        returnArrays = counter(mins, maxs, numsSize, &returnSize, &returnColumnSizes);
        dump(returnArrays, returnSize, returnColumnSizes, show_as_hex);
        cleanup(returnArrays, returnSize, returnColumnSizes);
    }
    {
        int show_as_hex = 0;
        int mins[] = { 0,0 };
        int maxs[] = { 2,2 };
        int numsSize = sizeof(maxs)/sizeof(int);
        engine_show("given mins: ", mins, numsSize);
        engine_show("given maxs: ", maxs, numsSize);
        returnArrays = counter(mins, maxs, numsSize, &returnSize, &returnColumnSizes);
        dump(returnArrays, returnSize, returnColumnSizes, show_as_hex);
        cleanup(returnArrays, returnSize, returnColumnSizes);
    }
    {
        int show_as_hex = 0;
        int mins[] = { 1,1 };
        int maxs[] = { 2,2 };
        int numsSize = sizeof(maxs)/sizeof(int);
        engine_show("given mins: ", mins, numsSize);
        engine_show("given maxs: ", maxs, numsSize);
        returnArrays = counter(mins, maxs, numsSize, &returnSize, &returnColumnSizes);
        dump(returnArrays, returnSize, returnColumnSizes, show_as_hex);
        cleanup(returnArrays, returnSize, returnColumnSizes);
    }

    return 0;
}

