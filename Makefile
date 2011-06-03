all: serial.beam parallel.beam reader.o creader.beam serialc.beam

serialc.beam: serialc.erl
	erlc serialc.erl

serial.beam: serial.erl
	erlc serial.erl

creader.beam: creader.erl
	erlc creader.erl

parallel.beam: parallel.erl
	erlc parallel.erl

reader.o: reader.c
	gcc -O3 -fPIC -flat_namespace -undefined suppress -shared -o reader.so reader.c -I/usr/local/lib/erlang/erts-5.8.4/include/


clean:
	rm -f *.beam

test: serial.beam parallel.beam
	erl -eval "eunit:test([serial, parallel])" -noshell -s init stop

docs:
	erl -noshell -run edoc_run files '["serial.erl", "parallel.erl"]' '[{dir, "doc"}, {private, true}]' -s init stop

clean-docs:
	rm -f doc/edoc-info doc/*.html doc/*.css doc/*.png

data:
	python mkdata.py 100000 > data.txt
