
#include "strategies.h"
#include "population.h"
#include <algorithm>
#include <math.h>
#include <iostream>

Strategies::Strategies() {
}

/* ----------------------------*/
/* Helper functions */
/* ----------------------------*/

/* Sorting function for individuals */
bool IndividualSort (Individual i,Individual j) { 
   return (i.GetFitness()>j.GetFitness()); 
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

   /* Create roullette wheel, normalize fitness values */
   float sum = pop.GetFitnessSum(); 
   float roulette[pop.GetAdults().size()];
   roulette[0]=0;
   
   for ( unsigned int i = 1 ; i < pop.GetAdults().size() ; i++ ) {
      roulette[i]=roulette[i-1] + pop.GetAdults().at(i-1).GetFitness() / sum;
   }
   
   /* Spin wheel untill we have enough children */
   /* Make sure an individual does not mate with itself */

   float spin1=0, spin2=0;
   int chosen1=0, chosen2=0;

   while (pop.GetChildren().size() < pop.GetChildrenSize()) {
      while (chosen1 == chosen2) {
         spin1 = (rand() * 1.0) / RAND_MAX;
         spin2 = (rand() * 1.0) / RAND_MAX;
         for ( unsigned int i = 0; i < pop.GetAdults().size(); i++) {
            if(roulette[i]<spin1) chosen1 = i;
            if(roulette[i]<spin2) chosen2 = i;
         } 
      }
      pop.GetChildren().push_back(pop.GetAdults().at(chosen1).Reproduce(pop.GetAdults().at(chosen2)));
      pop.GetChildren().push_back(pop.GetAdults().at(chosen2).Reproduce(pop.GetAdults().at(chosen1)));
      chosen1 = chosen2;
   }
}

void
Strategies::SigmaScaling( Population &pop ) {

   /* Create roullette wheel */
   float averagefitness = pop.GetAverageFit();
   float stddeviation = pop.GetStdDeviation();

   float max;
   float roulette[pop.GetAdults().size()];

   if ( stddeviation != 0 ) {
      float fitness;
      roulette[0] = 0;
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
   
   /* Spin wheel untill we have enough children */
   /* Make sure an individual does not mate with itself */
   float spin1=0, spin2=0;
   int chosen1=0, chosen2=0;

   while (pop.GetChildren().size() < pop.GetChildrenSize()) {
      while (chosen1 == chosen2) {
         spin1 = (rand() * max) / RAND_MAX;
         spin2 = (rand() * max) / RAND_MAX;
         for ( unsigned int i = 0; i < pop.GetAdults().size(); i++) {
            if(roulette[i]<spin1) chosen1 = i;
            if(roulette[i]<spin2) chosen2 = i;
         } 
      }
      pop.GetChildren().push_back(pop.GetAdults().at(chosen1).Reproduce(pop.GetAdults().at(chosen2)));
      pop.GetChildren().push_back(pop.GetAdults().at(chosen2).Reproduce(pop.GetAdults().at(chosen1)));
      chosen1 = chosen2;
   }
}

void
Strategies::BoltzmannSelection( Population &pop ) {
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


   /* Spin wheel untill we have enough children */
   /* Make sure an individual does not mate with itself */
   float spin1=0, spin2=0;
   int chosen1=0, chosen2=0;
   while (pop.GetChildren().size() < pop.GetChildrenSize()) {
      while (chosen1 == chosen2) {
         spin1 = (rand() * max) / RAND_MAX;
         spin2 = (rand() * max) / RAND_MAX;
         for ( unsigned int i = 0; i < pop.GetAdults().size(); i++) {
            if(roulette[i]<spin1) chosen1 = i;
            if(roulette[i]<spin2) chosen2 = i;
         } 
      }
      pop.GetChildren().push_back(pop.GetAdults().at(chosen1).Reproduce(pop.GetAdults().at(chosen2)));
      pop.GetChildren().push_back(pop.GetAdults().at(chosen2).Reproduce(pop.GetAdults().at(chosen1)));
      chosen1 = chosen2;
   }
}

void
Strategies::RankSelection( Population &pop ) {

   /* Create roullette wheel */
   float max = 0;
   float min = 0;
   float sum = 0;
   float roulette[pop.GetAdults().size()];

   /* Find min and max */
   sort(pop.GetAdults().begin(), pop.GetAdults().end(), IndividualSort);   
   
   max = pop.GetAdults().back().GetFitness();
   min = pop.GetAdults().at(0).GetFitness();
   
   roulette[0]=0;
   for ( unsigned int i = 1 ; i < pop.GetAdults().size() ; i++ ) {
      roulette[i] = roulette[i-1] + min+(max-min)*(i - 1) / (pop.GetAdults().size() - 1);
   }
   sum = roulette[pop.GetAdults().size()-1] + max;
   
   /* Spin wheel untill we have enough children */
   /* Make sure an individual does not mate with itself */
   float spin1=0, spin2=0;
   int chosen1=0, chosen2=0;
   while (pop.GetChildren().size() < pop.GetChildrenSize()) {
      while (chosen1 == chosen2) {
         spin1 = (rand() * sum) / RAND_MAX;
         spin2 = (rand() * sum) / RAND_MAX;
         for ( unsigned int i = 0; i < pop.GetAdults().size(); i++) {
            if(roulette[i]<spin1) chosen1 = i;
            if(roulette[i]<spin2) chosen2 = i;
         } 
      }
      pop.GetChildren().push_back(pop.GetAdults().at(chosen1).Reproduce(pop.GetAdults().at(chosen2)));
      pop.GetChildren().push_back(pop.GetAdults().at(chosen2).Reproduce(pop.GetAdults().at(chosen1)));
      chosen1 = chosen2;
   }
   
}
