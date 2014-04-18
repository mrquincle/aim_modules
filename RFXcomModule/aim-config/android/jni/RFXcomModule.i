/* swig -c++ -java -package org.dobots.picturetransformmodule -outdir src/org/dobots/picturetransformmodule -o jni/Module_wrap.cpp jni/Module.i
-outdir		location of the .java files
-o			location of the _wrap.cpp files

module name has to be equal to the LOCAL_MODULE in the Android.mk file
ndk-build clean && ndk-build -j
*/

%module AIM

/* Enable using vectors and strings in the functions */
%include "std_vector.i"
%include "std_string.i"
/* Options are: deque, list, map, pair, set, string, vector, shared_ptr */ 

/* Need to instantiate different types of vector */
namespace std {
	%template(vector_int) vector<int>;
	%template(vector_long) vector<long>;
	%template(vector_float) vector<float>;
	%template(vector_double) vector<double>;
};

/* Declarations */
%{
#include "../../../aim-core/inc/RFXcomModule.h"
#include "../../../inc/RFXcomModuleExt.h"
%}

/* What to export */
%include "../../../aim-core/inc/RFXcomModule.h"
%include "../../../inc/RFXcomModuleExt.h"
