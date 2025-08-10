from conan import ConanFile

class Pkg(ConanFile):
    generators = "CMakeDeps", "CMakeToolchain",
    default_options = {'boost/*:header_only ': ' True'}
    settings = "os", "arch", "compiler", "build_type"

    def requirements(self):
        self.requires("boost/1.86.0", force=True)
        self.requires("libinterpolate/2.6.2")
        self.requires("catch2/3.3.1")

    def build_requirements(self):
        pass


