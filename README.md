# BENCHMARK

A bunch of benchmarks for GCed languages.

## Recent results 

### 1000 iterations, 1 loop
```
AVERAGE TIMES (sorted fastest first):
1. C# (time: 30.035224s)
2. F# (time: 30.533563s)
3. D (time: 33.514821s)
4. Go (time: 39.168059s)
5. OCaml (time: 46.430299s)
Benchmarks finished with 1 loops and with 1000 iterations each
```

### 100 times, 100 iterations
```
AVERAGE TIMES (sorted fastest first):
1. C# (time: 3.686345s)
2. F# (time: 3.783578s)
3. D (time: 4.105567s)
4. Go (time: 4.760802s)
5. OCaml (time: 5.554541s)
Benchmarks finished with 100 loops and with 100 iterations each
```

## Running

Requires a C compiler (I am using `gcc`) to run the 'script' that executes all benchmarks.

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
gcc ./runbench.c ./scripts/utils.c -o ./runbench

# make executable
chmod +x ./runbench

# run
./runbench --loops [LOOPS] --iterations [ITERATIONS]
```
