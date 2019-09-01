#include <stdio.h>

#define WASM
#define ENGINE  // use engine implementation rather that original
//#define ALLOW_HELP

#ifdef WASM
#include <emscripten/emscripten.h>
#else
#include <stdlib.h>
#define EMSCRIPTEN_KEEPALIVE
#endif // WASM

// gcc any_base_counter.c -o runme ; ./runme

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

#ifndef WASM
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
#endif // WASM

#ifdef ENGINE
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

void wa_engine_show(char *label, int *returnArray, int returnSize) {
    int i;

    printf("%s[ ", label);
    for (i=0; i<returnSize; i++) {
        printf("%d", returnArray[i]);
        if (i < (returnSize-1))
            printf(", ");
    }
    printf(" ]\n");
}

void EMSCRIPTEN_KEEPALIVE wa_engine_dump(void) {
    wa_engine_show("engine_mins", engine_mins, ENGINE_SIZE);
    wa_engine_show("engine_maxs", engine_maxs, ENGINE_SIZE);
    wa_engine_show("engine_nums", engine_nums, ENGINE_SIZE);
    printf("engine_minhex: %d\n", engine_minhex);
    printf("engine_maxhex: %d\n", engine_maxhex);
}

void EMSCRIPTEN_KEEPALIVE wa_engine_min(
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
    int i = 0;
    engine_mins[i++] = digit09;
    engine_mins[i++] = digit08;
    engine_mins[i++] = digit07;
    engine_mins[i++] = digit06;
    engine_mins[i++] = digit05;
    engine_mins[i++] = digit04;
    engine_mins[i++] = digit03;
    engine_mins[i++] = digit02;
    engine_mins[i++] = digit01;
    engine_mins[i++] = digit00;
    engine_minhex = 0;
    for (i=0; i<ENGINE_SIZE && !engine_minhex; i++) {
        if (engine_mins[i] > 9) engine_minhex = 1;
    }
}

void EMSCRIPTEN_KEEPALIVE wa_engine_max(
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
    int i = 0;
    engine_maxs[i++] = digit09;
    engine_maxs[i++] = digit08;
    engine_maxs[i++] = digit07;
    engine_maxs[i++] = digit06;
    engine_maxs[i++] = digit05;
    engine_maxs[i++] = digit04;
    engine_maxs[i++] = digit03;
    engine_maxs[i++] = digit02;
    engine_maxs[i++] = digit01;
    engine_maxs[i++] = digit00;
    engine_maxhex = 0;
    for (i=0; i<ENGINE_SIZE && !engine_maxhex; i++) {
        if (engine_maxs[i] > 9) engine_maxhex = 1;
    }
}

void EMSCRIPTEN_KEEPALIVE wa_engine_reset(void) {
    int i;
    for (i=0; i<ENGINE_SIZE; i++) {
        engine_nums[i] = engine_mins[i];
    }
    printf("any_base_counter.wasm - wa_engine_reset\n");
}

