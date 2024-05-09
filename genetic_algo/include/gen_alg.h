#include <vector>
#include <algorithm>

namespace dl
{

class Genome
{

};

struct Offspring
{
    Genome genome;
    double fit;

    template <class FittnesFunction>
    static Offspring getRandomOffspring(FittnesFunction fitness_function)
    {

    };
};

struct OffspringComparator
{
    bool operator()(const Offspring& lhs, const Offspring& rhs)
    {
        return true;
    }
};

class GeneticOptimizer
{
    using Population = std::vector<Offspring>;

    size_t m_population_size;
    double m_eps;

    template <class FittnesFunction>
    void selection(FittnesFunction fitness_function)
    {
        
    }

    void crossover()
    {
        
    }

    void mutation()
    {
        
    }
public:
    GeneticOptimizer(size_t population_size, double eps) :
        m_eps(eps),
        m_population_size(population_size)
    {}

    template <class FittnesFunction>
    Genome optimize(FittnesFunction fitness_function)
    {   
        using ResultType = decltype(fitness_function);
        static_assert(std::is_convertible_v<ResultType, double>, "Fitness function must return real number");

        size_t generationCount = 0u;
        double last_best_result = 0.0;

        Population population(m_population_size);
        while (true)
        {
            std::cout << "Generation #" << generationCount << "\n";

            if (generationCount == 0)
                std::generate(population.begin(), population.end(), [&](){return Offspring::getRandomOffspring(fitness_function)});

            selection(fitness_function);
            crossover();
            mutation();

            // Now we have updated the population
            // Stopping criteria will be low fitness function difference between generations
            std::sort(population.begin(), population.end(), OffspringComparator());
            double current_best_result = population.begin()->fit;

            std::cout << "Current generation best fit value: " << current_best_result;
            
            if (std::abs(current_best_result - last_best_result) < m_eps)
                break;

            generationCount++;
            last_best_result = current_best_result;
        }

        return Genome{};
    }
};

}