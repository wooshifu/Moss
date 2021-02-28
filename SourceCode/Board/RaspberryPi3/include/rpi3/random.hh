#pragma once

#include "rpi3/mmio.hh"
#include "rpi3/namespaces.hh"

NAMESPACE_RPI3_BEGIN

unsigned int* RANDOM_GEN_CTRL       = reinterpret_cast<unsigned int*>(MMIO_BASE + 0x00104000);
unsigned int* RANDOM_GEN_STATUS     = reinterpret_cast<unsigned int*>(MMIO_BASE + 0x00104004);
const unsigned int* RANDOM_GEN_DATA = reinterpret_cast<unsigned int*>(MMIO_BASE + 0x00104008);
unsigned int* RANDOM_GEN_INT_MASK   = reinterpret_cast<unsigned int*>(MMIO_BASE + 0x00104010);

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

NAMESPACE_RPI3_END
