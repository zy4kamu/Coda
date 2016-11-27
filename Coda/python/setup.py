from setuptools import setup
import os

current_folder = os.path.dirname(os.path.realpath(__file__))

setup(name = "coda",
        version = "1.0",
        packages = ["coda"],
        scripts=["usage_example.py"],
        )
