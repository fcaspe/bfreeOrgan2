OBJS_TB = engine_tb.o cc_tb.o main.o output_alsa.o midi_alsa.o synth_organ.o voice_organ.o env_ar.o env_click.o efx_chorus.o
BIN_TB = ./bin/testbench

CPPFLAGS = -g -Wall -I./src/engine -I./src/engine/hardware -I./src/flexynth -I./src/flexynth/components -I./src/engine/midi -O2 -DnCLICK_TEST -DCHORUS_TEST -DnTESTBENCH -DnCHORUS_DEBUG
LINKFLAGS = -lasound

# This rule compiles the testbench.
all: mkdir_bin testbench_link 
	rm -f $(OBJS_TB)

mkdir_bin:
	mkdir bin

testbench_link: $(OBJS_TB)
	g++ -o $(BIN_TB) $(OBJS_TB) $(LINKFLAGS)

main.o: ./src/main.c ./src/engine/engine_tb.cpp
	g++ $(CPPFLAGS) -c ./src/main.c

engine_tb.o: ./src/engine/engine_tb.cpp ./src/engine/engine_tb.hpp ./src/engine/engine_base.hpp
	g++ $(CPPFLAGS) -c ./src/engine/engine_tb.cpp

cc_tb.o: ./src/engine/hardware/cc_base.hpp ./src/engine/hardware/cc_tb.hpp ./src/engine/hardware/cc_tb.cpp
	g++ $(CPPFLAGS) -c ./src/engine/hardware/cc_tb.cpp

output_alsa.o: ./src/engine/hardware/output_base.hpp ./src/engine/hardware/output_alsa.hpp ./src/engine/hardware/output_alsa.cpp
	g++ $(CPPFLAGS) -c ./src/engine/hardware/output_alsa.cpp

midi_alsa.o: ./src/engine/midi/midi_base.hpp ./src/engine/midi/midi_alsa.hpp ./src/engine/midi/midi_alsa.cpp
	g++ $(CPPFLAGS) -c ./src/engine/midi/midi_alsa.cpp

synth_organ.o: ./src/flexynth/flexynth_base.hpp ./src/flexynth/synth_organ.hpp ./src/flexynth/synth_organ.cpp ./src/flexynth/components/tables.hpp
	g++ $(CPPFLAGS) -c ./src/flexynth/synth_organ.cpp

voice_organ.o: ./src/flexynth/components/voice_base.hpp ./src/flexynth/components/voice_organ.hpp ./src/flexynth/components/voice_organ.cpp ./src/flexynth/components/tables.hpp
	g++ $(CPPFLAGS) -c ./src/flexynth/components/voice_organ.cpp

env_ar.o: ./src/flexynth/components/env_base.hpp ./src/flexynth/components/env_ar.hpp ./src/flexynth/components/env_ar.cpp
	g++ $(CPPFLAGS) -c ./src/flexynth/components/env_ar.cpp

env_click.o: ./src/flexynth/components/env_base.hpp ./src/flexynth/components/env_click.hpp ./src/flexynth/components/env_click.cpp
	g++ $(CPPFLAGS) -c ./src/flexynth/components/env_click.cpp

efx_chorus.o: ./src/flexynth/components/efx_base.hpp ./src/flexynth/components/efx_chorus.hpp ./src/flexynth/components/efx_chorus.cpp
	g++ $(CPPFLAGS) -c ./src/flexynth/components/efx_chorus.cpp
# A phony rule, to clean compiled code.

clean:
	rm -f $(BIN_TB) $(OBJS_TB) output.pcm
	rm -r bin
