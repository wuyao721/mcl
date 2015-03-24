# -*- coding: utf-8 -*-

from pyquery import PyQuery as pq
import urllib
import chengyudb

# http://chengyu.itlearner.com/cy0/1.html

def chengyuitleanercom_spider(index):
    i = (index-1) / 1000
    _url = "http://chengyu.itlearner.com/cy%s/%s.html" % (i, index)
    _html = pq(url=_url, opener=lambda url, **kw: urllib.urlopen(url).read().decode('gb18030'))
    name = _html('table').eq(0)('tr').eq(0)('td').eq(1).text()
    spell = _html('table').eq(0)('tr').eq(1)('td').eq(1).text()
    description = _html('table').eq(0)('tr').eq(2)('td').eq(1).text()
    source = _html('table').eq(0)('tr').eq(3)('td').eq(1).text()
    example = _html('table').eq(0)('tr').eq(4)('td').eq(1).text()
    near = u''
    antonym = u''
    if u'近义词' == _html('table').eq(0)('tr').eq(5)('td').eq(0).text():
        near = _html('table').eq(0)('tr').eq(5)('td').eq(1).text()
    if u'近义词' == _html('table').eq(0)('tr').eq(6)('td').eq(0).text():
        near = _html('table').eq(0)('tr').eq(6)('td').eq(1).text()
    if u'反义词' == _html('table').eq(0)('tr').eq(5)('td').eq(0).text():
        antonym = _html('table').eq(0)('tr').eq(5)('td').eq(1).text()
    if u'反义词' == _html('table').eq(0)('tr').eq(6)('td').eq(0).text():
        antonym = _html('table').eq(0)('tr').eq(6)('td').eq(1).text()
    chengyudb.add_chengyu(name, spell, description, source, example, near, antonym)
    file_object = open('thefile.txt', 'w')
    file_object.write(name.encode('utf-8'))
    file_object.close()
    #print index, type(name), name.encode('gb2312')
    print (index, name, spell, description, source, example)
    
if __name__ == '__main__':
    chengyuitleanercom_spider(1)
    #chengyuitleanercom_spider(10)
    #chengyuitleanercom_spider(100)
    #chengyuitleanercom_spider(1000)
    #chengyuitleanercom_spider(10000)
    #chengyuitleanercom_spider(12345)
    #chengyuitleanercom_spider(30000)
    #chengyuitleanercom_spider(30897)
#    for i in range(1, 30897):
#        chengyuitleanercom_spider(i)
#        print 'number', i, 'done'
