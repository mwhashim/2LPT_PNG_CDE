import subprocess
from distutils.core import setup

import os

# Compile c code
p = subprocess.Popen('make').wait()

subprocess.call(["./2LPT_CDE_PNG"])

setup(name='2LPTNG',
      version='1.0',
      packages=['2LPTNG', '2LPTNG.2LPTNG'],
      )