import os

# SRC_PATH = os.path.relpath(os.path.join(os.path.dirname(__file__), "../../src/libpyflatsurf/"))
# SRC_PATH = os.path.relpath(os.path.join(os.path.dirname(__file__), "./"))
SRC_PATH = os.path.dirname(__file__)

from distutils.core import setup

from setuptools.command.egg_info import egg_info

class EggInfoCommand(egg_info):
    def run(self):
        if "build" in self.distribution.command_obj:
            build_command = self.distribution.command_obj["build"]
            self.egg_base = build_command.build_base
            self.egg_info = os.path.join(self.egg_base, os.path.basename(self.egg_info))
            egg_info.run(self)

setup(
    name = 'pyflatsurf',
    version = '0.0.0',
    packages = ['pyflatsurf',],
    package_dir={
        '': SRC_PATH,
    },
    cmdclass={
        "egg_info": EggInfoCommand,
    },
    license = 'GPL 2.0+',
    long_description = open('../../README.md').read(),
)
