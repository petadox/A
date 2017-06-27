#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <string>
using namespace std;

//m -> minuts que volem banyarnos
//n -> numero de franjes
//x -> temps mínim a esperar entre franjes maximitzat

vector<vector<int> > precalculs;

void llegeixInput(vector<int>& franjes, int n) {
	int h, m, s;
	char car;
	for (int i = 0; i < n; ++i) {	//llegim les franjes
		cin >> h >> car >> m >> car >> s;
		s += 60*m + 3600*h;
		franjes[i] = s;
	}
}

bool pucNedarMCops(const vector<int>& franjes, int espera, int m) {
	int cops = 0;	//cops que nedem
	int last = 0;	//ultima franja on hem nedat
	int size = franjes.size();
	for (int i = 1; i < size; ++i) {
		if ((cops + size-i) < m-1) return false;	//Ja no es pot arribar a nedar m cops
		else {
			if (precalculs[last][i] != -1) {	//No repetim calculs
				if (precalculs[last][i] >= espera) {
					last = i;
					++cops;
				}
			}
			else {
				precalculs[last][i] = franjes[i] - (franjes[last] + 60);
				if (franjes[i] - (franjes[last] + 60) >= espera) {	//Podem nedar 1 cop entre aquestes franjes
					last = i;										//amb l'espera que estem considerant
					++cops;
				}
			}
		}
		if (cops == m-1) return true;
	}
	return false;
}

int main() {
	int m, n;
	while (cin >> m >> n && m|n) {
		vector<int> franjes(n);
		precalculs = vector<vector<int> >(n,vector<int>(n,-1));
		llegeixInput(franjes,n);
		sort(franjes.begin(),franjes.end());	//ordenem les franjes
		int left = 0, right = franjes[n-1]-franjes[0], sol = 0;
		while (left <= right) {	//Cerca binaria sobre el valor de la X maxima a esperar
			int medio = (right+left)/2;
			if (pucNedarMCops(franjes, medio, m)) {
				sol = max(sol, medio);
				left = medio+1;
			} 
			else right = medio-1;
		}
		cout << sol << endl;
	}
}
