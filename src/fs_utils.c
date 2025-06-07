#include "fs_utils.h"
#include "config.h"
#include <errno.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int ensure_dir_exists(const char *path) {
  struct stat st = {0};
  if (stat(path, &st) == -1) {
    if (mkdir(path, 0700) == -1) {
      fprintf(stderr, "Failed to create %s: %s\n", path, strerror(errno));
      return -1;
    }
  }
  return 0;
}

int ensure_home_exists(void) {
  char *spm_home = get_spm_home();
  if (!spm_home) {
    return -1;
  }
  int result = ensure_dir_exists(spm_home);
  return result;
}

char *get_spm_home(void) {
  static int initialized = 0;
  static char path[PATH_MAX];
  if (!initialized) {
    const char *HOME = getenv("HOME");
    if (!HOME) {
      fprintf(stderr, "Error: HOME environment variable is not set. Please set "
                      "it and try again\n");
      return NULL;
    }
    snprintf(path, sizeof(path), "%s/%s", HOME, SPM_HOME);
    initialized = 1;
  }
  return path;
}

char *get_spm_db_path(void) {
  static int initialized = 0;
  static char path[PATH_MAX];
  if (!initialized) {
    const char *spm_home = get_spm_home();
    if (!spm_home) {
      return NULL;
    }
    snprintf(path, sizeof(path), "%s/%s", spm_home, SPM_DB);
    initialized = 1;
  }
  return path;
}
