
#include "individual.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

Individual::Individual(int battles, int soldiers, float rf, float lf) {
   m_battles=battles;
   m_soldiers=soldiers;
   m_rf=rf;
   m_lf=lf;

   m_genolength = m_battles * m_soldiers;
   m_phenolength = m_battles;
   
   m_mask=new bool[m_genolength];
   m_genotype=new bool[m_genolength];
   m_phenotype=new int[m_phenolength];
   
   m_fitness=0;
   m_reproduced=false;
   
   for (int i = 0; i < m_genolength; i++) {
      m_mask[i]=false;
      m_genotype[i] = false;
   }
   
   for (int i = 0; i < m_phenolength; i++) {
      m_phenotype[i]=0;
   }
}

Individual::Individual(int battles, int soldiers, float rf, float lf, bool* genotype ) {
   m_battles=battles;
   m_soldiers=soldiers;
   m_rf=rf;
   m_lf=lf;
   
   m_genolength = m_battles * m_soldiers;
   m_phenolength = m_battles;
   
   m_mask = new bool[m_genolength];
   m_genotype = new bool[m_genolength];
   m_phenotype = new int[m_phenolength];
   
   m_reproduced=false;
   m_fitness=0;
   for (int i = 0; i < m_genolength; i++) {
      m_mask[i]=false;
      m_genotype[i] = genotype[i];
   }

   for (int i = 0; i < m_phenolength; i++) {
      m_phenotype[i]=0;
   }
}

Individual::Individual( const Individual& ind ) {
   m_battles=ind.m_battles;
   m_soldiers=ind.m_soldiers;
   m_rf=ind.m_rf;
   m_lf=ind.m_lf;
   
   m_genolength = m_battles * m_soldiers;
   m_phenolength = m_battles;
   
   m_mask=new bool[m_genolength];
   m_genotype=new bool[m_genolength];
   m_phenotype=new int[m_phenolength];
   
   m_reproduced=ind.m_reproduced;
   m_fitness=ind.m_fitness;
   
   for (int i = 0; i < m_genolength; i++) {
      m_genotype[i] = ind.m_genotype[i];
      m_mask[i] = ind.m_mask[i];
   }
   for (int i = 0; i < m_phenolength; i++)
      m_phenotype[i] = ind.m_phenotype[i];
}

/*Individual::~Individual() {
   delete [] m_phenotype;
   delete [] m_genotype;
   delete [] m_mask;
}*/

void
Individual::Init() {
   /* Initialize individual with random genotype */
   m_fitness=0;
   m_reproduced=false;
   
   int soldiers=0;
   int selection;
   
   /* initialize genotypes to false */
   for ( int i = 0; i<m_genolength; i++ )
      m_genotype[i]=false;
   
   /* pick random places in the genotype to make true */
   while ( soldiers < m_soldiers ) {
      selection = rand() % (m_soldiers*m_battles);
      if (!m_genotype[selection]) {
         m_genotype[selection] = true;
         soldiers++;
      }
   }
}

void 
Individual::Develop() {
   /* Implement algorithm to develop genotypes into phenotypes */
   for( int i = 0; i<m_phenolength; i++ ) {
      m_phenotype[i]=0;
      for (int j = 0; j<m_soldiers; j++) {
         if ( m_genotype[i*m_soldiers+j] ) {
            m_phenotype[i]++;
         }
      }
   }
}

