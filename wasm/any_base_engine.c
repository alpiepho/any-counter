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


void show(char *label, int *returnArray, int returnSize) {
    int i;

    printf("%s[ ", label);
    for (i=0; i<returnSize; i++) {
        printf("%d", returnArray[i]);
        if (i < (returnSize-1))
            printf(", ");
    }
    printf(" ]\n");
}

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
        printf("  %d: [ ", j);
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
// Question Functions
//////////////////////////////////////////////////////

#define ENGINE_SIZE 10
int engine_mins[ENGINE_SIZE];
int engine_maxs[ENGINE_SIZE];
int engine_nums[ENGINE_SIZE];
int engine_minhex = 0;
int engine_maxhex = 0;

// These functions are intended for use as WebAssembly.  I don't
// think there is memory allocation in that environment so we will
// use a fixed number of digits.  Also, the WASM <-> JS interface
// gets tricky defining the JS signature, so i am starting simple
// by generating seperate parameters.

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
        if (engine_mins[i] > 9) engine_maxhex = 1;
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

void counter_increment(int *mins, int* maxs, int numsSize, int* accum){
    int place = numsSize-1;

    accum[place] += 1;
    // work backwards, add 1 to buffer[col][n] and carry if needed to n-1
    while (accum[place] > maxs[place]) {
        accum[place] = mins[place]; // reset place
        if (place == 0)
            break;         // have we maxed out
        if (place > 0) accum[place-1] += 1; // carry to next place
        place -= 1;
    }
}

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

    accum = (int *)malloc(sizeof(int) * numsSize);
    for (i=0; i<numsSize; i++) {
        accum[i] = mins[i];
    }

    // run counter
    // copy accum
    n = array[0];
    for (i=0; i<n; i++) {
        buffer[0][i] = accum[i];
    }
    for (col=1; col<total; col++) {
        counter_increment(mins, maxs, numsSize, accum); 
        // copy accum
        n = array[col];
        for (i=0; i<n; i++) {
            buffer[col][i] = accum[i];
        }
    }

    free(accum);
    return buffer;
}




int** counter_engine(int *mins, int* maxs, int numsSize, int* returnSize, int** returnColumnSizes){
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

// TODO: need to finish using engine_* functions
// passing 10 parameters is getting to be an issue
    // int new_nims[ENGINE_SIZE];

    // engine_mins(
    //     new_nims[9],
    //     new_nims[8],
    //     new_nims[7],
    //     new_nims[6],
    //     new_nims[5],
    //     new_nims[4],
    //     new_nims[3],
    //     new_nims[2],
    //     new_nims[1],
    //     new_nims[0]
    // );

    // accum = engine_nums;
    // for (i=0; i<numsSize; i++) {
    //     accum[i] = mins[i];
    // }

    // // run counter
    // // copy accum
    // n = array[0];
    // for (i=0; i<n; i++) {
    //     buffer[0][i] = accum[i];
    // }
    // for (col=1; col<total; col++) {
    //     counter_increment(mins, maxs, numsSize, accum); 
    //     // copy accum
    //     n = array[col];
    //     for (i=0; i<n; i++) {
    //         buffer[col][i] = accum[i];
    //     }
    // }

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
        show("given: ", maxs, numsSize);
        returnArrays = counter(mins, maxs, numsSize, &returnSize, &returnColumnSizes);
        dump(returnArrays, returnSize, returnColumnSizes, show_as_hex);
        cleanup(returnArrays, returnSize, returnColumnSizes);
    }
    {
        int show_as_hex = 0;
        int mins[] = { 0 };
        int maxs[] = { 9 };
        int numsSize = sizeof(maxs)/sizeof(int);
        show("given: ", maxs, numsSize);
        returnArrays = counter(mins, maxs, numsSize, &returnSize, &returnColumnSizes);
        dump(returnArrays, returnSize, returnColumnSizes, show_as_hex);
        cleanup(returnArrays, returnSize, returnColumnSizes);
    }
    {
        int show_as_hex = 1;
        int mins[] = { 0 };
        int maxs[] = { 15 };
        int numsSize = sizeof(maxs)/sizeof(int);
        show("given: ", maxs, numsSize);
        returnArrays = counter(mins, maxs, numsSize, &returnSize, &returnColumnSizes);
        dump(returnArrays, returnSize, returnColumnSizes, show_as_hex);
        cleanup(returnArrays, returnSize, returnColumnSizes);
    }
    {
        int show_as_hex = 0;
        int mins[] = { 0,0,0 };
        int maxs[] = { 3,2,1 };
        int numsSize = sizeof(maxs)/sizeof(int);
        show("given: ", maxs, numsSize);
        returnArrays = counter(mins, maxs, numsSize, &returnSize, &returnColumnSizes);
        dump(returnArrays, returnSize, returnColumnSizes, show_as_hex);
        cleanup(returnArrays, returnSize, returnColumnSizes);
    }
    {
        int show_as_hex = 0;
        int mins[] = { 0,0 };
        int maxs[] = { 2,2 };
        int numsSize = sizeof(maxs)/sizeof(int);
        show("given: ", maxs, numsSize);
        returnArrays = counter(mins, maxs, numsSize, &returnSize, &returnColumnSizes);
        dump(returnArrays, returnSize, returnColumnSizes, show_as_hex);
        cleanup(returnArrays, returnSize, returnColumnSizes);
    }
    {
        int show_as_hex = 0;
        int mins[] = { 1,1 };
        int maxs[] = { 2,2 };
        int numsSize = sizeof(maxs)/sizeof(int);
        show("given: ", maxs, numsSize);
        returnArrays = counter_engine(mins, maxs, numsSize, &returnSize, &returnColumnSizes);
        dump(returnArrays, returnSize, returnColumnSizes, show_as_hex);
        cleanup(returnArrays, returnSize, returnColumnSizes);
    }

    return 0;
}

