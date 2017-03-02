# README #

Tool to record a sequence of PNG RGB-D images into a ```klg``` format log file

### Hardware setup

Tested with ```Ubuntu 14.04``` and ```OpenCV 2.4.13```.

### Compilation

```
#!shell

mkdir build
cd build
cmake ../src
make
```

### Usage

```
./png2klg <in:base_dir> <in:associations.txt> <out:klg_file> <width> <height>
```

### Reference
* **[ElasticFusion](https://github.com/mp3guy/ElasticFusion)**
* **[Logger2](https://github.com/mp3guy/Logger2)**
