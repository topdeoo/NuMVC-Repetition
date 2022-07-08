#pragma once
#include <iostream>
#include <algorithm>
#include <queue>
#include <ctime>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <cmath>
using namespace std;

typedef long long uint64;

// param for algorithm
int max_time;
int optimal;
int seeds;
double gama;
double roi = 0.3;


default_random_engine e; // random number gen

struct Edge {
	int v1, v2;
};


int v_num; // vec num
int e_num; // edge num
Edge* edge; // edge set

int* dscore; // dscore
uint64* age; // age of vec
int* weight; // edge weight
double weight_mean;
unordered_map<int, vector<int>> v_neig;
unordered_map<int, vector<int>> v_edges;

// 


// For compare dscore
struct cmp {
	bool operator() (const int &a, const int &b) const {
		return dscore[a] > dscore[b];
	}
};
set<int, cmp> dscore_vec; // heap for high dscore of all vec
struct cmp2 {
	bool operator() (const int& a, const int& b)const {
		if (dscore[a] == dscore[b]) {
			return age[a] > age[b];
		}
		return dscore[a] > dscore[b];
	}
};
set<int, cmp2> C_vec;


// sol struct
int* v_in_c; // vec in sol C
int c_size;
unordered_set<int> uncover_e;
int step;

// For CC
int* conf_change; 


// best sol
int* best_v_in_c;
int best_c_size;