void 
Individual::Evaluate(Individual &ind) {
   /* Evaluate fitness */
   /* War simulation */
   
   int* opposing_pheno=ind.GetPhenotype();
   
   int battles_won = 0;
   int battles_lost = 0;
   
   int my_surviving_troops=0;
   float my_strength=1;
   int my_soldiers;
   float my_force;
   
   int opposing_surviving_troops=0;
   float opposing_strength=1;
   int opposing_soldiers;
   float opposing_force;
   
   for( int i = 0; i < m_phenolength; i++ ) {
      my_soldiers = m_phenotype[i] + my_surviving_troops;
      my_force = my_strength * (float)my_soldiers;
      
      opposing_soldiers = opposing_pheno[i] + opposing_surviving_troops;
      opposing_force = opposing_strength * (float)opposing_soldiers;
      
      if ( my_force > opposing_force ) { // Victory
         battles_won++;
         opposing_strength -= m_lf;
         
         my_surviving_troops = m_rf * (my_soldiers - opposing_soldiers);
      }
      else if ( my_force < opposing_force ) { // Loss
         battles_lost++;
         my_strength -= m_lf;
         
         opposing_surviving_troops = m_rf * (opposing_soldiers - my_soldiers); 
      }
   }
   if ( battles_won == battles_lost ) {
      m_fitness+=1;
      ind.SetFitness(ind.GetFitness()+1);
   }
   else if ( battles_won > battles_lost ) {
      m_fitness+=2;
   }
   else {
      ind.SetFitness(ind.GetFitness()+2);
   }
}

void 
Individual::Mutate() {
   for (int i = 0; i<m_soldiers*m_battles/2; i++) {
      int flip1 = rand() % m_genolength;
      int flip2 = rand() % m_genolength;
      while ( m_genotype[flip1] != m_genotype[flip2] ) {
	 flip2 = rand() % m_genolength;
      }
      bool temp = m_genotype[flip1];
      m_genotype[flip1]=m_genotype[flip2];
      m_genotype[flip2]=temp;
   }
}

Individual
Individual::Reproduce(Individual &mate) {
   
   bool *mategeno = mate.GetGenotype();
   
   if (mate.GetReproduced()) {
      
      for ( int i=0; i<m_genolength; i++ )
         m_mask[i] = mate.GetMask()[i];
         
      mate.SetReproduced(false);
   }
   
   else {
      bool meh[m_genolength];
      bool mah[m_genolength];
      int half=0;
      
      /* mark the different genotypes */
      for ( int i = 0; i < m_genolength; i++ ) {
         meh[i] = m_genotype[i] && !mategeno[i];
         mah[i] = !m_genotype[i] && mategeno[i];
	 if (meh[i]) half++;
      }
      
      for (int i = 0; i<m_genolength; i++) {
         m_mask[i]=false;
      }
      
      half/=2;
      
      for ( int i = 0; i < m_genolength && half!=0; i++ ) {
         if ( meh[i] && (rand() % 6 < 4) ) {
            for ( int j = 0; j < m_genolength && half!=0; j++ ) {
               if ( mah[j] && (rand() % 6 < 4) && !m_mask[j]) {
                  m_mask[i]=m_mask[j]=true;
		  half--;
                  break;
               }
            }
         }
      }
   }

   bool new_genotype[m_genolength];
   for( int i = 0; i < m_genolength; i++ ) {
      if( m_mask[i] ) {
         new_genotype[i]=mategeno[i];
      }
      else {
         new_genotype[i]=m_genotype[i];
      }
   }
   
   return Individual(m_battles, m_soldiers, m_rf, m_lf, new_genotype);
}

float 
Individual::GetStrategyEntropy() {
   
   float hs=0;
   float fraction=0;
   for (int i = 0; i<m_battles; i++) {
      if (m_phenotype[i] > 0) {
	 fraction = (float)m_phenotype[i] / (float)m_soldiers;
	 hs += fraction * log2(fraction); 
      }
   }
   return hs;
}

/* Getters and setters */

float 
Individual::GetFitness() {
   return m_fitness;
}

bool*
Individual::GetGenotype() {
        return m_genotype;
}

int*
Individual::GetPhenotype() {
        return m_phenotype;
}

void 
Individual::SetReproduced(bool reproduced) {
        m_reproduced=reproduced;
}

void 
Individual::SetFitness(float fitness) {
        m_fitness=fitness;
}

bool 
Individual::GetReproduced() {
        return m_reproduced;
}

bool*
Individual::GetMask() {
        return m_mask;
}

void
Individual::PrintString() {
   
   for ( int i = 0; i<m_phenolength; i++ ) {
      std::cout << (float)m_phenotype[i]/(float)m_soldiers << " ";
   }
   std::cout << " " << " \n";
   
}
