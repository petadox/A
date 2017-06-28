#include <iostream>
#include <fstream>
#include <string>
#include <set>
using namespace std;

int unio = 0;
int interseccio = 0;

set<string> obtenerKshingles(const string & ruta, int k) {
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
		for (int j = i; j < i+k; ++j) {
			kshingle += texto[j];
		}
		kshingles.insert(kshingle);
		++i;
	}
	file.close();
	return kshingles;
}

int main() {
	
	cout << "Introduzca la constante k para computar los k-shingles: " << endl;
	int k;
	cin >> k;
	cout << "Introduzca la ruta del primer documento:" << endl; 
	string documento1;
	cin >> documento1;
	cout << "Introduzca la ruta del segundo documento:" << endl; 
	string documento2;
	cin >> documento2;
	
	set<string> kshingles1 = obtenerKshingles(documento1,k);
	set<string> kshingles2 = obtenerKshingles(documento2,k);
	unio += kshingles1.size(); 
	set<string>::iterator it;
	for (it = kshingles2.begin(); it != kshingles2.end(); ++it) {
		if (kshingles1.find(*it) != kshingles1.end()) ++interseccio;
		else ++unio;
	}	
	cout << "interseccio: " << interseccio << endl;
	cout << "unio: " << unio << endl;
	double similitud = (double)interseccio/unio;
	cout << "La similitud de Jaccard entre los dos documentos es: " << similitud << endl;
}	
