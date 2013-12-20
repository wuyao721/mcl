#!/usr/bin/env python
# -*- coding: utf-8 -*-

import re

re1 = re.compile('^([\w\d-]+\.)+[\w]+$')
def domainValid(domain):
    return re1.match(domain)

if __name__ == '__main__':
    print domainValid('www.baidu.com')
    print domainValid('www.bai-du.com')
    print domainValid('www.baidu..com')
    print domainValid('www.baidu.com.')
    print domainValid('.www.baidu.com')
