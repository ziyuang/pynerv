/*Copyright (C) Kristian Nybo, Jarkko Venna
 *
 *This software is released under the GNU Lesser General Public
 *License. See the included file LICENSE for details.*/

#ifndef COMMANDLINEARGS_HH
#define COMMANDLINEARGS_HH

#include <string>
#include <vector>
#include <set>

#include "exception.hh"

namespace dredviz {
class CommandLineArgs
{
protected:
  std::vector < std::string > args;

  size_t findArg (const std::string & name) const;
  size_t findArg (const char *name) const;

  std::string stringify (const char *) const;

public:
  CommandLineArgs (int argc, char **argv,
      std::set<std::string> &recognizedSwitches);

  void getString (const std::string & name, std::string & target) const;
  void getString (const char *name, std::string & target) const;

  double getDouble (const std::string & name) const;
  double getDouble (const char *name) const;

  size_t getUInt (const std::string & name) const;
  size_t getUInt (const char *name) const;

  bool hasArg (const std::string & name) const;
  bool hasArg (const char *name) const;


  class ArgNotFound:public Exception
  {
  public:
    ArgNotFound (std::string errMsg):Exception (errMsg)
    {
    }
  };

  class UnrecognizedOption : public Exception
  {
    public:
      UnrecognizedOption (std::string errMsg) : Exception(errMsg)
      {
      }
  };

};
}

#endif
