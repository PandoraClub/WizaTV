from setuptools import setup, find_packages

long_description = """\
Really it lets you cross build binary extensions.

You need to export PYTHONXCPREFIX and LDFLAGS, something like:
$ export PYTHONXCPREFIX=/opt/eldk/ppc_4xxFP/usr
$ export LDFLAGS="-L/opt/eldk/ppc_4xxFP/lib -L/opt/eldk/ppc_4xxFP/usr/lib"

It should pick up the correct include paths from the PYTHONXCPREFIX. To build
use:
$ python setup.py build -x
To make a cross compiled egg:
$ python setup.py build -x bdist_egg --plat-name linux-ppc --exclude-source-files
"""
setup(
    name = "distutilscross",
    version = "0.1",
    description="Cross Compile Python Extensions",
    long_description=long_description,
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: MIT License',
        'Topic :: Software Development :: Build Tools',
    ],
    keywords="distutils setuptools egg compile cross-compile",
    author="Chris Lambacher",
    author_email="chris@kateandchris.net",
    url="http://bitbucket.org/lambacck/distutilscross/",
    license='MIT',
    packages = find_packages(),
    entry_points = {
        "distutils.commands": [
            "build=distutilscross.crosscompile:build",
        ],
    },
)

