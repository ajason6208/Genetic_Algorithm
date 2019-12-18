#include "Crossover.h"

Parent Crossover::DoCrossOver(Clone& cl_obj, PopFit& pop_fit, Parent& parent) {
	// Get All lot number, because lot number in file is not continuously adding one => 1 2 5 6
	vector<LotNumber> lot_numbers = cl_obj.Get_lot_numbers();
	PopMachineLoad pop_machineLoad = cl_obj.Get_pop_machineLoad();
	
	
	// Let pop_fit into vector and sort it by second
	vector<PairPopFit> fit;
	for (auto& it : pop_fit)
		fit.push_back(pair<PopulationIdx, Fit>(it.first, it.second));
	sort(fit.begin(), fit.end(), sort_pair_second_greater<int, double>());

	// Random
	random_device rd;
	mt19937 gen(rd()); // Mersenne twister MT19937

	// Cross Over
	int better_group_size = static_cast<int>(fit.size() / 10 * 4);
	vector<int> v_better_idx;
	for (int i = 0; i < better_group_size; ++i)
		v_better_idx.push_back(i);

	int better_idx = 0;
	for (int worse_idx = POP_SIZE - 1; worse_idx >= v_better_idx.size(); --worse_idx) {
		// Get better_pop & worse_pop
		int better_pop = fit[better_idx].first;
		int worse_pop = fit[worse_idx].first;

		for (int num_change = 0; num_change < NUMBERTOCHOOSE; ++num_change) {
			// Choose random lot number change
			uniform_int_distribution<int> dis_idx(0, static_cast<int>(parent.find(better_pop)->second.size() - 1)); //Choose LotNumber(0 ~ 214)
			int change_lot_num_idx = dis_idx(gen);
			int change_lot_num = lot_numbers[change_lot_num_idx];

			// Get better_resource
			map<CombNumber, ProductResource> better_resource = parent.find(better_pop)->second.find(change_lot_num)->second;

			// 確定交換後是否會 <= Machine Load，如果不符合，我們就保留原本的資源不做更換
			auto it_better = parent.find(better_pop)->second.find(change_lot_num)->second.begin()->second;
			auto it_worse = parent.find(worse_pop)->second.find(change_lot_num)->second.begin()->second;
			if (it_better.no_machine == 1) {
				int change_machine = it_better.no_machine;
				if (it_better.no_machine != it_worse.no_machine) {
					if (pop_machineLoad.find(worse_pop)->second.at(change_machine - 1) + it_better.load < M_LOAD_SX40) {
						parent.find(worse_pop)->second.find(change_lot_num)->second = better_resource;
						// 更新 pop_machineLoad
						pop_machineLoad.find(worse_pop)->second.at(it_worse.no_machine - 1) -= it_worse.load; // 劣群扣除原本 load
						pop_machineLoad.find(worse_pop)->second.at(change_machine - 1) += it_better.load; // 劣群加上優群的 load
					}
				}
				else {
					if (pop_machineLoad.find(worse_pop)->second.at(change_machine - 1) - it_worse.load + it_better.load < M_LOAD_SX40) {
						parent.find(worse_pop)->second.find(change_lot_num)->second = better_resource;
						// 更新 pop_machineLoad
						pop_machineLoad.find(worse_pop)->second.at(it_worse.no_machine - 1) -= it_worse.load; // 劣群扣除原本 load
						pop_machineLoad.find(worse_pop)->second.at(change_machine - 1) += it_better.load; // 劣群加上優群的 load
					}
				}
			}
			else if (it_better.no_machine == 2) {
				int change_machine = it_better.no_machine;
				if (it_better.no_machine != it_worse.no_machine) {
					if (pop_machineLoad.find(worse_pop)->second.at(change_machine - 1) + it_better.load < M_LOAD_PRESS) {
						parent.find(worse_pop)->second.find(change_lot_num)->second = better_resource;
						// 更新 pop_machineLoad
						pop_machineLoad.find(worse_pop)->second.at(it_worse.no_machine - 1) -= it_worse.load; // 劣群扣除原本 load
						pop_machineLoad.find(worse_pop)->second.at(change_machine - 1) += it_better.load; // 劣群加上優群的 load
					}
				}
				else {
					if (pop_machineLoad.find(worse_pop)->second.at(change_machine - 1) - it_worse.load + it_better.load < M_LOAD_PRESS) {
						parent.find(worse_pop)->second.find(change_lot_num)->second = better_resource;
						// 更新 pop_machineLoad
						pop_machineLoad.find(worse_pop)->second.at(it_worse.no_machine - 1) -= it_worse.load; // 劣群扣除原本 load
						pop_machineLoad.find(worse_pop)->second.at(change_machine - 1) += it_better.load; // 劣群加上優群的 load
					}
				}
			}
			else if (it_better.no_machine == 3) {
				int change_machine = it_better.no_machine;
				if (it_better.no_machine != it_worse.no_machine) {
					if (pop_machineLoad.find(worse_pop)->second.at(change_machine - 1) + it_better.load < M_LOAD_RF70) {
						parent.find(worse_pop)->second.find(change_lot_num)->second = better_resource;
						// 更新 pop_machineLoad
						pop_machineLoad.find(worse_pop)->second.at(it_worse.no_machine - 1) -= it_worse.load; // 劣群扣除原本 load
						pop_machineLoad.find(worse_pop)->second.at(change_machine - 1) += it_better.load; // 劣群加上優群的 load
					}
				}
				else {
					if (pop_machineLoad.find(worse_pop)->second.at(change_machine - 1) - it_worse.load + it_better.load < M_LOAD_RF70) {
						parent.find(worse_pop)->second.find(change_lot_num)->second = better_resource;
						// 更新 pop_machineLoad
						pop_machineLoad.find(worse_pop)->second.at(it_worse.no_machine - 1) -= it_worse.load; // 劣群扣除原本 load
						pop_machineLoad.find(worse_pop)->second.at(change_machine - 1) += it_better.load; // 劣群加上優群的 load
					}
				}
			}
			else if (it_better.no_machine == 4) {
				int change_machine = it_better.no_machine;
				if (it_better.no_machine != it_worse.no_machine) {
					if (pop_machineLoad.find(worse_pop)->second.at(change_machine - 1) + it_better.load < M_LOAD_S350) {
						parent.find(worse_pop)->second.find(change_lot_num)->second = better_resource;
						// 更新 pop_machineLoad
						pop_machineLoad.find(worse_pop)->second.at(it_worse.no_machine - 1) -= it_worse.load; // 劣群扣除原本 load
						pop_machineLoad.find(worse_pop)->second.at(change_machine - 1) += it_better.load; // 劣群加上優群的 load
					}
				}
				else {
					if (pop_machineLoad.find(worse_pop)->second.at(change_machine - 1) - it_worse.load + it_better.load < M_LOAD_S350) {
						parent.find(worse_pop)->second.find(change_lot_num)->second = better_resource;
						// 更新 pop_machineLoad
						pop_machineLoad.find(worse_pop)->second.at(it_worse.no_machine - 1) -= it_worse.load; // 劣群扣除原本 load
						pop_machineLoad.find(worse_pop)->second.at(change_machine - 1) += it_better.load; // 劣群加上優群的 load
					}
				}
			}
		}
		better_idx = (better_idx == better_group_size - 1) ? 0 : ++better_idx;
	}
	// Update pop_machineLoad
	cl_obj.Set_pop_machineLoad(pop_machineLoad);

	return parent;
}