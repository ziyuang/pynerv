/*Copyright (C) Kristian Nybo, Jarkko Venna
 *
 *This software is released under the GNU Lesser General Public
 *License. See the included file LICENSE for details.*/

#ifndef CALCULATEDISTANCEMATRIX_HH
#define CALCULATEDISTANCEMATRIX_HH

#include "metric.hh"
#include "dataset.hh"
#include "datamatrix.hh"

namespace dredviz {
class CalculateDistanceMatrix
{
private:
public:
  virtual ~ CalculateDistanceMatrix ()
  {
  }
  void operator () (const DataMatrix & data, Metric & dist,
                    DataMatrix & target);
};
}

#endif
