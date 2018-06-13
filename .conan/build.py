#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import re
from cpt.packager import ConanMultiPackager


class BuilderSettings(object):
    @property
    def username(self):
        """ Set taocpp as package's owner
        """
        return os.getenv("CONAN_USERNAME", "taocpp")

    @property
    def upload(self):
        """ Set taocpp repository to be used on upload
        """
        bintray_url = "https://api.bintray.com/conan/taocpp/public-conan"
        return os.getenv("CONAN_UPLOAD", bintray_url)

    @property
    def upload_only_when_stable(self):
        """ Force to upload when running over tag branch
        """
        return os.getenv("CONAN_UPLOAD_ONLY_WHEN_STABLE", True)

    @property
    def stable_branch_pattern(self):
        """ Only upload the package the branch name is like a tag
        """
        return os.getenv("CONAN_STABLE_BRANCH_PATTERN", r"\d+\.\d+\.\d+")

    @property
    def reference(self):
        """ Read project version from CMake file to create Conan referece
        """
        pattern = re.compile(r"project \(pegtl VERSION (\d+\.\d+\.\d+) LANGUAGES CXX\)")
        version = None
        with open('CMakeLists.txt') as file:
            for line in file:
                result = pattern.match(line)
                if result:
                    version = result.group(1)
        if not version:
            raise Exception("Could not find version in CMakeLists.txt")
        return os.getenv("CONAN_REFERENCE", "pegtl/{}@taocpp/stable".format(version))

if __name__ == "__main__":
    settings = BuilderSettings()
    builder = ConanMultiPackager(
        reference=settings.reference,
        username=settings.username,
        upload=settings.upload,
        upload_only_when_stable=settings.upload_only_when_stable,
        stable_branch_pattern=settings.stable_branch_pattern)
    builder.add()
    builder.run()
