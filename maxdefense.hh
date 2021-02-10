///////////////////////////////////////////////////////////////////////////////
// maxdefense.hh
//
// Compute the set of armos that maximizes defense, within a gold budget,
// with the greedy method or exhaustive search.
//
///////////////////////////////////////////////////////////////////////////////


#pragma once


#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <vector>


// One armor item available for purchase.
class ArmorItem
{
	//
	public:
		
		//
		ArmorItem
		(
			const std::string& description,
			double cost_gold,
			double defense_points
		)
			:
			_description(description),
			_cost_gold(cost_gold),
			_defense_points(defense_points)
		{
			assert(!description.empty());
			assert(cost_gold > 0);
		}
		
		//
		const std::string& description() const { return _description; }
		double cost() const { return _cost_gold; }
		double defense() const { return _defense_points; }
	
	//
	private:
		
		// Human-readable description of the armor, e.g. "new enchanted helmet". Must be non-empty.
		std::string _description;
		
		// Cost, in units of gold; Must be positive
		double _cost_gold;
		
		// Defense points; most be non-negative.
		double _defense_points;
};


// Alias for a vector of shared pointers to ArmorItem objects.
typedef std::vector<std::shared_ptr<ArmorItem>> ArmorVector;


// Load all the valid armor items from the CSV database
// Armor items that are missing fields, or have invalid values, are skipped.
// Returns nullptr on I/O error.
std::unique_ptr<ArmorVector> load_armor_database(const std::string& path)
{
	std::unique_ptr<ArmorVector> failure(nullptr);
	
	std::ifstream f(path);
	if (!f)
	{
		std::cout << "Failed to load armor database; Cannot open file: " << path << std::endl;
		return failure;
	}
	
	std::unique_ptr<ArmorVector> result(new ArmorVector);
	
	size_t line_number = 0;
	for (std::string line; std::getline(f, line); )
	{
		line_number++;
		
		// First line is a header row
		if ( line_number == 1 )
		{
			continue;
		}
		
		std::vector<std::string> fields;
		std::stringstream ss(line);
		
		for (std::string field; std::getline(ss, field, '^'); )
		{
			fields.push_back(field);
		}
		
		if (fields.size() != 3)
		{
			std::cout
				<< "Failed to load armor database: Invalid field count at line " << line_number << "; Want 3 but got " << fields.size() << std::endl
				<< "Line: " << line << std::endl
				;
			return failure;
		}
		
		std::string
			descr_field = fields[0],
			cost_gold_field = fields[1],
			defense_points_field = fields[2]
			;
		
		auto parse_dbl = [](const std::string& field, double& output)
		{
			std::stringstream ss(field);
			if ( ! ss )
			{
				return false;
			}
			
			ss >> output;
			
			return true;
		};
		
		std::string description(descr_field);
		double cost_gold, defense_points;
		if (
			parse_dbl(cost_gold_field, cost_gold)
			&& parse_dbl(defense_points_field, defense_points)
		)
		{
			result->push_back(
				std::shared_ptr<ArmorItem>(
					new ArmorItem(
						description,
						cost_gold,
						defense_points
					)
				)
			);
		}
	}

	f.close();
	
	return result;
}


// Convenience function to compute the total cost and defense in an ArmorVector.
// Provide the ArmorVector as the first argument
// The next two arguments will return the cost and defense back to the caller.
void sum_armor_vector
(
	const ArmorVector& armors,
	double& total_cost,
	double& total_defense
)
{
	total_cost = total_defense = 0; //(1)
	for (auto& armor : armors) //(n)
	{
		total_cost += armor->cost(); //(2)
		total_defense += armor->defense(); //(2)
	}
}
//step count
//1 + n * 4
//4n + 1

// Convenience function to print out each ArmorItem in an ArmorVector,
// followed by the total kilocalories and protein in it.
void print_armor_vector(const ArmorVector& armors)
{
	std::cout << "*** Armor Vector ***" << std::endl;
	
	if ( armors.size() == 0 )
	{
		std::cout << "[empty armor list]" << std::endl;
	}
	else
	{
		for (auto& armor : armors)
		{
			std::cout
				<< "Ye olde " << armor->description()
				<< " ==> "
				<< "Cost of " << armor->cost() << " gold"
				<< "; Defense points = " << armor->defense()
				<< std::endl
				;
		}
		
		double total_cost, total_defense;
		sum_armor_vector(armors, total_cost, total_defense);
		std::cout
			<< "> Grand total cost: " << total_cost << " gold" << std::endl
			<< "> Grand total defense: " << total_defense
			<< std::endl
			;
	}
}


// Filter the vector source, i.e. create and return a new ArmorVector
// containing the subset of the armor items in source that match given
// criteria.
// This is intended to:
//	1) filter out armor with zero or negative defense that are irrelevant to our optimization
//	2) limit the size of inputs to the exhaustive search algorithm since it will probably be slow.
//
// Each armor item that is included must have at minimum min_defense and at most max_defense.
//	(i.e., each included armor item's defense must be between min_defense and max_defense (inclusive).
//
// In addition, the the vector includes only the first total_size armor items that match these criteria.
std::unique_ptr<ArmorVector> filter_armor_vector
(
	const ArmorVector& source,
	double min_defense,
	double max_defense,
	int total_size
)
{
	// TODO: implement this function, then delete the return statement below	
	std::unique_ptr<ArmorVector> filtered_source(new ArmorVector);
	//for each armor in copy from begin to total_size,
	for (auto& armor : source)
	{
		//can't chain comparisons
		if (max_defense > armor->defense() && armor->defense() > min_defense)
			filtered_source->push_back(armor);
		if (filtered_source->size() == total_size)
			break;
	}
	return filtered_source;
	//	if armor is greater than or equal to min_defense and less than or equal to max_defense,
	//		add to new vector
	//return pointer to new vector
	
}


