# EGG

The super thin Linux shell

## Build and run

```
make
./egg
```

## Built-in commands

```
cd - change directory
exit - quit the shell
eggzit - like exit, but cooler
```
## Unimplemented features

- redirect stdout to files
- history
- colors
- text buffer, e.g. go back and forth when typing, insert text in the middle
- keep track of environment and related features, e.g. run executables on path, command prompt customization
- fully-fledged scripting, i.e. run a script file with something like `egg ./myscript.gg`
- execute the shell interpreter without repl, e.g. running `egg` enters the console, running `egg ls ./mydir` lists everything in mydir using egg
- tests, bloody tests
- run profiler and find all the memory leaks I missed
