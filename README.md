# Compiler Front End
A scanner and a sdter(Syntax-directed translation) written in Cpp.

## Quick start

### clone the repo
```
git clone link
```

### run the exe file
```
cd compiler
./cmake-build-debug/main --input_path [input_file_path] --output_path [ouput_file_path] --log_state [0/1/2]
```
or you could compile it by yourself.
First, you need install gflags. This is the repo: https://github.com/gflags/gflags
```
cd compiler
mkdir build && cd build
cmake .. && make
./main
```
