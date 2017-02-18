import os, cffi

debug_or_release = False

def set_debug_mode():
    global debug_or_release
    debug_or_release = True

def get_build_path():
    return '/opt/coda/{}/lib'.format(
        'debug' if debug_or_release else 'release')

def load_cffi_library(src, name):
    ffi = cffi.FFI()
    ffi.cdef(src)
    build_path = get_build_path()
    full_path = os.path.join(build_path, 'lib{}.so'.format(name))
    lib = ffi.dlopen(full_path)
    return ffi, lib
