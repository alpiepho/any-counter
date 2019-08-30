

## Build WebAssembly

:warning: TODO need to clean this up

This started as rough notes for experimenting with WebAssembly.  my goal is to allow
the user of the any-base-counter to run the counter in Javascript or optionally in
WebAssembly.

I started from a couple blogs:
- From [medium blog](https://medium.com/@robaboukhalil/hit-the-ground-running-with-webassembly-56cf9b2fa35d)
- From [flaviocopes webassembly blog](https://flaviocopes.com/webassembly/)

And later found a couple others (listed at the end).
(There are lots of pieces out there dating back to 2017.  Considering the $29 [book](https://www.levelupwasm.com/
))

The Medium blog outlines how to use a docker image, rather that installing emcc and the proper version
of python.  Gives examples of calling the main function.

The blog from Flavio has a better description of how to run Web Assembly from Javascript. (still don't know where he got the WASM=1 option).  Uses cwrap and ccall to call C functions within the WebAssembly
file.


### Docker Image to Run Emscripten compiler on .c file

The Medium blog post describes using a Docker image to run the wasm compiler instead of using 
the Enscriptem [install](https://emscripten.org/docs/getting_started/downloads.html) procedure.  
I am using that to avoid issues with Python versions on my development system.

```
# Fetch image containing Emscripten
$ docker pull robertaboukhalil/emsdk:1.38.30
# Create a local folder for our WebAssembly adventures
$ mkdir ~/wasm
$ cd ~/wasm
# Launch a container called "wasm" (in the background)
$ docker run -dt --name wasm \
    --volume ~/wasm:/src \
    -p 12345:80 \
    robertaboukhalil/emsdk:1.38.30
```

IMPORTANT NOTE: The --volume settings essentially maps the /src directory INSIDE the Docker instance to a
"wasm" directory in your HOME directory.  

Then the instructions to do your first compile:

```
# Go inside the container we created above
$ docker exec -it wasm bash
# Now we're inside the container!

# going to skip Medium blog steps and use Flavio
# $ git clone https://github.com/pod32g/MD5.git
# cd MD5
#emcc -o md5.html md5.c
```

These were good instructions to see how to use the Docker image and how it maps to a local directory.  But I don't
want to use his "git clone" method.  My wasm files will be in a subdirectory of the any-base-counter project.











From the Flavio blog:

Create test.c:
```
#include <stdio.h>

int main(int argc, char ** argv) {
  printf("Hello World\n");
}
```


Compile:
```
emcc test.c -s WASM=1 -o test.html
```

Run (port set from docker launch -p 12345:80):
```
http://localhost:12345/MD5/test.html
```

Open Chrome Console:
```
Module.callMain([ "test" ])
```

```
#include <stdio.h>
#include <emscripten/emscripten.h>

int EMSCRIPTEN_KEEPALIVE hello(int argc, char ** argv) {
  printf("Hello!\n");
  return 8;
}

int main(int argc, char ** argv) {
  printf("Hello World\n");
}

emcc test.c -s WASM=1 -o test.html -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']"

```

Again, in Chrome Console:
```
Module.ccall('hello', 'number', null, null)
const hello = Module.cwrap('hello', 'number', null, null)
hello()

```


Captured some docker command line for reference:
```
// code snipets for emscripten console window
// Set mins
Module.ccall('engine_min', null, ['number','number','number','number','number','number','number','number','number','number'], [0,0,0,0,0,0,0,0,0,0])
// Set maxs
Module.ccall('engine_max', null, ['number','number','number','number','number','number','number','number','number','number'], [0,0,0,0,0,0,9,9,9,9])
// Reset
Module.ccall('engine_reset', null, null, null)
// Next
Module.ccall('engine_next', null, null, null)
// Get
Module.ccall('engine_get', null, null, null)
// Get
Module.ccall('engine_dump', null, null, null)
// Run 10 cycles
Module.ccall('engine_run', null, ['number'], [10])

// Sequence
Module.ccall('engine_min', null, ['number','number','number','number','number','number','number','number','number','number'], [0,0,0,0,0,0,0,0,0,0])
Module.ccall('engine_max', null, ['number','number','number','number','number','number','number','number','number','number'], [0,0,0,0,0,0,9,9,9,9])
Module.ccall('engine_reset', null, null, null)
Module.ccall('engine_dump', null, null, null)
Module.ccall('engine_run', null, ['number'], [10])
Module.ccall('engine_dump', null, null, null)
Module.ccall('engine_run', null, ['number'], [10])
Module.ccall('engine_dump', null, null, null)
Module.ccall('engine_run', null, ['number'], [10])
Module.ccall('engine_dump', null, null, null)

```




### Links for All Resources

- From [medium blog](https://medium.com/@robaboukhalil/hit-the-ground-running-with-webassembly-56cf9b2fa35d)
- From [flaviocopes webassembly blog](https://flaviocopes.com/webassembly/)


- [WebAssembly Standard](http://webassembly.github.io/spec/js-api/index.html)
- [WebAssembly](https://becominghuman.ai/passing-and-returning-webassembly-array-parameters-a0f572c65d97)
- [Explanation of cwrap and ccall](https://medium.com/@eliamaino/calling-c-functions-from-javascript-with-emscripten-first-part-e99fb6eedb22)

Also, for reference, the output from the emcc compiler (that was run from within a docker
image) is in the file emcc_help.txt
