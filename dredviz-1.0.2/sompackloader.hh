/*Copyright (C) Kristian Nybo, Jarkko Venna
 *
 *This software is released under the GNU Lesser General Public
 *License. See the included file LICENSE for details.*/

#ifndef SOMPACKLOADER_HH
#define SOMPACKLOADER_HH

#include "dataloader.hh"
#include "dataset.hh"
#include <fstream>
#include <string>

namespace dredviz {
class SOMPackLoader:public DataLoader
{
private:
  std::ifstream dataFile;
  std::string filename;

public:
  SOMPackLoader (std::string datafile);

  ~SOMPackLoader ();

  void loadData (DataSet &);
};

}

#endif
