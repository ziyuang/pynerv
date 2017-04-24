/*Copyright (C) Kristian Nybo, Jarkko Venna
 *
 *This software is released under the GNU Lesser General Public
 *License. See the included file LICENSE for details.*/

#ifndef DATAEXPORTER_HH
#define DATAEXPORTER_HH

#include "exception.hh"
#include "dataset.hh"

namespace dredviz {
class DataExporter
{
public:
  virtual void exportData (const DataSet & data) = 0;

  virtual ~ DataExporter ()
  {
  };

class OutputError:public Exception
  {
  public:
    OutputError (std::string errMsg):Exception (errMsg)
    {
    };
  };
};
}

#endif
