from conans import ConanFile, CMake, tools
import os

class ConanBuild(ConanFile):
    generators = "cmake", "virtualenv"
    requires = 'boost/1.72.0', 'libInterpolate/2.3.3@cd3/devel'


    def build(self):
      cmake = CMake(self)
      cmake.configure()
      cmake.build()
