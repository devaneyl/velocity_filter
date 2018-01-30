velocity_filter: velocity_filter.c
	 gcc -o $@ `pkg-config --libs alsa` $<
