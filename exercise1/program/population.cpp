
#include "population.h"
#include <time.h>
#include <math.h>
#include <cstdlib> 
#include <iostream>

Population::Population(unsigned int children_size,
							  unsigned int adults_size,
							  float mutation_rate, 
							  void(*selection)(Population&),
							  void(*reproduction)(Population&),
							  float temperature ) {

   /* initialize Population */
   m_children_size = children_size;
   m_adults_size = adults_size;
   m_temperature = temperature;
   
   m_select = selection;
   m_reproduce = reproduction;
   m_children = std::vector<Individual>(m_children_size, Individual());
   srand( time(0) );
   m_mutation_rate=mutation_rate;
   for( unsigned int i = 0; i < m_children.size(); i++ ) {
      m_children.at(i).Init();
   }
   
   sum_set=false;
   avg_set=false;
   stddev_set=false;
   best_set=false;
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

   sum_set=false;
   avg_set=false;
   stddev_set=false;
   best_set=false;
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
	if ( !sum_set ) {
		sum=0;
		for ( unsigned int i = 0; i < m_adults.size(); i++ ) {
			sum += m_adults.at(i).GetFitness();
		}
		sum_set=true;
	}
   return sum;
}

float
Population::GetBestFit() {
	if ( !best_set ) {
		best=0;
		for ( unsigned int i = 0; i < m_adults.size(); i++ ) {
			if ( best < m_adults.at(i).GetFitness() ) best = m_adults.at(i).GetFitness();
		}
		best_set=true;
	}
   return best;
}

float
Population::GetAverageFit() {
	if ( !avg_set ) {
		avg=0;
		for ( unsigned int i = 0; i < m_adults.size(); i++ ) {
			avg += m_adults.at(i).GetFitness();
		}
		avg /= m_adults.size();
		avg_set = true;
	}
   return avg ;
}

float
Population::GetStdDeviation() {
	if ( !stddev_set ) {
		stddev = 0;
		avg = GetAverageFit();
		for ( unsigned int i = 0; i < m_adults.size(); i++ ) {
			stddev += powf(m_adults.at(i).GetFitness()-avg, 2);
		}
		stddev_set = true;
		stddev = sqrt( stddev / m_adults.size() );
	}
   return stddev;
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
