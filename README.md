# Compiler
A scanner and a sdter(Syntax-directed translation) written in Cpp.

## Quick start

### clone the repo
```
git clone link
```

### install gflags
This is the repo: https://github.com/gflags/gflags

### run the exe file
```
cd compiler
./cmake-build-debug/main --input_path [input_file_path] --output_path [ouput_file_path] --log_state [0/1/2]
```
or you could compile it by yourself
```
cd compiler
mkdir build && cd build
cmake .. && make
./main
```
