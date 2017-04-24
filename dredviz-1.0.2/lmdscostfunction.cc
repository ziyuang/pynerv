/*Copyright (C) Kristian Nybo, Jarkko Venna
 *
 *This software is released under the GNU Lesser General Public
 *License. See the included file LICENSE for details.*/

#include "lmdscostfunction.hh"

#include "euclidean.hh"

#include <iostream>
#include <algorithm>

namespace dredviz {
LMDSCostFunction::LMDSCostFunction (const DistanceMatrix & origDist,
                                    const DataMatrix & projData,
                                    DynamicDouble & initialRadius,
                                    double lambda,
                                    size_t finalNeighborhoodSize)
    :SeparableCostFunction(projData), origDist(origDist), lambda(lambda),
    radius(initialRadius)
{
  calculateFinalNeighborhoods (finalNeighborhoodSize);
}


const
vector < double >&
LMDSCostFunction::getGradientComponent (const DataMatrix & projData,
                                        size_t i, size_t j)
{
  double coefficient = 0.0;
  Euclidean metric;

  double outputDistance = metric (projData, i, j);


  if (origDist (i, j) < radius.value ()
      || origDist (i, j) < finalNeighborhoods[i])
  {
    coefficient += lambda;
  }


  if (outputDistance < radius.value ()
      || outputDistance < finalNeighborhoods[i])
  {
    coefficient += 1.0 - lambda;
  }


  if (outputDistance != 0.0)
  {
    double temp = coefficient * (origDist (i, j) / outputDistance - 1.0);

    for (size_t dim = 0; dim < projData.getCols (); ++dim)
    {
      gradientComponent[dim] = temp * (projData (j, dim) - projData (i, dim));
    }
  }

  return gradientComponent;
}


void
LMDSCostFunction::updateDynamicParameters (size_t currentRound,
    size_t totalRounds,
    const DataMatrix & projData)
{
  radius.update (currentRound, totalRounds);
}


void
LMDSCostFunction::calculateFinalNeighborhoods (size_t finalNeighborhoodSize)
{
  CompareIndices cmp (origDist);


  std::vector < size_t > indices (origDist.getCols ());

  if (finalNeighborhoodSize > origDist.getCols ())
    finalNeighborhoodSize = origDist.getCols () - 1;

  for (cmp.i = 0; cmp.i < origDist.getRows (); cmp.i++)
  {
    for (size_t j = 0; j < origDist.getCols (); j++)
    {
      indices[j] = j;
    }

    std::sort (indices.begin (), indices.end (), cmp);


    finalNeighborhoods.push_back (origDist (cmp.i,
                                            indices[finalNeighborhoodSize]));
  }
}


double
LMDSCostFunction::getGradient (const DataMatrix & projData,
                               DataMatrix & gradient)
{
  double sqsum = 0.0;

  for (size_t i = 0; i < projData.getRows (); i++)
  {
    for (size_t d = 0; d < projData.getCols (); d++)
      gradient (i, d) = 0.0;

    for (size_t j = 0; j < projData.getRows (); j++)
    {
      if (i!=j)
      {
        const vector < double >&gradientPart =
          getGradientComponent (projData, i, j);
        for (size_t d = 0; d < projData.getCols (); d++)
          gradient (i, d) += gradientPart[d];
      }
    }
    for (size_t d = 0; d < projData.getCols (); d++)
      sqsum += gradient (i, d) * gradient (i, d);
  }
  return sqsum;
}


double
LMDSCostFunction::evaluate (const DataMatrix & location)
{
  double sum = 0.0;
  double outputDistance = 0.0;

  Euclidean metric;

  for (size_t point1 = 0; point1 < location.getRows (); point1++)
  {
    for (size_t point2 = 0; point2 < location.getRows (); point2++)
    {
      if (point2 != point1)
      {
        outputDistance = metric (location, point1, point2);

        if (outputDistance < radius.value () ||
            outputDistance < finalNeighborhoods[point1])
        {
          sum += (1.0 - lambda)
                 * (origDist (point1, point2) - outputDistance)
                 * (origDist (point1, point2) - outputDistance);
        }

        if (origDist (point1, point2) < radius.value () ||
            origDist (point1, point2) < finalNeighborhoods[point1])
        {
          sum += lambda
                 * (origDist (point1, point2) - outputDistance)
                 * (origDist (point1, point2) - outputDistance);
        }
      }
    }
  }

  return sum / 2.0;
}
}
