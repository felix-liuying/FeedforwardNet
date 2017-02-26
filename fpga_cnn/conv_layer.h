//
// Created by yaochen on 29/12/16.
//TODO-1: modify conv function to compute input arrays' dimensions that are not equal
//TODO-2: add conv function with pooling (conv fusion pool)

#ifndef _CONV_LAYER_H_
#define _CONV_LAYER_H_

#include <iostream>
#include <fstream>
#include <algorithm>
#include "data_type.h"
#include "activation_functions.h"
#include "config.h"

using namespace std;

extern const bool tbl[6][16];

template <typename T, int _INPUT_SIZE_, int _CONV_KERNEL_SIZE_, int _CONV_PADDING_, int _CONV_STRIDE_, int _POOL_KERNEL_SIZE_, int _POOL_PADDING_, int _POOL_STRIDE_, int _IN_CHANNEL_NUM_, int _OUT_CHANNEL_NUM_>
class conv_pool_layer {
//template <typename T, int _INPUT_SIZE_, int _CONV_KERNEL_SIZE_, int _CONV_PADDING_, int _CONV_STRIDE_, int _IN_CHANNEL_NUM_, int _OUT_CHANNEL_NUM_>
//class conv_layer {

private:
    int conv_layer_number;
    int out_data_size;
public:
	conv_pool_layer():conv_layer_number(0){ out_data_size = (((_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1) + _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1;};
//	conv_layer() :conv_layer_number(0) { out_data_size = (_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1; };

    /************************************************************************************************/
    void conv_kernel_a(
            T in_data[_INPUT_SIZE_ + _CONV_PADDING_ * 2][_INPUT_SIZE_ + _CONV_PADDING_ * 2],
            T kernel_weights[_CONV_KERNEL_SIZE_][_CONV_KERNEL_SIZE_],
            T out_data[(_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1]
		              [(_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1]) {

        T in_data_par[_INPUT_SIZE_ + _CONV_PADDING_ * 2][_INPUT_SIZE_ + _CONV_PADDING_ * 2] = {0};
        T kernel_weights_par[_CONV_KERNEL_SIZE_][_CONV_KERNEL_SIZE_] = {0};

        for (uint i = 0; i < _INPUT_SIZE_ + _CONV_PADDING_ * 2; i++) {
            for (uint j =0; j < _INPUT_SIZE_ + _CONV_PADDING_ * 2; j++) {
                in_data_par[i][j] = in_data[i][j];
            }
         }
        for (uint i = 0; i < _CONV_KERNEL_SIZE_; i++) {
            for (uint j = 0; j < _CONV_KERNEL_SIZE_; j++) {
                kernel_weights_par[i][j] = kernel_weights[i][j];
            }
        }

        for (int i = _CONV_KERNEL_SIZE_ / 2; i < _INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_ / 2; ++i) {
#ifdef _HLS_MODE_
#pragma HLS unroll factor=2
#endif
            for (int j = _CONV_KERNEL_SIZE_ / 2; j < _INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_ / 2; ++j) {
#ifdef _HLS_MODE_
#pragma HLS unroll factor=2
#endif
                for (int ii = -_CONV_KERNEL_SIZE_ / 2; ii <= _CONV_KERNEL_SIZE_ / 2; ++ii) {
#ifdef _HLS_MODE_
#pragma HLS unroll factor=4
#endif
                    for (int jj = -_CONV_KERNEL_SIZE_ / 2; jj <= _CONV_KERNEL_SIZE_ / 2; ++jj) {
#ifdef _HLS_MODE_
#pragma HLS pipeline
#endif
                        T data = in_data_par[i + ii][j + jj];
                        T weight = kernel_weights_par[ii + _CONV_KERNEL_SIZE_ / 2][jj + _CONV_KERNEL_SIZE_ / 2];
                        out_data[i - _CONV_KERNEL_SIZE_ / 2 ][j - _CONV_KERNEL_SIZE_ / 2] += data * weight;
                    }
                }
            }
        }

        #if _C_DEBUG_MODE_
        int conv_layer_count = 0;
//        cout << "array conv kernel output ...." << endl;
        ofstream conv_kernel_a;
        conv_kernel_a.open("conv_kernel_a.txt", ios::app);
        for (int j = 0; j < _INPUT_SIZE_ + _CONV_PADDING_ * 2; j++) {
            for (int k = 0; k < _INPUT_SIZE_ + _CONV_PADDING_ * 2; k++) {
                conv_kernel_a << in_data[j][k] << " "; // i?
            }
            conv_kernel_a << endl;
        }
        conv_kernel_a << endl;
        for (int j = 0; j < _CONV_KERNEL_SIZE_; j++) {
            for (int k = 0; k < _CONV_KERNEL_SIZE_; k++) {
                conv_kernel_a << kernel_weights[j][k] << " "; // i?
            }
            conv_kernel_a << endl;
        }
        conv_kernel_a << endl;
        for (int j = 0; j < _INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_ +1 ; j++) {
            for (int k = 0; k < _INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_ +1; k++) {
                conv_kernel_a << out_data[j][k] << " "; //
            }
            conv_kernel_a << endl;
        }
        conv_kernel_a << endl;
        conv_kernel_a.close();
//        cout << endl;
        #endif
    }

    /************************************************************************************************/
    //3D array to 3D array convolution layer with connection table
//    void conv_layer_a(
//            char activation_type,
//            T in_data3D[_IN_CHANNEL_NUM_][_INPUT_SIZE_][_INPUT_SIZE_],
//            T kernel_weights[_IN_CHANNEL_NUM_ * _OUT_CHANNEL_NUM_][_KERNEL_SIZE_][_KERNEL_SIZE_],
//            T kernel_bias[_OUT_CHANNEL_NUM_],
//            T out_data3D[_OUT_CHANNEL_NUM_][_INPUT_SIZE_ - _KERNEL_SIZE_ + 1][_INPUT_SIZE_ - _KERNEL_SIZE_ + 1] ) {
//
//        cout << "Processing convolution layer ...." << endl;
//
//        for (int b = 0; b < _OUT_CHANNEL_NUM_; b++) {//output kernel loop
//            for (int a = 0; a < _IN_CHANNEL_NUM_; a++) {//input kernel loop
//                T out_data2D[_INPUT_SIZE_ - _KERNEL_SIZE_ + 1][_INPUT_SIZE_ - _KERNEL_SIZE_ + 1] = {0};
//                conv_kernel_a(in_data3D[a],
//                              kernel_weights[b * _IN_CHANNEL_NUM_ + a],
//                              out_data2D);
//
//                for (int i = 0; i < _INPUT_SIZE_ - _KERNEL_SIZE_ + 1; i++){
//                    for (int j = 0; j < _INPUT_SIZE_ - _KERNEL_SIZE_ + 1; j++){
//                        out_data3D[b][i][j] += out_data2D[i][j];
//                    }
//                }
//            }
//            for(int j = 0; j < _INPUT_SIZE_ - _KERNEL_SIZE_ + 1; j++){
//                for (int k = 0; k < _INPUT_SIZE_ - _KERNEL_SIZE_ + 1; k++){
//                    out_data3D[b][j][k] = f(activation_type, (out_data3D[b][j][k] + kernel_bias[b]));
//					//out_data3D[b][j][k] = out_data3D[b][j][k] + kernel_bias[b];
//                }
//            }
//        }
//        cout << "Finished convolution layer ...." << endl;
//
//             //debugging output
//        #if _C_DEBUG_MODE_
//        cout << "finished convolution ...." << endl;
//        ofstream out_conv_a;
//        out_conv_a.open("conv_layer_a.txt", ios::app);
//        out_conv_a << "input 3D arry to conv layer ...................." << endl;
//        for (int i = 0; i < _IN_CHANNEL_NUM_; i++) {
//            for (int j = 0; j < _INPUT_SIZE_; j++) {
//                for (int k = 0; k < _INPUT_SIZE_; k++) {
//                    out_conv_a << in_data3D[i][j][k] << " ";
//                }
//                out_conv_a << endl;
//            }
//            out_conv_a << endl;
//        }
//        out_conv_a << endl;
//
//        out_conv_a << "input bias to conv layer ........................" << endl;
//        for (int i = 0; i < _IN_CHANNEL_NUM_; i++) {
//            out_conv_a << kernel_bias[i] << "  ";
//        }
//        out_conv_a << endl;
//
//        out_conv_a << "output from conv layer .........................." << endl;
//        for (int i = 0; i < _OUT_CHANNEL_NUM_; i++) {
//            for (int j = 0; j < _INPUT_SIZE_ - _KERNEL_SIZE_ + 1; j++) {
//                for (int k = 0; k < _INPUT_SIZE_ - _KERNEL_SIZE_ + 1; k++) {
//                    out_conv_a << out_data3D[i][j][k] << " ";
//                }
//                out_conv_a << endl;
//            }
//            out_conv_a << endl;
//        }
//        out_conv_a.close();
//        cout << endl;
//        #endif
//
//    }
//
//    /************************************************************************************************/
//    // convolution layer with array input and connection table
//    void conv_layer_a_table(
//            char activation_type,
//            bool has_connection_table,
//            T in_data3D[][_INPUT_SIZE_][_INPUT_SIZE_],  //in_data[6][
//            T kernel_weights[][_KERNEL_SIZE_][_KERNEL_SIZE_],
//            T kernel_bias[],
//            T out_data3D[][_INPUT_SIZE_ - _KERNEL_SIZE_ + 1][_INPUT_SIZE_ - _KERNEL_SIZE_ + 1] ) {
//
//        cout << "starting convolution ...." << endl;
//
//        for (int b = 0; b < _OUT_CHANNEL_NUM_; b++) {//output kernel loop
//            for (int a = 0; a < _IN_CHANNEL_NUM_; a++) {//input kernel loop
//                if (tbl[a][b]) {
//                    T out_data2D[_INPUT_SIZE_ - _KERNEL_SIZE_ + 1][_INPUT_SIZE_ - _KERNEL_SIZE_ + 1] = {0};
//                    conv_kernel_a(in_data3D[a],
//                                  kernel_weights[b * _IN_CHANNEL_NUM_ + a],
//                                  out_data2D);
//
//                    for (int i = 0; i < _INPUT_SIZE_ - _KERNEL_SIZE_ + 1; i++){
//                        for (int j = 0; j < _INPUT_SIZE_ - _KERNEL_SIZE_ + 1; j++){
//                            out_data3D[b][i][j] += out_data2D[i][j];
//                        }
//                    }
//                }
//                else {;}
//            }
//            for(int j = 0; j < _INPUT_SIZE_ - _KERNEL_SIZE_ + 1; j++){
//                for (int k = 0; k < _INPUT_SIZE_ - _KERNEL_SIZE_ + 1; k++){
//                    out_data3D[b][j][k] = f(activation_type, (out_data3D[b][j][k] + kernel_bias[b]));
//                }
//            }
//        }
//
//        //debugging output
//#if _C_DEBUG_MODE_
//        cout << "finished convolution ...." << endl;
//        ofstream out_conv_a;
//        out_conv_a.open("conv_layer_a.txt", ios::app);
//
//        out_conv_a << "input 3D arry to conv layer ..................." << endl;
//        for (int i = 0; i < _IN_CHANNEL_NUM_; i++) {
//            for (int j = 0; j < _INPUT_SIZE_; j++) {
//                for (int k = 0; k < _INPUT_SIZE_; k++) {
//                    out_conv_a << in_data3D[i][j][k] << " ";
//                }
//                out_conv_a << endl;
//            }
//            out_conv_a << endl;
//        }
//        out_conv_a << endl;
//
//        for (int i = 0; i < _OUT_CHANNEL_NUM_; i++) {
//            for (int j = 0; j < _INPUT_SIZE_ - _KERNEL_SIZE_ + 1; j++) {
//                for (int k = 0; k < _INPUT_SIZE_ - _KERNEL_SIZE_ + 1; k++) {
//                    out_conv_a << out_data3D[i][j][k] << " ";
//                }
//                out_conv_a << endl;
//            }
//            out_conv_a << endl;
//        }
//        out_conv_a.close();
//        cout << endl;
//#endif
//
//    }

    /************************************************************************************************/
    //3D array to 3D array convolution layer fuse pooling layer
    void conv_layer_w_pool_a(
            char activation_type,
            T in_data3D[_IN_CHANNEL_NUM_][_INPUT_SIZE_][_INPUT_SIZE_],
            T kernel_weights[_IN_CHANNEL_NUM_ * _OUT_CHANNEL_NUM_][_CONV_KERNEL_SIZE_][_CONV_KERNEL_SIZE_],
            T kernel_bias[_OUT_CHANNEL_NUM_],
            T out_data3D[_OUT_CHANNEL_NUM_]
		                [(((_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1) + _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1]
	                    [(((_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1) + _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1]) {
        T out_data3d_temp[_OUT_CHANNEL_NUM_]
			             [(_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1]
		                 [(_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1] = {0};
		T out_data3d_temp_padding[_OUT_CHANNEL_NUM_]
						         [(_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1 + _POOL_PADDING_ * 2]
						         [(_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1 + _POOL_PADDING_ * 2] = { 0 };
        T out_data_temp[_OUT_CHANNEL_NUM_]
			           [(((_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1) + _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1]
		               [(((_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1) + _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1] = {0};
		//T temp1, temp2;
		T temp = 0;

        cout << "starting convolution and pooling...." << endl;

        for (int b = 0; b < _OUT_CHANNEL_NUM_; b++) {//output kernel loop
            for (int a = 0; a < _IN_CHANNEL_NUM_; a++) {//input kernel loop
				T in_data2D[_INPUT_SIZE_ + _CONV_PADDING_ * 2][_INPUT_SIZE_ + _CONV_PADDING_ * 2] = { 0 };//conv padding
                T out_data2D[(_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1]
					        [(_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1] = {0};
						for (int i = 0; i < _INPUT_SIZE_;i++) {
							for (int j = 0; j < _INPUT_SIZE_; j++) {
								in_data2D[_CONV_PADDING_ + i][_CONV_PADDING_ + j] = in_data3D[a][i][j];
							}
						}
                conv_kernel_a(in_data2D,
                              kernel_weights[b * _IN_CHANNEL_NUM_ + a],
                              out_data2D);

                for (int i = 0; i < (_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1; i++){
                    for (int j = 0; j < (_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1; j++){
                        out_data3d_temp[b][i][j] += out_data2D[i][j];
                    }
                }
            }
            for(int j = 0; j < (_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1; j++) {
                for (int k = 0; k < (_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1; k++) {
                    out_data3d_temp[b][j][k] = f(activation_type, (out_data3d_temp[b][j][k] + kernel_bias[b]));
                }
            }

			for (int i = 0; i < (_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1; i++) {
				for (int j = 0; j < (_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1; j++) {
					out_data3d_temp_padding[b][_POOL_PADDING_ + i][_POOL_PADDING_ + j] = out_data3d_temp[b][i][j];//pool padding
				}
			}
            for(int j = 0; j < (((_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1) + _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1; j++) {
				for (int k = 0; k < (((_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1) + _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1; k++) {
					for (int m = 0; m < _POOL_KERNEL_SIZE_; m++) {
						for (int n = 0; n < _POOL_KERNEL_SIZE_; n++) {
							if (m == 0 && n == 0) {//originally make max equal the first pixel in each window 
								temp = out_data3d_temp_padding[b][_POOL_KERNEL_SIZE_ * j + m][_POOL_KERNEL_SIZE_ * k + n];
							}
							else {
								temp = (out_data3d_temp_padding[b][_POOL_KERNEL_SIZE_ * j + m][_POOL_KERNEL_SIZE_ * k + n] > temp ? out_data3d_temp[b][_POOL_KERNEL_SIZE_ * j + m][_POOL_KERNEL_SIZE_ * k + n] : temp);
							}
						}
					}
					//temp1 = (out_data3d_temp[b][2*j][2*k] > out_data3d_temp[b][2*j][2*k+1]) ? out_data3d_temp[b][2*j][2*k] : out_data3d_temp[b][2*j][2*k+1];
					//temp2 = (out_data3d_temp[b][2*j+1][2*k] > out_data3d_temp[b][2*j+1][2*k+1]) ? out_data3d_temp[b][2*j+1][2*k] : out_data3d_temp[b][2*j+1][2*k+1];
					//out_data_temp[b][j][k] = (temp1 > temp2) ? temp1 : temp2;
					out_data_temp[b][j][k] = temp;
					out_data3D[b][j][k] = f(activation_type, out_data_temp[b][j][k]);
				}
            }
        }

        //debugging output
#if _C_DEBUG_MODE_
        cout << "finished convolution and pooling...." << endl;
        ofstream out_pool_a;
		out_pool_a.open("conv_pool_layer_a.txt", ios::app);
		out_pool_a << "input 3D arry to conv and pooling layer ..................." << endl;
        for (int i = 0; i < _IN_CHANNEL_NUM_; i++) {
            for (int j = 0; j < _INPUT_SIZE_ + _CONV_PADDING_ * 2; j++) {
                for (int k = 0; k < _INPUT_SIZE_ + _CONV_PADDING_ * 2; k++) {
					out_pool_a << in_data3D[i][j][k] << " ";
                }
				out_pool_a << endl;
            }
			out_pool_a << endl;
        }
		out_pool_a << endl;

		out_pool_a << "conv layer out_data3D..................." << endl;
		for (int i = 0; i < _OUT_CHANNEL_NUM_; i++) {
			for (int j = 0; j < (_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1; j++) {
				for (int k = 0; k < (_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1; k++) {
					out_pool_a << out_data3d_temp[i][j][k] << " ";
				}
				out_pool_a << endl;
			}
			out_pool_a << endl;
		}
		out_pool_a << endl;

		out_pool_a << "pooling layer out_data3D..................." << endl;
        for (int i = 0; i < _OUT_CHANNEL_NUM_; i++) {
            for (int j = 0; j < (((_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1) + _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1; j++) {
                for (int k = 0; k < (((_INPUT_SIZE_ + _CONV_PADDING_ * 2 - _CONV_KERNEL_SIZE_) / _CONV_STRIDE_ + 1) + _POOL_PADDING_ * 2 - _POOL_KERNEL_SIZE_) / _POOL_STRIDE_ + 1; k++) {
					out_pool_a << out_data3D[i][j][k] << " ";
                }
				out_pool_a << endl;
            }
			out_pool_a << endl;
        }
		out_pool_a.close();
        cout << endl;
#endif

    }
};

#endif
