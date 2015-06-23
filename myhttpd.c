#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mruby.h"
#include "mruby/compile.h"
#include "mruby/string.h"
#include "mruby/array.h"
#include "/usr/include/sqlite3.h"
#include "utils.c"
#define PORT 8888

static int answer_to_connection (void *cls, struct MHD_Connection *connection, const char *url, const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **con_cls) {

  struct MHD_Response *response;

  mrb_state* mrb = mrb_open();

  mrb_loader(mrb, "/Users/juchino/myruby/src/ujun/mruby_build/mruby-myhttpd/myhttpd.rb");

  char* page = create_page(mrb, cls, "gchist");
  logger("log.log", page);

  mrb_close(mrb);

  return create_response(page, connection);
}


int main (int argc, char *argv[]) {
  struct MHD_Daemon *daemon;
  daemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL, &answer_to_connection, argv[1], MHD_OPTION_END);
  if (NULL == daemon) return 1;

  getchar();

  MHD_stop_daemon (daemon);

  return 0; 
}
