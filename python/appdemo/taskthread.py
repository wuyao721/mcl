#!/usr/bin/env python
# code from <<python cookbook 2nd Edition>> 9.2 and http://code.activestate.com/recipes/65222/
# modify by wuyao721

import threading

class TaskThread(threading.Thread):
    """Thread that executes a task every N seconds"""

    def __init__(self, name='TaskThread', interval=1.0):
        self._stopevent = threading.Event()
        self._interval = interval
        threading.Thread.__init__(self, name=name)

    def setInterval(self, interval):
        """Set the number of seconds we sleep between executing our task"""
        self._interval = interval
    
    def run(self):
        """ main control loop """
        self.taskprepare()
        try:
            while not self._stopevent.isSet():
                self.task()
                self._stopevent.wait(self._interval)
        except Exception, e:
            self.exchandler(e)
        finally:
            self.taskclean()
    
    def stop(self, timeout=None):
        """ Stop the thread and wait for it to end. """
        self._stopevent.set()
        threading.Thread.join(self, timeout)

    def task(self):
        """The task done by this thread - override in subclasses"""
        pass

    def taskprepare(self):
        """The task done by this thread - override in subclasses """
        pass

    def taskclean(self):
        """The task done by this thread - override in subclasses"""
        pass

    def exchandler(self, e):
        """exception handler - override in subclasses"""
        pass
