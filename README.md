# BENCHMARK

A bunch of benchmarks for GCed languages.

## Recent results 

### 1000 iterations, 1 loop
```
AVERAGE TIMES (sorted fastest first):
1. Java (JVM) (time: 27.954951s)
2. F# (time: 30.247424s)
3. C# (time: 34.137420s)
4. D (time: 38.494007s)
5. Go (time: 44.312376s)
6. OCaml (time: 46.043639s)
Benchmarks finished with 1 loops and with 1000 iterations each
```

### 100 times, 100 iterations
```
AVERAGE TIMES (sorted fastest first):
1. Java (JVM) (time: 3.249122s)
2. F# (time: 3.412731s)
3. C# (time: 3.483818s)
4. D (time: 3.695640s)
5. Go (time: 4.302772s)
6. OCaml (time: 5.085996s)
Benchmarks finished with 100 loops and with 100 iterations each
```

## Running

Requires a C compiler (I use `gcc`) to run the 'script' that executes all benchmarks.

Besides a C compiler, the following tools are required for the different benchmarks. If one is missing, the relevant benchmark will be skipped.

**D**
- `dmd`

**Go**
- `go`

**C#/F#**
- `dotnet` (10.0)

**OCaml**
- `opam`
- `dune` (`opam install dune`)

**Java**
- `java`
- optionally: `graalvm` (downloaded somewhere and the `native-image` binary passed in with the `--java-compiler` flag)

The run script takes in the following parameters:
```
ITERATIONS      How many internal iterations each benchmark will do. 
                The larger the number the more stress on the garbage 
                collector, and the longer each benchmark will run over.

LOOPS           The number of times all benchmarks will be run
```

If you are using a Posix shell (Bash, Dash, Zsh, etc) then just do the following
```zsh
# make script executable
chmod +x ./runbench.c

# run
./runbench.c --loops [LOOPS] --iterations [ITERATIONS]
```

If you aren't using a Posix compliant shell (Pwsh, Fish, NuShell) you can either do:
```
bash ./runbench.c --loops [LOOPS] --iterations [ITERATIONS]
```

Or the following:

```sh
# gcc is not necessary, just for example
# compile
gcc ./runbench.c ./scripts/utils.c ./scripts/args.c \
    ./scripts/benchmarks.c ./scripts/sort.c -o ./runbench

# make executable
chmod +x ./runbench

# run
./runbench --loops [LOOPS] --iterations [ITERATIONS]
```
