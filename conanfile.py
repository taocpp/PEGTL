from conans import ConanFile, CMake
import os

class PEGTLConan(ConanFile):
    name = "pegtl"
    description = "C++11 header-only parser combinator library for creating PEG parsers"
    homepage = "https://github.com/taocpp/PEGTL"
    url = homepage
    license = "MIT"
    author = "taocpp@icemx.net"
    exports_sources = "include*", "LICENSE", "CMakeLists.txt"

    def package(self):
        cmake = CMake(self)

        cmake.definitions["PEGTL_BUILD_TESTS"] = "OFF"
        cmake.definitions["PEGTL_BUILD_EXAMPLES"] = "OFF"
        cmake.definitions["PEGTL_INSTALL_DOC_DIR"] = "licenses"

        cmake.configure()
        cmake.install()

    def package_id(self):
        self.info.header_only()
