APP_PLATFORM := android-10

APP_STL := gnustl_static
APP_CPPFLAGS += -fexceptions
APP_CPPFLAGS += -frtti
APP_ABI := x86 # building for the emulator

NDK_TOOLCHAIN_VERSION := 4.9
