
#include "basicea.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>

void PrintHelp() {
	std::cout 
	<< "\n"
	<< "*******************\n" 
	<< "You need to fill in the required paramters correctly: \n"
	<< "*******************\n"
	<< "\n" 
	<< "1: Size of child pool [integer]\n" 
	<< "2: Size of adult pool [integer]\n" 
	<< "3: number of generations [integer]\n" 
	<< "4: mutation rate [float between 0.0 and 1.0]\n"

	<< "5: selection protocol [integer between 1 and 3]\n"
	<< "   1: Full Generational replacement (parameter 1 and 2 must be equal)\n"
	<< "   2: Over-production (parameter 1 must be larger than paramter 2)\n"
	<< "   3: Generational-mixing\n"
	
	<< "6: selection strategy [integer between 1 and 4]\n"
	<< "   1: Fitness proportionate\n"
	<< "   2: Sigma scaling\n"
	<< "   3: Boltzmann selection\n"
	<< "   4: Rank selection\n"
	
	<< "7: Temperature [float greater than 0] (only necessary when using Boltzmann selection)\n"
	<< "\n" 
	<< "*******************\n"
	<< "\n";
}

void PrintInfo(unsigned int children_size,
					unsigned int adults_size,
					unsigned int num_generations,
					float mutation_rate,
					unsigned int selection_protocol,
					unsigned int selection_strategy,
					float temperature) {
	
	std::string selection_protocol_string;
	std::string selection_strategy_string;
	
	switch ( selection_protocol ) {
		case 1 : selection_protocol_string = std::string("Full generational replacement");
					break;
		case 2 : selection_protocol_string = std::string("Over-production");
					break;
		case 3 : selection_protocol_string = std::string("Generational-mixing");
					break;
		default : selection_protocol_string = std::string("Full generational replacement");
					break;
	}
	
	switch ( selection_strategy ) {
		case 1 : selection_strategy_string = std::string("Fitness proportionate");
					break;
		case 2 : selection_strategy_string = std::string("Sigma scaling");
					break;
		case 3 : selection_strategy_string = std::string("Boltzmann selection");
					break;
		case 4 : selection_strategy_string = std::string("Rank selection");
					break;
					
		default : selection_strategy_string = std::string("Fitness proportionate");
					break;
	}
	
   std::ofstream plot_info;
  	plot_info.open("data/ea.info", std::ios::out);
	
	std::cout 
	<< "\n"
	<< "*******************\n" 
	<< "Plotting of evolutionary algorithm with settings: \n"
	<< "*******************\n"
	<< "\n" 
	<< "1: Size of child pool: " << children_size << "\n" 
	<< "2: Size of adult pool: " << adults_size << "\n" 
	<< "3: number of generations: " << num_generations << "\n" 
	<< "4: mutation rate: " << mutation_rate << "\n" 
	<< "5: selection protocol: " << selection_protocol_string << "\n" 
	<< "6: selection strategy: " << selection_strategy_string << "\n" 
	<< "7: Temperature: " << temperature << "\n" 
	<< "\n" 
	<< "*******************\n"
	<< "\n";

	plot_info 
	<< "Plotting of evolutionary algorithm with settings: \n"
	<< "1: Size of child pool: " << children_size << "\n" 
	<< "2: Size of adult pool: " << adults_size << "\n" 
	<< "3: number of generations: " << num_generations << "\n" 
	<< "4: mutation rate: " << mutation_rate << "\n" 
	<< "5: selection protocol: " << selection_protocol_string << "\n" 
	<< "6: selection strategy: " << selection_strategy_string << "\n" 
	<< "7: Temperature: " << temperature << "\n";

}

int main(int argc, char *argv[]) {
	
	int success=0;
	
	unsigned int children_size;
	unsigned int adults_size;
	unsigned int num_generations;
	float mutation_rate;
	unsigned int selection_protocol;
	unsigned int selection_strategy;
	float temperature;

	if (argc == 8) {
		children_size = strtoul(argv[1],0,0);
		adults_size = strtoul(argv[2],0,0);
		num_generations = strtoul(argv[3],0,0);
		mutation_rate = strtof(argv[4],0);
		selection_protocol = strtoul(argv[5],0,0);
		selection_strategy = strtoul(argv[6],0,0);
		temperature = strtof(argv[7],0);
		
		if ( (adults_size >= children_size && selection_protocol == 2) || // Over production
			  (adults_size != children_size && selection_protocol == 1) || // Full generational replacement
			  mutation_rate<0 || mutation_rate>1 || 
			  selection_protocol<1 || selection_protocol>3 || 
			  selection_strategy<1 || selection_strategy>4 ||
			  temperature<=0) {
			PrintHelp();
			success=1;
		}
		else {
			PrintInfo(children_size, adults_size, num_generations, mutation_rate, selection_protocol, selection_strategy, temperature);
			BasicEA ea(children_size, adults_size, num_generations, mutation_rate, selection_protocol, selection_strategy, temperature);
			ea.Run();
		}
	}
	else if (argc == 7) {
		children_size = strtoul(argv[1],0,0);
		adults_size = strtoul(argv[2],0,0);
		num_generations = strtoul(argv[3],0,0);
		mutation_rate = strtof(argv[4],0);
		selection_protocol = strtoul(argv[5],0,0);
		selection_strategy = strtoul(argv[6],0,0);
		if ( (adults_size >= children_size && selection_protocol == 2) || // Over production
			  (adults_size != children_size && selection_protocol == 1) || // Full generational replacement
			  mutation_rate<0 || mutation_rate>1 || 
			  selection_strategy == 3 || 
			  selection_protocol<1 || selection_protocol>3 || 
			  selection_strategy<1 || selection_strategy>4 ) {
			PrintHelp();
			success=1;
		}
		else {
			PrintInfo(children_size, adults_size, num_generations, mutation_rate, selection_protocol, selection_strategy, 0);
			BasicEA ea(children_size, adults_size, num_generations, mutation_rate, selection_protocol, selection_strategy, 0);
			ea.Run();
		}
	}
	else {
		PrintHelp();
		success=1;
	}

   return success;
}

