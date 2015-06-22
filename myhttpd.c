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

  mrb_value res = mrb_funcall(mrb, mrb_top_self(mrb), "gchist", 1, mrb_str_new(mrb, cls, strlen(cls)));

  for(int i = 0; i < RARRAY_LEN(res); i++){
    logger("log.log", mrb_str_to_cstr(mrb, mrb_ary_ref(mrb, res, i))); 
  }

  char *page = NULL;
  page = (char *)malloc(sizeof(char));
  for(int i = 0; i < RARRAY_LEN(res); i++){
    char *a = mrb_str_to_cstr(mrb, mrb_ary_ref(mrb, res, i)); 
    page = (char*)realloc(page, strlen(page) + strlen(a) + 1);
    strcat(page, a);
  }

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
