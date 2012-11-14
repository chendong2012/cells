/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

/*
 *1、上传字符串
 *2、下载字符串
 *3、上传整型
 *4、下载整型
 *5、下载布尔
 ***************/
#include <string.h>
#include <jni.h>
#include <fcntl.h>
#include <android/log.h>
#include <errno.h>
#include <dirent.h>
#include <assert.h>
#include <unistd.h>

#define I2CNAME "/sys/bus/i2c/devices/0-0019/name"
#define CPUINFO "/proc/cpuinfo"
static unsigned char ops_path[255];
static char buf[1000];

/*========================*/
#define LOG_TAG "chendong"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
/*========================*/


static int write_data_to_file(const char *path, const char *data)
{
	int nbytes;
	int fd;

	if (path == NULL)
		return -1;

	fd = open(path, O_WRONLY);
	if (fd < 0) {
		nbytes = -errno;
		goto fail;
	}
	nbytes = write(fd, data, strlen((const char *)data));
	if (-1 == nbytes) {
		nbytes = -errno;
		LOGE(strerror(errno));
		close(fd);
		goto fail;
	}
	close(fd);
	LOGI("%s", data);
fail:
	return nbytes;
}

/*
 *读指定路径下的文件的内容
 *参数:
 *	path:路径
 *	data:返回的字符串
 *	data_len:字符串空间的长度
 *
 *返回值：
 *	读到的个数或是错误号
 ********************************/
static int read_data_from_file(char const *path, char *data, int data_len)
{
	int nbytes;
	int fd;

	if (path == NULL)
		return -1;

	fd = open(path, O_RDONLY);
	if (fd < 0) {
		nbytes = -errno;
		goto fail;
	}
	nbytes = read(fd, data, data_len - 1);
	if (-1 == nbytes) {
		nbytes = -errno;
		close(fd);
		goto fail;
	}
	close(fd);
	data[nbytes] = '\0';
	LOGI("%s", data);
fail:
	return nbytes;
}

//=========================JNI API===============================
/*
 *upload string upto java from jni
 *public native String stringFromJNI();
 * */
jstring
Java_com_example_hellojni_HelloJni_stringFromJNI( JNIEnv* env, jobject thiz)
{
	//read_path_string(CPUINFO, buf, sizeof(buf));
	strcpy(buf, "hello world");
	return (*env)->NewStringUTF(env, buf);
}

/*
 *translate string into jni
 *public static native boolean stringToJni(String str)
 * */
jboolean
Java_com_example_hellojni_HelloJni_stringToJni(JNIEnv* env, jclass clazz, jstring str)
{
    // convert Java string to UTF-8
    const char *utf8 = (*env)->GetStringUTFChars(env, str, NULL);
    assert(NULL != utf8);
    LOGI("%s", utf8);

    return JNI_TRUE;
}

/*
 *translate bool var into jni
 *public static native void booleanToJni(boolean boolvalue);
 * */
void
Java_com_example_hellojni_HelloJni_booleanToJni(JNIEnv* env,
        jclass clazz, jboolean boolvalue)
{
	LOGI("booleanToJni is %d", boolvalue);
}

/*
 *java translate int value into jni
 *public static native void intToJni(boolean boolvalue);
 * */
void
Java_com_example_hellojni_HelloJni_intToJni( JNIEnv* env, jobject thiz, jint intvalue)
{
	LOGI("intToJni is %d", intvalue);
}
//===========================END=================================

/*
 *write datas to file and return status
 *java: static native int write_file(String filename, String command)
 *return:how many write bytes
 * */
jint Java_com_example_hellojni_HelloJni_write_file(JNIEnv* env,
				jclass clazz, jstring filename, jstring command)
{
	int ret;
	const char *jni_filename = (*env)->GetStringUTFChars(env, filename, NULL);
	assert(NULL != jni_filename);

	const char *jni_command= (*env)->GetStringUTFChars(env, command, NULL);
	assert(NULL != jni_command);

	ret = write_data_to_file(jni_filename, jni_command);
	return ret;
}

/*
 *read datas from file and return datas upto java
 *java: static native String read_file(String filename, int len)
 * */
jstring Java_com_example_hellojni_HelloJni_read_file(JNIEnv* env,
				jclass clazz, jstring filename, jint len)
{
	const char *jni_filename = (*env)->GetStringUTFChars(env, filename, NULL);
	assert(NULL != jni_filename);

	read_data_from_file(jni_filename, buf, sizeof(buf));
	return (*env)->NewStringUTF(env, buf);
}
