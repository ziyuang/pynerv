/*Copyright (C) Kristian Nybo, Jarkko Venna
 *
 *This software is released under the GNU Lesser General Public
 *License. See the included file LICENSE for details.*/

#ifndef SEPARABLECOSTFUNCTION_HH
#define SEPARABLECOSTFUNCTION_HH

#include "costfunction.hh"

#include <vector>

/* A 'separable' cost function is a cost function E for which the gradient
   \Nabla_i E ("gradient with respect to the ith point") is of the form

   \Sum_{j \not= i} G(i, j) .

   In other words \Nabla_i E is a sum where each term of the sum depends only
   on the point i with respect to which the gradient is being taken and one
   other point.

   The optimization algorithm used in CCA requires this property. */

namespace dredviz {

class SeparableCostFunction:public CostFunction
{
protected:
  vector < double >gradientComponent;

public:
  SeparableCostFunction (const DataMatrix &
                         projData):gradientComponent (projData.getCols ())
  {
  };

  virtual const vector <
  double >&getGradientComponent (const DataMatrix & projData, size_t t,
                                 size_t j) = 0;

};

}
#endif
