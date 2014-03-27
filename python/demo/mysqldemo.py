#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
computer with 1cpu,4kernel,4G mem 
mysql table t_waflogs:
+----------+---------------+------+-----+---------+----------------+
| Field    | Type          | Null | Key | Default | Extra          |
+----------+---------------+------+-----+---------+----------------+
| id       | int(11)       | NO   | PRI | NULL    | auto_increment |
| src_ip   | varchar(30)   | YES  |     | NULL    |                |
| url      | varchar(45)   | YES  |     | NULL    |                |
| location | varchar(45)   | YES  |     | NULL    |                |
| desc     | varchar(1024) | YES  |     | NULL    |                |
| rules_id | int(11)       | NO   | MUL | NULL    |                |
| time     | int(11)       | YES  |     | NULL    |                |
+----------+---------------+------+-----+---------+----------------+

execute result:
'func_mysql_client' ((), {}) 1.97 sec
'func_mysql_client2' ((), {}) 0.32 sec
'func_mysql_client3' ((), {}) 0.25 sec
'func_mysqldb' ((), {}) 2.09 sec
'func_mysqldb2' ((), {}) 0.26 sec
'func_sqlalchemy' ((), {}) 7.92 sec

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
def func_mysql_client():
    os.system("mysql -uroot -pbluedon -e 'source test.sql'")

@timeit
def func_mysql_client2():
    os.system("mysql -uroot -pbluedon -e 'source test2.sql'")

@timeit
def func_mysql_client3():
    os.system("mysql -uroot -pbluedon -e 'load data infile \"test3.sql\" replace into table waf.t_waflogs(src_ip, url, location, location, `desc`, rules_id)'")


@timeit
def func_mysqldb():
    try:
        conn=MySQLdb.connect(host='localhost',user='root',passwd='bluedon',db='waf',port=3306)
        cur=conn.cursor()
        for i in range(40000):
            cur.execute("insert into t_waflogs(src_ip, url, location, `desc`, rules_id) values('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345);")
        conn.commit()
        cur.close()
        conn.close()
    except MySQLdb.Error,e:
        print "Mysql Error %d: %s" % (e.args[0], e.args[1])

@timeit
def func_mysqldb2():
    try:
        conn=MySQLdb.connect(host='localhost',user='root',passwd='bluedon',db='waf',port=3306)
        cur=conn.cursor()
        for i in range(400):
            cur.execute("insert into t_waflogs(src_ip, url, location, `desc`, rules_id) values('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345),('wuyao', 'http://www.wuyao.org/index.html', 'wuyaowuyaowuyao', 'wuyaowuyaowuyaowuyaowuyao', 12345);")
        conn.commit()
        cur.close()
        conn.close()
    except MySQLdb.Error,e:
        print "Mysql Error %d: %s" % (e.args[0], e.args[1])

@timeit
def func_sqlalchemy():
    session = Session()
    for i in range(40000):
        s = WafLogSet()
        session.add(s)
    session.commit()

if __name__ == '__main__':
    func_mysql_client()
    func_mysql_client2()
    func_mysql_client3()
    func_mysqldb()
    func_mysqldb2()
    func_sqlalchemy()
