#include <microhttpd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int access_handler_callback(void* _contextInput,
    struct MHD_Connection* connection, const char* url, const char* method,
    const char* version, const char* upload_data, size_t* upload_data_size,
    void** contextOutput) {

  // if first time, just return MHD_YES
  if (*contextOutput == NULL) {
    static int old_connection_marker;
    *contextOutput = &old_connection_marker;
    return MHD_YES;
  } else {
    *contextOutput = NULL;
  }

  const char* html;
  unsigned int status_code;
  if (strcmp(method, "GET") != 0) {
    html = "Method not allowed";
    status_code = MHD_HTTP_METHOD_NOT_ALLOWED;
  } else if (*upload_data_size != 0) {
    html = "Bad request: can't upload data in a GET";
    status_code = MHD_HTTP_BAD_REQUEST;
  } else {
    html = "Success";
    status_code = MHD_HTTP_OK;
  }

  struct MHD_Response* response = MHD_create_response_from_buffer (strlen(html),
    (void*) html, MHD_RESPMEM_PERSISTENT);
  int did_queue_succeed = MHD_queue_response(connection, MHD_HTTP_OK, response);
  MHD_destroy_response(response);
  return did_queue_succeed;
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    printf("%s PORT\n", argv[0]);
    return 1;
  }
  int port = atoi(argv[1]);

  fprintf(stderr, "Running web server on port %d...\n", port);
  struct MHD_Daemon* d = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION,
    port, NULL, NULL, &access_handler_callback, NULL, MHD_OPTION_END);
  if (d == NULL) {
    fprintf(stderr, "Fatal error: MHD_start_daemon returned NULL\n");
    return 1;
  }
  pause(); /* never return; run web server until Ctrl-C pressed */
  return 0;
}
