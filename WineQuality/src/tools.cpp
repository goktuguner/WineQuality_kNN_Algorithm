#include "tools.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <time.h>
#include <cmath>
#include <algorithm>


void splitLine(string line, vector <double> & features, int  & quality){
	string token;
	stringstream splitter(line);
	int ind=0;
	while(getline(splitter,token, ';')){
		if(ind < 11){
			features.push_back(stod(token));
		}
		else{
			quality = stoi(token);
		}
		ind++;
	}
}


int findMostFrequentNumber(const vector<int> & numbers) {
    int maxCount = 0;
    int mostFrequentNumber = 0;

    for (int i = 0; i < int(numbers.size()); i++) {
        int count = 0;

        for (int j = 0; j < int(numbers.size()); j++) {
            if (numbers[j] == numbers[i]) {
                count++;
            }
        }
        if (count > maxCount) {
            maxCount = count;
            mostFrequentNumber = numbers[i];
        }
    }
    return mostFrequentNumber;
}

void readData(string file_name, vector <Wine> & saraplar){
	string line;
	ifstream okuyucu;
	okuyucu.open(file_name);
	int lineind=0;

	if(okuyucu){
		while(getline(okuyucu, line)){
			if(lineind !=0){
				Wine sarap;
				splitLine(line, sarap.features, sarap.quality);
				saraplar.emplace_back(sarap);
			}
			lineind++;
		}
		okuyucu.close();
	}
	else{
 		cout << "Dosya okunamadı: " << file_name << endl;
	}
}


void splitData(string main_file, string train_file, string test_file, double split_ratio){
	srand(time(0));
	ifstream okuyucu;
	ofstream train_writer, test_writer;
	okuyucu.open(main_file);
	string line;
	if(okuyucu){
		string first_line, data_line;
		getline(okuyucu,first_line);
		train_writer.open(train_file);
		test_writer.open(test_file);
		train_writer << first_line;
		test_writer << first_line;

		while(getline(okuyucu,data_line)){
			if((rand()%100) > split_ratio * 100){
				train_writer << data_line << endl;
			}
			else{
				test_writer << data_line << endl;
			}
		}
		test_writer.close();
		train_writer.close();
		okuyucu.close();
	}

	else{
		cout << "Dosya okunamadı: " << main_file << endl;
	}
}

void sortVector(vector<double>& vec, vector<int> quality, vector<int>& klikalite, int k) {

    vector<pair<double, double>> vec_quality_pairs;

    for (unsigned int i = 0; i < vec.size(); i++) {
        vec_quality_pairs.push_back(make_pair(vec[i], quality[i]));
    }

    sort(vec_quality_pairs.begin(), vec_quality_pairs.end());

    vec.clear();
    klikalite.clear();

    for (int i = 0; i < k; i++) {
        vec.push_back(vec_quality_pairs[i].first);
        klikalite.push_back(vec_quality_pairs[i].second);
    }
}

vector <int> get_train_quality(vector <Wine> train_wines){
	vector <int> train_quality;
	for(Wine sarap: train_wines){
		train_quality.push_back(sarap.quality);
	}
	return train_quality;
}

void tahmin_yap(vector <vector<int>> en_yakinlarin_kaliteleri, vector <double> & predictions){
	for(unsigned int i=0; i<en_yakinlarin_kaliteleri.size(); i++){
		int decision = findMostFrequentNumber(en_yakinlarin_kaliteleri[i]);
		predictions.push_back(decision);
	}
}

void dogruluk_hesapla(vector <Wine> test_wines, vector <double> & predictions, int sapma){
	vector <double> original;

	for(Wine sarap: test_wines){
		original.push_back(sarap.quality);
	}

	double dogru=0;
	double hepsi=0;

	for(int i=0; i<int(predictions.size()); i++){
		if (predictions[i] == original[i] or predictions[i] >= original[i]-sapma and predictions[i] <= original[i]+sapma) {
			dogru++;
		}
		hepsi++;
	}

	cout << "%" << dogru/hepsi*100 << endl;
}

void find_maxes(vector <Wine> train_wines, vector <double> & ratios){

	for(unsigned int j = 0; j <= 10; j++) {
			double enb=0;
			for(Wine train: train_wines){
				if(train.features[j] > enb){
					enb = train.features[j];
				}
			}
			ratios.push_back(enb);
		}
}

void kNN(vector <Wine> train_wines, vector <Wine> test_wines, vector <double> & predictions, int k){

	vector <vector <double>> distances_vector;
	vector <int> train_quality = get_train_quality(train_wines);

	vector <double> ratios;

	find_maxes(train_wines, ratios);

	for(Wine test_wine: test_wines){
		vector <double> distances;

		for(Wine train_wine: train_wines){
			double distance=0;
			for(unsigned int i=0; i<test_wine.features.size(); i++){
				distance += pow(test_wine.features[i]/ratios[i] - train_wine.features[i]/ratios[i], 2);
			}
			distances.push_back(sqrt(distance));
		}
		distances_vector.push_back(distances);
	}

	//Uzaklıkları bulduk

	vector <vector<double>> en_yakinlar;
	vector <vector<int>> en_yakinlarin_kaliteleri;

	for(unsigned int i=0; i<distances_vector.size(); i++){

		vector <double> en_yakin = distances_vector[i];
		vector <int> kaliteleri;

		sortVector(en_yakin, train_quality, kaliteleri, k);


		en_yakinlar.push_back(en_yakin);
		en_yakinlarin_kaliteleri.push_back(kaliteleri);

	}

	tahmin_yap(en_yakinlarin_kaliteleri, predictions);

}

