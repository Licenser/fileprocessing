-module(creader).
-export([read/2]).
-on_load(init/0).
init() ->
    ok = erlang:load_nif("./reader", 0).
    
read(_File, _Pid) ->
    exit(nif_library_not_loaded).