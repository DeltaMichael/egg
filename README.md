# EGG

The Linux shell that's super easy to break ;(

## Build and run

```
make
./egg
```

## Built-in commands

```
cd - change directory
dir - list contents of current dir
env - show env variables
exit - quit the shell
eggzit - like exit, but cooler
cls - clear screen
```
## Unimplemented features

- more built-in commands, e.g. rm, mkdir
- flag parsing
- history
- colors
- run executables
- pipes
- keep track of environment and related features, e.g. run executables on path, command prompt customization
- fully-fledged scripting, i.e. run a script file with something like `egg ./myscript.gg`
- tests, bloody tests
- run profiler and find all the memory leaks I missed
