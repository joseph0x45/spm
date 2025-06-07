#ifndef SPM_H
#define SPM_H

typedef struct {
  const char *name;
  const char *source;
  const char *type;
  const int installed;
  const char *version;
} Package;

int spm_add_package(const Package *package);

int spm_list(void);

int spm_check_option_argument(const char *optarg, const char *opt_name);

#endif
