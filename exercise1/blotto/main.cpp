
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
	<< "1: Number of battles per war [integer]\n"
	<< "2: Number of soldiers to be deployed per war [integer]\n"
	<< "3: Reployment fraction [float between 0.0 and 1.0]\n"
	<< "4: Strength reduction fraction [float between 0.0 and 1.0]\n"
	<< "\n" 
	<< "5: Size of child pool [integer]\n" 
	<< "6: Size of adult pool [integer]\n" 
	<< "7: number of generations [integer]\n" 
	<< "8: mutation rate [float between 0.0 and 1.0]\n"
	<< "9: crossover rate [float between 0.0 and 1.0]\n"

	<< "10: selection protocol [integer between 1 and 3]\n"
	<< "   1: Full Generational replacement (parameter 1 and 2 must be equal)\n"
	<< "   2: Over-production (parameter 1 must be larger than paramter 2)\n"
	<< "   3: Generational-mixing\n"
	
	<< "11: selection strategy [integer between 1 and 5]\n"
	<< "   1: Fitness proportionate\n"
	<< "   2: Sigma scaling\n"
	<< "   3: Boltzmann selection\n"
	<< "   4: Rank selection\n"
	<< "   5: Tournament selection\n"
	
	<< "12: Elitism [integer] \n"
	<< "13: If boltzmann selection: Temperature [float greater than 0]\n"
	<< "   If tournament selection: Tournament size [integer greater than 2 and less than half the size of adult pool]"
	<< "\n" 
	<< "*******************\n"
	<< "\n";
}

void PrintInfo(unsigned int battles,
					unsigned int soldiers,
					float rf,
					float lf,
					unsigned int children_size,
					unsigned int adults_size,
					unsigned int num_generations,
					float mutation_rate,
					float crossover_rate,
					unsigned int selection_protocol,
					unsigned int selection_strategy,
					unsigned int elitism,
					float temperature,
					unsigned int tournament_size) {
	
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
		case 5 : selection_strategy_string = std::string("Tournament selection");
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
	<< "1: Number of battles per war: " << battles << "\n" 
	<< "2: Number of soldiers per war: " << soldiers << "\n" 
	<< "3: Reployment fraction: " << rf << "\n" 
	<< "4: Strength reduction fraction: " << lf << "\n" 
	<< "\n" 
	<< "5: Size of child pool: " << children_size << "\n" 
	<< "6: Size of adult pool: " << adults_size << "\n" 
	<< "7: number of generations: " << num_generations << "\n" 
	<< "8: mutation rate: " << mutation_rate << "\n" 
	<< "9: crossover rate: " << crossover_rate << "\n" 
	<< "10: selection protocol: " << selection_protocol_string << "\n" 
	<< "11: selection strategy: " << selection_strategy_string << "\n" 
	<< "12: elitism: " << elitism << "\n" ;
	if ( selection_strategy == 3 ) {
		std::cout 	<< "9: Temperature: " << temperature << "\n" ;
	}
	else if ( selection_strategy == 5 ) {
		std::cout 	<< "9: Tournament size: " << tournament_size << "\n" ;
	}
	std::cout
	<< "\n" 
	<< "*******************\n"
	<< "\n";

	plot_info 
	<< "\n"
	<< "*******************\n" 
	<< "Plotting of evolutionary algorithm with settings: \n"
	<< "*******************\n"
	<< "\n" 
	<< "1: Number of battles per war: " << battles << "\n" 
	<< "2: Number of soldiers per war: " << soldiers << "\n" 
	<< "3: Reployment fraction: " << rf << "\n" 
	<< "4: Strength reduction fraction: " << lf << "\n" 
	<< "\n" 
	<< "5: Size of child pool: " << children_size << "\n" 
	<< "6: Size of adult pool: " << adults_size << "\n" 
	<< "7: number of generations: " << num_generations << "\n" 
	<< "8: mutation rate: " << mutation_rate << "\n" 
	<< "9: crossover rate: " << crossover_rate << "\n" 
	<< "10: selection protocol: " << selection_protocol_string << "\n" 
	<< "11: selection strategy: " << selection_strategy_string << "\n" 
	<< "12: elitism: " << elitism << "\n" ;
	if ( selection_strategy == 3 ) {
		plot_info 	<< "13: Temperature: " << temperature << "\n" ;
	}
	else if ( selection_strategy == 5 ) {
		plot_info 	<< "13: Tournament size: " << tournament_size << "\n" ;
	}
	plot_info
	<< "\n" 
	<< "*******************\n"
	<< "\n";

}

