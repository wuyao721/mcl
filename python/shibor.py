# -*- coding:utf-8 -*-

from pyquery import PyQuery as pq
import urllib

def getShiborInfo():
    l = {}
    url = 'http://www.shibor.org/shibor/web/html/shibor.html'
    html = pq(url=url, opener=lambda url, **kw: urllib.urlopen(url).read())
    tr = html('body')('table').eq(0)('tr').eq(2)('table').eq(1)('tr')
    if not tr:
        return l
    for i in range(8):
        key = tr.eq(i)('td').eq(1).text()
        value = tr.eq(i)('td').eq(2).text()
        l[key] = value
    return l

if __name__ == '__main__':
    shibor = getShiborInfo()
    print 'O/N: ', shibor['O/N']
    print '1W: ', shibor['1W']
    print '2W: ', shibor['2W']
    print '1M: ', shibor['1M']
    print '3M: ', shibor['3M']
    print '6M: ', shibor['6M']
    print '9M: ', shibor['9M']
    print '1Y: ', shibor['1Y']
