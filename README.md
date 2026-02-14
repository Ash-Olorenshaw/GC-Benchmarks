# BENCHMARK

A bunch of benchmarks for GCed languages.

## Recent results 

### 1000 iteraions, 1 loop
```
D bench time: 72.348683s
Go bench time: 79.035944s
C# bench time: 61.291214s
F# bench time: 60.237122s
OCaml bench time: 92.400584s
```

### 100 times, 100 iterations
```
AVERAGE TIMES:
D bench time: 4.820940s
Go bench time: 5.404200s
C# bench time: 4.493315s
F# bench time: 4.373591s
OCaml bench time: 6.446346s
```

## Running

Requires a C compiler (I am using `gcc`) to run the 'script' that executes all benchmarks.

Besides a C compiler, the following tools are required for the different benchmarks. If one is missing, the relevant benchmark will be skipped.

**D**:
- `dmd`

**Go**:
- `go`

**C#/F#**:
- `dotnet` (10.0)

**OCaml**:
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
