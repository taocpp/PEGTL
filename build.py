#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
from cpt.packager import ConanMultiPackager


class BuilderSettings(object):
    @property
    def username(self):
        return os.getenv("CONAN_USERNAME", "taocpp")

    @property
    def upload(self):
        bintray_url = "https://api.bintray.com/conan/taocpp/public-conan"
        return os.getenv("CONAN_UPLOAD", bintray_url)

    @property
    def upload_only_when_stable(self):
        return os.getenv("CONAN_UPLOAD_ONLY_WHEN_STABLE", True)

if __name__ == "__main__":
    settings = BuilderSettings()
    builder = ConanMultiPackager(
        username=settings.username,
        upload=settings.upload,
        upload_only_when_stable=settings.upload_only_when_stable)
    builder.add()
    builder.run()
