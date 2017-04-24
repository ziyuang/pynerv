/*Copyright (C) Kristian Nybo, Jarkko Venna
 *
 *This software is released under the GNU Lesser General Public
 *License. See the included file LICENSE for details.*/

#include <iostream>
#include <string>

#include "sompackloader.hh"
#include "dataset.hh"
#include "sompackexporter.hh"
#include "metric.hh"
#include "euclidean.hh"
#include "commandlineargs.hh"
#include "pca.hh"

using namespace dredviz;

int
main (int argc, char **argv)
{
  DataSet projData;
  DataSet origData;

  std::vector<std::string> switches;
  switches.push_back("--inputfile");
  switches.push_back("--outputfile");
  switches.push_back("--outputdim");

  std::set<std::string> recognizedSwitches(switches.begin(), switches.end());

  std::string usage =
    "Synopsis: pca [OPTION] --inputfile [INPUT_FILE] --outputfile [OUTPUT_FILE]"
    "\n"
    "Required parameters:"
    "\n"
    "--inputfile INPUT_FILE - The name of the file that contains the data points to be visualized in SOM_PAK format. If a distance matrix is provided via --inputdist (see below), this switch will be ignored. "
    "\n"
    "--outputfile OUTPUT_FILE - The name of the file to which the output will be written, also in SOM_PAK format."
    "\n\n"
    "Optional parameters:"
    "\n"
    "--outputdim DIM - The desired dimension of the output data. DIM should be a positive integer. The default value for DIM is 2. Note that --outputdim is ignored if initialization data is passed via --init, because the dimension of the output data must have the same dimension as the initialization."
    "\n";


  try
  {
    CommandLineArgs args (argc, argv, recognizedSwitches);
    std::string inputfile;
    std::string outputfile;

    if (argc == 1 || args.hasArg ("--help"))
    {
      std::cerr << usage;
    }

    args.getString ("--outputfile", outputfile);
    SOMPackExporter exporter (outputfile);

    args.getString ("--inputfile", inputfile);

    SOMPackLoader loader (inputfile);
    loader.loadData (origData);

    size_t outputDimension = 2;       /* The default dimension */

    if (args.hasArg ("--outputdim"))
    {
      outputDimension = args.getUInt ("--outputdim");
    }

    PCA<DataSet> pca(origData);
    projData = DataSet(origData.getRows(), outputDimension);
    pca.getComponents(projData);

    exporter.exportData (projData);
  }

  catch (Exception & exc)
  {
    std::cerr << "Error> " << exc.getErrMsg() << std::endl;
  }

  return 0;
}
