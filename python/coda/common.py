import ctypes, os

debug_or_release = False

def set_debug_mode():
    global debug_or_release
    debug_or_release = True

def get_build_path():
    return '/opt/coda/{}/lib'.format(
        'debug' if debug_or_release else 'release')

def load_library(name):
    build_path = get_build_path()
    full_path = os.path.join(build_path, 'lib{}.so'.format(name))
    return ctypes.CDLL(full_path)
