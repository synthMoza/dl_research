#include "genetic_optimizer.h"

#include <boost/program_options.hpp>

namespace po = boost::program_options;
using namespace dl;

double target_function(double x) {
    return x*x-3*x+4;
}

po::variables_map parse_arguments(int argc, char* argv[]) {
    po::options_description desc("Allowed options");
    desc.add_options()
        ("population_size", po::value<size_t>()->required(), "set population size")
        ("max_generations", po::value<size_t>()->required(), "set maximum amount of generations")
        ("mutation_probability", po::value<double>()->required(), "set probability of mutation in each genome")
        ("start", po::value<double>()->required(), "set search interval start")
        ("end", po::value<double>()->required(), "set search interval end")
    ;

    po::variables_map vm;        
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    return vm;
}

int main(int argc, char* argv[])
{   
    try {
        auto vm = parse_arguments(argc, argv);
        const size_t population_size = vm["population_size"].as<size_t>();
        const size_t max_generations = vm["max_generations"].as<size_t>();
        const double a = vm["start"].as<double>();
        const double b = vm["end"].as<double>();

        dl::GeneticOptimizer optimizer(population_size, max_generations);
        optimizer.register_pass(std::unique_ptr<PopulationPass>(new SelectionPass()));
        optimizer.register_pass(std::unique_ptr<PopulationPass>(new CrossoverPass()));
        optimizer.register_pass(std::unique_ptr<PopulationPass>(new MutationPass(vm["mutation_probability"].as<double>())));

        //const auto& fitness_function = [](double x) {double y = target_function(x); return -y / (1 + exp(y));};
        const auto& fitness_function = [](double x) {double y = target_function(x); return 100 * exp(-y);};
        // tranformer should map integer decoded genome value to double value
        const auto& tranformer = [a, b](Genome::GenomeType g) {
            double v = static_cast<double>(g) / static_cast<double>(Genome::getDistributionMaximumValue() - Genome::getDistributionMinimumValue()) * (b - a) + a;
            return v;
        };
        
        const auto& optimized_genome = optimizer.optimize(fitness_function, tranformer);
        
        const auto& x = tranformer(optimized_genome.getDecodedGenome());
        std::cout << "Result: x = " << x << ", y = " << target_function(x) << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Failed to parse arguments: " << e.what() << std::endl;
    }

    return 0;
}