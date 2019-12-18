#pragma once

#ifndef FITNESS_H_
#define FITNESS_H_

#include <vector>
#include <random>
#include <algorithm>
#include <functional>

#include "../schedule.h"
#include "../util.h"

using namespace std;

class Fitness {

public:
	PopFit GetFitness(const Parent& parent);
private:
	PopRank CalPopRank(const Parent& parent);
	PopFit CalFitness();
	DotDistance CalDistance();
	PopNiche CalNicheCount();
	PopModFit CalModifiedFitness();
	PopModNorFit CalNorFitness();

	PopCost pop_cost;
	PopRemainder pop_remainder;
	PopRank pop_rank; 
	RankNum rank_num;
	PopFit pop_fit;
	DotDistance dot_distance;
	MaxCost max_cost;
	MinCost min_cost;
	MaxRemainder max_remainder;
	MinRemainder min_remainder;
	SigmaShare sigma_share;
	PopNiche pop_niche;
	PopModFit pop_mod_fit;
	PopModNorFit pop_mod_nor_fit;
	PopSumModFitSameRank pop_sum_mod_fit_same_rank;
};


#endif