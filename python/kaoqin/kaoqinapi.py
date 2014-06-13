#!/usr/bin/env python
# -*- coding: utf-8 -*-
import urllib, urllib2, cookielib, md5, re

def dtstr2dict(data):
    from datetime import datetime, time
    t1 = datetime.strptime(data, '%Y-%m-%d %H:%M:%S')
    color = 'white'
    if (t1.time() > time(8, 30, 0) and t1.time() < time(12, 0, 0)) or \
       (t1.time() > time(13, 30, 0) and t1.time() < time(18, 0, 0)):
       color = 'red'
    return {'title': data, 'start': data, 'textColor': color}

class KaoQinAPI:
    '''login API for https://mp.weixin.qq.com'''
    url_login = 'http://113.108.195.66:38080/accounts/login/'
    url_query = 'http://113.108.195.66:38080/data/iclock/Transaction/'
    user = 'yanfa01'
    pwd = '111111'
    
    def __init__(self, imgcode=''):
        self.opener = urllib2.build_opener(urllib2.HTTPCookieProcessor(cookielib.CookieJar()))
        self.login()
        
    def login(self):
        body = (('username', self.user),
                ('password', self.pwd), # md5.new(pwd).hexdigest()
                ('template9', ''),
                ('finnger10', ''),
                ('finnger9', ''),
                ('template10', ''),
                ('login_type', 'pwd'),
                ('client_language', 'zh-cn'),)
        request = urllib2.Request(self.url_login)
        request.add_header('Referer', 'http://113.108.195.66:38080/accounts/login/')
        request.add_data(urllib.urlencode(body))
        result = self.opener.open(request).read()
        #print 'result', result
        #result = eval(result)
        
    def getdata(self, user):
        body = (('o', '-TTime,UserID__PIN'),
                ('stamp', '1401788986029'),
                #('UserID__EName__contains', '%E5%90%B4%E9%81%A5'),
                ('UserID__EName__contains', user),
                ('l', '200'),
                ('mnp', '200'),
                ('p', '1'),
                )
        request = urllib2.Request(self.url_query)
        request.add_header('Referer', 'http://113.108.195.66:38080/data/worktable/')
        request.add_data(urllib.urlencode(body))
        result = self.opener.open(request).read()
        #print 'result2', result
        heads = 'heads'
        data = 'data'
        fields = 'fields'
        record_count = 'record_count'
        item_from = 'item_from'
        current_page = 'current_page'
        page_limit = 'page_limit'
        page_count = 'page_count'
        order_by = 'order_by'
        k = eval(result)
        #return [ data[4].replace('\\u002D', '-') for data in k['data']]
        return [ dtstr2dict(data[4].replace('\\u002D', '-')) for data in k['data']]

if __name__ == '__main__':
    print KaoQinAPI().getdata('吴遥')
