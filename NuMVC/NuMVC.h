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

int randomSelect() {
	srand(unsigned(time(NULL)));
	return rand() % e_num;
}

void remove(int v) {
	c_size--;
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
	c_size++;
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
	if (!conf_change[e.v1])
		return e.v2;
	else if (!conf_change[e.v2])
		return e.v1;
	else {
		if (dscore[e.v1] > dscore[e.v2] || (dscore[e.v1] == dscore[e.v2] && age[e.v1] > age[e.v2]))
			return e.v1;
		else
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

void find_max_dscore() {
	int v = 0, maxn = -1000000;
	for (int i = 1; i <= v_num; i++) {
		if (v_in_c[i] == 0)continue;
		if (dscore[i] > maxn)
			v = i, maxn = dscore[i];
	}
	remove(v);
}


int find_remove_vec() {
	int ret = 0;
	int maxn = -100000;
	for (int i = 1; i < v_num; i++) {
		if (!v_in_c[i])continue;
		if (dscore[i] > maxn)
			ret = i, maxn = dscore[i];
		else if (dscore[i] == maxn) {
			if (age[i] > age[ret])
				ret = i;
		}
	}
	return ret;
}

void numvc() {
	step = 1;
	memset(age, 0, sizeof age);
	while (step <= max_time) {
		if (uncover_e.empty()) {
			update_sol();
			if (c_size == optimal)
				return;
			find_max_dscore();
			continue;
		}
		int v = find_remove_vec();
		remove(v);
		int idx = randomSelect();
		if (uncover_e.find(idx) == uncover_e.end())
			idx = *(uncover_e.begin());

		int v_add = select_vec(idx);
		add(v_add);
		age[v] = age[v_add] = step;
		update_weight();
		step++;
	}
}
