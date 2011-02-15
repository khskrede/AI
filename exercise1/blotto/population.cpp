
#include "population.h"
#include <time.h>
#include <math.h>
#include <stdlib.h> 
#include <iostream>
#include <stdio.h> 

Population::Population() {
}

void
Population::Init(unsigned int battles,
		 unsigned int soldiers,
		 float rf,
		 float lf,
	         unsigned int children_size,
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
   m_children = std::vector<Individual>(m_children_size, Individual(battles, soldiers, rf, lf));
   srand( time(0) );
   m_mutation_rate=mutation_rate;
   m_crossover_rate=crossover_rate;
   for( unsigned int i = 0; i < m_children.size(); i++ ) {
      m_children.at(i).Init();
   }
   
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
   /* Reset all individuals fitness to 0 */
   for ( unsigned int i = 0; i < m_children.size(); i++ )
      m_children.at(i).SetFitness(0);
   /* All individuals fight all other individuals and gain 1 in
    * fitness when winning a war */
   for ( unsigned int i = 0; i < m_children.size(); i++ ) {
      for ( unsigned int j = i+1; j < m_children.size(); j++ ) {
	 m_children.at(i).Evaluate(m_children.at(j));
      }
   }
}

void
Population::Select() {
   m_select(*this);
}

void
Population::Reproduce() {
   m_reproduce(*this);
   
}

void
Population::Mutate() {
   for ( unsigned int i = 0; i < m_children.size(); i++ ) {
      float flip = (float)rand()/(float)RAND_MAX;
      if ( flip <= m_mutation_rate) { 
         m_children.at(i).Mutate();
      }
   }
}

/* Statistics functions */

float
Population::GetFitnessSum() {
      float m_sum=0;
      for ( unsigned int i = 0; i < m_adults.size(); i++ ) {
	 m_sum += m_adults.at(i).GetFitness();
      }
   return m_sum;
}

float
Population::GetBestFit() {
      float best=0;
      for ( unsigned int i = 0; i < m_adults.size(); i++ ) {
	 if ( best < m_adults.at(i).GetFitness() ) {
	    best = m_adults.at(i).GetFitness();
	 }
      }
   return best;
}

float
Population::GetAverageFit() {
   float m_avg=0;
   for ( unsigned int i = 0; i < m_adults.size(); i++ ) {
      m_avg += m_adults.at(i).GetFitness();
   }
   m_avg /= m_adults.size();
   return m_avg ;
}

float
Population::GetStdDeviation() {
   float m_stddev = 0;
   float m_avg = GetAverageFit();
   for ( unsigned int i = 0; i < m_adults.size(); i++ ) {
      m_stddev += powf(m_adults.at(i).GetFitness()-m_avg, 2);
   }
   m_stddev = sqrt( m_stddev / m_adults.size() );
   return m_stddev;
}


float
Population::GetAvgStrategyEntropy() {

   float avg_entropy=0;

   for ( unsigned int i = 0; i < m_adults.size(); i++ ) {
      avg_entropy += m_adults.at(i).GetStrategyEntropy();
   }

   return avg_entropy / (float)m_adults.size();
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

   if ( m_adults.size() > 0 ) {
      Individual best=m_adults.at(0);
      float bestfit=best.GetFitness();
      
      for ( unsigned int i = 1; i < m_adults.size(); i++ ) {
	 if ( bestfit < m_adults.at(i).GetFitness() ) {
	    bestfit = m_adults.at(i).GetFitness();
	    best = m_adults.at(i);
	 }
      }
      return best;
   }
   else {
      return Individual(1,1,1,1);
   }
}
