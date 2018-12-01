#include <stdio.h>

#include "settings.h"

int main( int argc, char *argv[] )
{
	settings_init( "test.db", ( argc<2 ) );
	settings_setInt( "counter", settings_getInt( "counter", 0 ) + 1 );
	printf( "%s\n", settings_getString( "counter", "empty" ) );
	return 0;
}
