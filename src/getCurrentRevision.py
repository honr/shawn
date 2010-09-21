#!/usr/bin/env python
# -*- coding: utf-8 -*-

## Get SHA1 of last commit of current git repository

## Bjoern Hendriks 2010, hendriks@ibr.cs.tu-bs.de

import sys
import subprocess


unstagedTag = 'with unstaged changes'
uncommittedTag = 'with uncommitted changes'



# get current commit's SHA1
revParse = subprocess.Popen(args=["git", "rev-parse", "HEAD"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
if revParse.wait():     # returncode != 0?
    print "no git repository", revParse.returncode
    sys.exit(2)
sha1 = revParse.communicate()[0].strip()


# check if all staged
if subprocess.call(args=["git", "diff", "--exit-code"], stdout=subprocess.PIPE, stderr=subprocess.PIPE):
    print sha1 + " (" + unstagedTag + ")",
    sys.exit(1)

# check if all committed
if subprocess.call(args=["git", "diff", "--cached", "--exit-code"], stdout=subprocess.PIPE, stderr=subprocess.PIPE):
    print sha1 + " (" + uncommittedTag + ")",
    sys.exit(1)

# print result
print sha1,

