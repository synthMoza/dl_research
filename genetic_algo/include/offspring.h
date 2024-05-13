#ifndef OFFSPRING_HEADER
#define OFFSPRING_HEADER

#include <functional>
#include <bitset>

#include "genome.h"

namespace dl
{

using GenomeTranformer = std::function<double(Genome::GenomeType)>;
using FitnessFunction = std::function<double(double)>;

struct Offspring
{
    Genome genome;
    double fit;
    size_t generation;

    // for particle swarm optimization
    //std::vector<double> velocity;
    double velocity = 0;
    double best_fit = 0;
    Genome best_genome;

    Offspring(Genome g = {}, size_t gen = 0u) :
        genome(g.getEncodedGenome()),
        fit(0),
        generation(gen),
        best_genome(g.getEncodedGenome()) {
        //velocity.resize(sizeof(Genome::GenomeType) * 8);
    }
    
    bool operator<(const Offspring& rhs) {
        return fit < rhs.fit;
    }

    void update_fit(FitnessFunction fitness_function, GenomeTranformer tranformer) {
        fit = fitness_function(tranformer(genome.getDecodedGenome()));
        if (fit > best_fit) {
            best_fit = fit;
            best_genome = genome;
        }
    }

    void update_velocity(double c1, double c2, Genome g2) {
        // constexpr const size_t sz = sizeof(Genome::GenomeType) * 8;
        std::uniform_real_distribution distribution;

        double r1 = distribution(random_engine);
        double r2 = distribution(random_engine);
        Genome g1 = best_genome;
        Genome g = genome;

        // std::bitset<sz> g_bits(g.getEncodedGenome()),
        //                 g1_bits(g1.getEncodedGenome()),
        //                 g2_bits(g2.getEncodedGenome());
        
        // for (size_t i = 0; i < sz; ++i) {
        //     velocity[i] += c1 * r1 * (g1_bits[i] - g_bits[i]) + c2 * r2 * (g2_bits[i] - g_bits[i]);
        //     if ((velocity[i] > 0 && g_bits.test(i)) || (velocity[i] < 0 && !g_bits.test(i)))
        //         g_bits.flip(i);
        // }

        // genome = g_bits.to_ullong();

        velocity += c1 * r1 * (g1.getEncodedGenome() - g.getEncodedGenome()) + c2 * r2 * (g2.getEncodedGenome() - g.getEncodedGenome());
        genome = Genome(genome.getEncodedGenome() + velocity);
    }
};

} // namespace dl


#endif // #define OFFSPRING_HEADER
