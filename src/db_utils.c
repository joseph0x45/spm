#include "db_utils.h"
#include "fs_utils.h"
#include <sqlite3.h>
#include <stdio.h>
#include <unistd.h>

sqlite3 *get_db_connection(void) {
  const char *db_path = get_spm_db_path();
  sqlite3 *db;
  int rc = sqlite3_open(db_path, &db);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Database Error: %s\n", sqlite3_errmsg(db));
    return NULL;
  }
  return db;
}

int ensure_db_ready(void) {
  sqlite3 *db = get_db_connection();
  if (!db) {
    return -1;
  }
  char *err_msg = NULL;
  const char *sql = "create table if not exists packages ("
                    "name text primary key not null, "
                    "package_type text not null, "
                    "source_url text not null, "
                    "installed int not null default 0, "
                    "version text not null default '' "
                    ") without rowid";
  int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Database Error: %s\n", err_msg);
    sqlite3_free(err_msg);
    sqlite3_close(db);
    return -1;
  }
  sqlite3_close(db);
  return 0;
}
