
#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

class Individual {

   private:

   static const int m_genolength = 40;
   static const int m_phenolength = 40;

   float m_fitness;
   bool m_reproduced;
   bool m_mask[m_genolength];

   bool m_genotype[m_genolength];
   bool m_phenotype[m_phenolength];

   void SetReproduced(bool reproduced);
   bool GetReproduced();
   bool* GetMask();
   bool* GetPhenotype();
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

};

#endif
