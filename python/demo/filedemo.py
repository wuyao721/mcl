#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
computer with 1cpu,4kernel,4G mem 
14550 files, every file is about more than 1K, result is: 
'func_multifile' ((), {}) 0.35 sec
'func_multifile2' ((), {}) 0.17 sec
'func_multifile_multiline' ((), {}) 0.19 sec
'func_onefile' ((), {}) 0.13 sec
'func_onefile2' ((), {}) 0.03 sec
'func_onefile_multiline' ((), {}) 0.03 sec
'''


import os, time, re, threading
import MySQLdb
from db import Session, WafLogSet

def timeit(method):
    def timed(*args, **kw):
        ts = time.time()
        result = method(*args, **kw)
        te = time.time()
        print ('%r (%r, %r) %2.2f sec' % (method.__name__, args, kw, te-ts))
        return result
    return timed

@timeit
def func_multifile():
    path = '/usr/local/bluedon/bdwaf/logs/audit/20140311/20140311-1431'
    for name in os.listdir(path):
        fullpath = os.path.join(path, name)
        with open(fullpath) as fp:
            while fp.readline():
                pass

@timeit
def func_multifile2():
    path = '/usr/local/bluedon/bdwaf/logs/audit/20140311/20140311-1431'
    for name in os.listdir(path):
        fullpath = os.path.join(path, name)
        for line in open(fullpath):
            pass

@timeit
def func_multifile_multiline():
    path = '/usr/local/bluedon/bdwaf/logs/audit/20140311/20140311-1431'
    #i = 1
    for name in os.listdir(path):
        fullpath = os.path.join(path, name)
        with open(fullpath) as fp:
            while fp.readlines(10000):
                pass

@timeit
def func_onefile():
    '''
    14550 files convert to one file,
    all the file has 422182 lines.
    '''
    with open('/root/a.txt') as fp:
        while fp.readline():
            pass

@timeit
def func_onefile2():
    '''
    14550 files convert to one file,
    all the file has 422182 lines.
    '''
    for line in open('/root/a.txt'):
        pass

@timeit
def func_onefile_multiline():
    '''
    14550 files convert to one file,
    all the file has 422182 lines.
    '''
    with open('/root/a.txt') as fp:
        while fp.readlines(10000):
            pass


if __name__ == '__main__':
    func_multifile()
    func_multifile2()
    func_multifile_multiline()
    func_onefile()
    func_onefile2()
    func_onefile_multiline()


