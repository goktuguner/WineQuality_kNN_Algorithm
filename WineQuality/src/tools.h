#ifndef TOOLS_H_
#define TOOLS_H_
#include <iostream>
#include <vector>
using namespace std;

class Wine{
public:
	vector <double> features;
	int quality;
};

void splitLine(string line, vector <double> & features, int  & quality);

int findMostFrequentNumber(const vector<int>& numbers);

void readData(string file_name, vector <Wine> & saraplar);

void splitData(string main_file, string train_file, string test_file, double split_ratio);

void sortVector(vector<double>& vec, vector<int> quality, vector<int>& klikalite, int k);

vector <int> get_train_quality(vector <Wine> train_wines);

void tahmin_yap(vector <vector<int>> en_yakinlarin_kaliteleri, vector <double> & predictions);

void dogruluk_hesapla(vector <Wine> test_wines, vector <double> & predictions, int sapma);

void find_maxes(vector <Wine> train_wines, vector <double> & ratios);

void kNN(vector <Wine> train_wines, vector <Wine> test_wines, vector <double> & predictions, int k);



#endif /* TOOLS_H_ */
