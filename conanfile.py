from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake, cmake_layout
from conan.tools.files import copy
import os

class MassiveCppConan(ConanFile):
    name = "massive-cpp"
    version = "0.1.0"
    description = "Modern C++20 SDK for the Massive REST and WebSocket APIs"
    license = "MIT"
    author = "massive-cpp contributors"
    url = "https://github.com/your-org/massive-cpp"
    homepage = "https://github.com/your-org/massive-cpp"
    topics = ("api", "rest", "websocket", "market-data", "finance")
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "build_tests": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "build_tests": False,
    }
    exports_sources = "CMakeLists.txt", "include/*", "src/*", "cmake/*"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def requirements(self):
        self.requires("boost/1.86.0")
        self.requires("simdjson/3.9.2")
        self.requires("openssl/3.2.0")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.variables["MASSIVE_BUILD_TESTS"] = self.options.build_tests
        tc.variables["MASSIVE_VENDOR_DEPS"] = False
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
        copy(self, "LICENSE", self.source_folder, os.path.join(self.package_folder, "licenses"))

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "massive-cpp")
        self.cpp_info.set_property("cmake_target_name", "massive::core")
        self.cpp_info.components["core"].libs = ["massive_core"]
        self.cpp_info.components["core"].requires = ["boost::boost", "simdjson::simdjson", "openssl::openssl"]
        
        self.cpp_info.components["rest"].libs = ["massive_rest"]
        self.cpp_info.components["rest"].requires = ["core"]
        
        self.cpp_info.components["websocket"].libs = ["massive_websocket"]
        self.cpp_info.components["websocket"].requires = ["core"]

