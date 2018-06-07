#!/usr/bin/env python
# -*- coding: utf-8 -*-
from conans import ConanFile

class PEGTLConan(ConanFile):
    name = "pegtl"
    version = "2.5.2"
    description = "C++11 header-only parser combinator library for creating PEG parsers"
    homepage = "https://github.com/taocpp/PEGTL"
    url = homepage
    license = "MIT"
    author = "taocpp@icemx.net"
    exports = "LICENSE"
    exports_sources = "include/*"
    no_copy_source = True

    def build(self):
        pass

    def package(self):
        self.copy(pattern="LICENSE", dst="licenses")
        self.copy(pattern="*.hpp", dst="include", src="include")

    def package_id(self):
        self.info.header_only()
