#include <vector>
#include <algorithm>

namespace dl
{

class Genome
{

};

class Offspring
{
    Genome m_genome;
public:
    static Offspring getRandomOffspring()
    {

    };
};

class GeneticOptimizer
{
    using Population = std::vector<Offspring>;
    Population m_population;

    std::size_t m_population_size;

    template <class FittnesFunction>
    void selection(FittnesFunction fitness_function)
    {
        
    }
public:
    GeneticOptimizer(std::size_t population_size) :
        m_population_size(population_size)
    {}

    template <class FittnesFunction>
    Genome optimize(FittnesFunction fitness_function)
    {
        m_population.clear(), m_population.resize(m_population_size);

        // 0) Initial population
        std::generate(m_population.begin(), m_population.end(), Offspring::getRandomOffspring);

        // 1) Selection
        // 2) Crossover
        // 3) Mutation
        return Genome{};
    }
};

}