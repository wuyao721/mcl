# -*- coding:utf-8 -*-
#!/usr/bin/env python  
#http://matplotlib.org/gallery.html
#http://matplotlib.org/examples/pylab_examples/step_demo.html
#http://matplotlib.org/examples/pylab_examples/barh_demo.html

import sys
import numpy, pylab
import matplotlib.pyplot as pyplot
from collections import Counter
from operator import itemgetter
import gupiao

def nightpoint(code, start=None, end=None):
    cl = gupiao.threedaychange(code, start, end)
    ll = map(lambda x: abs(x[3]), cl[1:len(cl)])
    ll.sort()
    print len(ll)
    k = 1.0/len(ll)
    l = 0.0
    for i in ll:
        l = l + k
        if 0.05 > l > 0.04:
            print '0.05:', i
        if 0.255 > l > 0.25:
            print '25:', i
        if 0.510 > l > 0.500:
            print '50:', i
        if 0.751 > l > 0.750:
            print '75:', i
    d = sorted(Counter(ll).iteritems(), key=itemgetter(0), reverse=False)
    for i,j in d:
        if j >= 9:
            print [i,j]
    x = numpy.array(ll)
    n, bins, patches = pyplot.hist(x, (max(ll)+1)*10, normed=1, facecolor='green')
    pyplot.axis([0, (max(ll)+1), 0, 0.3])
    pyplot.grid(True)
    pyplot.show()

if __name__ == '__main__':
    #nightpoint('sh000001', start='2008-10-28')
    nightpoint('sh000001')
    pass
