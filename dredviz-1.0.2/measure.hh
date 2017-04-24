/*Copyright (C) Kristian Nybo, Jarkko Venna
 *
 *This software is released under the GNU Lesser General Public
 *License. See the included file LICENSE for details.*/

#ifndef MEASURE_HH
#define MEASURE_HH

#include "distancematrix.hh"
#include "exception.hh"
#include <iostream>

namespace dredviz {

class Measure
{
public:
  virtual ~ Measure ()
  {
  }
  virtual void measure (const DistanceMatrix & origData,
                        const DistanceMatrix & projData,
                        std::ostream & ostr) = 0;

class RowMismatch:public Exception
  {
  public:
    RowMismatch (std::string errMsg):Exception (errMsg)
    {
    }
  };
};

}

#endif
