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
	gama = 1.0 * v_num / 2;
}

void update_sol() {
	while (!C_vec_dscore.empty())C_vec_dscore.pop();
	for (auto& i : C_vec)
		C_vec_dscore.emplace(i);
}

void cover(int e) {
	uncover_e.erase(e);
}

void uncover(int e) {
	uncover_e.insert(e);
}

int randomSelect() {
	srand(unsigned(time(NULL)));
	return rand() % uncover_e.size();
}

void remove(int v) {
	C_vec.erase(v);
	dscore[v] = -dscore[v];
	conf_change[v] = 0;
	for (int i = 0; i < v_edges[v].size(); i++) {
		int e = v_edges[v][i], n = v_neig[v][i];
		if (C_vec.find(n) == C_vec.end()) {
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
	dscore[v] = -dscore[v];
	for (int i = 0; i < v_edges[v].size(); i++) {
		int e = v_edges[v][i], n = v_neig[v][i];
		if (C_vec.find(n) == C_vec.end()) {
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
	while(!uncover_e.empty()) {
		int best_v = *(dscore_vec.begin());
		dscore_vec.erase(dscore_vec.begin());
		C_vec.insert(best_v);
		dscore[best_v] = -dscore[best_v];
		for (int j = 0; j < v_edges[best_v].size(); j++) {
			int e = v_edges[best_v][j], n = v_neig[best_v][j];
			if (C_vec.find(n)==C_vec.end()) {
				dscore[n] -= weight[e];
				conf_change[n] = 1;
				cover(e);
			}
			else
				dscore[n] += weight[e];
		}
	}
	cout << "Greedy: " << C_vec.size() << endl;
	update_sol();
}

void find_max_dscore() {
	int v = C_vec_dscore.top();
	C_vec_dscore.pop();
	remove(v);
}


int find_remove_vec() {
	int ret = 0;
	int maxn = -100000;
	for (int i = 1; i < v_num; i++) {
		if (C_vec.find(i) == C_vec.end())continue;
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
	while (step <= max_time) {
		if (uncover_e.empty()) {
			update_sol();
			if (C_vec.size() == optimal)
				return;
			find_max_dscore();
			continue;
		}
		int v = find_remove_vec();
		remove(v);
		auto it = uncover_e.begin();
		advance(it, randomSelect());
		int idx = *it;
		int v_add = select_vec(idx);
		add(v_add);
		age[v_add]++, age[v]++;
		update_weight();
		step++;
	}
}
