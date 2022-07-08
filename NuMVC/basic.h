#pragma once
#include <iostream>
#include <algorithm>
#include <queue>
#include <ctime>
#include <random>
#include <unordered_map>
#include <unordered_set>
using namespace std;

typedef long long uint64;

// param for algorithm
int max_iter;
int max_coff;


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
unordered_map<int, vector<int>> v_neig;
unordered_map<int, vector<int>> v_edges;

// 


// For compare dscore
struct cmp {
	bool operator() (int &a, int &b){
		return dscore[a] < dscore[b];
	}
};
priority_queue<int, vector<int>, cmp> dscore_vec; // heap for high dscore vec


// sol struct
int* v_in_c; // vec in sol C
int c_size;
unordered_set<int> uncover_e;

// For CC
int* conf_change; 

