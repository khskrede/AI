
#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <string>

class Individual {

   private:

   static const int m_genolength = 40;

   // tau, k, v, u, I, b
   static const int m_phenolength = 10;
   
   bool solution[m_genolength];

   float m_fitness;
   bool m_reproduced;
   bool m_mask[m_genolength];

   bool m_genotype[m_genolength];
   int m_phenotype[m_phenolength];

   void SetReproduced(bool reproduced);
   bool GetReproduced();
   bool* GetMask();
   bool* GetGenotype();

   public:

   Individual();
   Individual( bool* geno );

   void Init();
   void Develop();
   void Evaluate();
   void Mutate();
   Individual Reproduce(Individual mate);
   float GetFitness();
   
   std::string ToString();

};

#endif
