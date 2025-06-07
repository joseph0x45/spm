#ifndef DB_UTILS_H
#define DB_UTILS_H
#include "sqlite3.h"

#define ENSURE_DB_READY                                                        \
  do {                                                                         \
    if (ensure_db_ready() == -1)                                               \
      return -1;                                                               \
  } while (0)

sqlite3 *get_db_connection(void);

int ensure_db_ready(void);

#endif
