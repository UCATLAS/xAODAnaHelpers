from __future__ import print_function
import atexit
from time import time
import logging

logger = logging.getLogger("timing")
logger.setLevel(10) # we use info
print = logger.info

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
    print("")

def endlog():
    end = time()
    elapsed = end-start
    print("")
    log("End Program", secondsToStr(elapsed))

def now():
    return secondsToStr(time())

start = time()
atexit.register(endlog)
log("Start Program")

