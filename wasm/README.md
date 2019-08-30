

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


## My Steps

I tried to capture the set of steps I have used.

#### change to wasm directory
```
cd ~/Projects/gatsby/any-counter/wasm
```

#### creates docker container named anyWasm:
```
docker run -dt --name anyWasm \
    --volume ~/Projects/gatsby/any-counter/wasm:/src \
    -p 23456:80 \
    robertaboukhalil/emsdk:1.38.30
```

#### start anyWasm container
```
docker exec -it anyWasm bash
```

#### from within container
```
emcc any_base_counter.c -s WASM=1 -o any_base_counter.html -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']"
```

#### this should output:
```
root@7ab7d94feb22:/src# emcc any_base_counter.c -s WASM=1 -o any_base_counter.html -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']"
shared:INFO: (Emscripten: Running sanity checks)
cache:INFO: generating system library: libdlmalloc.bc... (this will be cached in "/root/.emscripten_cache/asmjs/libdlmalloc.bc" for subsequent builds)
cache:INFO:  - ok
cache:INFO: generating system library: libpthreads_stub.bc... (this will be cached in "/root/.emscripten_cache/asmjs/libpthreads_stub.bc" for subsequent builds)
cache:INFO:  - ok
cache:INFO: generating system library: libc.bc... (this will be cached in "/root/.emscripten_cache/asmjs/libc.bc" for subsequent builds)
cache:INFO:  - ok
cache:INFO: generating system library: libcompiler_rt.a... (this will be cached in "/root/.emscripten_cache/asmjs/libcompiler_rt.a" for subsequent builds)
cache:INFO:  - ok
cache:INFO: generating system library: libc-wasm.bc... (this will be cached in "/root/.emscripten_cache/asmjs/libc-wasm.bc" for subsequent builds)
cache:INFO:  - ok
cache:INFO: generating system asset: generated_struct_info.json... (this will be cached in "/root/.emscripten_cache/asmjs/generated_struct_info.json" for subsequent builds)
cache:INFO:  - ok
```

#### this should produce:
```
root@7ab7d94feb22:/src# ls -1
README.md
any_base_counter.c
any_base_counter.html
any_base_counter.js
any_base_counter.wasm
emcc_help.txt
```

#### from another terminal in ~/Projects/gatsby/any-counter/wasm:
```
open http://localhost:23456/any_base_counter.html 

To open the developer console window on Chrome, use the keyboard shortcut CmdShiftJ (on Windows) or CmdOptionJ (on Mac).
```


#### An example of exercising the engine code from console

```
// code snipets for emscripten console window
// Set mins
Module.ccall('wa_engine_min', null, ['number','number','number','number','number','number','number','number','number','number'], [0,0,0,0,0,0,0,0,0,0])
// Set maxs
Module.ccall('wa_engine_max', null, ['number','number','number','number','number','number','number','number','number','number'], [0,0,0,0,0,0,9,9,9,9])
// Reset
Module.ccall('wa_engine_reset', null, null, null)
// Next
Module.ccall('wa_engine_next', null, null, null)
// Get
Module.ccall('wa_engine_get', null, null, null)
// Get
Module.ccall('wa_engine_dump', null, null, null)
// Run 10 cycles
Module.ccall('wa_engine_run', null, ['number'], [10])

// Sequence
Module.ccall('wa_engine_min', null, ['number','number','number','number','number','number','number','number','number','number'], [0,0,0,0,0,0,0,0,0,0])
Module.ccall('wa_engine_max', null, ['number','number','number','number','number','number','number','number','number','number'], [0,0,0,0,0,0,9,9,9,9])
Module.ccall('wa_engine_reset', null, null, null)
Module.ccall('wa_engine_dump', null, null, null)
Module.ccall('wa_engine_run', null, ['number'], [10])
Module.ccall('wa_engine_dump', null, null, null)
Module.ccall('wa_engine_run', null, ['number'], [10])
Module.ccall('wa_engine_dump', null, null, null)
Module.ccall('wa_engine_run', null, ['number'], [10])
Module.ccall('wa_engine_dump', null, null, null)
```

Then you can run the following to see the engine work:
```
Module.ccall('wa_engine_run', null, ['number'], [10])
Module.ccall('wa_engine_dump', null, null, null)
```

### Integrating with React/Gatsby
- copy any_base_counter.[js|wasm] to static
- import in Helmet (in seo.js)

```
...
    >
      <script src={withPrefix('any_base_counter.js')} />
    </Helmet> 
```


### Alternate WebAssembly Techniques

One thing I am finding with WebAssembly, is there are multiple ways to build
and integrate .wasm files.

This [link](https://brightinventions.pl/blog/using-wasm-with-react/) describes integrating 
wasm files with React, primarily for generating fractals.  The author uses an [online](https://mbebenita.github.io/WasmExplorer/) tool to generate .wasm files.  The downside is they
manually access the mangled compiled name for a function.

### Links for All Resources

- From [medium blog](https://medium.com/@robaboukhalil/hit-the-ground-running-with-webassembly-56cf9b2fa35d)
- From [flaviocopes webassembly blog](https://flaviocopes.com/webassembly/)

- [WebAssembly Standard](http://webassembly.github.io/spec/js-api/index.html)
- [WebAssembly](https://becominghuman.ai/passing-and-returning-webassembly-array-parameters-a0f572c65d97)
- [Explanation of cwrap and ccall](https://medium.com/@eliamaino/calling-c-functions-from-javascript-with-emscripten-first-part-e99fb6eedb22)

- [Wasm with React](https://brightinventions.pl/blog/using-wasm-with-react/)
- [Online WASM compiler](https://mbebenita.github.io/WasmExplorer/)
- [Name Mangling](https://en.wikipedia.org/wiki/Name_mangling)

- [Using AssemblyScipt](https://blog.scottlogic.com/2019/06/14/add-webassembly-to-react-app.html)


Also, for reference, the output from the emcc compiler (that was run from within a docker
image) is in the file emcc_help.txt
