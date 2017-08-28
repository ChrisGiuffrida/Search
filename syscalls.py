#! /usr/bin/env python2.7


import os

print ("# search /etc")
os.system('strace ./search /etc 2>&1 > /dev/null | cut -d "(" -f 1 | sort | uniq -c | sort -nr | grep -v ")" ')

print

print ("# find /etc")
os.system('strace find /etc 2>&1 > /dev/null | cut -d "(" -f 1 | sort | uniq -c | sort -nr | grep -v ")" | grep -v "SIG"')

print

print ("# search /etc -exec echo \{\} \;")
os.system('strace ./search /etc -exec echo \{\} \; 2>&1 > /dev/null | cut -d "(" -f 1 | sort | uniq -c | sort -nr | grep -v ")" | grep -v "SIG"')

print

print ("# find /etc -exec echo \{\} \;")
os.system('strace find /etc -exec echo \{\} \; 2>&1 > /dev/null | cut -d "(" -f 1 | sort | uniq -c | sort -nr | grep -v ")" | grep -v "SIG" ')
