/*Copyright (C) Kristian Nybo, Jarkko Venna
 *
 *This software is released under the GNU Lesser General Public
 *License. See the included file LICENSE for details.*/

#include "sompackloader.hh"
#include "datamatrix.hh"
#include "dataloader.hh"

#include <vector>
#include <string>
#include <sstream>


using
std::string;

namespace dredviz {
SOMPackLoader::SOMPackLoader (std::string filename) : filename(filename)
{
  dataFile.open (filename.c_str ());

  if (!dataFile)
    throw (DataLoader::InputError ("File " + filename + " could not be"
                                   + " opened."));
}


void
SOMPackLoader::loadData (DataSet & target)
{
  std::vector < string > buffer;
  string line;

  unsigned int columns = 0;


  while (dataFile.good ())
  {
    getline (dataFile, line);

    /* Skip empty lines and lines beginning with a '#' (comments) */

    if (!dataFile.fail () && line != "" && line[0] != '#')
      buffer.push_back (line);
  }

  //In the SOMPACK format the first line specifies the number of columns
  //in the matrix.
  //

  if (buffer.size() == 0)
  {
    std::ostringstream errMsg;
    errMsg << "The file " << filename << " is empty.";
    throw (DataLoader::InputError (errMsg.str() ));
  }

  std::istringstream strs (buffer[0]);

  if (!(strs >> columns))
    throw (DataLoader::InputError ("The first item in the input file is not"
                                   " a valid integer."));


  DataMatrix datamatrix (buffer.size () - 1, columns);


  for (size_t i = 0; i < datamatrix.getRows(); i++)
  {

    // The actual data starts on the second line that has been read
    //
    std::istringstream istr (buffer[i+1]);
    for (size_t j = 0; j < datamatrix.getCols(); j++)
      if (!(istr >> datamatrix (i, j)))
      {
        std::ostringstream errMsg;
        errMsg << "Error reading element (" << i << ", " << j
        << ") from input file.";
        throw DataLoader::InputError (errMsg.str ());
      }
  }

  DataSet data (datamatrix);
  target = data;
}


SOMPackLoader::~SOMPackLoader ()
{
  dataFile.close ();
}
}
