#include "Select.h"

using namespace std;

Parent Select::InitPopulation(const SolutionSet& solution_set, const int& pop_size){
	
	random_device rd;
	default_random_engine gen = default_random_engine(rd());
	
	Parent parent;
	for (int i = 0; i < pop_size; ) {
		vector<double> machine_load(4, 0.0);
		SolutionSet tmp;
		for (auto it = solution_set.begin(); it != solution_set.end(); ++it) {
			// Random
			uniform_int_distribution<int> dis(1, static_cast<int>(it->second.size()));
			// Get Combination
			LotNumber lot_num = it->first;
			CombNumber comb_num = dis(gen);
			ProductResource prod_resource = it->second.find(comb_num)->second;
			// Cal Machine Load
			if (prod_resource.no_machine == 1)	machine_load[0] += prod_resource.load;
			else if (prod_resource.no_machine == 2)	machine_load[1] += prod_resource.load;
			else if (prod_resource.no_machine == 3)	machine_load[2] += prod_resource.load;
			else if (prod_resource.no_machine == 4)	machine_load[3] += prod_resource.load;
			// Save result
			tmp.insert(pair<LotNumber, map<CombNumber, ProductResource>>(lot_num, { { comb_num, prod_resource } }));
		}
		// Judge over load or not 
		if (machine_load[0] <= M_LOAD_SX40 && machine_load[1] <= M_LOAD_PRESS && machine_load[2] <= M_LOAD_RF70 && machine_load[3] <= M_LOAD_S350) {
			//cout << "Generate solution idx = " << i << endl;
			parent.insert(pair<PopulationIdx, SolutionSet>(i, tmp));
			++i; // 解正確，才尋找下一組解
		}
		else {
			//cout << "Re Generate Solution !" << endl;
		}		
	}
	return parent;
}

Parent Select::SamplingPopulation(Parent& parent, const int& sampling_size) {
	vector<PairPopCost> cost; // pair(PopulationIdx, cost)
	vector<PairPopRemainder> remainder; // pair(PopulationIdx, remainder)

	// Cal Each population value(cost & remainder)
	for (auto it = parent.begin(); it != parent.end(); ++it) {
		double sum_cost = 0.0, sum_remainder = 0.0;
		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
			for (auto it3 = it2->second.begin(); it3 != it2->second.end(); ++it3) {
				sum_cost += it3->second.cost;
				sum_remainder += it3->second.excess_material;
			}
		}
		cost.push_back(pair<PopulationIdx, Cost>(it->first, sum_cost));
		remainder.push_back(pair<PopulationIdx, Remainder>(it->first, sum_remainder));
	}
	// Sort vector by second
	sort(cost.begin(), cost.end(), sort_pair_second_less<int, double>());
	sort(remainder.begin(), remainder.end(), sort_pair_second_less<int, double>());
	
	// Random Range
	random_device rd;
	mt19937 gen(rd());
	const int size_idx = static_cast<int>(cost.size() - 1);
	const int half_size_idx = static_cast<int>(cost.size() / 2);
	

	// Sampling => 6(重視) : 2(不重視) : 2(隨機抽)
	int focus = sampling_size / 10 * 6;
	int not_focus = focus + sampling_size / 10 * 2;
	if (IMPORTANCE == 1) { // 重餘料
		for (int i = 0; i < sampling_size; ++i) {
			if (i < focus) {
				uniform_int_distribution<int> dis(0, half_size_idx);
				parent[i] = parent[remainder[dis(gen)].first];
			}
			else if(i > focus && i < not_focus){
				uniform_int_distribution<int> dis(0, half_size_idx);
				parent[i] = parent[cost[dis(gen)].first];
			}
			else {
				uniform_int_distribution<int> dis(half_size_idx + 1, size_idx);
				if(i % 2 == 0)
					parent[i] = parent[cost[dis(gen)].first]; 
				else 
					parent[i] = parent[remainder[dis(gen)].first];
			}
		}
	}
	else { // 重成本
		for (int i = 0; i < sampling_size; ++i) {
			if (i < focus) {
				uniform_int_distribution<int> dis(0, half_size_idx);
				parent[i] = parent[cost[dis(gen)].first];
			}
			else if (i > focus && i < not_focus) {
				uniform_int_distribution<int> dis(0, half_size_idx);
				parent[i] = parent[remainder[dis(gen)].first];
			}
			else {
				uniform_int_distribution<int> dis(half_size_idx + 1, size_idx);
				if (i % 2 == 0)
					parent[i] = parent[cost[dis(gen)].first];
				else
					parent[i] = parent[remainder[dis(gen)].first];
			}
		}
	}
	// Set Into private variable
	this->cost = cost;
	this->remainder = remainder;

	return parent;
}