#include <jni.h>
#include "nitrotorOnLoad.hpp"

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*) {
  return margelo::nitro::nitrotor::initialize(vm);
}
