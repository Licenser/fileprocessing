#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "erl_nif.h"

char* read_line(FILE *fp, char* line) {
  char* linepos = line;
  int c;
  while ((c = getc(fp)) != EOF &&
         c != 10 &&
         (linepos - line) < 2047
        ) {
          *linepos = (unsigned char) c;
          linepos++;
  }
  *linepos = 0;
  if (c == EOF) {
    return NULL;
  }
  
  return line;
};

int read_file(char* file, ErlNifPid* pid, ErlNifEnv* env, ErlNifEnv* msg_env) {
  int len = 0, c;
  char line[2048];
  ErlNifBinary bin;
  FILE *fp = fopen (file, "r");
  if (!fp)
    return -42;
  while (c = fgets(line, 2047, fp) != 0) {
    enif_clear_env(msg_env);
    enif_alloc_binary(c, &bin);
    memcpy(bin.data, line, c);
    enif_send(env, pid, msg_env, enif_make_tuple2(msg_env, 
      enif_make_atom(msg_env, "line"),
      enif_make_binary(msg_env, &bin)
    ));
    len += 1;
  }
  fclose(fp);
  return len;
}

static ERL_NIF_TERM read_nif(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
  int ret;
  char file[2048];
  ErlNifPid* pid;
  ErlNifEnv* msg_env = enif_alloc_env();  
  
  if (0 >= enif_get_string(env, argv[0], file, 2048, ERL_NIF_LATIN1)) {
  	return enif_make_badarg(env);
  };
  enif_get_local_pid(env, argv[1], pid);
  ret = read_file(file, pid, env, msg_env);
      printf("6\n");
  //enif_clear_env(msg_env);
  printf("7\n");
  enif_send(env, pid, msg_env, enif_make_atom(msg_env, "eof"));
  printf("8\n");
  enif_free_env(msg_env);
  printf("9\n");
  return enif_make_int(env, ret);
}

static ErlNifFunc nif_funcs[] = {
    {"read", 2, read_nif}
};

ERL_NIF_INIT(creader, nif_funcs, NULL, NULL, NULL, NULL)
