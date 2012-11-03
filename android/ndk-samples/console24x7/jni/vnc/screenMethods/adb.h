#ifndef ADB_CONNECT_METHOD
#define ADB_CONNECT_METHOD

int initADB(void);
unsigned int *readBufferADB(void);
void closeADB(void);

#endif
