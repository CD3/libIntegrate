from conans import ConanFile, CMake, tools
import os

class ConanBuild(ConanFile):
    generators = "cmake", "virtualenv"
    requires = 'boost/1.69.0@conan/stable'


    def build(self):
      cmake = CMake(self)
      cmake.configure()
      cmake.build()
