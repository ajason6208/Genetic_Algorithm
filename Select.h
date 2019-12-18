#pragma once

#ifndef SELECT_H_
#define SELECT_H_

#include <vector>
#include <random>
#include "../schedule.h"
#include "../util.h"

using namespace std;

class Select {

public:
	Parent InitPopulation(const SolutionSet& solution_set, const int& pop_size);
	Parent SamplingPopulation(Parent& parent, const int& sampling_size);
private:
	vector<PairPopCost> cost; // pair(PopulationIdx, cost)
	vector<PairPopRemainder> remainder; // pair(PopulationIdx, remainder)
};


#endif