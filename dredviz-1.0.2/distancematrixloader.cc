#include "distancematrixloader.hh"

#include <sstream>

namespace dredviz {
void
DistanceMatrixLoader::
loadMatrix (std::string filename, DistanceMatrix & target)
{
  dataFile.open (filename.c_str ());

  if (!dataFile)
    throw (Exception ("File " + filename + " could not be" + " opened."));


  std::vector < string > buffer;
  string line;

  unsigned int columns = 0;


  //In the SOMPACK format the first line specifies the number of columns
  //in the matrix.

  getline (dataFile, line);

  if (dataFile.fail ())
    throw (Exception ("Could not read first line of SOM_PAK file."));

  std::istringstream strs (line);

  if (!(strs >> columns))
    throw (Exception ("The first item in the input file is not"
                      " a valid integer."));

  while (dataFile.good ())
  {
    getline (dataFile, line);

    /* Skip empty lines and lines beginning with a '#' (comments) */

    if (!dataFile.fail () && line != "" && line[0] != '#')
      buffer.push_back (line);
  }

  if (buffer.size () != columns)
  {
    throw (Exception ("Error while reading in distance matrix: number of "
                      "rows does not match number of columns."));
  }


  DistanceMatrix datamatrix (buffer.size ());


  for (size_t i = 0; i < buffer.size (); i++)
  {
    std::istringstream istr (buffer[i]);
    for (size_t j = 0; j < columns; j++)
      if (!(istr >> datamatrix (i, j)))
      {
        std::ostringstream errMsg;
        errMsg << "Error reading element (" << i << ", " << j
        << ") from input file.";
        throw Exception (errMsg.str ());
      }
  }

  target = datamatrix;

  dataFile.close ();
}
}
