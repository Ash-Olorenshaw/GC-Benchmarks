# BENCHMARK

A bunch of benchmarks for GCed languages.

## Running

If you are using a Posix shell (Bash, Dash, Zsh, etc) then just do the following
```bash
# make script executable
chmod +x ./runbench.c

# run
./runbench.c
```

If you aren't using a Posix compliant shell (Pwsh, Fish, NuShell) then you need to do the following:

```bash
# gcc is not necessary, just for example
# compile
gcc ./runbench.c ./scripts/utils.c -o ./runbench

# make executable
chmod +x ./runbench

# run
./runbench
```
