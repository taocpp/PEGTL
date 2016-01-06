#!/bin/bash

(
  cd jni
  ndk-build
) || exit 1

ant debug
