#ifndef DISPLAY_BINDER
#define DISPLAY_BINDER

#ifdef __cplusplus
#define L(...) __android_log_print(ANDROID_LOG_INFO,"VNCserver",__VA_ARGS__);printf(__VA_ARGS__);


extern "C" {
#endif
  unsigned int * readfb_flinger();
  int init_flinger();
  void close_flinger();
#ifdef __cplusplus
}

#endif

#endif
