import os

################################################################################
## BASE VARIABLES

BASE_DIR = Dir('#').abspath
INCLUDE_DIR = os.path.join(BASE_DIR, 'include')

# Version of library
MAJOR = 0
MINOR = 0
PATCH = 1

PRE_RELEASE = '-alpha.2'

VERSION = '{major}.{minor}.{patch}{pre_release}'.format(
    major=MAJOR,
    minor=MINOR,
    patch=PATCH,
    pre_release=PRE_RELEASE
)

################################################################################
## PREPARING

# Get environment
if os.name == 'nt':
    tools = ['mingw']
else:
    tools = []

env = Environment(tools=tools)

debug = bool(ARGUMENTS.get('debug', False))
examples = bool(ARGUMENTS.get('examples', False))
docs = bool(ARGUMENTS.get('docs', False))

env['VERSION'] = VERSION
env['CPPPATH'] = INCLUDE_DIR

################################################################################
## BUILDING

SConscript('src/SConscript', exports=['env'])

if examples:
    SConscript('examples/SConscript', exports=['env'])
