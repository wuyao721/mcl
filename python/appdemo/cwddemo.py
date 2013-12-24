#!/usr/bin/env python
# -*- coding:utf-8 -*-

import os
cwd = os.getcwd()
print 'cwd:', cwd
os.chdir(os.path.join(cwd, '..'))
print 'change to up dir, new cwd:', os.getcwd()
