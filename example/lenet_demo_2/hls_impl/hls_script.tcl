## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 2015 Xilinx Inc. All rights reserved.
############################################################
open_project hls_proj_temp

set_top inference_net

#add lib headers
add_files ../../../fpga_cnn/activation_functions.h
add_files ../../../fpga_cnn/conv_pool_layer.h
add_files ../../../fpga_cnn/pool_layer_one_dim.h
add_files ../config.h
add_files ../construct_net.h
add_files ../../../fpga_cnn/conv_layer_one_dim.h
add_files ../../../fpga_cnn/data_type.h
add_files ../../../fpga_cnn/fc_layer_one_dim.h
add_files ../../../fpga_cnn/lrn_layer.h
add_files ../../../fpga_cnn/dropout_layer.h

#add testbench files
add_files -tb ../../../fpga_cnn/accuracy.h
add_files -tb ../../../fpga_cnn/data_quantize.h
add_files -tb ../../../fpga_cnn/weight_bias.h

add_files -tb ../../../stb_image/stb_image.h
add_files -tb ../../../stb_image/stb_image_resize.h
add_files -tb ../../../stb_image/stb_image_write.h

add_files -tb ../../../fpga_cnn/read_mnist.h
add_files -tb ../../../fpga_cnn/softmax_one_dim.h
add_files -tb ../../../fpga_cnn/predict_one_dim.h


add_files ../ff_test.cpp

#add input files
add_files -tb ../input_3.txt
add_files -tb ../net_weights.txt
add_files -tb ../data/t10k-images-idx3-ubyte
add_files -tb ../data/t10k-labels-idx1-ubyte

#add compilation pragmas
add_files -cflags "-I/nfs/app/Xilinx/Vivado_HLS/2015.4/include -std=c++0x -fpermissive -pedantic -Wall -Wextra" -tb ../ff_test.cpp

#hls project settings
open_solution -reset "fpga_cnn"
#vc709
set_part {xc7vx690tffg1761-2}
#zynq-7000 board
#set_part {xc7z045ffg900-1} 
# zedboard
#set_part {xc7z020clg484-1}  
create_clock -period 10 -name default

csim_design -clean -argv {*.txt}

csynth_design

#cosim_design -argv {*.txt} -trace_level none -rtl verilog -tool xsim
