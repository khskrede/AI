
#include "individual.h"
#include <iostream>
#include <stdlib.h>

Individual::Individual() {
}

void
Individual::Init() {
   /* Initialize individual with random genotype */
   m_fitness=0;
   m_reproduced=false;
   for( int i = 0; i<m_genolength; i++ ) {
      m_genotype[i] = rand() % 2 == 1;
   }
}

Individual::Individual( bool* genotype ) {
   m_fitness=0;
   for (int i = 0; i < m_genolength; i++) {
      m_genotype[i] = genotype[i];
   }
}

void 
Individual::Develop() {
   /* Implement algorithm to develop genotypes into phenotypes */
   for( int i = 0; i<m_phenolength; i++ ) {
      m_phenotype[i]=0;
      for (int j = 0; j<4; j++) {
	 if (m_genotype[i*4+j]) {
	    m_phenotype[i]++;
	 }
      }
   }
}

void 
Individual::Evaluate() {
   /* Evaluate fitness */
   m_fitness=0;
   for( int i = 0; i < m_phenolength; i++ ) {
      m_fitness+=m_phenotype[i];
   }
}

void 
Individual::Mutate() {
   /* Mutate genotype */
   int flip = rand() % m_genolength;
   m_genotype[flip] = !m_genotype[flip];
}

Individual
Individual::Reproduce(Individual mate) {

   if (mate.GetReproduced()) {
      for ( int i=0; i<m_genolength; i++ )
	 m_mask[i] = mate.GetMask()[i];
      mate.SetReproduced(false); 
   }
   else {
      SetReproduced(true);
      for( int i = 0; i<m_genolength; i++ )
	 m_mask[i] = rand() % 2 == 1;
   }

   bool *mategeno = mate.GetGenotype();
   bool new_genotype[m_genolength];
   for( int i = 0; i<m_genolength; i++ ) {
      new_genotype[i] = 
      (m_mask[i] && m_genotype[i]) || (!m_mask[i] && mategeno[i]);
   }
   
   return Individual(new_genotype);
}

float 
Individual::GetFitness() {
   return m_fitness;
}

bool*
Individual::GetGenotype() {
	return m_genotype;
}

void 
Individual::SetReproduced(bool reproduced) {
	m_reproduced=reproduced;
}

bool 
Individual::GetReproduced() {
	return m_reproduced;
}

bool*
Individual::GetMask() {
	return m_mask;
}
