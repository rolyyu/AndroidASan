#include <jni.h>
#include <string>
#include <cstdlib>
#include <thread>
#include <android/log.h>

#define LOGE(...)                   \
  ((void)__android_log_print(       \
      ANDROID_LOG_ERROR, "ASan",    \
      __VA_ARGS__))


#define IMPL_FUNC(RETURN_TYPE, NAME, ...) \
  extern "C" { \
  JNIEXPORT RETURN_TYPE \
    Java_com_rolyyu_asan_MainActivity_ ## NAME \
      (JNIEnv* env, jobject thiz, ##__VA_ARGS__);\
  } \
  JNIEXPORT RETURN_TYPE \
    Java_com_rolyyu_asan_MainActivity_ ## NAME \
      (JNIEnv* env, jobject thiz, ##__VA_ARGS__)\


IMPL_FUNC(void,doHeapUseAfterFree) {

    // https://docs.microsoft.com/en-us/cpp/sanitizers/error-heap-use-after-free?view=msvc-160
    // heap-use-after-free error
    char * volatile p = new char[10];
    delete[] p;

    p[5] = 42; // Boom!
}

IMPL_FUNC(void,doHeapBufferOverflow) {

    // https://docs.microsoft.com/en-us/cpp/sanitizers/error-heap-buffer-overflow?view=msvc-160
    // heap-buffer-overflow error
    char *x = (char*)malloc(10 * sizeof(char));
    memset(x, 0, 10);

    int res = x[10];  // Boom!

    LOGE("res: %d",res);
    free(x);
}

IMPL_FUNC(void,doStackBufferOverflow) {

    // https://docs.microsoft.com/en-us/cpp/sanitizers/error-stack-buffer-overflow?view=msvc-160
    // stack-buffer-overflow error
    char x[10];
    memset(x, 0, 10);
    int res = x[10];  // Boom! Classic stack buffer overflow

    LOGE("res: %d",res);
}

IMPL_FUNC(void,doStackBufferUnderflow) {

    // https://docs.microsoft.com/en-us/cpp/sanitizers/error-stack-buffer-underflow?view=msvc-160
    // stack-buffer-underflow error
    int subscript = -1;
    char buffer[42];
    buffer[subscript] = 42; // Boom!
}

char* x;
void foo() {
    char stack_buffer[42];
    x = &stack_buffer[13];
}

IMPL_FUNC(void,doStackUseAfterReturn) {
    // https://docs.microsoft.com/en-us/cpp/sanitizers/error-stack-use-after-return?view=msvc-160
    // stack-use-after-return error
    foo();
    *x = 42; // Boom!

    // This check requires code generation that's activated by an extra compiler option,
    // fsanitize-address-use-after-return, and by setting the environment variable
    // ASAN_OPTIONS=detect_stack_use_after_return=1.
    LOGE("This should set the environment variable ASAN_OPTIONS=detect_stack_use_after_return=1.");

}

IMPL_FUNC(void,doDoubleFree) {
    // https://docs.microsoft.com/en-us/cpp/sanitizers/error-double-free?view=msvc-160
    // double-free error
    char * volatile p = new char[16];
    delete[] p;

    delete[] p;  // Boom!
}


IMPL_FUNC(void,doNullpointerDereference) {
    char * volatile p = (char *)nullptr;
    p[42] = 1;  // Boom!
}

IMPL_FUNC(void,doMemcpyParamOverlap) {
    // https://docs.microsoft.com/en-us/cpp/sanitizers/error-memcpy-param-overlap?view=msvc-160
    // memcpy-param-overlap
    // The function memcpy doesn't support overlapping memory.
    // It provides an alternative to memcpy that does support overlapping memory: memmove.

    char buffer[] = "hello";

    memcpy(buffer, buffer + 1, 5); // BOOM!
}

IMPL_FUNC(void,doStrncatParamOverlap) {
    // https://docs.microsoft.com/en-us/cpp/sanitizers/error-strncat-param-overlap?view=msvc-160
    // strncat-param-overlap
    // Code that moves memory in overlapping buffer can cause hard-to-diagnose errors.

    char buffer[] = "hello\0XXX";
    strncat(buffer, buffer + 1, 3); // BOOM
}