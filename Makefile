compiler= gcc
cflags= -Os -pipe -Wall -march=native 
includes=`sdl-config --cflags`
libdir=`sdl-config --libs`
link= -lm -lSDL_mixer
target=ds_sound_sdl

source_files=$(wildcard *.c)
object_files=$(patsubst %.c,%.o,$(source_files))

$(target): $(object_files)
	$(compiler) $(cflags) $^ $(libdir) $(link) -o $@

%.o: %.c
	$(compiler) $(cflags) $(includes)  -c $<
	
clean: 
	rm -f $(target)
	rm -f $(object_files)





