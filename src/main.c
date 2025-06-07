#include "spm.h"
#include <getopt.h>
#include <stdio.h>

#define CHECK_OPTARG(name)                                                     \
  do {                                                                         \
    if (!spm_check_option_argument(optarg, name)) {                            \
      return 1;                                                                \
    }                                                                          \
  } while (0)

void print_usage(void) { printf("spm (Simple Package Manager)\n"); }

int main(int argc, char **argv) {
  opterr = 0;
  int opt;
  int add_flag = 0;
  char *package_name = NULL;
  char *package_source = NULL;
  char *package_type = NULL;

  static struct option long_options[] = {
      {"help", no_argument, 0, 'h'},
      {"add", no_argument, 0, 'a'},
      {"name", required_argument, 0, 'n'},
      {"source", required_argument, 0, 's'},
      {"type", required_argument, 0, 't'},
      {0, 0, 0, 0},
  };

  while ((opt = getopt_long(argc, argv, "hi:r:u:lan:s:t:", long_options,
                            NULL)) != -1) {
    switch (opt) {
    case 'h':
      print_usage();
      return 0;
    case 'a':
      add_flag = 1;
      break;
    case 'n':
      CHECK_OPTARG("name");
      package_name = optarg;
      break;
    case 's':
      CHECK_OPTARG("source");
      package_source = optarg;
      break;
    case 't':
      CHECK_OPTARG("type");
      package_type = optarg;
      break;
    case '?':
      if (optopt == 'n' || optopt == 's' || optopt == 't') {
        fprintf(stderr, "Error: Option -%c requires an argument.\n", optopt);
      } else {
        fprintf(stderr, "Error: Unrecognized option '%c'\n", optopt);
      }
      print_usage();
      return 1;
    }
  }
  if (add_flag) {
    Package package = {.name = package_name,
                       .source = package_source,
                       .type = package_type,
                       .installed = 0,
                       .version = ""};
    return spm_add_package(&package);
  }
  return 0;
}
