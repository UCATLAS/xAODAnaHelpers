#!/usr/bin/env python
# -*- coding: utf-8 -*-,
from __future__ import absolute_import
from __future__ import print_function
import logging

logger = logging.getLogger("xAH.timing")
logger.setLevel(10) # we use info
print = logger.info

import atexit
from time import time

def secondsToStr(t):
    return "%d:%02d:%02d.%03d" % \
        reduce(lambda ll,b : divmod(ll[0],b) + ll[1:],
            [(t*1000,),1000,60,60])

line = "="*40
def log(s, elapsed=None):
    print(line)
    print("{0}-{1}".format(secondsToStr(time()), s))
    if elapsed:
        print("Elapsed time: {0}".format(elapsed))
    print(line)

def endlog():
    end = time()
    elapsed = end-start
    log("End Program", secondsToStr(elapsed))

def now():
    return secondsToStr(time())

start = time()
atexit.register(endlog)
log("Start Program")
