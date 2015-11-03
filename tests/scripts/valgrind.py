#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Copyright 2015 (c) Ewerton Assis
#

import os
import sys


def sh(command):
    return os.system(command)

if __name__ == '__main__':
    if sys.argv > 1:
        code = sh("valgrind --leak-check=yes --error-exitcode=1 {0}".format(sys.argv[1]))
        sys.exit(code)
