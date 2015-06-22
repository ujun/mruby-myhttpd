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
