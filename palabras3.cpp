#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
using namespace std;

int min(int x, int y, int z) { return min(min(x, y), z); }

int editDist(const string& str1, const string& str2) {
	int x = str1.length()+1; int y = str2.length()+1;
	vector<vector<int> > d(x,vector<int>(y));
	int i, j, cost;
	//Inicialitzem d, posant a la primera fila el valor que costaria esborrar la 
	//posicio i de str1, i a la primera columna el valor que costaria esborrar la
	//posicio j de str2.
	for (i = 0; i < x; ++i) d[i][0] = i*10;
	for (j = 0; j < y; ++j) d[0][j] = j*10;
	for (i = 1; i < x; ++i) {
	    for (j = 1; j < y; ++j) {
		   if (str1[i-1] == str2[j-1]) cost = 0;	//Mateixos chars no hem de fer res
		   else cost = min(20,abs(str1[i-1]-str2[j-1]));
		   d[i][j] = min(
						 d[i-1][j] + 10,   		//"Borrem" de str1 el char i-1
						 d[i][j-1] + 10,    	//"Borrem" de str2 el char j-1
						 d[i-1][j-1] + cost   	//Incrementem/Decrementem el char 
					 );							//str1[i-1] o str2[j-1], o "borrem" 
		}										//str1[i-1] i str2[j-1].
	}
    return d[x-1][y-1];
}

int main() {
    string str1,str2;
    while (cin >> str1 >> str2) {
		cout << editDist( str1 , str2) << endl;
	}
}
