#include "Clone.h"

Parent Clone::ClonePopulation(const SolutionSet& solution_set, Parent& parent) {
	// Get All lot number, because lot number in file is not continuously adding one => 1 2 5 6
	PopMachineLoad pop_machineLoad;
	for (auto it = solution_set.begin(); it != solution_set.end(); ++it){
		lot_numbers.push_back(it->first);
	}
	// Get All Population Machine Load
	for (auto it = parent.begin(); it != parent.end(); ++it) {
		vector<double> machine_load(4, 0.0);
		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
			for (auto it3 = it2->second.begin(); it3 != it2->second.end(); ++it3) {
				if (it3->second.no_machine == 1)	machine_load[0] += it3->second.load;
				else if (it3->second.no_machine == 2)	machine_load[1] += it3->second.load;
				else if (it3->second.no_machine == 3)	machine_load[2] += it3->second.load;
				else if (it3->second.no_machine == 4)	machine_load[3] += it3->second.load;
			}
		}
		pop_machineLoad.insert(pair<PopulationIdx, MachineLoad>(it->first, machine_load));
	}

	random_device rd;
	mt19937 gen(rd()); // Mersenne twister MT19937
	for (auto it = parent.begin(); it != parent.end(); ++it) {	
		uniform_int_distribution<int> dis_idx(0, static_cast<int>(it->second.size() - 1)); //Choose LotNumber(0 ~ 214)
		for (int i = 0; i < MUTATIONLAYER; ++i) {
			for (int j = 0; j < CLONE; ++j) {
				// tmp : �n�洫���귽�զX
				int change_lot_num_idx = dis_idx(gen);
				int change_lot_num = lot_numbers[change_lot_num_idx];

				uniform_int_distribution<int> dis_comb(1, static_cast<int>(solution_set.find(change_lot_num)->second.size())); //Choose LotNumber	
				int change_lot_comb = dis_comb(gen);

				map<CombNumber, ProductResource> tmp;
				tmp.insert(pair<CombNumber, ProductResource>(
					solution_set.find(change_lot_num)->second.find(change_lot_comb)->first, 
					solution_set.find(change_lot_num)->second.find(change_lot_comb)->second
				));
				// ����洫���ؼЬݬO�_�|���Ӫ���n�A�� dominate�A�@���� fitness �Ӯɶ��A�洫�᪺�զX���ŦX���I
				// 1. �l�ƶq��� & ������C 
				// 2. �洫��O�_�| <= Machine Load
				// �u�n�W�z���ŦX�A�ڭ̴N�O�d�쥻���귽������
				auto it_original = it->second.find(change_lot_num)->second.begin()->second;
				auto it_fix = tmp.begin()->second;
				if (it_fix.cost < it_original.cost && it_fix.excess_material < it_original.excess_material){
					int change_machine = it_fix.no_machine;
					if (it_fix.no_machine == 1){
						if (it_original.no_machine != it_fix.no_machine) {
							if ((pop_machineLoad.find(it->first)->second.at(change_machine - 1) + it_fix.load) < M_LOAD_SX40) {
								it->second.find(change_lot_num)->second = tmp; // ����
								// ��s pop_machineLoad
								pop_machineLoad.find(it->first)->second.at(it_original.no_machine - 1) -= it_original.load; //�쥻�����x��������
								pop_machineLoad.find(it->first)->second.at(change_machine - 1) += it_fix.load; // �s���x���t���[�W
							}
						}
						else {
							if ((pop_machineLoad.find(it->first)->second.at(change_machine - 1) - it_original.load + it_fix.load) < M_LOAD_SX40) {
								it->second.find(change_lot_num)->second = tmp; // ����
								// ��s pop_machineLoad
								pop_machineLoad.find(it->first)->second.at(it_original.no_machine - 1) -= it_original.load; //�쥻�����x��������
								pop_machineLoad.find(it->first)->second.at(change_machine - 1) += it_fix.load; // �s���x���t���[�W
							}
						}		
					}
					else if (it_fix.no_machine == 2){
						if (it_original.no_machine != it_fix.no_machine) {
							if ((pop_machineLoad.find(it->first)->second.at(change_machine - 1) + it_fix.load) < M_LOAD_PRESS) {
								it->second.find(change_lot_num)->second = tmp;
								// ��s pop_machineLoad
								pop_machineLoad.find(it->first)->second.at(it_original.no_machine - 1) -= it_original.load; //�쥻�����x��������
								pop_machineLoad.find(it->first)->second.at(change_machine - 1) += it_fix.load; // �s���x���t���[�W
							}
						}
						else {
							if ((pop_machineLoad.find(it->first)->second.at(change_machine - 1) - it_original.load + it_fix.load) < M_LOAD_PRESS) {
								it->second.find(change_lot_num)->second = tmp;
								// ��s pop_machineLoad
								pop_machineLoad.find(it->first)->second.at(it_original.no_machine - 1) -= it_original.load; //�쥻�����x��������
								pop_machineLoad.find(it->first)->second.at(change_machine - 1) += it_fix.load; // �s���x���t���[�W
							}
						}
					}
					else if (it_fix.no_machine == 3){
						if (it_original.no_machine != it_fix.no_machine) {
							if ((pop_machineLoad.find(it->first)->second.at(change_machine - 1) + it_fix.load) < M_LOAD_RF70) {
								it->second.find(change_lot_num)->second = tmp;
								// ��s pop_machineLoad
								pop_machineLoad.find(it->first)->second.at(it_original.no_machine - 1) -= it_original.load; //�쥻�����x��������
								pop_machineLoad.find(it->first)->second.at(change_machine - 1) += it_fix.load; // �s���x���t���[�W
							}
						}
						else {
							if ((pop_machineLoad.find(it->first)->second.at(change_machine - 1) - it_original.load + it_fix.load) < M_LOAD_RF70) {
								it->second.find(change_lot_num)->second = tmp;
								// ��s pop_machineLoad
								pop_machineLoad.find(it->first)->second.at(it_original.no_machine - 1) -= it_original.load; //�쥻�����x��������
								pop_machineLoad.find(it->first)->second.at(change_machine - 1) += it_fix.load; // �s���x���t���[�W
							}
						}
					}
					else{
						if (it_original.no_machine != it_fix.no_machine) {
							if ((pop_machineLoad.find(it->first)->second.at(change_machine - 1) + it_fix.load) < M_LOAD_S350) {
								it->second.find(change_lot_num)->second = tmp;
								// ��s pop_machineLoad
								pop_machineLoad.find(it->first)->second.at(it_original.no_machine - 1) -= it_original.load; //�쥻�����x��������
								pop_machineLoad.find(it->first)->second.at(change_machine - 1) += it_fix.load; // �s���x���t���[�W
							}
						}
						else {
							if ((pop_machineLoad.find(it->first)->second.at(change_machine - 1) - it_original.load + it_fix.load) < M_LOAD_S350) {
								it->second.find(change_lot_num)->second = tmp;
								// ��s pop_machineLoad
								pop_machineLoad.find(it->first)->second.at(it_original.no_machine - 1) -= it_original.load; //�쥻�����x��������
								pop_machineLoad.find(it->first)->second.at(change_machine - 1) += it_fix.load; // �s���x���t���[�W
							}
						}
					}			
				}
			}
		}
	}
	// Set Into private variable
	this->pop_machineLoad = pop_machineLoad;

	return parent;
}

vector<LotNumber> Clone::Get_lot_numbers() {
	return this->lot_numbers;
}
PopMachineLoad Clone::Get_pop_machineLoad() {
	return this->pop_machineLoad;
}
bool Clone::Set_pop_machineLoad(PopMachineLoad pop_machine_load) {
	this->pop_machineLoad = pop_machine_load;
	return true;
}