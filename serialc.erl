-module(serialc).

-export([start/1, my_loop/1]).

my_loop(Count) ->
  receive
    {line, _String} -> my_loop(Count + 1);
    eof -> io:format("~B~n", [Count]),
    {_, Delta} = erlang:statistics(wall_clock),
    error_logger:info_msg("~p ~p~n", [self(), Delta / 1000]),
    init:stop()
  end.


start([File]) ->
  erlang:statistics(wall_clock),
  
  %% Open file
  Loop = spawn(serialc, my_loop, [0]),
  %% Scan file
  creader:read(File, Loop).
