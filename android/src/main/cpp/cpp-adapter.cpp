#include <jni.h>
#include "NitroTorOnLoad.hpp"

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*) {
  return margelo::nitro::nitrotor::initialize(vm);
}
