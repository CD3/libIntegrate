from conan import ConanFile

class Pkg(ConanFile):
    generators = "CMakeDeps", "CMakeToolchain",
    settings = "os", "arch", "compiler", "build_type"

    def requirements(self):
        self.requires("boost/1.85.0")
        self.requires("libinterpolate/2.7")

    def build_requirements(self):
        pass


