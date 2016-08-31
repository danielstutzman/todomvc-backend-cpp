#include <microhttpd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "./vendor/json/libjson/libjson.h"

char* create_json_for_todos_route() {
  JSONNODE *n = json_new(JSON_NODE);
  json_push_back(n, json_new_a("String Node", "String Value"));
  json_push_back(n, json_new_i("Integer Node", 42));
  json_push_back(n, json_new_f("Floating Point Node", 3.14));
  json_push_back(n, json_new_b("Boolean Node", 1));

  JSONNODE *c = json_new(JSON_ARRAY);
  json_set_name(c, "ArrayOfNumbers");
  json_push_back(c, json_new_i(NULL, 16));
  json_push_back(c, json_new_i(NULL, 42));
  json_push_back(c, json_new_i(NULL, 128));
  json_push_back(n, c);

  json_char *jc = json_write_formatted(n);
  char* json_string = strdup(jc);
  json_free(jc);
  json_delete(n);
  return json_string;
}

static int access_handler_callback(void* _contextInput,
    struct MHD_Connection* connection, const char* url, const char* method,
    const char* version, const char* upload_data, size_t* upload_data_size,
    void** contextOutput) {

  if (*contextOutput == NULL) {
    // first time: don't respond
    static int old_connection_marker;
    *contextOutput = &old_connection_marker;
    return MHD_YES;
  } else {
    // second time: do respond
    *contextOutput = NULL;
  }

  const char* output;
  unsigned int status_code;
  enum MHD_ResponseMemoryMode response_memory_mode;
  if (strcmp(method, "GET") != 0) {
    output = "Method not allowed";
    response_memory_mode = MHD_RESPMEM_MUST_COPY;
    status_code = MHD_HTTP_METHOD_NOT_ALLOWED;
  } else if (*upload_data_size != 0) {
    output = "Bad request: can't upload data in a GET";
    response_memory_mode = MHD_RESPMEM_MUST_COPY;
    status_code = MHD_HTTP_BAD_REQUEST;
  } else {
    if (strcmp(url, "/") == 0) {
      output = "Success";
      response_memory_mode = MHD_RESPMEM_MUST_COPY;
      status_code = MHD_HTTP_OK;
    } else if (strcmp(url, "/todos") == 0) {
      output = create_json_for_todos_route();
      response_memory_mode = MHD_RESPMEM_MUST_FREE;
      status_code = MHD_HTTP_NOT_FOUND;
    } else {
      output = "Unknown route";
      response_memory_mode = MHD_RESPMEM_MUST_COPY;
      status_code = MHD_HTTP_NOT_FOUND;
    }
  }

  struct MHD_Response* response = MHD_create_response_from_buffer (strlen(output),
    (void*) output, MHD_RESPMEM_PERSISTENT);
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
