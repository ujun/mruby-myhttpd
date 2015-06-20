#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>
#include <stdio.h>
#include <string.h>
#include "mruby.h"
#include "mruby/compile.h"
#include "mruby/string.h"
#include "mruby/array.h"
#include "/usr/include/sqlite3.h"
#define PORT 8888

static int answer_to_connection (void *cls, struct MHD_Connection *connection, const char *url, const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **con_cls) {

  mrb_state* mrb = mrb_open();

  FILE* f = fopen("/Users/juchino/myruby/src/ujun/mruby_build/mruby-libmicrohttpd/myhttpd.rb", "r");
  mrb_load_file(mrb, f);
  fclose(f);

  mrb_value res;

  char *s = "";
  mrb_value str = mrb_str_new(mrb, s, 0);
  res = mrb_funcall(mrb, mrb_top_self(mrb), "gchist", 1, str);

  char *page = mrb_str_to_cstr(mrb, res);

  // FILE* log = fopen("myhttpd.log", "a");
  // fprintf(log, "%d\n", RARRAY_LEN(res));
  // fclose(log);

  mrb_close(mrb);

  struct MHD_Response *response;

  int ret;
  response = MHD_create_response_from_buffer (strlen (page), (void *) page, MHD_RESPMEM_PERSISTENT);
  ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
  MHD_destroy_response (response);
  return ret; 
}

int main (int argc, char *argv[]) {
  struct MHD_Daemon *daemon;
  daemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL, &answer_to_connection, NULL, MHD_OPTION_END);
  if (NULL == daemon) return 1;

  getchar();

  MHD_stop_daemon (daemon);

  return 0; 
}
