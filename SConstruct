import os

# Get environment
env = Environment(tools=['mingw'])

# Version of library
MAJOR = 0
MINOR = 0
PATCH = 1

VERSION = '{major}.{minor}.{patch}'.format(major=MAJOR, minor=MINOR, patch=PATCH)

env['VERSION'] = VERSION

SConscript('src/SConscript', exports=['env'])
SConscript('development/SConscript', exports=['env'])
