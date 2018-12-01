#ifndef SETTINGS_H
#define SETTINGS_H

#ifdef __cplusplus
extern "C" {
#endif

int settings_init( char *path, int wr );
int settings_setInt( char *name, int value );
int settings_setString( char *name, char *value );
int settings_getInt( char *name, int defaultValue );
char* settings_getString( char *name, char *defaultValue );

#ifdef __cplusplus
}
#endif

#endif
