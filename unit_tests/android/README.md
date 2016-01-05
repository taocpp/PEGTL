# Running unit test under Android

## SDK and NDK

In order to run the unit tests under Android, you'll need to install the Android SDK and NDK.
Download it from the [http://developer.android.com/sdk/index.html#Other](official site). If
you're an OS X user, you can also install SDK and NDK using home brew.

You will need to set the environment variables `ANDROID_HOME` and `NDK_HOME` to point to the
installation directories.

## Setting up

Once the SDK and NDK are installed, you can set up an Android project:

    android update project -p .

This will create `build.xml` and `local.properties` for you.

## Running a unit tests

To run a unit test, you will have to prepare it for Android. The unit tests in `ascii_classes.cc`
is prepared by the command:

    ./mk_unittest ascii_classes

To build the unit test as a native activity, you'll do

    cd jni
    ndk-build
    cd ..
    ant debug

The native activity is packaged as an Android app, and it can be installed on a device using the command:

    ant installd

and finally, you can run it with the command

    ./run_unittest
