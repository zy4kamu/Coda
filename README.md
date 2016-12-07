# DESCRIPTION
Python and C++ realization of NLP stack for **Russian** and **English** language. Includes:

* Sentence splitter
* Tokenizer
* Morphology disambiguation
* Dependency parser
* Stresser
* Inflector
* ...

# REQUIREMENTS

* *Ubuntu* 14.04 or higher
* *cmake* version >= 2.8
* *gcc* version >= 4.8
* *python* version 2.7
* *graphviz* (to visualize trees).

# INSTALLATION
1. *sudo bash build_cpp.sh* 
    + This script builds C++ part of application and puts libraries, execution files and configs to */opt/coda*
2. *bash install_python.sh*
    + This script installs Python wrapper .egg over C++ to corresponding site_packages folder)

# USAGE EXAMPLE
* Try: *python coda/python/usage_examply.py*

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
![example_tree-1](https://cloud.githubusercontent.com/assets/6217675/20967627/b846dbb4-bc80-11e6-931e-93f7ebc97c44.png)
