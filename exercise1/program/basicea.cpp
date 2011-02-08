
#include "basicea.h"
#include "strategies.h"

BasicEA::BasicEA(unsigned int children_size, 
					  unsigned int adults_size, 
					  unsigned int generations, 
					  float mutation_rate, 
					  float crossover_rate, 
					  unsigned int selection, 
					  unsigned int reproduction, 
					  unsigned int elitism,
					  float temperature,
					  unsigned int tournaments) {
						  
	m_generations = generations;	
	
	void (*selection_strat)( Population &pop );
	void (*reproduction_strat)( Population &pop );
	
	switch ( selection ) {
		case 1 : selection_strat = Strategies::FullGenerationalReplacement;
					break;
		case 2 : selection_strat = Strategies::OverProduction;
					break;
		case 3 : selection_strat = Strategies::GenerationalMixing;
					break;
					
		default : selection_strat = Strategies::FullGenerationalReplacement;
					break;
	}
	
	switch ( reproduction ) {
		case 1 : reproduction_strat = Strategies::FitnessProportionate;
					break;
		case 2 : reproduction_strat = Strategies::SigmaScaling;
					break;
		case 3 : reproduction_strat = Strategies::BoltzmannSelection;
					break;
		case 4 : reproduction_strat = Strategies::RankSelection;
					break;
		case 5 : reproduction_strat = Strategies::TournamentSelection;
					break;
					
		default : reproduction_strat = Strategies::FitnessProportionate;
					break;
	}
	
	m_population = new Population(children_size, 
											adults_size, 
											mutation_rate, 
											crossover_rate, 
											selection_strat, 
											reproduction_strat, 
											elitism,
											temperature,
											tournaments);
	
	out_file.open("data/ea.data", std::ios::out);
}

BasicEA::~BasicEA() {
}

void
BasicEA::Run() {
	for (int i = 0; i < m_generations; i++) {
		m_population->Develop();
		m_population->Fitness();
		m_population->Select();
		m_population->Reproduce();
		m_population->Mutate();
		DumpStats(i);
	}
}

void BasicEA::DumpStats( int i ) {
		out_file << i << " "
		<< m_population->GetBestFit() << " " 
		<< m_population->GetAverageFit() << " "
		<< m_population->GetStdDeviation() << "\n";	
}
