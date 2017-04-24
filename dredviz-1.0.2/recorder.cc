#include "recorder.hh"
#include "sompackexporter.hh"
#include <sstream>

namespace dredviz {

void Recorder::record(const DataMatrix& data)
{
   std::ostringstream filename;
   filename << filename_stem << counter << ".dat";

   SOMPackExporter exporter(filename.str());
   exporter.exportData(data);
   counter++;
}
}
