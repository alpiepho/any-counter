#include <stdio.h>
#include <emscripten/emscripten.h>

// TEST with
// Module.ccall('hello', 'number', null, null)
// or
// const hello = Module.cwrap('hello', 'number', null, null)
// hello()
//
// signature:
// Module.ccall('hello', 'number', ['string', 'string'], ['hello', 'world'])
int EMSCRIPTEN_KEEPALIVE hello(int argc, char ** argv) {
  printf("Hello!\n");
  return 8;
}

// MY TEST
//Module.ccall('hello', 'number', null, null)
// int EMSCRIPTEN_KEEPALIVE mytest(int argc, char ** argv) {
//   int i;
//   printf("argc: %d\n", argc);
//   printf("argv:\n");
//   for (i=0; i<argc; i++) {
//     printf("  %s\n", argv[i]);
//   }
//   return 7;
// }
Module.ccall('mytest', 'number', ['number', 'number'], [1, 3])
var mytest = Module.ccall('mytest', 'number', ['number', 'number'], [1, 3])
int EMSCRIPTEN_KEEPALIVE mytest(int cycles, int digit09) {
  printf("mytest!\n");
  printf("cycles  %d\n", cycles);
  printf("digit09 %d\n", digit09);
  return 0;
}

// IDEA
  // engine_min, give fixed digits (10)
  // engine_max, give fixed digits (10)
  // engine_get, print string, let js decode?
  // engine_next,
  // engine_reset,


int main(int argc, char ** argv) {
  printf("Hello World\n");
}
