# -*- coding:utf-8 -*-
#!/usr/bin/env python  
#http://matplotlib.org/gallery.html
#http://matplotlib.org/examples/pylab_examples/step_demo.html
#http://matplotlib.org/examples/pylab_examples/barh_demo.html

import numpy
import matplotlib.pyplot as pyplot
import gupiao

if __name__ == '__main__':
    l = gupiao.threedaychang(gupiao.gp000001ss)
    #l = [100, 100, 500, 500, 2080, 2080]
    #l = [100, 500, 2080]
    #open('l.txt', 'w').write(str(l))
    x = numpy.arange(0, len(l), 1)
    y = numpy.array(map(lambda x:x[1], l))
    #y = numpy.array(l)
    pyplot.xticks(x, map(lambda x:x[0], l))
    pyplot.step(x, y, label='pre (default)')
    pyplot.legend()
    pyplot.xlim(-1, len(l))
    pyplot.ylim(0, 7000)
    pyplot.show()
