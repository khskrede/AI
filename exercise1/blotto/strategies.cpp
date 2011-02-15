
#include "strategies.h"
#include "population.h"
#include <algorithm>
#include <math.h>
#include <iostream>

Strategies::Strategies() {
}

/* ----------------------------*/
/* Selection protocols */  
/* ----------------------------*/

void
Strategies::FullGenerationalReplacement( Population &pop ) {
   /* All adults die, all children become adults */
   pop.SetAdults(pop.GetChildren());

   /* Clear children */
   pop.GetChildren().clear();
}

void
Strategies::OverProduction( Population &pop ) {
   
   /* All adults die, children fight to become adults */
   pop.GetAdults().clear();
   
   int num;
   while ( pop.GetAdults().size() < pop.GetAdultsSize() ) {
      num = rand() % pop.GetChildren().size();
      pop.GetAdults().push_back( pop.GetChildren().at( num ) );
      pop.GetChildren().erase( pop.GetChildren().begin()+num );
   }

   /* Clear children */
   pop.GetChildren().clear();
}

void
Strategies::GenerationalMixing( Population &pop ) {
   /* Put all adults in child pool */
   while ( pop.GetAdults().size() > 0 ) {
      pop.GetChildren().push_back( pop.GetAdults().back() );
      pop.GetAdults().pop_back();
   }
   
   /* Clear adults */
   pop.GetAdults().clear();
   
   /* Select adults with equal probability */
   while ( pop.GetAdults().size() < pop.GetChildrenSize() ) {
      pop.GetAdults().push_back( pop.GetChildren().at( rand() % pop.GetChildrenSize() ) );
   }
   
   /* Clear children */
   pop.GetChildren().clear();
}

/* ----------------------------*/
/* Global selection mechanisms */
/* ----------------------------*/

void
Strategies::FitnessProportionate( Population &pop ) {
   /* Take care of elitism first */
   Elitism( pop );

   /* Create roullette wheel, normalize fitness values */
   float sum = pop.GetFitnessSum(); 
   float roulette[pop.GetAdults().size()];
   roulette[0]=0;
   
   for ( unsigned int i = 1 ; i < pop.GetAdults().size() ; i++ ) {
      roulette[i]=roulette[i-1] + pop.GetAdults().at(i-1).GetFitness() / sum;
   }
   
   Select( pop, roulette, pop.GetAdults().size(), 1 );
}

void
Strategies::SigmaScaling( Population &pop ) {
   /* Take care of elitism first */
   Elitism( pop );
   
   /* Create roullette wheel */
   float averagefitness = pop.GetAverageFit();
   float stddeviation = pop.GetStdDeviation();

   float max;
   float roulette[pop.GetAdults().size()];
   roulette[0] = 0;
      
   if ( stddeviation != 0 ) {
      float fitness;
      for ( unsigned int i = 1 ; i < pop.GetAdults().size() ; i++ ) {
         fitness = pop.GetAdults().at(i-1).GetFitness();
         roulette[i] = roulette[i-1] + 1 
                       + (fitness-averagefitness)/(2*stddeviation);
      }
      fitness = pop.GetAdults().back().GetFitness();
      max = roulette[pop.GetAdults().size()-1] + 1 + (fitness-averagefitness)/(2*stddeviation);
   }
   else {
      for ( unsigned int i = 1 ; i < pop.GetAdults().size() ; i++ ) {
         roulette[i]=i;
      }
      max = pop.GetAdults().size();
   }
   
   Select( pop, roulette, pop.GetAdults().size(), max );
}

void
Strategies::BoltzmannSelection( Population &pop ) {
   /* Take care of elitism first */
   Elitism( pop );
   
   float temperature = pop.GetTemperature();

   /* Create roullette wheel */
   float max = 0;
   float roulette[pop.GetAdults().size()];

   /* e ^ ( f(i) / T ) / <e ^ ( f(i) / T )>g */
   roulette[0]=0;
   for ( unsigned int i = 1 ; i < pop.GetAdults().size() ; i++ ) {
      roulette[i] = roulette[i-1] + exp( pop.GetAdults().at(i).GetFitness() / temperature );
   }
   max = roulette[pop.GetAdults().size()-1] + exp( pop.GetAdults().back().GetFitness() / temperature ) ;

   Select( pop, roulette, pop.GetAdults().size(), max );
}

void
Strategies::RankSelection( Population &pop ) {
   /* Take care of elitism first */
   Elitism( pop );
   
   /* Create roullette wheel */
   float max = 0;
   float min = 999999999999999999;
   float sum = 0;
   float roulette[pop.GetAdults().size()];

   /* Find min and max */

   for ( unsigned int i = 0 ; i < pop.GetAdults().size() ; i++ ) {
      if ( max < pop.GetAdults().at(i).GetFitness() ) max = pop.GetAdults().at(i).GetFitness();
      if ( min > pop.GetAdults().at(i).GetFitness() ) min = pop.GetAdults().at(i).GetFitness();
   }
   
   max = pop.GetAdults().back().GetFitness();
   min = pop.GetAdults().at(0).GetFitness();
   
   roulette[0]=0;
   for ( unsigned int i = 1 ; i < pop.GetAdults().size() ; i++ ) {
      roulette[i] = roulette[i-1] + min+(max-min)*(i - 1) / (pop.GetAdults().size() - 1);
   }
   sum = roulette[pop.GetAdults().size()-1] + max;
   
   Select( pop, roulette, pop.GetAdults().size(), sum );
}

