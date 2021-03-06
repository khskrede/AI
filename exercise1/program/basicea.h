#ifndef GENERIC_EA_H
#define GENERIC_EA_H

#include "population.h"
#include <iostream>
#include <fstream>

class BasicEA {

	private:

	Population *m_population;
	int m_generations;
	
   std::ofstream out_file;
	
	public:
   
	BasicEA(unsigned int children_size, 
			  unsigned int adults_size, 
			  unsigned int generations, 
			  float mutation_rate, 
			  float crossover_rate, 
			  unsigned int selection, 
			  unsigned int reproduction, 
			  unsigned int elitism, 
			  float temperature,
			  unsigned int tournaments);
			  
	~BasicEA();

	void Run();
	void DumpStats( int i );

};

#endif
