
#include "jWorkClass_jWorkClass.h"

#include "wc_thin_api.h"


JNIEXPORT jlong JNICALL Java_jWorkClass_jWorkClass_Create
  (JNIEnv *, jobject)
{
	if( sizeof(jlong) != sizeof(intptr_t) ) return -1;

	intptr_t x = WC_Create();
	return x;
}

JNIEXPORT jint JNICALL Java_jWorkClass_jWorkClass_Delete
  (JNIEnv *, jobject, jlong x)
{
	return WC_Delete(x);
}


JNIEXPORT jint JNICALL Java_jWorkClass_jWorkClass_Calculate
  (JNIEnv *a , jobject b, jlong c, jint x)
{
	return WC_Calculate(c,x);
}

JNIEXPORT jdouble JNICALL Java_jWorkClass_jWorkClass_GetElement
  (JNIEnv *a , jobject b, jlong c, jint x)
{
	return WC_GetElement(c,x);
}


JNIEXPORT jint JNICALL Java_jWorkClass_jWorkClass_SendData
  (JNIEnv *a , jobject b, jlong c, jint nx, jint ny, jintArray x, jdoubleArray y)
{
	jint * sx = a->GetIntArrayElements(x,0);
	jdouble * sy = a->GetDoubleArrayElements(y,0);

	int * sxi = new int[nx];
	for( int i=0; i<nx; i++ ) sxi[i] = sx[i];

	jint r = WC_SendData(c,nx,ny,sxi,sy);

	delete [] sxi;

	a->ReleaseDoubleArrayElements(y, sy, 0);
	a->ReleaseIntArrayElements(x, sx, 0);

	return r;
}



