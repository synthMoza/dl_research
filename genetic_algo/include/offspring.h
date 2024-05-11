#ifndef OFFSPRING_HEADER
#define OFFSPRING_HEADER

#include "genome.h"

namespace dl
{

using GenomeTranformer = double(*)(Genome::GenomeType);
using FitnessFunction = double (*)(double);

struct Offspring
{
    Genome genome;
    double fit;
    size_t generation;

    bool operator<(const Offspring& rhs) {
        return fit < rhs.fit;
    }

    void update_fit(FitnessFunction fitness_function, GenomeTranformer tranformer) {
        fit = fitness_function(tranformer(genome.getDecodedGenome()));
    }
};

} // namespace dl


#endif // #define OFFSPRING_HEADER
