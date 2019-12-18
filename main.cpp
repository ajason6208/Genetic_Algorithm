#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>

#include "util.h"
#include "Select/Select.h"
#include "Fitness/Fitness.h"
#include "Crossover/Crossover.h"
#include "Clone/Clone.h"
#include "schedule.h"

using namespace std;

bool cmp_cost(const KPI& lhs, const KPI& rhs)
{
	return lhs.sum_cost < rhs.sum_cost;
}
bool cmp_remainder(const KPI& lhs, const KPI& rhs)
{
	return lhs.sum_remainder < rhs.sum_remainder;
}

SolutionSet Get_Solution_Set(const string& file_name){
	SolutionSet solution_set;

	ifstream infile(file_name, ios::in);
	if (!infile){
		cout << "Unable to open file" << endl;
	}
	else{
		istringstream  entry;
		string line;
		LotNumber lot_num;
		CombNumber comb_num;
		ProductResource pr;
		// Read data
		while (getline(infile, line)) {
			entry.str(line);
			entry >> lot_num >> comb_num >> pr.no_machine >> pr.weighting >> pr.bag >> pr.excess_material >> pr.excess_material_rate >> pr.cost >> pr.load;
			if (solution_set.count(lot_num) == 0){
				solution_set.insert(pair<LotNumber, map<CombNumber, ProductResource>>(lot_num, { { comb_num, pr } }));
			}
			else{
				auto& it = solution_set.find(lot_num)->second;
				if (it.count(comb_num) == 0){
					it.insert(pair<CombNumber, ProductResource>(comb_num, pr));
				}
				else{
					cout << "Data Error !" << endl; // 表示 txt 的 (LotNumber , CombNumber) 不唯一
					system("pause");
				}
			}
			entry.clear();
		}
	}
	return solution_set;
}

KPI GetLoaclBestSolution(const PopFit& pop_fit, const Parent& parent) {
	auto max = get_max(pop_fit);
	SolutionSet solution_set = parent.find(max.first)->second;
	double cost = 0.0, remainder = 0.0;
	for (auto it = solution_set.begin(); it != solution_set.end(); ++it) {
		cost += it->second.begin()->second.cost;
		remainder += it->second.begin()->second.excess_material;
	}
	cout << "成本 : " << cost << "\t餘料 : " << remainder << endl;
	return KPI{cost, remainder};
}

int main(){

	// Read all the possible resources of each lot number
	SolutionSet solution_set;
	solution_set = Get_Solution_Set(FILE_PATH);

	// 這裡其實可以針對問題編碼後再對母體做選擇 (只是在該問題沒編碼 - 可擴充)

	// Select (Initialize POP_SIZE populations)
	Select select;
	Parent parent = select.InitPopulation(solution_set, POP_SIZE);

	// Start GA Alog
	PopFit pop_fit;
	Fitness f_obj;
	Clone cl_obj;
	Crossover c_obj;
	vector<KPI> kpi;
	for (int i = 0; i < N_GENERATIONS; ++i){
		// Sampling Population
		select.SamplingPopulation(parent, SAMPLING_SIZE);

		// Alog 1 : Clone Solution (T.J Hsieh) / Use "dominate" judge the solution quality (T.H Wu)
		cl_obj.ClonePopulation(solution_set, parent);

		// Get Fitness
		pop_fit = f_obj.GetFitness(parent);
		
		// Algo 2 : Cross over
		c_obj.DoCrossOver(cl_obj, pop_fit, parent);

		// Calculate the best solution in this generation 
		kpi.push_back(GetLoaclBestSolution(pop_fit, parent));
	}

	auto max_cost = std::min_element(kpi.begin(), kpi.end(), cmp_cost);
	auto max_remainder = std::min_element(kpi.begin(), kpi.end(), cmp_remainder);

	cout << "最佳成本 : " << max_cost->sum_cost << endl;
	cout << "最佳餘料 : " << max_remainder->sum_remainder << endl;

	system("pause");
}