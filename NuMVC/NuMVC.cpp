#include "NuMVC.h"

ifstream is;

void init() {
	for (int i = 1; i <= v_num; i++) {
		v_in_c[i] = 0;
		dscore[i] = 0;
		age[i] = 0;
		conf_change[i] = 1;
	}

	for (int i = 0; i < e_num; i++) {
		uncover_e.insert(i);
		is >> edge[i].v1 >> edge[i].v2;
		weight[i] = 1;
		dscore[edge[i].v1] += weight[i];
		dscore[edge[i].v2] += weight[i];
		v_neig[edge[i].v1].push_back(edge[i].v2);
		v_neig[edge[i].v2].push_back(edge[i].v1);
		v_edges[edge[i].v1].push_back(i);
		v_edges[edge[i].v2].push_back(i);
		weight_mean += weight[i];
	}

	weight_mean /= e_num;

	for (int i = 1; i <= v_num; i++) {
		dscore_vec.insert(i);
	}
	/*cout << dscore_vec.top() << endl;*/
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	// cin algorithm param
	/*is.open("data.txt");*/
	is.open("brock400-2.txt");

	is >> max_time >> optimal >> seeds;
	is >> v_num >> e_num;
	init_instance();

	init();
	greedy_search();

	for (int i = 1; i <= v_num; i++) {
		if (v_in_c[i]) {
			C_vec.insert(i);
			age[i]++;
		}
	}

	if(C_vec.size() + uncover_e.size() > optimal)
		numvc();

	cout << "MVC size is " << C_vec.size() << endl;
	//for (auto& i : C_vec)
	//	cout << "Vec: " << i << endl;

	return 0;
}

/*
10 2 5
4 3
1 2
1 3
2 3
1 4
*/