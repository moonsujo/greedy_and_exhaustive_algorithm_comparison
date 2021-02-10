

#include "maxdefense.hh"
#include "timer.hh"
#include <iostream>

int main()
{
	auto all_armors = load_armor_database("armor.csv");
	assert( all_armors );
	//exhaustive algorithm can only handle n > 64
	//n = 6
	auto filtered_armors_1 = filter_armor_vector(*all_armors, 1, 2500, 6);
	std::cout << "setting timer" << std::endl;
	Timer timer;
	std::unique_ptr<ArmorVector> soln_greedy_1 = greedy_max_defense(*filtered_armors_1, 3000);
	std::cout <<"time elapsed for greedy solution 1: " << timer.elapsed() << std::endl;
	timer.reset();
	std::unique_ptr<ArmorVector> soln_exhaustive_1 = exhaustive_max_defense(*filtered_armors_1, 3000);
	std::cout << "time elapsed for exhaustive solution 1: " << timer.elapsed() << std::endl;
	timer.reset();

	//n = 12
	auto filtered_armors_2 = filter_armor_vector(*all_armors, 1, 2500, 12);
	std::unique_ptr<ArmorVector> soln_greedy_2 = greedy_max_defense(*filtered_armors_2, 3000);
	std::cout << "time elapsed for greedy solution 2: " << timer.elapsed() << std::endl;
	timer.reset();
	std::unique_ptr<ArmorVector> soln_exhaustive_2 = exhaustive_max_defense(*filtered_armors_2, 3000);
	std::cout << "time elapsed for exhaustive solution 2: " << timer.elapsed() << std::endl;
	timer.reset();

	//n= 18
	auto filtered_armors_3 = filter_armor_vector(*all_armors, 1, 2500, 18);
	std::unique_ptr<ArmorVector> soln_greedy_3 = greedy_max_defense(*filtered_armors_3, 3000);
	std::cout << "time elapsed for greedy solution 3: " << timer.elapsed() << std::endl;
	timer.reset();
	std::unique_ptr<ArmorVector> soln_exhaustive_3 = exhaustive_max_defense(*filtered_armors_3, 3000);
	std::cout << "time elapsed for exhaustive solution 3: " << timer.elapsed() << std::endl;
	timer.reset();

	//n=24
	auto filtered_armors_4 = filter_armor_vector(*all_armors, 1, 2500, 24);
	std::unique_ptr<ArmorVector> soln_greedy_4 = greedy_max_defense(*filtered_armors_4, 3000);
	std::cout << "time elapsed for greedy solution 4: " << timer.elapsed() << std::endl;
	timer.reset();
	std::unique_ptr<ArmorVector> soln_exhaustive_4 = exhaustive_max_defense(*filtered_armors_4, 3000);
	std::cout << "time elapsed for exhaustive solution 4: " << timer.elapsed() << std::endl;
	timer.reset();
	return 0;
}


