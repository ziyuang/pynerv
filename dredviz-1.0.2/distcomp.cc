/*Copyright (C) Kristian Nybo, Jarkko Venna
 *
 *This software is released under the GNU Lesser General Public
 *License. See the included file LICENSE for details.*/


#include "distcomp.hh"

namespace dredviz {
bool
CompareIndicesDist::operator () (size_t j1, size_t j2)
{
  if (onlydist)
  {
    return (distdat(index,j1)<distdat(index,j2));
  }
  if (distdat(index, j1) == distdat(index, j2))
  {
    if (best)
      return (origRanks(index, j1) < origRanks(index, j2));
    else
      return !(origRanks(index, j1) < origRanks(index, j2));
  }
  else
  {
    return (distdat(index, j1) < distdat(index, j2));
  }
}
else
{
  if (distdat(index, j1) == distdat(index, j2))
  {
    return (origRanks(index, j1) < origRanks(index, j2));
  }
  else
  {
    return (distdat(index, j1) < distdat(index, j2));
  }

}
}
}
