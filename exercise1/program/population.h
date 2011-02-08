#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include "individual.h"

class Population {
   
   private:
   float m_mutation_rate;
   float m_crossover_rate;
   unsigned int m_elitism;
   unsigned int m_tournament_size;
   
   int m_children_size;
   int m_adults_size;
   float m_temperature;
   
   std::vector<Individual> m_children;
   std::vector<Individual> m_adults;
   
   void (*m_select)( Population &pop );
   void (*m_reproduce)( Population &pop );
   
   float sum;
   float avg;
   float stddev;
   float best;
   
   bool best_set;
   bool sum_set;
   bool avg_set;
   bool stddev_set;
   
   public:
   
   Population(unsigned int children_size, 
	      unsigned int adults_size, 
	      float mutation_rate, 
	      float crossover_rate,
	      void (*selection)( Population &pop ),
	      void (*reproduction)( Population &pop),
	      unsigned int elitism,
	      float temperature,
	      unsigned int tournament_size);
				  
   void Develop();
   void Fitness();
   void Mutate();
   void Select();
   void Reproduce();
   
   float GetBestFit();
   float GetFitnessSum();
   float GetAverageFit();
   float GetStdDeviation();
   void SetChildren( std::vector<Individual> children );
   std::vector<Individual>& GetChildren();
   void SetAdults( std::vector<Individual> adults );
   std::vector<Individual>& GetAdults();
   
   unsigned int GetChildrenSize();
   unsigned int GetAdultsSize();
	
   float GetTemperature();
   float GetCrossoverRate();
   unsigned int GetElitism();
   unsigned int GetTournamentSize();

};

#endif
