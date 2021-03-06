#include "Types.h"
#include "GameRes.h"

extern "C" {
  typedef struct engine_options_S engine_options_t;
  extern engine_options_t* create_engine_options(char **argv, int argc);
  extern void free_engine_options(engine_options_t *);
  extern char * get_stracciatella_home(const engine_options_t *);
  extern char * get_vanilla_data_dir(const engine_options_t *);
  extern UINT32 get_number_of_mods(const engine_options_t *);
  extern char * get_mod(const engine_options_t *, uint32_t index);
  extern UINT16 get_resolution_x(const engine_options_t *);
  extern UINT16 get_resolution_y(const engine_options_t *);
  extern GameVersion get_resource_version(const engine_options_t *);
  extern void free_rust_string(char *);
  extern bool should_show_help(const engine_options_t *);
  extern bool should_run_unittests(const engine_options_t *);
  extern bool should_run_editor(const engine_options_t *);
  extern bool should_start_in_fullscreen(const engine_options_t *);
  extern bool should_start_in_window(const engine_options_t *);
  extern bool should_start_in_debug_mode(const engine_options_t *);
  extern bool should_start_without_sound(const engine_options_t *);
}
