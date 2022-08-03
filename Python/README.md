# YUV2PNG - Python Implementation
This is the part of the project "YUV Video Super Resolution Scheme".   
YUV2PNG is one of the preprocessing processes of the [ Video super resolution](https://github.com/Hyunmin-jasper-Cho/VideoSuperResolution) project,  
and it makes each .yuv sequence into .png/.tiff format, which consists of y000.png, u000.png and v000.png.  

- PNG-> Make an image as grayscale RGB format.  
- TIFF> Make an image as YUV format.  

## Environment 
To run this project, you should complete build setting. 
> OS: Linux Ubuntu 20.04 LTS  
> build base: python3

## Preparation & Installation
> 1. After downloading the cloned zip file, save it in the local folder.
> 2. After moving into cloned directory, put the .yuv file in YUV420_X/`your_yuv_name.yuv`.  
> 3. Run demo.sh file

## Run & Test
- When executed, the corresponding .png/.tiff file is automatically created in `IMG_root`/`your_yuv_name`/ directory.  

**Warning**
> When you execute the .sh file, you should match your yuv's **width and height**.  
> You can run just simply type `sh demo.sh`.  
> More information about execution is in demo.sh file. 
