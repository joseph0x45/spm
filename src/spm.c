#include "spm.h"
#include "db_utils.h"
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

#define NUMBER_OF_VALID_TYPES 3

static const char *valid_types[NUMBER_OF_VALID_TYPES] = {"deb", "appimage",
                                                         "bin"};

int spm_check_option_argument(const char *optarg, const char *opt_name) {
  if (optarg == NULL || optarg[0] == '-' || strcmp(optarg, "") == 0) {
    fprintf(stderr, "Error: --%s requires an argument\n", opt_name);
    return 0;
  }
  return 1;
}

// static void print_package(const Package *package) {
//   printf("Name: %s\n", package->name);
//   printf("Type: %s\n", package->type);
//   printf("Source: %s\n", package->source);
// }

static int is_valid_type(const char *type) {
  for (int i = 0; i < NUMBER_OF_VALID_TYPES; i++) {
    if (strcmp(valid_types[i], type) == 0) {
      return 1;
    }
  }
  return 0;
}

static int package_is_valid(const Package *package) {
  if (!package->name) {
    fprintf(stderr, "Package name is required!\n");
    return 0;
  }
  if (!package->source) {
    fprintf(stderr, "Package source is required!\n");
    return 0;
  }
  if (!package->type) {
    fprintf(stderr, "Package type is required!\n");
    return 0;
  } else if (!is_valid_type(package->type)) {
    fprintf(stderr,
            "Invalid package type '%s'!\nPackage type can only be 'deb' "
            "'appimage' or 'bin'\n",
            package->type);
    return 0;
  }
  return 1;
}

int spm_add_package(const Package *package) {
  if (!package_is_valid(package)) {
    return -1;
  }
  ENSURE_DB_READY;
  sqlite3 *db = get_db_connection();
  if (!db) {
    return -1;
  }
  const char *sql =
      "insert into packages(name, package_type, source_url) values(?, ?, ?);";
  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Error: Failed to prepare statement: %s\n",
            sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }
  rc = sqlite3_bind_text(stmt, 1, package->name, -1, SQLITE_STATIC);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Error: Failed to bind 'name': %s\n", sqlite3_errmsg(db));
    goto cleanup;
  }
  rc = sqlite3_bind_text(stmt, 2, package->type, -1, SQLITE_STATIC);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Error: Failed to bind 'type': %s\n", sqlite3_errmsg(db));
    goto cleanup;
  }
  rc = sqlite3_bind_text(stmt, 3, package->source, -1, SQLITE_STATIC);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Error: Failed to bind 'source': %s\n", sqlite3_errmsg(db));
    goto cleanup;
  }
  rc = sqlite3_step(stmt);
  if (rc == SQLITE_CONSTRAINT) {
    fprintf(stderr, "Error: Package '%s' already exists\n", package->name);
    goto cleanup;
  } else if (rc != SQLITE_DONE) {
    fprintf(stderr, "Error: Failed to execute statement: %s\n",
            sqlite3_errmsg(db));
    goto cleanup;
  }
  printf("Package '%s' successfully added!\n", package->name);
  sqlite3_finalize(stmt);
  sqlite3_close(db);
  return 0;

cleanup:
  sqlite3_finalize(stmt);
  sqlite3_close(db);
  return 1;
}

int spm_list(void) { return 0; }
