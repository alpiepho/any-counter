

### Build WebAssembly

:warning: TODO need to clean this up



From [medium blog](https://medium.com/@robaboukhalil/hit-the-ground-running-with-webassembly-56cf9b2fa35d)
and
From [flaviocopes webassembly blog](https://flaviocopes.com/webassembly/)

Others:

http://webassembly.github.io/spec/js-api/index.html
https://becominghuman.ai/passing-and-returning-webassembly-array-parameters-a0f572c65d97
https://medium.com/@eliamaino/calling-c-functions-from-javascript-with-emscripten-first-part-e99fb6eedb22


The Medium blog outlines how to use a docker image, rather that installing emcc and the proper version
of python.

The blog from Flavio has a better description of how to run Web Assembly from Javascript.

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

```
# Go inside the container we created above
$ docker exec -it wasm bash
# Now we're inside the container!

# going to skip Medium blog steps and use Flavio
# $ git clone https://github.com/pod32g/MD5.git
# cd MD5
#emcc -o md5.html md5.c
```

From the Flavio blog:

Create test.c:
```
#include <stdio.h>

int main(int argc, char ** argv) {
  printf("Hello World\n");
}```


Compile:
```
emcc test.c -s WASM=1 -o test.html
```

Run (I think this may be from the docker container.  Flavio blog talks about run npm http_server):
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