void
Strategies::TournamentSelection( Population &pop ) {

   /* Take care of elitism first */
   Elitism( pop );
      
   float crossover_rate = pop.GetCrossoverRate();

   std::vector<Individual> group_a;
   std::vector<Individual> group_b;
   while ( pop.GetChildren().size() < pop.GetChildrenSize() ) {

      unsigned int selector;
         
      /* Reproduce or clone best individual */
      if( (float)rand()/(float)RAND_MAX <= crossover_rate ) {


         /* select individuals for group a */
         for (unsigned int i = 0; i<pop.GetTournamentSize(); i++) {
            while( group_a.size() < pop.GetTournamentSize() ) {
               selector = rand() % pop.GetAdults().size();
               group_a.push_back( pop.GetAdults().at( selector ) );
               pop.GetAdults().erase( pop.GetAdults().begin() + selector );
            }
         }
         float temp_fit=0;
         /* Find best individual in group a */
         unsigned int best_in_a=0;
         for ( unsigned int i = 0; i < group_a.size(); i++ ) {
            if ( temp_fit < group_a.at(i).GetFitness() ) {
               best_in_a = i;
               temp_fit = group_a.at(i).GetFitness();
            }
         }
         
         /* select individuals for group b */
         unsigned int selector;
         for (unsigned int i = 0; i<pop.GetTournamentSize(); i++) {
            while( group_b.size() < pop.GetTournamentSize() ) {
               selector = rand() % pop.GetAdults().size();
               group_b.push_back( pop.GetAdults().at( selector ) );
               pop.GetAdults().erase( pop.GetAdults().begin() + selector );
            }
         }
         /* Find best individual in group b */
         unsigned int best_in_b=0;
         for ( unsigned int i = 0; i < group_b.size(); i++ ) {
            if ( temp_fit < group_b.at(i).GetFitness() ) {
               best_in_b = i;
               temp_fit = group_b.at(i).GetFitness();
            }
         }

         pop.GetChildren().push_back( group_a.at( best_in_a ).Reproduce( group_b.at( best_in_b ) ) );
         pop.GetChildren().push_back( group_b.at( best_in_b ).Reproduce( group_a.at( best_in_a ) ) );
      }
      else{
         /* select individuals for group 1 */
         for (unsigned int i = 0; i<pop.GetTournamentSize(); i++) {
            while( group_a.size() < pop.GetTournamentSize() ) {
               selector = rand() % pop.GetAdults().size();
               group_a.push_back( pop.GetAdults().at( selector ) );
               pop.GetAdults().erase( pop.GetAdults().begin() + selector );
            }
         }
         
         /* Find best individual */
         unsigned int best_in_a=0;
         float temp_fit=0;
         for ( unsigned int i = 0; i < group_a.size(); i++ ) {
            if ( temp_fit < group_a.at(i).GetFitness() ) {
               best_in_a = i;
               temp_fit = group_a.at(i).GetFitness();
            }
         }
         
         pop.GetChildren().push_back( group_a.at( best_in_a ) );
      }
      
      for ( unsigned int i = 0; i<group_a.size(); i++)
         pop.GetAdults().push_back(group_a.at(i));
      group_a.clear();
      for ( unsigned int i = 0; i<group_b.size(); i++)
         pop.GetAdults().push_back(group_b.at(i));
      group_b.clear();
      
   }
}


/* ----------------------------*/
/* Private helper functions */
/* ----------------------------*/

void
Strategies::Select( Population &pop, float roulette[], int size, float max ) {
   
   /* Spin wheel untill we have enough children */
   /* Make sure an individual does not mate with itself */
   float spin1=0, spin2=0;
   int chosen1=0, chosen2=0;
   
   float crossover_rate = pop.GetCrossoverRate();
   
   while (pop.GetChildren().size() < pop.GetChildrenSize()) {
      /* Crossover */
      if( (float)rand()/(float)RAND_MAX <= crossover_rate ) {
         while (chosen1 == chosen2) {
            spin1 = ((float)rand() / (float)RAND_MAX) * (float)max;
            spin2 = ((float)rand() / (float)RAND_MAX) * (float)max;
            for ( unsigned int i = 0; i < pop.GetAdults().size(); i++) {
               if(roulette[i]<spin1) chosen1 = i;
               if(roulette[i]<spin2) chosen2 = i;
            } 
         }
         pop.GetChildren().push_back(pop.GetAdults().at(chosen1).Reproduce(pop.GetAdults().at(chosen2)));
         pop.GetChildren().push_back(pop.GetAdults().at(chosen2).Reproduce(pop.GetAdults().at(chosen1)));
      }
      /* Clone */
      else {
         spin1 = ((float)rand() / (float)RAND_MAX) * (float)max;
         for ( unsigned int i = 0; i < pop.GetAdults().size(); i++) {
            if(roulette[i]<spin1) { 
               chosen1 = i;
            }
            else {
               break;
            }
         } 
         pop.GetChildren().push_back(pop.GetAdults().at(chosen2));
      }
      chosen1 = chosen2;
   }
}

void
Strategies::Elitism( Population &pop ) {
   unsigned int elitism = pop.GetElitism();
   if (elitism > 0) {
      sort(pop.GetAdults().begin(), pop.GetAdults().end(), IndividualSort);
      for (unsigned int i = 0; i<elitism; i++) {
         pop.GetChildren().push_back(pop.GetAdults().at(i));
      }
   }
}

/* Sorting function for individuals */
bool Strategies::IndividualSort (Individual i,Individual j) { 
   return (i.GetFitness()>j.GetFitness()); 
}
