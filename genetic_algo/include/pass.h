#include <vector>
#include <unordered_map>
#include <stdexcept>

#include "offspring.h"

namespace dl
{

using Population = std::vector<Offspring>;

class PopulationPass {
public:
    virtual void run(Population& population, size_t generation) const = 0;
    virtual ~PopulationPass() {}
};

class SelectionPass : public PopulationPass {
    enum SelectionStrategy {
        ROULETTE_WHEEL_SELECTION,
        RANK_SELECTION,
        TOURNAMENT_SELECTION,
        INVALID_SELECTION,
    } m_selection_strategy;
    float m_rate;
    
    template <typename It>
    size_t roulette_wheel_selection(It begin, It end) const {
        std::uniform_real_distribution distribution{};
        size_t population_size = std::distance(begin, end);
        size_t winner_idx = 0;
        double population_fitness = 0.0;

        for (auto it = begin; it != end; ++it)
            population_fitness += it->second.fit;
        
        std::vector<double> probabilities;
        
        probabilities.reserve(population_size);
        for (auto it = begin; it != end; ++it)
            probabilities.push_back(it->second.fit / population_fitness);
        
        // accumulate probabilities
        for (size_t i = 1; i < population_size; ++i)
            probabilities[i] += probabilities[i - 1];
        
        double roulette_probability = distribution(random_engine);
        for (size_t i = 0; i < probabilities.size(); ++i) {
            if (probabilities[i] > roulette_probability) {
                winner_idx = i;
                break;
            }
        }

        auto it = begin;
        std::advance(it, winner_idx);
        return it->first;
    }
public:
    SelectionPass(SelectionStrategy selection_strategy = ROULETTE_WHEEL_SELECTION, float rate = 0.5) :
        m_selection_strategy(selection_strategy),
        m_rate(rate) {}

    void run(Population& population, size_t generation) const override {
        (void) generation;

        size_t new_population_size = static_cast<size_t>(population.size() * m_rate);
        std::unordered_map<size_t, Offspring> population_hashmap;

        std::sort(population.rbegin(), population.rend());
        for (size_t i = 0; i < population.size(); ++i)
            population_hashmap[i] = population[i];
        
        Population new_population;
        new_population.reserve(new_population_size);

        for (size_t i = 0; i < new_population_size; ++i) {
            size_t winner_idx = 0;
            switch (m_selection_strategy) {
                case ROULETTE_WHEEL_SELECTION:
                    winner_idx = roulette_wheel_selection(population_hashmap.begin(), population_hashmap.end());
                    break;
                default:
                    throw std::runtime_error("unknown selection strategy");
            }

            new_population.push_back(population[winner_idx]);
            population_hashmap.erase(winner_idx);
        }

        population = std::move(new_population);
    }
};

class CrossoverPass : public PopulationPass {
    enum CrossoverStrategy {
        ONE_POINT_CROSSOVER,
        INVLAID_CROSSOVER,
    } m_crossover_strategy;
    size_t m_birth_rate;

    Offspring one_point_crossover(const Offspring& lhs, const Offspring& rhs, size_t generation) const {
        using GenomeType = Genome::GenomeType;
        const size_t genome_size = Genome::getGenomeSize();

        std::uniform_int_distribution<size_t> distribution(0, genome_size);
        size_t point_idx = distribution(random_engine);

        const GenomeType lhs_genome = lhs.genome.getEncodedGenome();
        const GenomeType rhs_genome = rhs.genome.getEncodedGenome();

        const GenomeType mask1 = (static_cast<GenomeType>(-1) >> point_idx) << point_idx;
        const GenomeType mask2 = static_cast<GenomeType>(-1) ^ mask1;
        const GenomeType child_genome = (lhs_genome & mask1) ^ (rhs_genome & mask2); 
            
        return Offspring{.genome = child_genome, .fit = 0, .generation = generation}; // we update fit later
    }
public:
    CrossoverPass(CrossoverStrategy crossover_strategy = ONE_POINT_CROSSOVER, size_t birth_rate = 2) :
        m_crossover_strategy(crossover_strategy),
        m_birth_rate(birth_rate) {}

    void run(Population& population, size_t generation) const override {
        const size_t initial_population_size = population.size();
        population.reserve(m_birth_rate * initial_population_size);

        for (size_t i = 0; i < (m_birth_rate - 1) * initial_population_size; ++i) {
            size_t first_parent, second_parent;
            std::uniform_int_distribution<size_t> distribution(0, initial_population_size);
            do {
                first_parent = distribution(random_engine);
                second_parent = distribution(random_engine);
            } while (first_parent == second_parent);

            Offspring child{};
            switch (m_crossover_strategy) {
                case ONE_POINT_CROSSOVER:
                    child = one_point_crossover(population[first_parent], population[second_parent], generation);
                    break;
                default:
                    throw std::runtime_error("unknown crossover strategy");
            }

            population.push_back(child);
        }
    }
};

class MutationPass : public PopulationPass {
    double m_mutation_probability;
public:
    MutationPass(double mutation_probability) :
        m_mutation_probability(mutation_probability) {}

    void run(Population& population, size_t generation) const override {
        for (auto it = population.begin(); it != population.end(); ++it) {
            if (it->generation == generation)
                it->genome.mutate(m_mutation_probability);
        }
    }
};

} // namespace dl