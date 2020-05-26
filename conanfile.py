from conans import ConanFile, CMake, tools
import os

class ConanBuild(ConanFile):
    generators = "cmake", "virtualenv"
    requires = 'boost/1.72.0'


    def build(self):
      cmake = CMake(self)
      cmake.configure()
      cmake.build()
