#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import re
from cpt.packager import ConanMultiPackager
from cpt.ci_manager import CIManager
from cpt.printer import Printer


class BuilderSettings(object):
    @property
    def username(self):
        """ Set taocpp as package's owner
        """
        return os.getenv("CONAN_USERNAME", "taocpp")

    @property
    def upload(self):
        """ Set taocpp repository to be used on upload.
            The upload server address could be customized by env var
            CONAN_UPLOAD. If not defined, the method will check the branch name.
            Only master or CONAN_STABLE_BRANCH_PATTERN will be accepted.
            The master branch will be pushed to testing channel, because it does
            not match the stable pattern. Otherwise it will upload to stable
            channel.
        """
        if os.getenv("CONAN_UPLOAD", None) is not None:
            return os.getenv("CONAN_UPLOAD")

        printer = Printer(None)
        ci_manager = CIManager(printer)
        branch = ci_manager.get_branch()

        patterns = [r"v?\d+\.\d+\.\d+-.*", self.stable_branch_pattern]
        for pattern in patterns:
            prog = re.compile(pattern)
            if branch and prog.match(branch):
                return "https://api.bintray.com/conan/taocpp/public-conan"

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
        stable_branch_pattern=settings.stable_branch_pattern,
        test_folder=os.path.join(".conan", "test_package"))
    builder.add()
    builder.run()
