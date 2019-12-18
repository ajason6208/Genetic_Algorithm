#pragma once

#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#include "schedule.h"

using namespace std;

//--------------- Define Block ----------------//
#define FILE_PATH  (string) "......./Result_can_schedule_data_for_read.txt"
//----------------------------------------------


//-------- Genetic Algorithm Parameter --------//
#define N_GENERATIONS 300
#define POP_SIZE 300
#define SAMPLING_SIZE 300
#define IMPORTANCE 2 
#define MUTATIONLAYER 4
#define CLONE 4
#define NUMBERTOCHOOSE 5 
//----------------------------------------------

//--------- Constrain Parameter ---------//
#define M_LOAD_SX40 608000
#define M_LOAD_PRESS 960000
#define M_LOAD_RF70 486000
#define M_LOAD_S350 425000

//----------------------------------------------


//--------------- Struct Block ----------------//
struct ProductResource
{
	int no_machine;					
	double weighting;				
	int bag;						
	double excess_material;			
	double excess_material_rate;	
	double cost;					
	double load;					
};
struct KPI {
	double sum_cost;
	double sum_remainder;
};
//----------------------------------------------


//--------------- Typedef Block ----------------//
typedef int LotNumber, CombNumber, PopulationIdx, Rank, Num;
typedef double Fit, Cost, Remainder, Distance, MaxCost, MinCost, MaxRemainder, MinRemainder, SigmaShare, NicheCount, SumModFitSameRank;
typedef vector<double> MachineLoad;
typedef map<LotNumber, map<CombNumber, ProductResource>> SolutionSet;
typedef map<PopulationIdx, SolutionSet> Parent;
typedef map<PopulationIdx, Rank> PopRank;
typedef map<PopulationIdx, Fit> PopFit, PopModFit, PopModNorFit;
typedef map<PopulationIdx, Cost> PopCost;
typedef map<PopulationIdx, Remainder> PopRemainder;
typedef map<Rank, Num> RankNum;
typedef map<pair<PopulationIdx, PopulationIdx>, Distance> DotDistance;
typedef map<PopulationIdx, NicheCount> PopNiche;
typedef map<PopulationIdx, SumModFitSameRank> PopSumModFitSameRank;
typedef pair<PopulationIdx, Cost> PairPopCost;
typedef pair<PopulationIdx, Remainder> PairPopRemainder;
typedef pair<PopulationIdx, Fit> PairPopFit;
typedef map<PopulationIdx, MachineLoad> PopMachineLoad;
//----------------------------------------------

#endif