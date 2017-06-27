#include <iostream>
#include <vector>
using namespace std;
 
void distMaxim(int size, const vector<vector<int> >& graf, vector<int>& dist) {
	int temp = 0;
	for (int k = 0; k < size; ++k) {
		if (k == 0) temp = max(temp, graf[k][size]);
		else {
			if (dist[k] == -999999) distMaxim(k, graf, dist); //No visitat
			temp = max(temp, dist[k] + graf[k][size]);
		}
	}
	dist[size] = temp;
}
 
int main() {
	int size;	
	cin >> size;
	vector<vector<int> > graf(size, vector<int>(size));
	for (int x = 0; x < size; ++x) {
		for (int y = 0; y < size; ++y) cin >> graf[x][y];
	}
	vector<int> dist(size, -999999);
	distMaxim(size - 1, graf, dist);
	int maxim = dist[0];
	for (int i = 1; i < size; ++i) maxim = max(maxim, dist[i]);
	cout << maxim << endl;
}

