#include <stdio.h>

#include "settings.h"

int main()
{
	settings_init( "test.db" );
	settings_setInt( "counter", settings_getInt( "counter", 0 ) + 1 );
	printf( "%s\n", settings_getString( "counter", 0 ) );
	return 0;
}
