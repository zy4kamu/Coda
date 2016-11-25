import ctypes, os

build_path = '/home/stepan/veles.nlp/Coda-build/bin'

tokenizer_lib = ctypes.CDLL(os.path.join(build_path, 'libtokenizer.so'))

if __name__ == '__main__':
    tokenizer_lib.echo()
    tokenizer_lib.CreateTokenizer()
