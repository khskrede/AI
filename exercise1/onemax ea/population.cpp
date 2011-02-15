
#include "population.h"
#include <time.h>
#include <math.h>
#include <cstdlib> 

Population::Population() {};

void
Population::Init(unsigned int children_size,
		       unsigned int adults_size,
		       float mutation_rate, 
		       float crossover_rate, 
		       void(*selection)(Population&),
		       void(*reproduction)(Population&),
		       unsigned int elitism,
		       float temperature,
		       unsigned int tournament_size ) {

   /* initialize Population */
   m_children_size = children_size;
   m_adults_size = adults_size;
   m_temperature = temperature;
   m_elitism = elitism;
   m_tournament_size = tournament_size;
   
   m_select = selection;
   m_reproduce = reproduction;
   m_children = std::vector<Individual>(m_children_size, Individual());
   srand( time(0) );
   m_mutation_rate=mutation_rate;
   m_crossover_rate=crossover_rate;
   for( unsigned int i = 0; i < m_children.size(); i++ ) {
      m_children.at(i).Init();
   }
   
   m_sum_set=false;
   m_avg_set=false;
   m_stddev_set=false;
   m_best_set=false;
}

/* Main loop functions */

void
Population::Develop() {
   for ( unsigned int i = 0; i < m_children.size(); i++ ) {
      m_children.at(i).Develop();
   }
}

void
Population::Fitness() {
   /* All individuals fight all other individuals */
   for ( unsigned int i = 0; i < m_children.size(); i++ ) {
      m_children.at(i).Evaluate();
   }
}

void
Population::Select() {
   m_select(*this);
}

void
Population::Reproduce() {
   m_reproduce(*this);
   
   m_sum_set=false;
   m_avg_set=false;
   m_stddev_set=false;
   m_best_set=false;
}

void
Population::Mutate() {
   for ( unsigned int i = 0; i < m_children.size(); i++ ) {
      float flip = (float)rand()/(float)RAND_MAX;
      if ( flip < m_mutation_rate) { 
         m_children.at(i).Mutate();
      }
   }
}

/* Statistics functions */

float
Population::GetFitnessSum() {
   if ( !m_sum_set ) {
      m_sum=0;
      for ( unsigned int i = 0; i < m_adults.size(); i++ ) {
	 m_sum += m_adults.at(i).GetFitness();
      }
      m_sum_set=true;
   }
   return m_sum;
}

float
Population::GetBestFit() {
   if ( !m_best_set ) {
      m_best=0;
      for ( unsigned int i = 0; i < m_adults.size(); i++ ) {
	 if ( m_best < m_adults.at(i).GetFitness() ) {
	    m_best = m_adults.at(i).GetFitness();
	    m_best_individual = m_adults.at(i);
	 }
      }
      m_best_set=true;
   }
   return m_best;
}

float
Population::GetAverageFit() {
   if ( !m_avg_set ) {
      m_avg=0;
      for ( unsigned int i = 0; i < m_adults.size(); i++ ) {
	 m_avg += m_adults.at(i).GetFitness();
      }
      m_avg /= m_adults.size();
      m_avg_set = true;
   }
   return m_avg ;
}

float
Population::GetStdDeviation() {
   if ( !m_stddev_set ) {
      m_stddev = 0;
      m_avg = GetAverageFit();
      for ( unsigned int i = 0; i < m_adults.size(); i++ ) {
	 m_stddev += powf(m_adults.at(i).GetFitness()-m_avg, 2);
      }
      m_stddev_set = true;
      m_stddev = sqrt( m_stddev / m_adults.size() );
   }
   return m_stddev;
}

/* Getters and setters */

std::vector<Individual>&
Population::GetAdults() {
   return m_adults;
}

void
Population::SetAdults( std::vector<Individual> adults ) {
   m_adults = adults;
}

std::vector<Individual>&
Population::GetChildren() {
   return m_children;
}

void
Population::SetChildren( std::vector<Individual> children ) {
   m_children = children;
}

unsigned int
Population::GetChildrenSize() {
   return m_children_size;
}

unsigned int
Population::GetAdultsSize() {
   return m_adults_size;
}

float
Population::GetTemperature() {
   return m_temperature;
}

float
Population::GetCrossoverRate() {
   return m_crossover_rate;
}

unsigned int
Population::GetElitism() {
   return m_elitism;
}

unsigned int
Population::GetTournamentSize() {
   return m_tournament_size;
}

Individual
Population::GetBestIndividual() {
   if ( !m_best_set ) {
      GetBestFit();
   }
   return m_best_individual;
}
