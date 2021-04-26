cpp implementation of "Image Quilting for Texture Synthesis and Transfer"
## requirement
```shell
opencv3
gtest
gflags
```
## compile
```shell
mkdir build
cd build
cmake ..
make
```

## usage
```shell
./src/ImageQuilting -input [img path] -output [img path]
```
if you want to know more usage
```shell
image_quilting git:(master) ✗ ./build/src/ImageQuilting -help                                                                                          
ImageQuilting: Usage : ./ImageQuilting 

  Flags from xxx/image_quilting/src/main.cpp:
    -block_size (block size) type: int32 default: 20
    -input (input image path) type: string default: ""
    -output (output image path) type: string default: ""
    -overlap (overlap ratio of block_size) type: double
      default: 0.16666666666666666
    -scale (scaling w.r.t. to image size) type: double default: 2
    -tolerance (error tolerance) type: double default: 0.10000000000000001
```