OBJS_TB = engine_tb.o cc_tb.o main.o output_alsa.o midi_alsa.o synth_organ.o voice_organ.o env_ar.o env_click.o efx_chorus.o
BIN_TB = testbench

CPPFLAGS = -g -Wall -Iengine -I./engine/hardware -Iflexynth -I./flexynth/components -I./engine/midi -O2 -DnCLICK_TEST -DCHORUS_TEST -DnTESTBENCH -DnCHORUS_DEBUG
LINKFLAGS = -lasound

# This rule compiles the testbench.
all: testbench_link 
	rm -f $(OBJS_TB)

testbench_link: $(OBJS_TB)
	g++ -o $(BIN_TB) $(OBJS_TB) $(LINKFLAGS)

main.o: main.c ./engine/engine_tb.cpp
	g++ $(CPPFLAGS) -c main.c

engine_tb.o: ./engine/engine_tb.cpp ./engine/engine_tb.hpp ./engine/engine_base.hpp
	g++ $(CPPFLAGS) -c ./engine/engine_tb.cpp

cc_tb.o: ./engine/hardware/cc_base.hpp ./engine/hardware/cc_tb.hpp ./engine/hardware/cc_tb.cpp
	g++ $(CPPFLAGS) -c ./engine/hardware/cc_tb.cpp

output_alsa.o: ./engine/hardware/output_base.hpp ./engine/hardware/output_alsa.hpp ./engine/hardware/output_alsa.cpp
	g++ $(CPPFLAGS) -c ./engine/hardware/output_alsa.cpp

midi_alsa.o: ./engine/midi/midi_base.hpp ./engine/midi/midi_alsa.hpp ./engine/midi/midi_alsa.cpp
	g++ $(CPPFLAGS) -c ./engine/midi/midi_alsa.cpp

synth_organ.o: ./flexynth/flexynth_base.hpp ./flexynth/synth_organ.hpp ./flexynth/synth_organ.cpp ./flexynth/components/tables.hpp
	g++ $(CPPFLAGS) -c ./flexynth/synth_organ.cpp

voice_organ.o: ./flexynth/components/voice_base.hpp ./flexynth/components/voice_organ.hpp ./flexynth/components/voice_organ.cpp ./flexynth/components/tables.hpp
	g++ $(CPPFLAGS) -c ./flexynth/components/voice_organ.cpp

env_ar.o: ./flexynth/components/env_base.hpp ./flexynth/components/env_ar.hpp ./flexynth/components/env_ar.cpp
	g++ $(CPPFLAGS) -c ./flexynth/components/env_ar.cpp

env_click.o: ./flexynth/components/env_base.hpp ./flexynth/components/env_click.hpp ./flexynth/components/env_click.cpp
	g++ $(CPPFLAGS) -c ./flexynth/components/env_click.cpp

efx_chorus.o: ./flexynth/components/efx_base.hpp ./flexynth/components/efx_chorus.hpp ./flexynth/components/efx_chorus.cpp
	g++ $(CPPFLAGS) -c ./flexynth/components/efx_chorus.cpp
# A phony rule, to clean compiled code.

clean:
	rm -f $(BIN_TB) $(OBJS_TB) output.pcm
