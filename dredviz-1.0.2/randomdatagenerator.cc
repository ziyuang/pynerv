/*Copyright (C) Kristian Nybo, Jarkko Venna
 *
 *This software is released under the GNU Lesser General Public
 *License. See the included file LICENSE for details.*/

#include <stdlib.h>
#include <cmath>
#include "randomdatagenerator.hh"

namespace dredviz {

RandomDataGenerator::RandomDataGenerator (size_t numberOfPoints,
    size_t dimension, double range, unsigned int seed):
    range (range),
    matrix (numberOfPoints, dimension)
{
  std::cout << "Using " << seed << " as RNG seed.\n";
  srand (seed);
}


void
RandomDataGenerator::loadData (DataSet & target)
{
  for (size_t i = 0; i < matrix.getRows (); i++)
    for (size_t j = 0; j < matrix.getCols (); j++)
      matrix (i, j) = rand () * (range / RAND_MAX);

  DataSet data (matrix);

  target = data;
}
}
