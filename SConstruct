import os

# Get environment
if os.name == 'nt':
    env = Environment(tools=['mingw'])
else:
    env = Environment()

debug = bool(ARGUMENTS.get('debug', False))
examples = bool(ARGUMENTS.get('examples', False))
docs = bool(ARGUMENTS.get('docs', False))

# Version of library
MAJOR = 0
MINOR = 0
PATCH = 2

VERSION = '{major}.{minor}.{patch}'.format(major=MAJOR, minor=MINOR, patch=PATCH)

env['VERSION'] = VERSION

SConscript('src/SConscript', exports=['env'])

if examples:
    SConscript('examples/SConscript', exports=['env'])

if docs:
    SConscript('docs/SConscript', exports=['env'])
