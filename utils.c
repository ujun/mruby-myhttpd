void logger(char* log, char* message) {
  FILE* f = fopen(log, "a");
  fprintf(f, "%s\n", message);
  fclose(f);
}

void mrb_loader(mrb_state* mrb, char* file){
  FILE *f = fopen(file, "r");
  mrb_load_file(mrb, f);
  fclose(f);
}

int create_response(char* page, struct MHD_Connection* connection) {
  struct MHD_Response* response = MHD_create_response_from_buffer(strlen (page), (void *) page, MHD_RESPMEM_PERSISTENT);
  int ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
  MHD_destroy_response (response);

  return ret;
}

char* create_page(mrb_state* mrb, void* cls, char* func){

  mrb_value res = mrb_funcall(mrb, mrb_top_self(mrb), func, 1, mrb_str_new(mrb, cls, strlen(cls)));

  // for(int i = 0; i < RARRAY_LEN(res); i++){
  //   logger("log.log", mrb_str_to_cstr(mrb, mrb_ary_ref(mrb, res, i))); 
  // }

  char *page = NULL;
  page = (char *)malloc(sizeof(char));
  for(int i = 0; i < RARRAY_LEN(res); i++){
    char *a = mrb_str_to_cstr(mrb, mrb_ary_ref(mrb, res, i)); 
    page = (char*)realloc(page, strlen(page) + strlen(a) + 1);
    strcat(page, a);
  }

  return page;
}
