CFLAGS=-D_LINUX -fPIC -DNSF_PLAYER
OBJS=src/cpu/nes6502/dis6502.o src/sndhrdw/fds_snd.o src/sndhrdw/fmopl.o src/log.o src/memguard.o src/sndhrdw/mmc5_snd.o src/cpu/nes6502/nes6502.o src/sndhrdw/nes_apu.o NoseFartDLL.o src/machine/nsf.o src/sndhrdw/vrc7_snd.o src/sndhrdw/vrcvisnd.o

nosefart-i486-linux.so: $(OBJS)
	gcc -fPIC -shared -o $@ $(OBJS) `cat ../../../XBMC/xbmc/cores/DllLoader/exports/wrapper.def` ../../../XBMC/xbmc/cores/DllLoader/exports/wrapper.o

clean:
	$(RM) $(OBJS) *.so

%o : %c
	gcc $(CFLAGS) -O2 -fPIC -c $< -o $@

%o : %cpp
	g++ $(CFLAGS) -O2 -fPIC -c $< -o $@
