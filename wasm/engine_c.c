#include <stdio.h>
#include <emscripten/emscripten.h>

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

void EMSCRIPTEN_KEEPALIVE engine_dump(void) {
    engine_show("engine_mins", engine_mins, ENGINE_SIZE);
    engine_show("engine_maxs", engine_maxs, ENGINE_SIZE);
    engine_show("engine_nums", engine_nums, ENGINE_SIZE);
    printf("engine_minhex: %d\n", engine_minhex);
    printf("engine_maxhex: %d\n", engine_maxhex);
}

void EMSCRIPTEN_KEEPALIVE engine_min(
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

void EMSCRIPTEN_KEEPALIVE engine_max(
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

void EMSCRIPTEN_KEEPALIVE engine_reset(void) {
    int i;
    for (i=0; i<ENGINE_SIZE; i++) {
        engine_nums[i] = engine_mins[i];
    }
}

void EMSCRIPTEN_KEEPALIVE engine_get(void) {
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

void EMSCRIPTEN_KEEPALIVE engine_next(void) {
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

void EMSCRIPTEN_KEEPALIVE engine_run(int cycles) {
    int i;

    for(i=0; i<cycles; i++) {
        engine_next();
    }
}


int main(int argc, char ** argv) {
  return 0;
}
