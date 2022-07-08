#include "NuMVC.h"

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	// cin algorithm param
	cin >> v_num >> e_num;
	init_instance();

	init();
	greedy_search();

	return 0;
}

/*
4 3
1 2
1 3
2 3
*/