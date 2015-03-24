#!/usr/bin/env python
# -*- coding:utf-8 -*-

g = {
    'sites_dir': 'c:/Program Files/nginx/conf/sites_enabled',
    'cachepath': 'c:/Program Files/nginx/cache',
    'proxy_temp_path': 'c:/Program Files/nginx/temp/proxy_temp',
}

sites = [
    { 'host': 'www.deny.com',
      'ports': [80],
      'servers': [['172.0.0.1', '8080', '1'],],
      'hatype': 'weight',
      'healthcheck': True,
      'cache': True,
      'cache_size': '10m',
      'cache_max_size': '30m',
      'modsecurity': False,
  },
]
