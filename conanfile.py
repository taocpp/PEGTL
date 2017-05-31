from conans import ConanFile, CMake, tools
import os


class PEGTLConan(ConanFile):
    name = "PEGTL"
    version = "2.0.0"
    license = "MIT"
    username = "taocpp"
    url = "https://github.com/%s/PEGTL.git" % username

    def source(self):
        tools.download("https://github.com/%s/PEGTL/archive/%s.zip" % (self.username, self.version),
                       "PEGTL.zip")
        tools.unzip("PEGTL.zip" )

    def package(self):
        self.copy("*", "include/tao", "PEGTL-%s/include/tao"%self.version)

