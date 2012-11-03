#include <dlfcn.h>

#include "gralloc.h"
#include "common.h"

void *gralloc_lib = NULL;

close_fn_type close_gralloc = NULL;
readfb_fn_type readfb_gralloc = NULL;
getscreenformat_fn_type getscreenformat_gralloc = NULL;

int initGralloc(void)
{
  L("--Loading gralloc native lib--\n");
  int i,len;
  char lib_name[64];

  len=ARR_LEN(compiled_sdks);
  L("library array length: len=%d\n",len);
  for (i=0;i<len;i++) {
    sprintf(lib_name, DVNC_LIB_PATH "libdvnc_gralloc_sdk%d.so",compiled_sdks[i]);
    if (gralloc_lib != NULL) //remove previous instance
      dlclose(gralloc_lib);
    L("Loading lib: %s\n",lib_name);
    gralloc_lib = dlopen(lib_name, RTLD_NOW);
    if (gralloc_lib == NULL) {
      L("Couldn't load library %s! Error string: %s\n",lib_name,dlerror());
      continue;
    }

    init_fn_type init_gralloc = dlsym(gralloc_lib,"init_gralloc");
    if(init_gralloc == NULL) {
      L("Couldn't load init_gralloc! Error string: %s\n",dlerror());
      continue;
    }

    close_gralloc = dlsym(gralloc_lib,"close_gralloc");
    if(close_gralloc == NULL) {
      L("Couldn't load close_gralloc! Error string: %s\n",dlerror());
      continue;
    }

    readfb_gralloc = dlsym(gralloc_lib,"readfb_gralloc");
    if(readfb_gralloc == NULL) {
      L("Couldn't load readfb_gralloc! Error string: %s\n",dlerror());
      continue;
    }

    getscreenformat_gralloc = dlsym(gralloc_lib,"getscreenformat_gralloc");
    if(getscreenformat_gralloc == NULL) {
      L("Couldn't load get_screenformat! Error string: %s\n",dlerror());
      continue;
    }

    int ret = init_gralloc();
    if (ret == -1) {
      L("Gralloc method not supported by this device!\n");
      continue;
    }

    screenformat = getScreenFormatGralloc();
    if ( screenformat.width <= 0 ) {
      L("Error: I have received a bad screen size from gralloc.\n");
      continue;
    }
L("AKI1\n");
    return 0;
  }
L("AKI2\n");
  return -1;
}

screenFormat getScreenFormatGralloc(void)
{
  screenFormat f;
  if (getscreenformat_gralloc)
     f = getscreenformat_gralloc();
  return f;
}

void closeGralloc(void)
{
  if (close_gralloc)
    close_gralloc();
  if (gralloc_lib)
    dlclose(gralloc_lib);
}

unsigned char *readBufferGralloc(void)
{
  if (readfb_gralloc)
    return readfb_gralloc();
  return NULL;
}

