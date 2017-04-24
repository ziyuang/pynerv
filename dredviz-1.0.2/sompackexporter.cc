/*Copyright (C) Kristian Nybo, Jarkko Venna
 *
 *This software is released under the GNU Lesser General Public
 *License. See the included file LICENSE for details.*/

#include "sompackexporter.hh"

#include <sstream>

namespace dredviz {
SOMPackExporter::SOMPackExporter (std::string filename)
  : filename(filename), commentCharacter("#"), comment("")
{
}


void
SOMPackExporter::exportData (const DataSet & data)
{
  outputFile.open (filename.c_str ());

  if (!outputFile)
    throw (DataExporter::
           OutputError ("Could not open output file " + filename));

  /* Add comments (if any) to the beginning of the file */

  if(comment != "")
  {
    outputFile << commentCharacter;
    for(size_t i = 0; i < comment.size(); i++)
    {
      if(comment[i] != '\n')
        outputFile << comment[i];
      else
        outputFile << "\n" << commentCharacter;
    }
      
  }



  //The first line should contain only the number of columns in the matrix.

  outputFile << data.getCols () << '\n';


  for (size_t i = 0; i < data.getRows (); i++)
  {
    for (size_t j = 0; j < data.getCols (); j++)
      if (!(outputFile << data (i, j) << ' '))
        throw (DataExporter::OutputError ("Could not write data to designated"
                                          " output file."));
    if (!(outputFile << '\n'))
      throw (DataExporter::OutputError ("Could not write data to designated"
                                        " output file."));
  }

  outputFile.close();
}

void
SOMPackExporter::exportData (const DataMatrix & data)
{
  outputFile.open (filename.c_str ());

  if (!outputFile)
    throw (DataExporter::
           OutputError ("Could not open output file " + filename));

  /*Add comments (if any) to the beginning of the file */

  if(comment != "")
  {
    outputFile << commentCharacter;
    for(size_t i = 0; i < comment.size(); i++)
    {
      if(comment[i] != '\n')
        outputFile << comment[i];
      else
        outputFile << "\n" << commentCharacter;
    }

    outputFile << "\n";
      
  }
  //The first line should contain only the number of columns in the matrix.

  outputFile << data.getCols () << '\n';


  for (size_t i = 0; i < data.getRows (); i++)
  {
    for (size_t j = 0; j < data.getCols (); j++)
      if (!(outputFile << data (i, j) << ' '))
        throw (DataExporter::OutputError ("Could not write data to designated"
                                          " output file."));
    if (!(outputFile << '\n'))
      throw (DataExporter::OutputError ("Could not write data to designated"
                                        " output file."));
  }

  outputFile.close();
}

void
SOMPackExporter::exportData (const NeRVProbability& data)
{
  outputFile.open (filename.c_str ());

  if (!outputFile)
    throw (DataExporter::
           OutputError ("Could not open output file " + filename));

  /*Add comments (if any) to the beginning of the file */

  if(comment != "")
  {
    outputFile << commentCharacter;
    for(size_t i = 0; i < comment.size(); i++)
    {
      if(comment[i] != '\n')
        outputFile << comment[i];
      else
        outputFile << "\n" << commentCharacter;
    }

    outputFile << "\n";
      
  }
  //The first line should contain only the number of columns in the matrix.

  outputFile << data.getCols () << '\n';


  for (size_t i = 0; i < data.getRows (); i++)
  {
    for (size_t j = 0; j < data.getCols (); j++)
      if (!(outputFile << data (i, j) << ' '))
        throw (DataExporter::OutputError ("Could not write data to designated"
                                          " output file."));
    if (!(outputFile << '\n'))
      throw (DataExporter::OutputError ("Could not write data to designated"
                                        " output file."));
  }

  outputFile.close();
}


void SOMPackExporter::addComment(const std::string& newcomment)
{
  comment = comment + newcomment;
}

SOMPackExporter::~SOMPackExporter ()
{
}
}
