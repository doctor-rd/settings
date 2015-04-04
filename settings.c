#include <stdio.h>
#include <string.h>
#include <sqlite3.h>

static sqlite3 *db = NULL;

static sqlite3_stmt* stmt_get( char *name )
{
	sqlite3_stmt* stmt;

	if( db == NULL )
		return NULL;
	if( sqlite3_prepare_v2( db, "SELECT value FROM settings WHERE name=?", -1, &stmt, NULL ) != SQLITE_OK )
	{
		fprintf( stderr, "Failed to prepare data: %s\n", sqlite3_errmsg( db ) );
		return NULL;
	}
	if( sqlite3_bind_text( stmt, 1, name, -1, SQLITE_STATIC ) != SQLITE_OK )
	{
		sqlite3_finalize( stmt );
		return NULL;
	}
	if( sqlite3_step( stmt ) != SQLITE_ROW )
	{
		sqlite3_finalize( stmt );
		return NULL;
	}
	return stmt;
}

static sqlite3_stmt* stmt_set( char *name )
{
	sqlite3_stmt* stmt;

	if( db == NULL )
		return NULL;
	if( sqlite3_prepare_v2( db, "INSERT OR REPLACE INTO settings VALUES( ?, ? )", -1, &stmt, NULL ) != SQLITE_OK )
	{
		fprintf( stderr, "Failed to prepare data: %s\n", sqlite3_errmsg( db ) );
		return NULL;
	}
	if( sqlite3_bind_text( stmt, 1, name, -1, SQLITE_STATIC ) != SQLITE_OK )
	{
		sqlite3_finalize( stmt );
		return NULL;
	}
	return stmt;
}

int settings_init( char *path )
{
	sqlite3_stmt* stmt;

	if( db != NULL )
		return -1;
	if( sqlite3_open( path, &db ) != SQLITE_OK )
	{
		fprintf( stderr, "Cannot open %s: %s\n", path, sqlite3_errmsg(db) );
		db = NULL;
		return -1;
	}
	if( sqlite3_prepare_v2( db, "CREATE TABLE IF NOT EXISTS settings ( name TEXT UNIQUE, value )", -1, &stmt, NULL ) != SQLITE_OK )
	{
		fprintf( stderr, "Failed to prepare data: %s\n", sqlite3_errmsg( db ) );
		return -1;
	}
	sqlite3_step( stmt );
	sqlite3_finalize( stmt );
	return 0;
}

int settings_setInt( char *name, int value )
{
	sqlite3_stmt *stmt = stmt_set( name );
	if( stmt == NULL )
		return -1;
	if( sqlite3_bind_int( stmt, 2, value ) != SQLITE_OK )
	{
		sqlite3_finalize( stmt );
		return -1;
	}
	sqlite3_step( stmt );
	sqlite3_finalize( stmt );
	return 0;
}

int settings_setString( char *name, char *value )
{
	sqlite3_stmt *stmt = stmt_set( name );
	if( stmt == NULL )
		return -1;
	if( sqlite3_bind_text( stmt, 2, value, -1, SQLITE_STATIC ) != SQLITE_OK )
	{
		sqlite3_finalize( stmt );
		return -1;
	}
	sqlite3_step( stmt );
	sqlite3_finalize( stmt );
	return 0;
}

int settings_getInt( char *name, int defaultValue )
{
	int res;
	sqlite3_stmt *stmt = stmt_get( name );
	if( stmt == NULL )
		return defaultValue;
	res = sqlite3_column_int( stmt, 0 );
	sqlite3_finalize( stmt );
	return res;
}

char* settings_getString( char *name, char *defaultValue )
{
	static char res[256];
	sqlite3_stmt *stmt = stmt_get( name );
	if( stmt == NULL )
		return defaultValue;
	const char* tmp = sqlite3_column_text( stmt, 0 );
	if( strlen( tmp ) >= sizeof( res ) )
	{
		sqlite3_finalize( stmt );
		return defaultValue;
	}
	strcpy( res, tmp );
	sqlite3_finalize( stmt );
	return res;
}
