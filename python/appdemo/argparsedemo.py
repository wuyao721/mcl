# -*- coding:utf-8 -*-
# refer to http://docs.python.org/2.7/library/argparse.html

import argparse

parser = argparse.ArgumentParser(description='This is a parse args sample program')
parser.add_argument("-a", action='store_true', dest='a', default=False)
parser.add_argument("--add", action='store_true', dest='a', default=False)
parser.add_argument("-b", action='store_true', dest='b')
parser.add_argument("-c", action='store', dest='c', type=int)
parser.add_argument("--count", action='store', dest='c', type=int)
parser.add_argument("domain", action='store', help='set the domain')
parser.add_argument('-v', '--version', action='version', version='2.0')

print parser.parse_args()
print parser.parse_args(['-a', 'www.google.com'])
print parser.parse_args(['-a', '-c', '13', '-b', 'www.google.com'])
print parser.parse_args(['www.google.com', '-a', '--count=12', '-b'])

