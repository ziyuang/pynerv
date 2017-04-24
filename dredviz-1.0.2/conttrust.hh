/*Copyright (C) Kristian Nybo, Jarkko Venna
 *
 *This software is released under the GNU Lesser General Public
 *License. See the included file LICENSE for details.*/

#ifndef CONTTRUST_HH
#define CONTTRUST_HH

#include "measure.hh"
#include "distancematrix.hh"
#include "datamatrix.hh"

namespace dredviz {
class ContTrust:public Measure
{
private:
  size_t maxNeighborhood;
  DataMatrix measures; //best case & worst case trustworthiness & continuity
//  DataMatrix precrec; //best case precision & recall

public:
  virtual ~ ContTrust ()
  {
  }
  ContTrust (size_t maxNeighborhood);

  void measure(const DistanceMatrix & origDist,
               const DistanceMatrix & projDist, std::ostream & ostr);
  // wcase =0 average, wcase=1 best, wcase=2 worst
  double getContinuity(size_t numNeighbors, int wcase=0);
  double getTrustworthiness(size_t numNeighbors, int wcase=0);

};
}

#endif
