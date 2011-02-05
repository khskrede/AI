
#ifndef STRATEGIES_H
#define STRATEGIES_H

#include <vector>
#include "population.h"

class Strategies {
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
   
};

#endif
