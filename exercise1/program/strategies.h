
#ifndef STRATEGIES_H
#define STRATEGIES_H

#include <vector>
#include "individual.h"
#include "population.h"

class Strategies {
   private:
   static void Elitism( Population &pop );
   static void Select( Population &pop, float roulette[], int size, float max );
   static bool IndividualSort(Individual i,Individual j);
   
   public:
	Strategies();
   
   /* Selection protocols */
   static void FullGenerationalReplacement( Population &pop );
   static void OverProduction( Population &pop );
   static void GenerationalMixing( Population &pop );
   
   /* Selection mechanisms */
   static void FitnessProportionate( Population &pop );
   static void SigmaScaling( Population &pop );
   static void BoltzmannSelection( Population &pop );
   static void RankSelection( Population &pop );
   static void TournamentSelection( Population &pop );
   
};

#endif
