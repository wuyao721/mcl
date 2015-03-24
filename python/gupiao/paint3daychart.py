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

def threeday(code, start, end):
    l = gupiao.threedaychange(code, start, end)
    label = "(max, min, lastday, percent): (%s, %s, %s, %s)" % (gupiao.gpmax(code, start, end), gupiao.gpmin(code, start, end), l[-1][1], l[-1][3])
    x = numpy.arange(0, len(l), 1)
    y = numpy.array(map(lambda x:x[1], l))
    pyplot.xticks(x, map(lambda x:x[0], l))
    pyplot.step(x, y, label=label)
    pyplot.legend()
    pyplot.xlim(-1, len(l))
    mi = gupiao.gpmin(code, start, end)
    ma = gupiao.gpmax(code, start, end)
    pyplot.ylim(mi-10, ma+50)
    pyplot.show()

if __name__ == '__main__':
    #python paint3daychart.py "sh000001" "2010-03-05" "2010-04-27"
    threeday(sys.argv[1], sys.argv[2], sys.argv[3])
    #threeday('sh000001', start='2010-03-05', end='2010-04-27')
    #nightpoint('sh000001', start='2010-03-05', end='2010-04-27')
    pass