void EMSCRIPTEN_KEEPALIVE wa_engine_get(void) {
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

int EMSCRIPTEN_KEEPALIVE wa_engine_getN(int i) {
    return engine_nums[i];
}


void EMSCRIPTEN_KEEPALIVE wa_engine_next(void) {
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

void EMSCRIPTEN_KEEPALIVE wa_engine_run(int cycles) {
    int i;

    for(i=0; i<cycles; i++) {
        wa_engine_next();
    }
}
#endif // ENGINE

//////////////////////////////////////////////////////
// Question Functions
//////////////////////////////////////////////////////

#ifndef ENGINE
void counter_increment(int* nums, int numsSize, int* accum){
    int place = numsSize-1;

    accum[place] += 1;
    // work backwards, add 1 to buffer[col][n] and carry if needed to n-1
    while (accum[place] > nums[place]) {
        accum[place] = 0; // reset place
        if (place == 0)
            break;         // have we maxed out
        if (place > 0) accum[place-1] += 1; // carry to next plae
        place -= 1;
    }
}
#endif // ENGINE

#ifndef WASM
int** counter(int* mins, int* maxs, int numsSize, int* returnSize, int** returnColumnSizes){
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

#ifdef ENGINE
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
        temp[0],
        temp[1],
        temp[2],
        temp[3],
        temp[4],
        temp[5],
        temp[6],
        temp[7],
        temp[8],
        temp[9]
    );
    wa_engine_show("mins", mins, numsSize);
    wa_engine_show("engine_mins2", engine_mins, ENGINE_SIZE);

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
        temp[0],
        temp[1],
        temp[2],
        temp[3],
        temp[4],
        temp[5],
        temp[6],
        temp[7],
        temp[8],
        temp[9]
    );
    wa_engine_reset();

    // run counter
    // copy accum
    n = array[0];
    for (i = ENGINE_SIZE-1, j = n-1; j>=0; i--, j--) {
        buffer[0][j] = engine_nums[i];
    }
    for (col=1; col<total; col++) {
        wa_engine_next(); 
        // copy accum
        n = array[col];
        for (i = ENGINE_SIZE-1, j = n-1; j>=0; i--, j--) {
            buffer[col][j] = engine_nums[i];
        }
    }

#else
    accum = (int *)malloc(sizeof(int) * total);
    for (i=0; i<total; i++) {
        accum[i] = 0;
    }

    // run counter
    // copy accum
    n = array[0];
    for (i=0; i<n; i++) {
        buffer[0][i] = accum[i];
    }
    for (col=1; col<total; col++) {
        counter_increment(maxs, numsSize, accum); 
        // copy accum
        n = array[col];
        for (i=0; i<n; i++) {
            buffer[col][i] = accum[i];
        }
    }

    free(accum);
#endif // ENGINE
    return buffer;
}
#endif // WASM

