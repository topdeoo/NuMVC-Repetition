#pragma once
#include "basic.h"

void init_instance() {
	c_size = 0;
	edge = new Edge[e_num];
	dscore = new int[v_num + 1];
	weight = new int[e_num];
	conf_change = new int[v_num + 1];
	age = new uint64[v_num + 1];
	v_in_c = new int[v_num + 1];

}

void init() {
	for (int i = 1;i <= v_num; i++) {
		v_in_c[i] = 0;
		dscore[i] = 0;
		age[i] = 0;
		conf_change[i] = 1;
	}

	for (int i = 0; i < e_num; i++) {
		uncover_e.insert(i);
		cin >> edge[i].v1 >> edge[i].v2;
		weight[i] = 1;
		dscore[edge[i].v1] += weight[i];
		dscore[edge[i].v2] += weight[i];
		v_neig[edge[i].v1].push_back(edge[i].v2);
		v_neig[edge[i].v2].push_back(edge[i].v1);
		v_edges[edge[i].v1].push_back(i);
		v_edges[edge[i].v2].push_back(i);
	}

	for (int i = 1; i <= v_num; i++) {
		dscore_vec.push(i);
	}
	/*cout << dscore_vec.top() << endl;*/
}

void cover(int e) {
	uncover_e.erase(e);
}

void greedy_search() {
	int i = 0;
	for (i = 0; !uncover_e.empty();) {
		int best_v = dscore_vec.top();
		if (dscore[best_v] > 0) {
			dscore_vec.pop();
			v_in_c[best_v] = 1;
			dscore[best_v] = -dscore[best_v];
			for (int j = 0; j < v_edges[best_v].size(); j++) {
				int e = v_edges[best_v][j], n = v_neig[best_v][j];
				if (!v_in_c[n]) {
					dscore[n] -= weight[e];
					conf_change[n] = 1;
					cover(e);
				}
				else
					dscore[n] += weight[e];
			}
			i++;
		}
	}
	c_size = i;
	cout << c_size << endl;
}
