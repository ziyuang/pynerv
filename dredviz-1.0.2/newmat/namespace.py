#!/usr/bin/python
import sys
lines = file(sys.argv[1]).readlines()
outputfile = file(sys.argv[1], 'w')
outputfile.write('using namespace std;\n')
outputfile.writelines(lines)
