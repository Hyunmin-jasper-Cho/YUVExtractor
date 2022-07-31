# YUV2PNG 
YUV2PNG is one of the preprocessing processes of the [ Video super resolution](https://github.com/Hyunmin-jasper-Cho/YU-VSR) project,  
and it makes each .yuv sequence into .png format, which consists of y000.png, u000.png and v000.png.  

## Sample Result
> About sample  
> Viewer: [YUView](https://github.com/IENT/YUView)  
> Sample: [BVI-DVC](https://research-information.bris.ac.uk/en/datasets/bvi-dvc)  
>  
> Original YUV images  
![original](./figures/original.png)  
> Y, U and V component  
![Y](./figures/Y.png)  
![U](./figures/U.png)  
![V](./figures/V.png)  


## Environment 
To run this project, you should complete build setting. 
> OS: Linux Ubuntu 20.04 LTS / Mac OS 12  
> Require libraries: opencv(for c++ build)  
> build base: gnu++20 / python3  

## Preparation & Installation
> Follow each build preparation & installation

## Compile
> Follow each build compilation 
  
  
**Warning**
> In current version, c++ build only supports YUV4:2:0 8bit(I420) only.  
> In python build, you can also handle 10bit 4:2:0 yuv.
