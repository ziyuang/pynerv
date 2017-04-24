/*Copyright (C) Kristian Nybo, Jarkko Venna
 *
 *This software is released under the GNU Lesser General Public
 *License. See the included file LICENSE for details.*/

#include "commandlineargs.hh"

#include <sstream>
#include <set>

namespace dredviz {
CommandLineArgs::CommandLineArgs (int argc, char **argv, 
    std::set<std::string> &recognizedSwitches)
{
  for (int i = 1; i < argc; i++)
  {

    /* If the argument is a switch (begins with "--") and is not a
       recognized switch, throw an exception */
    if(argv[i][0] == '-' && argv[i][1] == '-'
        && recognizedSwitches.count(stringify(argv[i])) == 0)
    {
      std::string err = "Unrecognized option `";
      err += stringify(argv[i]) + "'";
      throw(UnrecognizedOption(err));
    }
    args.push_back (stringify (argv[i]));
  }
}


size_t
CommandLineArgs::findArg (const std::string & name) const
{
  size_t argIndex = 0;
  bool argWasFound = false;

  for (unsigned int i = 0; i < args.size () && !argWasFound; i++)
    if (args[i] == name)
    {
      argIndex = i;
      argWasFound = true;
    }

  if (!argWasFound)
  {
    throw (ArgNotFound (""));
  }

  return argIndex;
}


size_t
CommandLineArgs::findArg (const char *name) const
{
  return findArg (stringify (name));
}


bool
CommandLineArgs::hasArg (const std::string & name) const
{
  bool found = false;

  try
  {
    findArg (name);
    found = true;
  }
  catch (ArgNotFound e)
  {
  }

  return found;
}


bool
CommandLineArgs::hasArg (const char *name) const
{
  return hasArg (stringify (name));
}


void
CommandLineArgs::getString (const std::string & name, std::string & target) const
{
  size_t argIndex = 0;

  try
  {
    argIndex = findArg (name);
  }
  catch (ArgNotFound exception)
  {
    std::string errMsg;

    if (argIndex == args.size ())
    {
      errMsg = "Switch " + name + " requires a parameter.";
      throw (ArgNotFound (errMsg));
    }
    else
    {
      std::string errMsg = "Expected switch " + name + " not given.";
      throw (ArgNotFound (errMsg));
    }
  }

  target = args[argIndex + 1];
}


void
CommandLineArgs::getString (const char *name, std::string & target) const
{
  getString (stringify (name), target);
}



double
CommandLineArgs::getDouble (const std::string & name) const
{
  size_t argIndex = 0;

  try
  {
    argIndex = findArg (name);
  }
  catch (ArgNotFound)
  {
    std::string errMsg;

    if (argIndex == args.size ())
    {
      errMsg = "Switch " + name + " requires a parameter.";
      throw (ArgNotFound (errMsg));
    }
    else
    {
      std::string errMsg = "Expected switch " + name + " not given.";
      throw (ArgNotFound (errMsg));
    }
  }


  double param;
  std::istringstream istr (args[argIndex + 1]);

  if (!(istr >> param))
  {
    std::string errMsg = name + " is not a valid number.";
    throw (ArgNotFound (errMsg));
  }

  return param;
}


double
CommandLineArgs::getDouble (const char *name) const
{
  return getDouble (stringify (name));
}


size_t
CommandLineArgs::getUInt (const std::string & name) const
{
  size_t argIndex = 0;

  try
  {
    argIndex = findArg (name);
  }
  catch (ArgNotFound)
  {
    std::string errMsg;

    if (argIndex == args.size ())
    {
      errMsg = "Switch " + name + " requires a parameter.";
      throw (ArgNotFound (errMsg));
    }
    else
    {
      std::string errMsg = "Expected switch " + name + " not given.";
      throw (ArgNotFound (errMsg));
    }
  }



  size_t param;
  std::istringstream istr (args[argIndex + 1]);

  if (!(istr >> param))
  {
    std::string errMsg = name + " is not a valid unsigned integer.";
    throw (ArgNotFound (errMsg));
  }


  return param;
}


size_t
CommandLineArgs::getUInt (const char *name) const
{
  return getUInt (stringify (name));
}



std::string CommandLineArgs::stringify (const char *s) const
{
  std::string str;

  for (size_t i = 0; s[i] != '\0'; i++)
    str += s[i];

  return str;
}
}
