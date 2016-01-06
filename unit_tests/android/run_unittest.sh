#!/bin/bash

adb logcat -c
adb shell am start -a android.intent.action.MAIN -n colinh.pegtl.unittest/android.app.NativeActivity
while [ true ]; do
    sleep 10
    isdone=$(adb logcat -d | grep "PEGTL   : Done running unit test." | tr -d "\r")
    if [ "$isdone" != "" ]; then
        report=$(adb logcat -d | grep "PEGTL   : Unittest" | awk '{print $9}')
        echo "$report"
        adb shell am force-stop colinh.pegtl.unittest
        break
    fi
done
