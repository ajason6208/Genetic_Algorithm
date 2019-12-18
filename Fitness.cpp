#include "Fitness.h"

PopRank Fitness::CalPopRank(const Parent& parent) {
	PopRank pop_rank;
	PopCost pop_cost;
	PopRemainder pop_remainder;
	// Cal Each population value(cost & remainder)
	for (auto it = parent.begin(); it != parent.end(); ++it) {
		double sum_cost = 0.0, sum_remainder = 0.0;
		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
			for (auto it3 = it2->second.begin(); it3 != it2->second.end(); ++it3) {
				sum_cost += it3->second.cost;
				sum_remainder += it3->second.excess_material;
			}
		}
		pop_cost.insert(pair<PopulationIdx, Cost>(it->first, sum_cost));
		pop_remainder.insert(pair<PopulationIdx, Remainder>(it->first, sum_remainder));
	}
	// Ranking the population , Rank = KPI1_RANK + KPI2_RANK (One better than other = Dominate)
	for (auto it = pop_cost.begin(); it != pop_cost.end(); ++it) {
		int rank = 1;
		for (auto it2 = pop_cost.begin(); it2 != pop_cost.end(); ++it2) {
			if (it->first == it2->first) continue; // Same Point
			else {
				if (it->second >= it2->second)	rank++; // Dominate
			}
		}
		pop_rank.insert(pair<PopulationIdx, Rank>(it->first, rank));
	}
	for (auto it = pop_remainder.begin(); it != pop_remainder.end(); ++it) {
		int rank = pop_rank.at(it->first);
		for (auto it2 = pop_remainder.begin(); it2 != pop_remainder.end(); ++it2) {
			if (it->first == it2->first) continue;
			else {
				if (it->second >= it2->second)	rank++;
			}
		}
		pop_rank.at(it->first) = rank;
	}
	// Set Into private variable
	this->pop_rank = pop_rank;
	this->pop_cost = pop_cost;
	this->pop_remainder = pop_remainder;

	return pop_rank;
}

PopFit Fitness::CalFitness(){
	PopFit pop_fit;
	// Cal the number of each rank
	for (auto it = pop_rank.begin(); it != pop_rank.end(); ++it){
		if (rank_num.count(it->second) == 0){
			rank_num.insert(pair<Rank, Num>(it->second, 1));
		}
		else{
			rank_num.find(it->second)->second += 1;
		}
	}
	// Cal the fitness value
	for (auto it = pop_rank.begin(); it != pop_rank.end(); ++it){	
		int sum_nk = 0;
		for (auto it2 = rank_num.begin(); it2 != rank_num.end(); ++it2){
			if (it2->first < it->second)
				sum_nk += it2->second;
			else
				break; // next solution		
		}
		int n_R = rank_num.find(it->second)->second; // the number of solution with rank R(x,t)
		double fitness = 0.0;
		fitness = POP_SIZE - sum_nk - (0.5 * (n_R - 1));
		pop_fit.insert(pair<PopulationIdx, Fit>(it->first, fitness));
	}
	// Set Into private variable
	this->pop_fit = pop_fit;

	return pop_fit;
}

DotDistance Fitness::CalDistance(){
	DotDistance dot_distance;
	// Get Max & Min value of objective function
	max_cost = std::max_element(pop_cost.begin(), pop_cost.end(), [](const pair<PopulationIdx, Cost>& p1, const pair<PopulationIdx, Cost>& p2) {return p1.second < p2.second; })->second;
	min_cost = std::min_element(pop_cost.begin(), pop_cost.end(), [](const pair<PopulationIdx, Cost>& p1, const pair<PopulationIdx, Cost>& p2) {return p1.second < p2.second; })->second;
	max_remainder = std::max_element(pop_remainder.begin(), pop_remainder.end(), [](const pair<PopulationIdx, Remainder>& p1, const pair<PopulationIdx, Remainder>& p2) {return p1.second < p2.second; })->second;
	min_remainder = std::min_element(pop_remainder.begin(), pop_remainder.end(), [](const pair<PopulationIdx, Remainder>& p1, const pair<PopulationIdx, Remainder>& p2) {return p1.second < p2.second; })->second;
	// Cal Distance
	const size_t size = pop_cost.size();
	for (int i = 0; i < size; ++i){
		for (int j = 0; j < size; ++j){
			if (i != j){
				double dis = sqrt(pow((pop_cost[i] - pop_cost[j]) / (max_cost - min_cost), 2) + pow((pop_remainder[i] - pop_remainder[j]) / (max_remainder - min_remainder), 2));
				dot_distance.insert(pair<pair<PopulationIdx, PopulationIdx>, Distance>(make_pair(make_pair(i, j),dis)));
				sigma_share += dis;
			}
		}
	}
	// Cal sigma_share
	sigma_share /= dot_distance.size();
	// Set Into private variable
	this->dot_distance = dot_distance;

	return dot_distance;
}

