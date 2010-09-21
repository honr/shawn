#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Generate a C++ header which assigns the current SHA1 to a variable

# Bjoern Hendriks 2010, hendriks@ibr.cs.tu-bs.de

# parameter 1:  (mandatory) name of header to generate
# parameter 2:  (optional) name of string variable to assign SHA1 to
# parameter 3:  (optional) name of namespace (omitted if not given)


# Remark: The generated header only works for C++ and not for C as
#         constants have by default internal linkage in C++ as opposed
#         to C.

import sys
import os
import subprocess


# check for presence of mandatory parameters
if len(sys.argv) < 2:
    sys.exit("ERROR: header name missing")

# get header name
header = sys.argv[1]
# append '.h' if necessary
if header.find('.') < 0:
    header = header + '.h'
# get variable name if passed
variable = 'revision'
if len(sys.argv) >= 3:
    variable = sys.argv[2]
# get namespace name if passed
isNamespace = False
if len(sys.argv) >= 4:
    isNamespace = True
    namespace = sys.argv[3]

# construct include guard out of header name
guard = header.replace('.', '_')        # substitute '.' -> '_'
guard = guard.replace('/', '_')         # substitute '/' -> '_'
guard = guard.lstrip('_')               # remove leading '_'
guard = guard.upper()                   # convert to upper case

# get name of this script for comment
scriptname = os.path.basename(sys.argv[0])

# get revision
getRevisionProcess = subprocess.Popen(args='getCurrentRevision.py', stdout=subprocess.PIPE, stderr=subprocess.PIPE)
getRevisionProcess.wait()
revision = getRevisionProcess.communicate()[0].strip()

# make namespace code
if isNamespace:
    beginNamespace = 'namespace ' + namespace + '\n{'
    endNamespace   = '}   // namespace ' + namespace + '\n'
else:
    beginNamespace = ''
    endNamespace   = ''

# template for header file
template = """
/**********************************************************************

        Header generated by {scriptname}, DO NOT EDIT

**********************************************************************/

#ifndef {guard}
#define {guard}

#include <string>

{beginNamespace}

const std::string {variable} = "{revision}";

{endNamespace}

#endif  // #ifndef {guard}

"""

# do substitutions
output = template.format(scriptname=scriptname, guard=guard, beginNamespace=beginNamespace, variable=variable, revision=revision, endNamespace=endNamespace)

# write header file
headerFile = open(header, 'w')
headerFile.write(output)
headerFile.close()


