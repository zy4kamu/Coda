# DESCRIPTION
Python and C++ realization of NLP stack for **Russian** and **English** language. Includes:

* Sentence splitter
* Tokenizer
* Morphology disambiguation (POS-tagging and POS+MORPH-tagging)
* Dependency parser
* Stresser
* Inflector
* ...

# REQUIREMENTS

* Ubuntu 14.04 or higher
* cmake version >= 2.8
* gcc version >= 4.8
* python version 2.7
* graphviz (to visualize trees).

# INSTALLATION
1. sudo bash build_cpp.sh (This script builds c++ part of application and put libraries, exec files and configs to /opt/coda)
2. bash install_python.sh (This script installs python wrapper .egg over c++ to corresponding site_packages folder)

# USAGE EXAMPLE
* Try:
**1. cd coda/python
**2. python usage_example.py
  
* usage_example.py content:
```python 
# -*- coding: utf-8 -*-
from coda.tokenizer import Tokenizer
from coda.disambiguator import Disambiguator
from coda.syntax_parser import SyntaxParser

if __name__ == '__main__':
    tokenizer = Tokenizer("RU")
    disambiguator = Disambiguator("RU")
    syntax_parser = SyntaxParser("RU")

    sentence = u'МИД пригрозил ограничить поездки американских дипломатов по России.'
    tokens = tokenizer.tokenize(sentence)
    disambiguated = disambiguator.disambiguate(tokens)
    tree = syntax_parser.parse(disambiguated)

    print tree.to_string()
    tree.draw(dot_file="/tmp/tree1.dot", show=True)
```
* Expected output:
``` 
0 1 МИД S@ЕД@МУЖ@ИМ@НЕОД мид
1 -1 пригрозил V@СОВ@ИЗЪЯВ@ПРОШ@ЕД@МУЖ пригрозить
2 1 ограничить V@СОВ@ИНФ ограничить
3 2 поездки S@МН@ЖЕН@ВИН@НЕОД поездка
4 5 американских A@МН@РОД американский
5 3 дипломатов S@МН@МУЖ@РОД@ОД дипломат
6 3 по PR по
7 6 России S@ЕД@ЖЕН@ДАТ@НЕОД россия
```
![alt tag](https://github.com/zy4kamu/Coda/files/636557/example_tree.pdf)