PopNiche Fitness::CalNicheCount(){
	PopNiche pop_niche;
	for (auto it = pop_rank.begin(); it != pop_rank.end(); ++it){
		double sum_niche_count = 0.0;
		for (auto it2 = pop_rank.begin(); it2 != pop_rank.end(); ++it2){
			if (it->first != it2->first && it->second == it2->second){
				sum_niche_count += max(  (1 - (dot_distance[make_pair(it->first, it2->first)] / (sigma_share))   )  , 0.0);
			}
		}
		pop_niche.insert(pair<PopulationIdx, NicheCount>(it->first, sum_niche_count));
	}
	// Set Into private variable
	this->pop_niche = pop_niche;

	return pop_niche;
}

PopModFit Fitness::CalModifiedFitness(){
	PopModFit pop_mod_fit;
	for (int i = 0; i < static_cast<int>(pop_fit.size()); ++i){
		if (pop_niche[i] == 0){
			pop_mod_fit[i] = pop_fit[i]; // ¤£­×§ï
		}
		else{
			pop_mod_fit.insert(pair<PopulationIdx, Fit>(i, pop_fit[i] / pop_niche[i]));
		}
	}
	// Set Into private variable
	this->pop_mod_fit = pop_mod_fit;

	return pop_mod_fit;
}

PopModNorFit Fitness::CalNorFitness(){
	PopModNorFit pop_mod_nor_fit;
	PopSumModFitSameRank pop_sum_mod_fit_same_rank;
	// Cal sum of modified fitness	
	for (auto it = pop_rank.begin(); it != pop_rank.end(); ++it) {
		double sum_mod_fit = 0.0;
		for (auto it2 = pop_rank.begin(); it2 != pop_rank.end(); ++it2) {
			if (it->first != it2->first && it->second == it2->second) {
				sum_mod_fit += pop_mod_fit.find(it->first)->second;
			}
		}
		pop_sum_mod_fit_same_rank.insert(pair<PopulationIdx, SumModFitSameRank>(it->first, sum_mod_fit));
	}
	for (int i = 0; i < POP_SIZE; ++i) {
		if(pop_sum_mod_fit_same_rank[i] == 0)
			pop_mod_nor_fit[i] = pop_mod_fit[i] * rank_num.find(pop_rank[i])->second * pop_fit[i];
		else
			pop_mod_nor_fit[i] = pop_mod_fit[i] * rank_num.find(pop_rank[i])->second * pop_fit[i] / pop_sum_mod_fit_same_rank[i];
	}
	// Set Into private variable
	this->pop_mod_nor_fit = pop_mod_nor_fit;
	this->pop_sum_mod_fit_same_rank = pop_sum_mod_fit_same_rank;

	return pop_mod_nor_fit;
}

PopFit Fitness::GetFitness(const Parent& parent) {
	// Ranking the population
	PopRank pop_rank = CalPopRank(parent);
	
	// Calculating fitness value
	PopFit pop_fit = CalFitness();

	// Calculating distance between two points in the solution space & sigma_share
	DotDistance dot_distance = CalDistance();

	// Calculating niche count
	PopNiche pop_niche = CalNicheCount();

	// Calculating modified fitness + Normalized
	PopModFit pop_mod_fit = CalModifiedFitness();

	// Calculating normalized fitness
	PopModNorFit pop_mod_nor_fit = CalNorFitness();

	//return pop_fit;
	return pop_mod_nor_fit;
}