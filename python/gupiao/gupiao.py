# -*- coding:utf-8 -*-
#!/usr/bin/env python  

from collections import Counter
from operator import itemgetter
from gpdata import getgpdata

def threedaychange(code, start=None, end=None, trend=1):
    gp = getgpdata(code, start, end)
    changlist = []
    if trend:
        changlist.append([gp[0]['date'], gp[0]['low']])
        changlist.append([gp[0]['date'], gp[0]['low']])
    else:
        changlist.append([gp[0]['date'], gp[0]['high']])
        changlist.append([gp[0]['date'], gp[0]['high']])
    
    first, second, third = gp[0], gp[1], gp[2]
    for i in range(3, len(gp)+1):
        if first['close'] > first['open'] and \
                second['close'] > second['open'] and \
                third['close'] > third['open'] and \
                third['high'] > second['high'] > first['high']:
            if trend:
                changlist[-1] = [third['date'], third['high']]
            else:
                trend = 1
                changlist.append([third['date'], third['high']])
        elif first['close'] < first['open'] and \
                second['close'] < second['open'] and \
                third['close'] < third['open'] and \
                third['low'] < second['low'] < first['low']:
            if trend:
                trend = 0
                changlist.append([third['date'], third['low']])
            else:
                changlist[-1] = [third['date'], third['low']]
        elif trend and \
                third['high'] > second['high'] and \
                third['high'] > first['high'] and \
                third['close'] > third['open']:
            changlist[-1] = [third['date'], third['high']] if third['high'] > changlist[-1][1] else changlist[-1]
        elif not trend and \
                third['low'] < second['low'] and \
                third['low'] < first['low'] and \
                third['close'] < third['open']:
            changlist[-1] = [third['date'], third['low']] if third['low'] < changlist[-1][1] else changlist[-1]
        elif third['high'] > second['high'] and \
                third['high'] > first['high'] and \
                third['close'] > second['close'] and \
                third['close'] > first['close']:
            if trend:
                changlist[-1] = [third['date'], third['high']] if third['high'] > changlist[-1][1] else changlist[-1]
            else:
                trend = 1
                changlist.append([third['date'], third['high']])
        elif third['low'] < second['low'] and \
                third['low'] < first['low'] and \
                third['close'] < second['close'] and \
                third['close'] < first['close']:
            if trend:
                trend = 0
                changlist.append([third['date'], third['low']])
            else:
                changlist[-1] = [third['date'], third['low']] if third['low'] < changlist[-1][1] else changlist[-1]
        else:
            if trend and third['high'] > changlist[-1][1]:
                changlist[-1] = [third['date'], third['high']]
            if not trend and third['low'] < changlist[-1][1]:
                changlist[-1] = [third['date'], third['low']]
        if i < len(gp):
            first, second, third = second, third, gp[i]
    if changlist:
        changlist[0] = [changlist[0][0], changlist[0][1], 0]
        for i in range(1, len(changlist)):
            dis = changlist[i][1]-changlist[i-1][1]
            baifenbi = "%.2f" % (abs(dis) / (( changlist[i][1]+changlist[i-1][1] ) / 2) * 100)
            changlist[i] = [changlist[i][0], changlist[i][1], int(round(dis)), float(baifenbi)]
    return changlist

def gpmax(code, start=None, end=None):
    gp = getgpdata(code, start, end)
    m = 0
    for l in gp:
        m = l['high'] if l['high'] > m else m
    return m

def gpmin(code, start=None, end=None):
    gp = getgpdata(code, start, end)
    m = 1000000000
    for l in gp:
        m = l['low'] if l['low'] < m else m
    return m

def fenbu(changlist):
    l = []
    for i in range(1, len(changlist)):
        l.append(int(round(changlist[i][1]-changlist[i-1][1])))
    return sorted(Counter(l).iteritems(), key=itemgetter(0), reverse=False)

if __name__ == '__main__':
    #cl = threedaychange('sh000001', start='2010-03-05', end='2010-04-15')
    cl = threedaychange('sh000001')
    fp = open('cl.txt', 'w')
    for l in cl:
        fp.write(str(l))
        fp.write('\n')
    fp.close()
    fp = open('fb.txt', 'w')
    fb = fenbu(cl)
    for i,j in fb:
        if j >= 9:
            print [i,j]
    for i,j in fb:
        fp.write(str([i,j]))
        fp.write('\n')
    fp.close()
