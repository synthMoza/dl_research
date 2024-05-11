#ifndef GENETIC_OPTIMIZER_HEADER
#define GENETIC_OPTIMIZER_HEADER

#include <vector>
#include <algorithm>
#include <iostream>
#include <array>
#include <memory>

#include "rng.h"
#include "pass.h"

namespace dl
{

using Passes = std::vector<std::unique_ptr<PopulationPass>>;

class GeneticOptimizer
{
    Passes m_passes;

    // hyperparameters
    size_t m_population_size;
    size_t m_max_generations;

    // utility
    void update_fits(Population& population, FitnessFunction fitness_function, GenomeTranformer transformer) {
        std::for_each(population.begin(), population.end(), [&](Offspring& o){o.update_fit(fitness_function, transformer);});
    }

    void update_population(Population& population, size_t generation) {
        for (const auto& pass : m_passes)
            pass->run(population, generation);
    }

    void dump_population(const Population& population) const {
        #ifdef NDEBUG
        std::cout << "Population dump:" << std::endl;
        for (const auto& offspring : population)
            std::cout << "\t" << "genome:" << offspring.genome.getEncodedGenome() << "; fit: " << offspring.fit << std::endl;
        #else
        (void) population;
        #endif
    }

public:
    GeneticOptimizer(size_t population_size, size_t max_generations) :
        m_population_size(population_size),
        m_max_generations(max_generations)
    {}

    void register_pass(std::unique_ptr<PopulationPass> pass) {
        m_passes.push_back(std::move(pass));
    }

    Genome optimize(FitnessFunction fitness_function, GenomeTranformer transformer)
    {
        size_t generation = 0u;

        // initial population
        Population population(m_population_size);
        update_fits(population, fitness_function, transformer);
        dump_population(population);

        while (true)
        {
            std::cout << "Generation #" << generation << std::endl;
            update_population(population, generation);
            update_fits(population, fitness_function, transformer);
            dump_population(population);

            // Stopping criteria will be low fitness function difference between generations
            std::sort(population.rbegin(), population.rend());
            double current_best_result = population.begin()->fit;

            std::cout << "Current generation best fit value: " << current_best_result << std::endl;
            
            if (generation >= m_max_generations) {
                std::cout << "Generation limit exceeded!" << std::endl;
                break;
            }

            generation++;
        }

        return population.begin()->genome;
    }
};

}

#endif // #define GENETIC_OPTIMIZER_HEADER
