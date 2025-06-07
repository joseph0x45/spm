#ifndef FS_UTILS_H
#define FS_UTILS_H

int ensure_dir_exists(const char *path);

int ensure_home_exists(void);

char *get_spm_home(void);

char *get_spm_db_path(void);

#endif
