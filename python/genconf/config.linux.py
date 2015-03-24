#!/usr/bin/env python
# -*- coding:utf-8 -*-

g = {
    'sites_dir': '/usr/local/nginx/conf/sites_enabled',
    'cachepath': '/usr/local/nginx/cache',
    'proxy_temp_path': '/usr/local/nginx/temp/proxy_temp',
}

sites = [
    { 'host': 'www.test1.com',
      'ports': [80, 8080],
      'servers': [['192.168.1.1', '80', '1'], ['172.16.5.21', '8080', '2']],
      'hatype': 'weight',
      'healthcheck': True,
      'cache': True,
      'cache_size': '10m',
      'cache_max_size': '30m',
      'modsecurity': False,
  },
]
