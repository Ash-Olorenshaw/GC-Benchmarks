# BENCHMARK

A bunch of benchmarks for GCed languages.

Last results:
```
D bench time: 72.348683s
Go bench time: 79.035944s
C# bench time: 61.291214s
F# bench time: 60.237122s
OCaml bench time: 92.400584s
Benchmarks finished with '1000' iterations each
```

Second-last run results:
```
D bench time: 66.280061s
Go bench time: 66.965441s
C# bench time: 59.955868s
F# bench time: 58.816417s
OCaml bench time: 84.978751s
Benchmarks finished with '1000' iterations each
```

## Running

If you are using a Posix shell (Bash, Dash, Zsh, etc) then just do the following
```bash
# make script executable
chmod +x ./runbench.c

# run
./runbench.c
```

If you aren't using a Posix compliant shell (Pwsh, Fish, NuShell) you can either do:
```
bash ./runbench.c
```

Or the following:

```bash
# gcc is not necessary, just for example
# compile
gcc ./runbench.c ./scripts/utils.c -o ./runbench

# make executable
chmod +x ./runbench

# run
./runbench
```
