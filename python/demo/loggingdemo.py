#!/usr/bin/env python
# -*- coding:utf-8 -*-

import logging  
import os
logging.basicConfig(filename = os.path.join(os.getcwd(), 'log.txt'), \
                    level = logging.WARN, filemode = 'a', format = '%(asctime)s %(filename)s:%(lineno)d [%(levelname)s] %(message)s')  
logger = logging.getLogger('root.main')
#logger.setLevel(logging.CRITICAL)
#logger.setLevel(logging.ERROR)
#logger.setLevel(logging.WARN)
#logger.setLevel(logging.WARNING)
#logger.setLevel(logging.INFO)
logger.setLevel(logging.DEBUG)
#logger.setLevel(logging.NOTSET)
logger.critical('critical')
logger.fatal('fatal equal to critical')
logger.error('error')
#logger.exception('exception')
logger.info('info')
logger.debug('debug')
logger.warning('warnning')
logger.warn('warn equal to warning')
