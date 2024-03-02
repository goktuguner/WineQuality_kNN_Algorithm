#include <iostream>
#include <vector>
#include "tools.h"

int main() {
	string main_file = "winequality-red.csv";
	string test_file = "test_file.csv";
	string train_file = "train_file.csv";
	double split_ratio = 0.2;

	vector <Wine> test_wines;
	vector <Wine> train_wines;

	splitData(main_file, train_file, test_file, split_ratio);

	readData(test_file, test_wines);
	readData(train_file, train_wines);

	vector <double> predictions;

	kNN(train_wines, test_wines, predictions, 13);

	cout << "Exact value: ";
	dogruluk_hesapla(test_wines, predictions, 0);
	cout << "Approximate value: ";
	dogruluk_hesapla(test_wines, predictions, 1);

	return 0;
}
