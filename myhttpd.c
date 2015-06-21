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
#define PORT 8888

static int answer_to_connection (void *cls, struct MHD_Connection *connection, const char *url, const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **con_cls) {

  struct MHD_Response *response;
  static int aptr;
  if (0 != strcmp (method, "GET")) return MHD_NO;
  if (&aptr != *con_cls) {
    *con_cls = &aptr;
    return MHD_YES;
  }
  *con_cls = NULL;

  mrb_state* mrb = mrb_open();

  FILE* f = fopen("/Users/juchino/myruby/src/ujun/mruby_build/mruby-myhttpd/myhttpd.rb", "r");
  mrb_load_file(mrb, f);
  fclose(f);

  mrb_value res;

  char *s = "centos";
  mrb_value str = mrb_str_new(mrb, s, strlen(s));
  res = mrb_funcall(mrb, mrb_top_self(mrb), "gchist", 1, str);

  // FILE* log = fopen("myhttpd.log", "a");
  // for(int i = 0; i < RARRAY_LEN(res); i++){
  //   fprintf(log, "%s\n", mrb_str_to_cstr(mrb, mrb_ary_ref(mrb, res, i))); 
  // }
  // fclose(log);

  char *page = NULL;
  page = (char *)malloc(sizeof(char));
  for(int i = 0; i < RARRAY_LEN(res); i++){
    char *a = mrb_str_to_cstr(mrb, mrb_ary_ref(mrb, res, i)); 
    page = (char*)realloc(page, strlen(page) + strlen(a) + 1);
    strcat(page, a);
  }

  mrb_close(mrb);


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
