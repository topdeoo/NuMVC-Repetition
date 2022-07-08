#pragma once
#include "basic.h"

void init_instance() {
	c_size = 0;
	best_c_size = 0;
	edge = new Edge[e_num];
	dscore = new int[v_num + 1];
	weight = new int[e_num];
	conf_change = new int[v_num + 1];
	age = new uint64[v_num + 1];
	memset(age, 0, sizeof age);
	v_in_c = new int[v_num + 1];
	best_v_in_c = new int[v_num + 1];
	weight_mean = 0;
	gama = v_num / 2;
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
		weight_mean += weight[i];
	}

	weight_mean /= e_num;

	for (int i = 1; i <= v_num; i++) {
		dscore_vec.insert(i);
	}
	/*cout << dscore_vec.top() << endl;*/
}

void update_sol() {
	for (int i = 1; i <= v_num; i++) {
		best_v_in_c[i] = v_in_c[i];
	}
	best_c_size = c_size;
}

void cover(int e) {
	uncover_e.erase(e);
}

void uncover(int e) {
	uncover_e.insert(e);
}

unsigned randomSelect() {
	e.seed(seeds);
	uniform_int_distribution<unsigned> u(0, e_num);
	return u(e);
}

void remove() {
	int v = *(C_vec.begin());
	C_vec.erase(v);
	v_in_c[v] = 0;
	dscore[v] = -dscore[v];
	conf_change[v] = 0;
	for (int i = 0; i < v_edges[v].size(); i++) {
		int e = v_edges[v][i], n = v_neig[v][i];
		if (!v_in_c[n]) {
			dscore[n] += weight[e];
			conf_change[n] = 1;
			uncover(e);
		}
		else
			dscore[n] -= weight[e];
	}
}

void add(int v) {
	C_vec.insert(v);
	v_in_c[v] = 1;
	dscore[v] = -dscore[v];
	for (int i = 0; i < v_edges[v].size(); i++) {
		int e = v_edges[v][i], n = v_neig[v][i];
		if (!v_in_c[n]) {
			dscore[n] -= weight[e];
			conf_change[n] = 1;
			cover(e);
		}
		else
			dscore[n] += weight[e];
	}
}

void forget_weight() {
	for (int i = 0; i < e_num; i++) {
		weight[i] = floor(roi * weight[i]);
	}
}

void update_weight() {
	weight_mean = 0;
	for (int i = 0; i < e_num; i++) {
		weight[i] += 1;
		weight_mean += weight[i];
	}
	weight_mean /= 1.0*e_num;
	if (weight_mean >= gama)
		forget_weight();
}

int select_vec(int idx) {
	Edge e = edge[idx];
	if (conf_change[e.v1])
		return e.v1;
	else if (conf_change[e.v2])
		return e.v2;
	else if (conf_change[e.v1] && conf_change[e.v2]) {
		if (dscore[e.v1] > dscore[e.v2])
			return e.v1;
		else
			return e.v2;
	}
	else {
		if (age[e.v1] > age[e.v2])
			return e.v1;
		return e.v2;
	}
}

void greedy_search() {
	int i = 0;
	for (i = 0; !uncover_e.empty();) {
		int best_v = *(dscore_vec.begin());
		if (dscore[best_v] > 0) {
			dscore_vec.erase(best_v);
			v_in_c[best_v] = 1;
			dscore[best_v] = -dscore[best_v];
			for (int j = 0; j < v_edges[best_v].size(); j++) {
				int e = v_edges[best_v][j], n = v_neig[best_v][j];
				if (!v_in_c[n]) {
					dscore[n] -= weight[e];
					conf_change[n] = 1;
					dscore_vec.erase(n);
					dscore_vec.insert(n);
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
	update_sol();
}


void numvc() {
	step = 1;
	while (step <= max_time) {
		if (!uncover_e.empty()) {
			update_sol();
			if (c_size == optimal)
				return;
			remove();
			continue;
		}

		remove();
		int idx = randomSelect();
		int v_add = select_vec(idx);
		add(v_add);
		age[v_add] = step;
		update_weight();
		step++;
	}
}
