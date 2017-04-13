
#ifndef _RESIZE_IMAGE_H_
#define _RESIZE_IMAGE_H_

#include <iostream>
template<int channels, int width, int height, int size>
void resize_image(data_type(&x)[channels][height][width], int h, int w, data_type(&y)[channels][size][size]) {
	//(1)计算源图像与目标图像宽与高的比例
	//	w0 : 表示源图像的宽度
	//	h0 : 表示源图像的高度
	//	w1 : 表示目标图像的宽度
	//	h1 : 表示目标图像的高度
	int w0 = w;
	int h0 = h;
	int w1 = size;
	int h1 = size;
	data_type fw = data_type(w0) / (w1);
	data_type fh = data_type(h0) / (h1);

	data_type image_max = 0.0;
	data_type image_min = x[0][0][0];
	for (int i = 0; i < channels; i++) {
		for (int j = 0; j < height; j++) {
			for (int k = 0; k < width; k++) {
				if (x[i][j][k]>image_max)
					image_max = x[i][j][k];
				if (x[i][j][k]<image_min)
					image_min = x[i][j][k];
			}
		}
	}
	
	for (int i = 0; i < channels; i++) {
		for (int j = 0; j < height; j++) {
			for (int k = 0; k < width; k++) {
				x[i][j][k] = (x[i][j][k] - image_min) / (image_max - image_min);
			}
		}
	}

	for (int i = 0; i < channels; i++) {
		for (int j = 0; j < h1; j++) {
			for (int k = 0; k < w1; k++) {
				//(2)针对目标图像的一个点(x, y), 计算在源图像中的对应坐标，结果为浮点数。
				data_type x0 = ((data_type)(k + 0.5)) * fw - (data_type)0.5;
				data_type y0 = ((data_type)(j + 0.5)) * fh - (data_type)0.5;

				int x1 = int(x0);
				int x2 = x1 + 1;
				int y1 = int(y0);
				int y2 = y1 + 1;
				//(3)求周围四个点所占的权重比值
				data_type fx1 = x0 - x1;
				data_type fx2 = (data_type)1.0f - fx1;
				data_type fy1 = y0 - y1;
				data_type fy2 = (data_type)1.0f - fy1;

				data_type s1 = fx1*fy1;
				data_type s2 = fx2*fy1;
				data_type s3 = fx2*fy2;
				data_type s4 = fx1*fy2;

				y[i][j][k] = (x[i][y2][x2] * s1 + x[i][y2][x1] * s2 + x[i][y1][x1] * s3 + x[i][y1][x2] * s4) 
					* (image_max - image_min) + image_min;
			}
		}
	}

	
	cout << "image_max: " << image_max << endl;
	cout << "image_min: " << image_min << endl;

	//ofstream in_data_crop;
	//in_data_crop.open("in_data_crop.txt", ios::app);
	//for (int i = 0; i < channels; i++) {
	//	//in_data_crop << "channel " << i << "****************" << endl;
	//	for (int j = 0; j < size; j++) {
	//		//in_data_crop << "row " << j << "****************" << endl;
	//		for (int k = 0; k < size; k++) {
	//			in_data_crop << y[i][j][k] << " ";
	//		}
	//		in_data_crop << endl;
	//	}
	//	in_data_crop << endl;
	//}
	//in_data_crop.close();
}

#endif 