// Compute the optimal set of armor items with a greedy algorithm.
// Specifically, among the armor items that fit within a total_cost gold budget,
// choose the armors whose defense is greatest.
// Repeat until no more armor items can be chosen, either because we've run out of armor items,
// or run out of gold.
std::unique_ptr<ArmorVector> greedy_max_defense
(
	const ArmorVector& armors,
	double total_cost
)
{
  // TODO: implement this function, then delete the return statement below
  //todo = copy of armors
  std::unique_ptr<ArmorVector> todo(new ArmorVector(armors));
  //print_armor_vector(*todo);
  //result = empty_vector
  std::unique_ptr<ArmorVector> result(new ArmorVector);
  //result_cost = 0
  float result_cost = 0; //(1)
  //while todo is not empty:
  while (!todo->empty())  { //(2n)
    std::shared_ptr<ArmorItem> max_armor = nullptr; //(1)
    double max_armor_value = 0; //(1)
    size_t max_armor_index = -1; //(1)
 

  //max_defense = 0 // defensiveness per cost
    int max_defense = 0; //(1)
    //int order = 0;

  //for armor in todo 
    for (size_t i = 0; i < todo->size(); i++ ) //(1 + (2n))
      {
	auto armor = todo->at(i); // (2)

  //	if result_cost + g <= total_cost then	
	if ( result_cost + armor->cost() <= total_cost ) // (3)
	  {
	    double armor_value = armor->defense() / armor->cost(); //(4)

  //	if armor's max_defense_value > max_defense_value then	    
	    if ( max_armor_index == -1 || armor_value > max_armor_value ) //(3)
	      {

  //		max_defense = armor
		max_armor_index = i; //(1)
		max_armor = armor; //(1)
		max_armor_value = armor_value; //(1)
	      }
	  }
      }
    // If we could not locate a suitable candidate, we're done
    if ( max_armor_index == -1 ) //(1)
      {
	break;
      }
    
   
  // result.add_back(armor) 
    result->push_back(max_armor); //(1)

  // result_cost += g
    result_cost += max_armor->cost(); //(2)

    //remove 'a' from todo
    todo->erase(todo->begin() + max_armor_index); //(3)
  }
 
  return result;
}
//algorithm analysis
//1 + 2n * (5 + 2n * (22))
//1 + 2n * (5 + 44n)
//1 + 10n + 88n^2
//88n^2 + 10n + 1
//O(n^2)

// Compute the optimal set of armor items with an exhaustive search algorithm.
// Specifically, among all subsets of armor items,
// return the subset whose gold cost fits within the total_cost budget,
// and whose total defense is greatest.
// To avoid overflow, the size of the armor items vector must be less than 64.
std::unique_ptr<ArmorVector> exhaustive_max_defense
(
	const ArmorVector& armors,
	double total_cost
)
{
	const int n = armors.size(); //(1)
	assert(n < 64); //(1)
	
	//best = nil
	std::unique_ptr<ArmorVector> best(new ArmorVector);
	//for i = 0 to 2^n -1,
	for (size_t i = 0; i < pow(2, n); i++) //(1 + 2 * 2^n)
	{
		std::unique_ptr<ArmorVector> candidate(new ArmorVector);
	
	//	for j from 0 to n-1,		
		for (int j = 0; j < n; j++) //(1 + 2n)
		{

	//		if ((bits >> j) & i) == 1
			if (((i >> j) & 1) == 1) //(3)	
			{
		
	//			candidate.add_back(armors[j])	
				candidate->push_back(armors.at(j)); //(1)
			}
		}
		double* total_cost_candidate = new double(); //(1)
		double* total_defense_candidate = new double(); //(1)
		sum_armor_vector(*candidate, *total_cost_candidate, *total_defense_candidate); //(4n + 1)
		double* total_cost_best = new double(); //(1)
		double* total_defense_best = new double(); //(1)
		sum_armor_vector(*best, *total_cost_best, *total_defense_best); //(4n + 1)

	//	if total_gold_cost(candidate) <= G then
		if (*total_cost_candidate <= total_cost) //(1)
		{

	//		if best is nil or total_defense(candidate) > total_defense(best) then
			if (best->empty() or *total_defense_candidate > *total_defense_best) //(3)
			{

	//			best = candidate
				*best = *candidate; //(1)
			}
		}
	}
	//return best
	return best;
}
//algorithm analysis
//2 + 1 + 2 * 2^(n) * (1 + 2n * (4) + 2 + 4n + 1 + 2 + 4n + 1 + 5)
//3 + 2 * 2^(n) * (12 + 16n)
//3 + 24 * 2^(n) + 32 * 2^(n) * n
//32 * 2^(n) * n + 24 * 2^(n) + 3





