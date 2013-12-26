#!/usr/bin/env python

import os, sys, time, argparse

if os.name == 'nt':
    from pysvc import PySvc as ServerApplication
    from pysvc import main
elif os.name == 'posix':
    from daemon import Daemon as ServerApplication
    from daemon import main
else:
    sys.stderr.write("OS NOT SUPPORT: %s\n" % (os.name, ))
    sys.exit(1)

if __name__ == '__main__':
    class MyServerApp(ServerApplication):
        def begin(self):
            print 'begin.'
        def end(self):
            print 'end.'
    main(MyServerApp)
