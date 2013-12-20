#!/usr/bin/env python
# -*- coding:utf-8 -*-

from pyquery import PyQuery as pq
import urllib
import urllib2

def getHtmlData(url, **kwargs):
    print 'url:', url
    opener = urllib2.build_opener()
    request = urllib2.Request(url)
    #print 'url verify: ' request.geturl()
    request.add_header('User-Agent', 'Mozilla/5.0 (Windows NT 5.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/27.0.1453.116 Safari/537.36')
    if 'referer' in kwargs:
        request.add_header('Referer', kwargs['referer'])
    data = opener.open(request).read()
    #print 'url verify: ' request.geturl()
    if 'charset' in kwargs:
        data = data.decode(kwargs['charset'], 'ignore')
    return data

def getPyquryHtml(url, referer=None, charset='utf-8'):
    opener = urllib2.build_opener(urllib2.HTTPCookieProcessor())
    request = urllib2.Request(url)
    request.add_header('User-Agent', 'Mozilla/5.0 (Windows NT 5.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/27.0.1453.116 Safari/537.36')
    if referer:
        request.add_header('Referer', referer)
    html = pq(url=url, opener=lambda url, **kw: opener.open(request).read().decode(charset, 'ignore'))
    return html

if __name__ == '__main__':
    tiebaurl='http://tieba.baidu.com/f?kw=%E4%BB%99%E9%AD%94%E5%8F%98'
    referer='http://tieba.baidu.com'
    charset='gbk'
    html2 = pq(url=tiebaurl, opener=getHtmlData, referer=referer, charset=charset)
    html = getPyquryHtml('http://read.qidian.com/BookReader/2502372.aspx', referer='http://read.qidian.com')
