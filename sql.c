
#include <stdio.h>
#include "sqlite3.h"

static int callback(void *cookie, int argc, char **argv, char **azColName)
{
  int i;

  for(i=0; i<argc; i++)
  {
    printf("%s ", argv[i] ? argv[i] : "NULL");
  }
  printf("\n");

  return 0;
}

int main(int argc, char **argv){
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;

  if(argc != 3)
  {
    fprintf(stderr, "Usage: %s DATABASE SQL-STATEMENT\n", argv[0]);
    return 1;
  }

  /* You can use ":memory:" to use an in-memory, non-persistent database. */
  rc = sqlite3_open(argv[1], &db);
  if (rc != SQLITE_OK)
  {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  /* Always enforce constraints. */
  rc = sqlite3_exec(db, "PRAGMA foreign_keys = ON;", callback, (void*) 1, &zErrMsg);
  if (rc != SQLITE_OK)
  {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    return 1;
  }

  rc = sqlite3_exec(db, argv[2], callback, (void*) 2, &zErrMsg);
  if(rc != SQLITE_OK)
  {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }

  sqlite3_close(db);
  return 0;
}

