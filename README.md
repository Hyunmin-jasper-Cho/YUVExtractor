# YUV2PNG 
This is the part of the project "YUV Video Super Resolution Scheme".   
YUV2PNG is one of the preprocessing processes of the [ Video super resolution](https://github.com/Hyunmin-jasper-Cho/VideoSuperResolution) project,  
and it makes each .yuv sequence into .png format, which consists of y000.png, u000.png and v000.png. 

## Environment 
To run this project, you should complete build setting. 
> OS: Linux Ubuntu 20.04 LTS  
> Require libraries: opencv & opencv_contrib   
> build base: gnu++20  

## Preparation & Installation
> 1. After downloading the cloned zip file, save it in the local folder.
> 2. After moving into cloned Directory, put the .yuv file in YUVs/`your_yuv_name`/`your_yuv_name.yuv`.  
> 3. Compile

## Compile
> 1. Goto YUV2PNG directory, and make directory 'build'  
> `cd YUV2PNG` and `mkdir build`
> 2. move into build directory, then compile using cmake.  
> `cd build` and `cmake ..`
> 3. In build directory, run makefile to create executable file 
> `make`

## Run & Test
- When executed, the corresponding .png file is automatically created in PNGs/`your_yuv_name`/ directory.  

**Warning**
> In current version, it only support YUV4:2:0 8bit(I420) only.  

> 1. After finish compilation, in build directory, execute YUV2PNG executable file with bit information.  
> You can run `./YUV2PNG ../YUVs ../PNGs **width** **height** **bit** `. 
