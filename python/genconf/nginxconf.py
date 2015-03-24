#!/usr/bin/env python
# -*- coding:utf-8 -*-

import os, re
from jinja2 import Environment, PackageLoader, FileSystemLoader
tenv = None
from config import g, sites

def init_tenv() :
    global tenv
    tenv = Environment(loader=FileSystemLoader('template'))

class NginxConf():
    ''' nginx 配置文件产生器 '''

    nginx_dir = ''
    activated_rules_dir = ''
    sites_dir = ''

    def __init__(self, installed_dir='/usr/local/nginx/'):
        self.nginx_dir = installed_dir
        self.activated_rules_dir = os.path.join(self.nginx_dir, 'conf/activated_rules/')
        self.custom_rules_dir = os.path.join(self.nginx_dir, 'conf/custom_rules/')
        self.sites_dir = os.path.join(self.nginx_dir, 'conf/sites_enabled/')

    def genReverseProxyConf(self):
        tenv.get_template('reverse-proxy.jinja2').stream(g=g, sites=sites).dump(os.path.join(g['sites_dir'], 'proxy.conf'))

class NginxController():
    ''' nginx 控制器 '''

    exe_name = 'nginx'
    nginx_dir = '/usr/local/nginx/'

    def __init__(self, name='nginx', installed_dir='/usr/local/nginx/'):
        self.exe_name = name
        self.nginx_dir = installed_dir

    def start(self):
        return os.system(os.path.join(self.nginx_dir, 'sbin/', self.exe_name))

    def restart(self):
        self.stop()
        self.start()

    def stop(self):
        return os.system(os.path.join(self.nginx_dir, 'sbin/', self.exe_name) + ' -s stop')

    def reload(self):
        return os.system(os.path.join(self.nginx_dir, 'sbin/', self.exe_name) + ' -s reload')

    def test(self):
        return os.system(os.path.join(self.nginx_dir, 'sbin/', self.exe_name) + ' -t')

if __name__ == '__main__':
    init_tenv()
    NginxConf().genReverseProxyConf()
