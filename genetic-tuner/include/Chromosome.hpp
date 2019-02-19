#pragma once

#include <Randn.hpp>

/** Chromosome is a double array of size N. */
template <size_t N>
using Chromosome = ColVector<N>;

/**
 * Perform crossing-over between two parent chromosomes to create two child
 * chromosomes.
 */
template <size_t N>
void crossOver(const Chromosome<N> &parent1, const Chromosome<N> &parent2,
               Chromosome<N> &child1, Chromosome<N> &child2) {
    static std::default_random_engine generator;
    static std::uniform_int_distribution<size_t> distr(0, N);
    size_t idx = distr(generator);

    for (size_t i = 0; i < idx; i++) {
        child1[i] = parent1[i];
        child2[i] = parent2[i];
    }
    for (size_t i = idx; i < N; i++) {
        child1[i] = parent2[i];
        child2[i] = parent1[i];
    }
}

/**
 * Mutate the given chromosome by adding an extra dChrom to the chromosome,
 * where dChrom[i] = factor*randn*chrom[i].
 */
// Als je parameters "by value" gebruikt, moet je er geen const aan toevoegen,
// want `factor` is sowieso een kopie, en de functie kan dus geen effect hebben
// in de calling scope.
// De const kan wel belang hebben in de scope van de functie, waar `factor` dan 
// niet kan veranderen. Dit wordt meestal niet expliciet met const aangeduid.
template <size_t N>
void mutate(Chromosome<N> &chrom, const double factor) {

    // Standard normal distribution
    static std::default_random_engine generator;
    static std::normal_distribution<double> distribution(0.0, 1.0);

    for (size_t i = 0; i < N; i++) {
        double random = distribution(generator);
        chrom[i] += factor * random * chrom[i];
    }
}