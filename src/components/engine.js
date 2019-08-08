
var str = "1 1"
var minDigits = []
var maxDigits = []
var allDigits = []

// void counter_increment(int* nums, int numsSize, int* accum){
//     int place = numsSize-1;

//     accum[place] += 1;
//     // work backwards, add 1 to buffer[col][n] and carry if needed to n-1
//     while (accum[place] > nums[place]) {
//         accum[place] = 0; // reset place
//         if (place == 0)
//             break;         // have we maxed out
//         if (place > 0) accum[place-1] += 1; // carry to next plae
//         place -= 1;
//     }
// }

// int** counter(int* nums, int numsSize, int* returnSize, int** returnColumnSizes){
//     int i, j, n, col = 0;
//     int total = 1;
//     int *array;
//     int **buffer;
//     int *accum;

//     for (i = 0; i < numsSize; i++) {
//         total *= (nums[i]+1);     
//     } 
//     *returnSize = total;

//     // all columns are same size, n
//     array = (int *)malloc(sizeof(int) * total);
//     for (i=0; i < total; i++) array[i] = numsSize;
//     *returnColumnSizes = array;

//     buffer = (int **)malloc(sizeof(int *) * total);
//     for (j=0; j < total; j++) {
//         n = array[j];
//         buffer[j] = (int *)malloc(sizeof(int) * n);
//     }

//     accum = (int *)malloc(sizeof(int) * total);
//     for (i=0; i<total; i++) {
//         accum[i] = 0;
//     }

//     // run counter
//     // copy accum
//     n = array[0];
//     for (i=0; i<n; i++) {
//         buffer[0][i] = accum[i];
//     }
//     for (col=1; col<total; col++) {
//         counter_increment(nums, numsSize, accum); 
//         // copy accum
//         n = array[col];
//         for (i=0; i<n; i++) {
//             buffer[col][i] = accum[i];
//         }
//     }

//     free(accum);
//     return buffer;
// }



export function engine_min(newValue) {
    console.log('Engine.engine_min')
}

export function engine_max(newValue) {
    console.log('Engine.engine_max')
}

export function engine_get() {
    let result = allDigits.reduce((prev, digit) => {
        return prev + ' ' + digit 
    }, '')
    return result
}

export function engine_add() {
    console.log('Engine.engine_add')
    allDigits = [...allDigits, 1]
}

export function engine_sub() {
    console.log('Engine.engine_sub')
    allDigits.pop()
}

export function engine_reset() {
    console.log('Engine.engine_reset')
    allDigits = [...minDigits]
}