int main(int argc, char *argv[]) {
	
	int success=0;

	unsigned int battles;
	unsigned int soldiers;
	float rf;
	float lf;
					
	unsigned int children_size;
	unsigned int adults_size;
	unsigned int num_generations;
	float mutation_rate;
	float crossover_rate;
	unsigned int selection_protocol;
	unsigned int selection_strategy;
	unsigned int elitism;
	float temperature=1;
	unsigned int tournament_size=2;

	if (argc == 14) {
		// read in battle parameters
		battles = strtoul(argv[1],0,0);
		soldiers = strtoul(argv[2],0,0);
		rf = strtof(argv[3],0);
		lf = strtof(argv[4],0);
		
		// read in EA parameters
		children_size = strtoul(argv[5],0,0);
		adults_size = strtoul(argv[6],0,0);
		num_generations = strtoul(argv[7],0,0);
		mutation_rate = strtof(argv[8],0);
		crossover_rate = strtof(argv[9],0);
		selection_protocol = strtoul(argv[10],0,0);
		selection_strategy = strtoul(argv[11],0,0);
		elitism = strtoul(argv[12],0,0);
		if ( selection_strategy == 3 ) { // boltzmann
			temperature = strtof(argv[13],0);
		}
		else if ( selection_strategy == 5 ) { // tournament
			tournament_size = strtoul(argv[13],0,0);
		}
		
		if ( (adults_size >= children_size && selection_protocol == 2) || // Over production
			  (adults_size != children_size && selection_protocol == 1) || // Full generational replacement
			  mutation_rate<0 || mutation_rate>1 || 
			  crossover_rate<0 || crossover_rate>1 || 
			  selection_protocol<1 || selection_protocol>3 || 
			  selection_strategy<1 || selection_strategy>5 ||
			  elitism > children_size ||
			  temperature<=0 ||
			  tournament_size<=1 || tournament_size >adults_size/2 ) {
			PrintHelp();
			success=1;
		}
		else {
			PrintInfo(battles, soldiers, rf, lf, children_size, adults_size, num_generations, mutation_rate, crossover_rate, selection_protocol, selection_strategy, elitism, temperature, tournament_size);
			BasicEA ea(battles, soldiers, rf, lf, children_size, adults_size, num_generations, mutation_rate, crossover_rate, selection_protocol, selection_strategy, elitism, temperature, tournament_size);
			ea.Run();
		}
	}
	else if (argc == 13) {
		// read in battle parameters
		battles = strtoul(argv[1],0,0);
		soldiers = strtoul(argv[2],0,0);
		rf = strtof(argv[3],0);
		lf = strtof(argv[4],0);
		
		// read in EA parameters
		children_size = strtoul(argv[5],0,0);
		adults_size = strtoul(argv[6],0,0);
		num_generations = strtoul(argv[7],0,0);
		mutation_rate = strtof(argv[8],0);
		crossover_rate = strtof(argv[9],0);
		selection_protocol = strtoul(argv[10],0,0);
		selection_strategy = strtoul(argv[11],0,0);
		elitism = strtoul(argv[12],0,0);
		if ( (adults_size >= children_size && selection_protocol == 2) || // Over production
			  (adults_size != children_size && selection_protocol == 1) || // Full generational replacement
			  mutation_rate<0 || mutation_rate>1 || 
			  crossover_rate<0 || crossover_rate>1 || 
			  selection_strategy == 3 || 
			  selection_protocol<1 || selection_protocol>3 || 
			  selection_strategy<1 || selection_strategy>4 ||
			  elitism > children_size) {
			PrintHelp();
			success=1;
		}
		else {
			PrintInfo(battles, soldiers, rf, lf, children_size, adults_size, num_generations, mutation_rate, crossover_rate, selection_protocol, selection_strategy, elitism, 0, 0);
			BasicEA ea(battles, soldiers, rf, lf, children_size, adults_size, num_generations, mutation_rate, crossover_rate, selection_protocol, selection_strategy, elitism, 0, 0);
			ea.Run();
		}
	}
	else {
		
		PrintHelp();
		success=1;
	}

   return success;
}

