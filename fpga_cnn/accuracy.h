
#ifndef __ACCURACY_H__
#define __ACCURACY_H__

#include <iostream>

template<int num, int size, int label_size>
void accuracy(float(&x)[num][label_size][size][size], string(&y)[num][2]) {//for imagenet dataset
	float right_count = 0;
	float all_count = num;
	float accuracy = 0;
	for (int i = 0; i < num; i++)
	{
		int predict_label = 0;
		float max = 0;
		for (int j = 0; j < label_size; j++) {
			if (x[i][j][0][0] > max) {
				predict_label = j;
				max = x[i][j][0][0];
			}
		}
		if (predict_label == int(atof(y[i][1].c_str()))) {
			right_count++;
		}
	}
	accuracy = right_count / all_count;
	cout << "finished calculating accuracy ...." << endl;
	cout << endl;
	ofstream accuracy_a;
	accuracy_a.open("accuracy_a.txt", ios::app);
	accuracy_a << "predicted accuracy is:  " << accuracy << endl;
	accuracy_a << endl;
	accuracy_a.close();
	cout << "predicted accuracy is:  " << accuracy << endl;
	cout << endl;
}
template<int size,int label_size>
void accuracy(float (&x)[size][label_size],float (&y)[size][label_size]) {//for mnist dataset
	float right_count = 0;
	float all_count = size;
	float accuracy = 0;
	for (int i = 0; i < size; i++)
	{
		int predict_label = 0;
		float max = 0;
		for (int j = 0; j < label_size; j++) {
			if (x[i][j] > max) {
				predict_label = j;
				max = x[i][j];
			}
		}
		if (y[i][predict_label] == 1) {
			right_count++;
		}
	}
	accuracy = right_count / all_count;
	cout << "finished calculating accuracy ..................................." << endl;
	cout << "................................................................." << endl;
	cout << endl;
	ofstream accuracy_a;
	accuracy_a.open("accuracy_a.txt", ios::app);
	accuracy_a << "predicted accuracy is:  " << accuracy << endl;
	accuracy_a << endl;
	accuracy_a.close();
	cout << "predicted accuracy is:  " << accuracy << endl;
	cout << endl;
}
#endif
