#pragma once

#ifndef CLONE_H_
#define CLONE_H_

#include <vector>
#include <random>
#include <algorithm>
#include <functional>
#include <iterator>   

#include "../schedule.h"
#include "../util.h"

using namespace std;

class Clone {

public:
	Parent ClonePopulation(const SolutionSet& solution_set, Parent& parent);
	
	// Get
	vector<LotNumber> Get_lot_numbers();
	PopMachineLoad Get_pop_machineLoad();

	// Set
	bool Set_pop_machineLoad(PopMachineLoad);
private:
	PopFit pop_fit;
	vector<LotNumber> lot_numbers;
	PopMachineLoad pop_machineLoad;
};


#endif