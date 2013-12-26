#!/usr/bin/env python

import taskthread

class MyTask(taskthread.TaskThread):
    """Thread that executes a task every N seconds"""

    def __init__(self):
        taskthread.TaskThread.__init__(self, name='MyTaskThread')

    def task(self):
        """The task done by this thread - override in subclasses"""
        print 'do task.'

    def taskprepare(self):
        """The task done by this thread - override in subclasses """
        print 'task prepare.'

    def taskclean(self):
        """The task done by this thread - override in subclasses"""
        print 'task clean.'

    def exchandler(self, e):
        """exception handler - override in subclasses"""
        print e

if __name__ == "__main__":
    task = MyTask()
    task.start()
    import time
    time.sleep(10.0)
    task.stop()