//////////////////////////////////////////////////////
// Main
//////////////////////////////////////////////////////
#ifdef WASM
int main(int argc, char ** argv) {
#ifdef ALLOW_HELP
  printf("Hello, welcome to any_base_counter in WebAssembly\n");
  printf("open Chrome tools console then you can run engine functions.\n");
  printf("(To open the developer console window on Chrome, use the keyboard shortcut CmdShiftJ (on Windows) or CmdOptionJ (on Mac).\n");


  printf("For example:\n\b");
  printf("// Set mins\n");
  printf("Module.ccall('wa_engine_min', null, ['number','number','number','number','number','number','number','number','number','number'], [0,0,0,0,0,0,0,0,0,0])\n");
  printf("// Set maxs\n");
  printf("Module.ccall('wa_engine_max', null, ['number','number','number','number','number','number','number','number','number','number'], [0,0,0,0,0,0,9,9,9,9])\n");
  printf("// Reset\n");
  printf("Module.ccall('wa_engine_reset', null, null, null)\n");
  printf("// Next\n");
  printf("Module.ccall('wa_engine_next', null, null, null)\n");
  printf("// Get\n");
  printf("Module.ccall('wa_engine_get', null, null, null)\n");
  printf("// Get\n");
  printf("Module.ccall('wa_engine_dump', null, null, null)\n");
  printf("// Run 10 cycles\n");
  printf("Module.ccall('wa_engine_run', null, ['number'], [10])\n");
#else
  printf("any_base_counter.wasm - loaded\n");
#endif
}
#else
int main(void) {
    int returnSize;
    int **returnArrays;
    int *returnColumnSizes;

    printf("any_base_counter.c:\n");
    {
        int show_as_hex = 0;
        int mins[] = { 0,0 };
        int maxs[] = { 1,1 };
        int numsSize = sizeof(maxs)/sizeof(int);
#ifdef ENGINE
        wa_engine_show("given mins: ", mins, numsSize);
        wa_engine_show("given maxs: ", maxs, numsSize);
#else
        show("given mins: ", mins, numsSize);
        show("given maxs: ", maxs, numsSize);
#endif
        returnArrays = counter(mins, maxs, numsSize, &returnSize, &returnColumnSizes);
        dump(returnArrays, returnSize, returnColumnSizes, show_as_hex);
        cleanup(returnArrays, returnSize, returnColumnSizes);
    }
    {
        int show_as_hex = 0;
        int mins[] = { 0 };
        int maxs[] = { 9 };
        int numsSize = sizeof(maxs)/sizeof(int);
#ifdef ENGINE
        wa_engine_show("given mins: ", mins, numsSize);
        wa_engine_show("given maxs: ", maxs, numsSize);
#else
        show("given mins: ", mins, numsSize);
        show("given maxs: ", maxs, numsSize);
#endif
        returnArrays = counter(mins, maxs, numsSize, &returnSize, &returnColumnSizes);
        dump(returnArrays, returnSize, returnColumnSizes, show_as_hex);
        cleanup(returnArrays, returnSize, returnColumnSizes);
    }
    {
        int show_as_hex = 1;
        int mins[] = { 0 };
        int maxs[] = { 15 };
        int numsSize = sizeof(maxs)/sizeof(int);
#ifdef ENGINE
        wa_engine_show("given mins: ", mins, numsSize);
        wa_engine_show("given maxs: ", maxs, numsSize);
#else
        show("given mins: ", mins, numsSize);
        show("given maxs: ", maxs, numsSize);
#endif
        returnArrays = counter(mins, maxs, numsSize, &returnSize, &returnColumnSizes);
        dump(returnArrays, returnSize, returnColumnSizes, show_as_hex);
        cleanup(returnArrays, returnSize, returnColumnSizes);
    }
    {
        int show_as_hex = 0;
        int mins[] = { 0,0,0 };
        int maxs[] = { 3,2,1 };
        int numsSize = sizeof(maxs)/sizeof(int);
#ifdef ENGINE
        wa_engine_show("given mins: ", mins, numsSize);
        wa_engine_show("given maxs: ", maxs, numsSize);
#else
        show("given mins: ", mins, numsSize);
        show("given maxs: ", maxs, numsSize);
#endif
        returnArrays = counter(mins, maxs, numsSize, &returnSize, &returnColumnSizes);
        dump(returnArrays, returnSize, returnColumnSizes, show_as_hex);
        cleanup(returnArrays, returnSize, returnColumnSizes);
    }
    {
        int show_as_hex = 0;
        int mins[] = { 0,0 };
        int maxs[] = { 2,2 };
        int numsSize = sizeof(maxs)/sizeof(int);
#ifdef ENGINE
        wa_engine_show("given mins: ", mins, numsSize);
        wa_engine_show("given maxs: ", maxs, numsSize);
#else
        show("given mins: ", mins, numsSize);
        show("given maxs: ", maxs, numsSize);
#endif
        returnArrays = counter(mins, maxs, numsSize, &returnSize, &returnColumnSizes);
        dump(returnArrays, returnSize, returnColumnSizes, show_as_hex);
        cleanup(returnArrays, returnSize, returnColumnSizes);
    }
    {
        int show_as_hex = 0;
        int mins[] = { 1,1 };
        int maxs[] = { 2,2 };
        int numsSize = sizeof(maxs)/sizeof(int);
#ifdef ENGINE
        wa_engine_show("given mins: ", mins, numsSize);
        wa_engine_show("given maxs: ", maxs, numsSize);
#else
        show("given mins: ", mins, numsSize);
        show("given maxs: ", maxs, numsSize);
#endif
        returnArrays = counter(mins, maxs, numsSize, &returnSize, &returnColumnSizes);
        dump(returnArrays, returnSize, returnColumnSizes, show_as_hex);
        cleanup(returnArrays, returnSize, returnColumnSizes);
    }

    return 0;
}
#endif // WASM

