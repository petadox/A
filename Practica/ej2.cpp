#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <math.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

typedef vector<int> Fila;
typedef vector<Fila> Matriz;

//Variables globales

map<string,int> taula;

int unio = 0;
int interseccio = 0;
int k = 5;		//k-shingles
int thash = 5000;	//#thash
int base = 24;
unsigned long long p;	//primo entre sizeUniverso y sizeUniverso*2
unsigned long long sizeUniverso;	//tamaño del universal set (27^k)
string doc1, doc2;

//Funciones

unsigned long long power(int base, int exponente) {	
	unsigned long long res = 1;
	for (int i = exponente; i > 0; --i) {
		res = res * (unsigned long long)base;
		--exponente;
	}
	return res;
}

unsigned long long primerandom() {	//Funció que troba un primer random
	while(1) {						//entre sizeUniverso y 2*sizeUniverso
		bool isPrime = true;
		unsigned long long tanteo = sizeUniverso + (rand() % sizeUniverso);
		for(int i = 2; isPrime and i <= tanteo/2; ++i) {
			  if(tanteo % i == 0) isPrime = false;
		}
		if (isPrime) return tanteo;
	}
}

unsigned long long hashF(int row) {	//Generador de la universal hash
	unsigned long long a = 1 + (rand() % (p-1));
	unsigned long long b = rand() % p;
	return ((a*(unsigned long long)row + b) % p) % sizeUniverso;
}

set<string> obtenerKshingles(const string & ruta, int k) {	//Llegeix document
	ifstream file(&ruta[0]);
	string palabra;
	string texto = "";
	while (!file.eof()) {
		file >> palabra;
		texto += palabra;
	}
	int i = 0;
	set<string> kshingles;
	while (i+k <= texto.size()) {
		string kshingle = "";
		for (int j = i; j < i+k; ++j) kshingle += texto[j];
		kshingles.insert(kshingle);
		++i;
	}
	set<string>::iterator it;
	it = kshingles.find(" ");
	if (it != kshingles.end()) kshingles.erase(" ");
	file.close();
	return kshingles;
}

void ompleMap2(Matriz& matriu, set<string> kshingles1, set<string> kshingles2) {
	set<string>::iterator it1, it2;							//Inicialitza map i matriu
	it1 = kshingles1.begin();	it2 = kshingles2.begin();
	int i = 0;
	vector<pair<int,int> > ordenar;
	while (it1 != kshingles1.end() and it2 != kshingles2.end()) {
		if (*it1 < *it2) {
			ordenar.push_back(pair<int,int>(i,0));
			taula.insert(pair<string,int>(*it1,taula.size()));
			++it1;	++unio;
		}
		else if (*it2 < *it1) {
			ordenar.push_back(pair<int,int>(i,1));
			taula.insert(pair<string,int>(*it2,taula.size()));
			++it2;	++unio;
		}
		else {
			ordenar.push_back(pair<int,int>(i,0));
			ordenar.push_back(pair<int,int>(i,1));
			taula.insert(pair<string,int>(*it1,taula.size()));
			++it1;	++it2;	++interseccio;	++unio;
		}
		++i;
	}
	
	while (it1 != kshingles1.end()) {
		ordenar.push_back(pair<int,int>(i,0));
		taula.insert(pair<string,int>(*it1,taula.size()));
		++it1;	++unio;	 ++i;
	}
	
	while (it2 != kshingles2.end()) {
		ordenar.push_back(pair<int,int>(i,1));
		taula.insert(pair<string,int>(*it2,taula.size()));
		++it2;	++unio;	  ++i;
	}
	
	matriu = vector<vector<int> >(taula.size(),vector<int>(2,0));
	for (int j = 0; j < ordenar.size(); ++j) matriu[ordenar[j].first][ordenar[j].second] = 1;
}

void minhashing(const Matriz& matriu) {		//Algorisme d'aproximació minhashing
	srand(time(NULL));
	vector<unsigned long long> minhashes(thash);
	vector<vector<unsigned long long> >signatures(thash,vector<unsigned long long>(2,999999999));
	if (k <= 5) sizeUniverso = power(base,k);
	else if (unio < 2000) sizeUniverso = power(10,k);
	else {
		thash = 2000;
		sizeUniverso = power(15,k);
	}
	p = primerandom();
	for (int i = 0; i < matriu.size(); ++i) { //filas
		for (int t = 0; t < thash; ++t) { //hashFunctions
			minhashes[t] = hashF(i);
		}
		for (int j = 0; j < 2; ++j) {
			if (matriu[i][j] == 1) {
				for (int z = 0; z < thash; ++z) {
					if (minhashes[z] < signatures[z][j]) signatures[z][j] = minhashes[z];
				}
			}
		}
	}
	int sum = 0;
	for (int x = 0; x < thash; ++x) if (signatures[x][0] == signatures[x][1]) ++sum;
	cout << "Aproximacio hashing entre " << doc1 << " " << doc2 << " " << (double)sum/thash << endl;
}

int main() {
	cout << "Introdueix la K per formar els k-shingles: " << endl;
	cin >> k;
	cout << "Introdueix el document 1" << endl;
	cin >> doc1;
	set<string> kshingle1 = obtenerKshingles(doc1,k);
	cout << "Introdueix el document 2" << endl;
	cin >> doc2;
	set<string> kshingle2 = obtenerKshingles(doc2,k);
	vector<vector<int> > matriu;
	ompleMap2(matriu,kshingle1,kshingle2);
	cout << "Jaccard: " << (double)interseccio/unio << endl;
	minhashing(matriu);
}
