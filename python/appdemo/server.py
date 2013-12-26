#!/usr/bin/env python

import sys, time, argparse
from daemon import Daemon

class ServerApplication(Daemon):
    def run(self):
        self.begin()
        while True:
            time.sleep(1)
        self.end()
    def begin(self):
        pass
    def end(self):
        pass

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='A Daemon Application of Demo Writen by WuYao.')
    parser.add_argument('-v', action='version', dest='version', version='0.51')
    parser.add_argument("-p", action='store', dest='port')
    parser.add_argument('command', action='store', choices=['start', 'stop', 'restart'], help='action')
    args = parser.parse_args()
    app = ServerApplication('/tmp/daemon-wuyao-server.pid')
    if args.command == 'start':
        app.start()
    elif args.command == 'stop':
        app.stop()
    elif args.command == 'restart':
        app.restart()
