#include "genetic_optimizer.h"

using namespace dl;

int main(int argc, char* argv[])
{
    (void) argc, (void) argv;

    size_t population_size = 100;
    size_t max_generations = 100;

    dl::GeneticOptimizer optimizer(population_size, max_generations);
    optimizer.register_pass(std::unique_ptr<PopulationPass>(new SelectionPass()));
    optimizer.register_pass(std::unique_ptr<PopulationPass>(new CrossoverPass()));
    optimizer.register_pass(std::unique_ptr<PopulationPass>(new MutationPass(0.1)));

    // fit function is built following the rules:
    // target_function = 0 => fit = 100
    // target_function > 0 | target_function < 0 => fit < 100
    // target_function = +- inf => fit = 0
    // fit(x) = 100 * exp(-|f(x)|)
    const auto& target_function = [](double x) {return sin(x) + sin(10 * x / 3);};
    const auto& fitness_function = [](double x) {return  exp(-(sin(x) + sin(10 * x / 3)));};
    // tranformer should map integer decoded genome value to double value
    constexpr double a = 0.0;
    constexpr double b = 10.0;
    const auto& tranformer = [](Genome::GenomeType g) {
        double v = static_cast<double>(g) / static_cast<double>(Genome::getDistributionMaximumValue() - Genome::getDistributionMinimumValue()) * (b - a) + a;
        return v;
    };
    
    const auto& optimized_genome = optimizer.optimize(fitness_function, tranformer);
    
    const auto& x = tranformer(optimized_genome.getDecodedGenome());
    std::cout << "Result: x = " << x << ", y = " << target_function(x) << std::endl;

    return 0;
}