#include <iostream>
#include <queue>
#include <vector>
using namespace std;
 
int MAXIMO = 100000;

struct tripleta {	//Per fer 1 sol djikstra
	int dist;
	int bomber;		//servira per saber quin bomber arriba en dist d
	bool newVert;
};

struct comp {
	bool operator() (const pair<int,int> &a, const pair<int,int> &b) {
		return a.second > b.second;
	}
};

void dijkstra(vector<tripleta>& gestio, const vector<vector<pair<int,int> > >& graph, const vector<int>& firefighters) {
	int u, v, w, sz;
	priority_queue< pair<int,int>, vector< pair<int,int> >, comp > cua;
	
	for(int w = 0; w < graph.size(); ++w){	//inicialitzem el vector
		gestio[w].dist = MAXIMO;
		gestio[w].bomber = MAXIMO;
		gestio[w].newVert = false;
	}
	
	for (int aux = 0; aux < firefighters.size(); ++aux) {
		gestio[firefighters[aux]].dist = 0;
		gestio[firefighters[aux]].bomber = firefighters[aux];	//propagarem els bombers!
		cua.push(pair<int,int>(firefighters[aux],0));
	}
 
	while(!cua.empty()) {
		u = cua.top().first;
		cua.pop();
		if(gestio[u].newVert) continue;
		sz = graph[u].size();
		for(int i = 0; i < sz; ++i) {
			v = graph[u][i].first;
			w = graph[u][i].second;
			if((!gestio[v].newVert and gestio[u].dist + w < gestio[v].dist) or 
			(!gestio[v].newVert and gestio[u].dist + w == gestio[v].dist and gestio[u].bomber < gestio[v].bomber)) {
				gestio[v].dist = gestio[u].dist + w;
				gestio[v].bomber = gestio[u].bomber;	//Propaguem bomber per saber quin arriba a cada ciutat
				cua.push(pair<int,int>(v, gestio[v].dist));
			}
		}
		gestio[u].newVert = true;
	}
}

int main() {
// create graph
	int cases, cities, roads;
	cin >> cases;
	for (int i = 1; i <= cases; ++i) {	//numero de casos
		cin >> cities >> roads;
		vector< vector <pair<int,int> > > graph(cities);
		int x, y, z;
		for (int j = 0; j < roads; ++j) {	//carreteras
			cin >> x >> y >> z;
			graph[x].push_back(pair<int,int>(y,z));
			graph[y].push_back(pair<int,int>(x,z));
		}
		
		int k; cin >> k;
		vector<int> firefighters(k);	
		for (int d = 0; d < k; ++d) cin >> firefighters[d];
		
		int q; cin >> q; 
		vector<int> fires(q);
		for (int e = 0; e < q; ++e) cin >> fires[e];
		
		vector<tripleta> gestio(cities);
		dijkstra(gestio,graph,firefighters);

		cout << "Case #" << i << endl;
		for (int b = 0; b < fires.size(); ++b) {
			cout << "To get to " << fires[b] << ", distance " << gestio[fires[b]].dist; 
			cout << ", from city " << gestio[fires[b]].bomber << "." << endl;
		}
		
		if (i != cases) cout << endl;
	}
}
