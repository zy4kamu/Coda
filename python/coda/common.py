import ctypes, os

build_path = '/opt/coda/lib'

def load_library(name):
    full_path = os.path.join(build_path, 'lib{}.so'.format(name))
    return ctypes.CDLL(full_path)
