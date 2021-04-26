cpp implementation of "Image Quilting for Texture Synthesis and Transfer"
## results
<figure>
  <img src="./imgs/test_000.png" width=180/>
  <img src="./outputs/texture_000.png" width=360/>
  <br> <br>
  <img src="./imgs/test_001.png" width=180/>
  <img src="./outputs/texture_001.png" width=360/>
  <br> <br>
  <img src="./imgs/test_006.png" width=180/>
  <img src="./outputs/texture_006.png" width=360/>
  <br> <br>
  <img src="./imgs/test_007.png" width=180/>
  <img src="./outputs/texture_007.png" width=360/>
  <br> <br>
  <img src="./imgs/test_008.png" width=180/>
  <img src="./outputs/texture_008.png" width=360/>
  <br> <br>
  <img src="./imgs/test_010.jpg" width=180/>
  <img src="./outputs/texture_010.jpg" width=360/>
  <br> <br>
  <img src="./imgs/test_011.jpg" width=180/>
  <img src="./outputs/texture_011.jpg" width=360/>
  <br> <br>
  <img src="./imgs/test_012.jpg" width=180/>
  <img src="./outputs/texture_012.jpg" width=360/>
  <br> <br>
  <img src="./imgs/test_013.png" width=180/>
  <img src="./outputs/texture_013.png" width=360/>
  <br> <br>
  <img src="./imgs/test_014.png" width=180/>
  <img src="./outputs/texture_014.png" width=360/>
  <br> <br>
  <img src="./imgs/test_015.png" width=180/>
  <img src="./outputs/texture_015.png" width=360/>
  <br> <br>
  <img src="./imgs/test_016.png" width=180/>
  <img src="./outputs/texture_016.png" width=360/>
  <br> <br>
  <img src="./imgs/test_017.jpg" width=180/>
  <img src="./outputs/texture_017.jpg" width=360/>
  <br> <br>
  <img src="./imgs/test_018.jpg" width=180/>
  <img src="./outputs/texture_018.jpg" width=360/>
  <br> <br>
  <img src="./imgs/test_019.jpg" width=180/>
  <img src="./outputs/texture_019.jpg" width=360/>
  <br> <br>
  <img src="./imgs/test_020.png" width=180/>
  <img src="./outputs/texture_020.png" width=360/>
  <br> <br>
  <img src="./imgs/test_021.png" width=180/>
  <img src="./outputs/texture_021.png" width=360/>
  <br> <br>
  <img src="./imgs/test_022.png" width=180/>
  <img src="./outputs/texture_022.png" width=360/>
  <br> <br>
  <img src="./imgs/test_023.png" width=180/>
  <img src="./outputs/texture_023.png" width=360/>
  <br> <br>
  <img src="./imgs/test_024.jpg" width=180/>
  <img src="./outputs/texture_024.jpg" width=360/>
  <br> <br>
  <img src="./imgs/test_025.jpg" width=180/>
  <img src="./outputs/texture_025.jpg" width=360/>
  <br> <br>
  <img src="./imgs/test_026.png" width=180/>
  <img src="./outputs/texture_026.png" width=360/>
  <br> <br>
  <img src="./imgs/test_027.png" width=180/>
  <img src="./outputs/texture_027.png" width=360/>
  <br> <br>
  <img src="./imgs/test_028.png" width=180/>
  <img src="./outputs/texture_028.png" width=360/>
  <br> <br>
  <img src="./imgs/test_029.png" width=180/>
  <img src="./outputs/texture_029.png" width=360/>
  <br> <br>
  <img src="./imgs/test_030.png" width=180/>
  <img src="./outputs/texture_030.png" width=360/>
  <br> <br>
  <img src="./imgs/test_031.png" width=180/>
  <img src="./outputs/texture_031.png" width=360/>
  <br> <br>
  <img src="./imgs/test_032.jpg" width=180/>
  <img src="./outputs/texture_032.jpg" width=360/>
  <br> <br>
  <img src="./imgs/test_033.jpg" width=180/>
  <img src="./outputs/texture_033.jpg" width=360/>
  <br> <br>
  <img src="./imgs/test_034.png" width=180/>
  <img src="./outputs/texture_034.png" width=360/>
  <br> <br>
  <img src="./imgs/test_035.jpg" width=180/>
  <img src="./outputs/texture_035.jpg" width=360/>
  <br> <br>
  <img src="./imgs/test_036.jpg" width=180/>
  <img src="./outputs/texture_036.jpg" width=360/>
  <br> <br>
  <img src="./imgs/test_038.jpg" width=180/>
  <img src="./outputs/texture_038.jpg" width=360/>
  <br> <br>
</figure>

## requirement
```shell
opencv3
gtest
gflags
```
make sure your `opencv binary file` is in your bash_profile, my opencv lib files are in path: `/usr/local/lib` and my opencv binary files are in path:`/usr/local/bin`

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

## running time
I simply takes one example to test running time.

| input size   | block size | scale | time |
| ---------- | -------- | ------------ | -------- |
| 396x658        | 32  | 2.0         | 22s |

## concat information
Welcome to email sukamayluo@sjtu.edu.cn if you have any questions about it.