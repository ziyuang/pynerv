/*Copyright (C) Kristian Nybo, Jarkko Venna
 *
 *This software is released under the GNU Lesser General Public
 *License. See the included file LICENSE for details.*/

#ifndef SOMPACKEXPORTER_HH
#define SOMPACKEXPORTER_HH

#include <fstream>
#include <vector>
#include "dataexporter.hh"
#include "nervprobability.hh"

namespace dredviz {
class SOMPackExporter:public DataExporter
{
private:

  std::string filename;
  std::ofstream outputFile;

  std::string commentCharacter;
  std::string comment;

public:
  ~SOMPackExporter ();
  SOMPackExporter (std::string filename);

  void exportData (const DataSet & data);
  void exportData (const DataMatrix& data);
  void exportData (const NeRVProbability& data);

  void addComment (const std::string& comment);
  void clearComments () { comment = ""; }

  const std::string& getFilename() { return filename; }
};
}

#endif
