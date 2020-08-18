#pragma once

/**
 * initialize the random generator
 */
void init_random_generator();

/**
 * return a random number between [min..max]
 * @param min min
 * @param max max
 * @return generated random number
 */
unsigned int generate_random(unsigned int min, unsigned int max);
