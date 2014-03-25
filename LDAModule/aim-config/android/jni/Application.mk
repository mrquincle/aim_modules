#APP_STL := stlport_static
APP_STL := gnustl_static
APP_CPPFLAGS := -frtti -fexceptions

# Build both ARMv5TE and ARMv7-A machine code.
#APP_ABI := armeabi armeabi-v7a
#APP_OPTIM:=debug