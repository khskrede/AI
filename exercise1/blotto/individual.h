
#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <string>

class Individual {

   private:

   int m_battles;
   int m_soldiers;
   int m_rf;
   float m_lf;
   
   int m_genolength;
   int m_phenolength;

   float m_fitness;
   bool m_reproduced;
   bool* m_mask;

   bool* m_genotype;
   int* m_phenotype;

   void SetReproduced(bool reproduced);
   bool GetReproduced();
   bool* GetMask();
   bool* GetGenotype();
   int* GetPhenotype();

   public:

   Individual(int battles, int soldiers, float rf, float lf);
   Individual(int battles, int soldiers, float rf, float lf, bool* geno);
   Individual(const Individual& ind);
   //~Individual();
   

   void Init();
   void Develop();
   void Evaluate(Individual &ind);
   void Mutate();
   Individual Reproduce(Individual &mate);
   float GetFitness();
   void SetFitness(float fitness);
   float GetStrategyEntropy();
   
   void PrintString();

};

#endif
