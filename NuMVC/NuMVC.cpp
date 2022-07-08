#include "NuMVC.h"

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	// cin algorithm param
	cin >> max_time >> optimal >> seeds;
	cin >> v_num >> e_num;
	init_instance();

	init();
	greedy_search();

	for (int i = 1; i <= v_num; i++) {
		if (v_in_c[i]) {
			C_vec.insert(i);
			age[i]++;
		}
	}

	if(best_c_size + uncover_e.size() > optimal)
		numvc();

	cout << "MVC size is " << C_vec.size() << endl;
	for (auto& i : C_vec)
		cout << "Vec: " << i << endl;

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