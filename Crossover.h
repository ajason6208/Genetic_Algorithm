#pragma once

#ifndef CROSSOVER_H_
#define CROSSOVER_H_

#include <vector>
#include <random>
#include <algorithm>
#include <functional>

#include "../schedule.h"
#include "../util.h"
#include "../Clone/Clone.h"

using namespace std;

class Crossover {

public:
	Parent DoCrossOver(Clone& cl_obj, PopFit& pop_fit, Parent& parent);
private:
	vector<LotNumber> lot_numbers;
	PopMachineLoad pop_machineLoad;
};

#endif