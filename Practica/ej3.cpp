#include <fstream>
#include <iostream>
#include <map>
#include <math.h>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>

using namespace std;

map<string, int> taula;

typedef vector<int> Fila;
typedef vector<Fila> Matriz;

int BIGSIZE = 1000000;
int size = 20;
int unio = 0;
int interseccio = 0;
int k;		//k-shingles
int thash;	//#thash
int p;
int sizeUniverso;	//tamaño del universal set (27^k)
int bands;
int t;

int primo(int ini, int fi)
{
	int p, num_columnas;
	num_columnas = 0 ;
	while (ini <= fi) {
		for (p = 2; ini % p != 0; p++);
		if (p == ini)
			return ini;
		ini++;
	}
	return ini;
}

int hashF(int row)
{
	int a = 1 + rand() % (p - 1);
	int b = rand() % p;
	return ((a * row + b) % p) % taula.size();
}

int criba(vector<bool> m, int tam) {
	m = vector<bool>(tam);
	m[0] = false;	m[1] = false;
	for(int i = 2; i <= tam; ++i) m[i] = true;

	for(int j = 2; j*j <= tam; ++j) {
		if (m[j]) {
			for(int h = 2; j*h <= tam; ++h) m[j*h] = false;
		}
	}
	for (int l = sizeUniverso; l < tam; ++l) if (m[l]) return l;
}

int hash_column(vector<int> vec)
{
	int seed = vec.size();
	for (auto &i : vec) {
		seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
	return abs(seed%20000);
}

set<string> obtenerKshingles(const string &ruta, int k)
{
	ifstream file;
	file.open(&ruta[0], ifstream::in);
	string palabra;
	string texto = "";
	while (getline(file, palabra)) {
		texto += palabra;
	}
	int i = 0;
	set<string> kshingles;
	while (i + k <= texto.size()) {
		string kshingle = "";
		for (int j = i; j < i + k; ++j) {
			kshingle += texto[j];
		}
		kshingles.insert(kshingle);
		++i;
	}
	return kshingles;
}

Matriz ajustaSize(const Matriz &matriu, int sz)
{
	vector<vector<int> > res(sz, vector<int>(size));
	for (int i = 0; i < sz; ++i) {
		for (int j = 0; j < size; ++j)
			res[i][j] = matriu[i][j];
	}
	return res;
}

void escriuMap()
{
	auto it = taula.begin();
	while (it != taula.end()) {
		cout << it->first << endl;
		++it;
	}
}

void escriuMatriu(const Matriz &matriu);

void ompleMap(Matriz &matriu, set<string> kshingles, bool first, int num)
{
	int i = 0;
	for (auto it2 = kshingles.begin(); it2 != kshingles.end(); ++it2) {
		if (first) {
			taula.insert(pair<string, int>(*it2, taula.size()));
			for (int r = 0; r < num; r++)
				matriu[i][r] = 0;
			matriu[i][0] = 1;
			unio++;
		} else {
			auto it = taula.find(*it2);
			if (it == taula.end()) {
				taula.insert(pair<string, int>(*it2, taula.size()));
				// Go to row corresponding to newly inserted element and only
				// set the current column to one
				for (int r = 0; r < num; r++) {
					matriu[taula.size()][0] = 0;
				}
				matriu[taula.size()][num] = 1;
				unio++;
			} else {
				matriu[it->second][num] = 1;
				interseccio++;
			}
		}
		i++;
	}
}

void escriuMatriu(const Matriz &matriu)
{
	for (int i = 0; i < matriu.size(); ++i) {
		for (int j = 0; j < size; ++j) {
			cout << matriu[i][j] << " ";
		}
		cout << endl;
	}
}

// Returns a value between 0 and 1 that represents the percentage of rows of two
// documents that are equal
float check_similitude(int c1, int c2, vector<vector<int>> &minh)
{
	float equal = 0;
	for (int i = 0; i < minh.size(); i++) {
		if (minh[i][c1] == minh[i][c2])
			equal++;
	}
	return equal/minh.size();
}

void lsh(int band_size, vector<vector<int>> &minh)
{
	// Each band has a different bucket. We want to find which vector of ints
	// are hashed to the same value. 20K is an arbitrary number to minimize
	// collisions.
	vector<vector<vector<int>>> bucket(minh.size()/band_size, vector<vector<int>>(20000));
	// To improve performance, we mark the entries that contain duplicates so
	// that we don't iterate over the whole hash table.
	vector<pair<int,int>> dup;
	for (int j = 0; j < minh[0].size(); j++) {
		vector<int> v;
		int ct = 0;
		int i;
		for (i = 0; i < minh.size(); i++, ct++) {
			if (ct == band_size) {
				ct = 0;
				auto &bk = bucket[(i-1)/band_size][hash_column(v)];
				if (bk.size() > 0) {
					for (int m = 0; m < bk.size(); m++) {
						dup.push_back(make_pair(bk[m], j));
					}
				}
				bk.push_back(j);
				v.empty();
			}
			v.push_back(minh[i][j]);
		}
	}

	for (int i = 0; i < dup.size(); i++) {
		float sim = check_similitude(dup[i].first, dup[i].second, minh);
		if (sim < t)
			continue;
		cout << "Similitude between " << dup[i].first << " and " << dup[i].second << ": ";
		cout << sim << endl;
	}
}

vector<vector<int>> minhashing(const Matriz &matriu)
{
	srand(time(NULL));
	vector<int> minhashes(thash);
	vector<vector<int> >signatures(thash, vector<int>(size, 999999999));
	sizeUniverso = pow(27,k);
	vector<bool> m;
	p = criba(m,sizeUniverso*2);
	for (int i = 0; i < matriu.size(); ++i) { //filas
		for (int t = 0; t < thash; ++t) { //hashFunctions
			minhashes[t] = hashF(i);
		}
		for (int j = 0; j < size; ++j) {
			if (matriu[i][j] == 1) {
				for (int z = 0; z < thash; ++z) {
					if (minhashes[z] < signatures[z][j]) signatures[z][j] = minhashes[z];
				}
			}
		}
	}
	return signatures;
}

int main()
{
	cout << "Introdueix la K per formar els k-shingles: " << endl;
	cin >> k;
	cout << "Introdueix el nombre de funcions de hash: " << endl;
	cin >> thash;
	cout << "Introdueix el nombre de bands: " << endl;
	cin >> bands;
	cout << "Introdueix la t per indicar la similitud mínima per tal que siguin considerades similars: " << endl;
	cin >> t;

	vector<vector<int>> matriu(BIGSIZE, vector<int>(size, -1));
	vector<set<string>> shingles;
	for (int i = 1; i <= size; i++) {
		string s = "test" + to_string(i) + ".txt";
		auto sh = obtenerKshingles(s, k);
		shingles.push_back(sh);
	}
	bool first = true;
	for (int i = 0; i < shingles.size(); i++) {
		ompleMap(matriu, shingles[i], first, i);
		first = false;
	}
	matriu = ajustaSize(matriu, taula.size());
	auto sig = minhashing(matriu);
	lsh(bands, sig);
}
