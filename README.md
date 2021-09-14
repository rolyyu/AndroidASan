# [Android Bug Swatting with Sanitizers](https://android-developers.googleblog.com/2017/08/android-bug-swatting-with-sanitizers.html)
Sample of enabling **Address Sanitizer** for android native project.

Enabling the sanitizer is a matter of:
- Linking to the shared version of the c++ runtime
- Providing the needed compiler and linker flags
- Copying the asan shared libraries from the ndk toolchain dir to a place where they can be found and packaged
- Package the wrap.sh scripts in the APK's lib folder (They can be found in ndk's root dir)

Additionally, here we set up an additional build type in gradle, so all relevant operations are
only performed for the new "sanitize" build type, and won't affect either the debug or the release build.

More information: https://rolyyu.github.io/2021/09/13/Android-NDK-%E4%BD%BF%E7%94%A8-Address-Sanitizer/

