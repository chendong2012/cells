#include <dlfcn.h>

#include "flinger.h"
#include "common.h"

void *flinger_lib = NULL;

close_fn_type close_flinger = NULL;
readfb_fn_type readfb_flinger = NULL;
getscreenformat_fn_type getscreenformat_flinger = NULL;

int initFlinger(void)
{
  L("--Loading flinger native lib--\n");
  int i,len;
  char lib_name[64];

  len=ARR_LEN(compiled_sdks);
  L("library array length: len=%d\n",len);
  for (i=0;i<len;i++) {
    sprintf(lib_name, DVNC_LIB_PATH "libdvnc_flinger_sdk%d.so",compiled_sdks[i]);
    if (flinger_lib != NULL) //remove previous instance
      dlclose(flinger_lib);
    L("Loading lib: %s\n",lib_name);
    flinger_lib = dlopen(lib_name, RTLD_NOW);
    if (flinger_lib == NULL){
      L("Couldn't load library %s! Error string: %s\n",lib_name,dlerror());
      continue; 
    }
  
    init_fn_type init_flinger = dlsym(flinger_lib,"init_flinger");
    if(init_flinger == NULL) {
      L("Couldn't load init_flinger! Error string: %s\n",dlerror());
      continue;
    }
  
    close_flinger = dlsym(flinger_lib,"close_flinger");
    if(close_flinger == NULL) {
      L("Couldn't load close_flinger! Error string: %s\n",dlerror());
      continue;
    }

    readfb_flinger = dlsym(flinger_lib,"readfb_flinger");
    if(readfb_flinger == NULL) {
      L("Couldn't load readfb_flinger! Error string: %s\n",dlerror());
      continue;
    }

    getscreenformat_flinger = dlsym(flinger_lib,"getscreenformat_flinger");
    if(getscreenformat_flinger == NULL) {
      L("Couldn't load get_screenformat! Error string: %s\n",dlerror());
      continue;
    }

    int ret = init_flinger();
    if (ret == -1) {
       L("flinger method not supported by this device!\n");
       continue;
    }

    screenformat = getScreenFormatFlinger();
    if ( screenformat.width <= 0 ) {
      L("Error: I have received a bad screen size from flinger.\n");
      continue;;
    }
  
    if ( readBufferFlinger() == NULL) {
      L("Error: Could not read surfaceflinger buffer!\n");
      continue;
    }
L("AKI1\n");
    return 0;
  }
L("AKI2\n");
  return -1;
}

screenFormat getScreenFormatFlinger(void)
{
  screenFormat f;
  if (getscreenformat_flinger)
     f = getscreenformat_flinger();
  return f;
}

void closeFlinger(void)
{
  if (close_flinger)
    close_flinger();
  if (flinger_lib)
    dlclose(flinger_lib);
}

unsigned char *readBufferFlinger(void)
{
  if (readfb_flinger)
    return readfb_flinger();
  return NULL;
}

