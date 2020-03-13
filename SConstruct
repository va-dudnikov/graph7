import os

################################################################################
## BASE VARIABLES

BASE_DIR = Dir('#').abspath
INCLUDE_DIR = os.path.join(BASE_DIR, 'include')

BUILD_DIR = 'build'
BUILD_SRC_DIR = os.path.join(BUILD_DIR, 'src')
BUILD_EXAMPLES_DIR = os.path.join(BUILD_DIR, 'examples')
BUILD_TESTS_DIR = os.path.join(BUILD_DIR, 'tests')
BUILD_DOCS_DIR = os.path.join(BUILD_DIR, 'docs')

# Version of library
MAJOR = 0
MINOR = 2
PATCH = 0

PRE_RELEASE = '-alpha.1'

VERSION = '{major}.{minor}.{patch}{pre_release}'.format(
    major=MAJOR,
    minor=MINOR,
    patch=PATCH,
    pre_release=PRE_RELEASE
)

################################################################################
## PREPARING

tools = []

# Get environment
if os.name == 'nt':
    # Let it be for now
    tools += ['mingw']

env = Environment(
    ENV=os.environ,
    tools=tools,
)

examples = bool(ARGUMENTS.get('examples', False))
docs = bool(ARGUMENTS.get('docs', False))
tests = bool(ARGUMENTS.get('tests', False))

env['VERSION'] = VERSION
env['CPPPATH'] = [INCLUDE_DIR]

################################################################################
## BUILDING

SConscript('src/SConscript', variant_dir=BUILD_SRC_DIR, duplicate=0, exports=['env'])

if docs:
    SConscript('docs/SConscript', variant_dir=BUILD_DOCS_DIR, duplicate=0, exports=['env'])

if examples:
    SConscript('examples/SConscript', variant_dir=BUILD_EXAMPLES_DIR, duplicate=0, exports=['env'])

if tests:
    SConscript('tests/SConscript', variant_dir=BUILD_TESTS_DIR, duplicate=0, exports=['env'])
