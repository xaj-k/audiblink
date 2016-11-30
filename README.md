# audiblink
Audio with cool coordinated visuals meant for leds

For now, I intend to use JACK Audio Connection Kit to intercept raw sound from an mp3 player, 
before it makes it to the hardware, and do realtime signal analysis on each chunk and display results to an animated 
display.  Essentially, the goal is to make an automatic light show with music.  Eventually, the computer display data 
will be ported to an LED driver.


My step by step process

install jack:

	sudo apt-get install jack qjackctl

Select Yes when asked if to enable real time processing for user

tried
	pkg-config --cflags --libs jack
and got nothing so I googled "Package jack was not found in the pkg-config search path." and found: https://ubuntuforums.org/archive/index.php/t-1727080.html
so I did 
	sudo apt-get install libjack-dev
which failed so I subsiquently did
	  sudo apt-get install libjack0
and got
	-ljack -lpthread
note that I got the info for doing this from dis-dot-dat.net/index.cgi?item=jacktuts/starting/compiling
tried:
	gcc -o test `pkg-config --cflags --libs jack` main.c
and got a bunch of errors saying undefined reference <some jack function>
then following advice from http://stackoverflow.com/questions/15182640/undefined-reference-to-jack-client-close-et-al
	gcc -o test main.c `pkg-config --cflags --libs jack`
and it worked!! aparently order matters!
when running
	./test
I got
	jackd 0.122.0
	Copyright 2001-2009 Paul Davis, Stephane Letz, Jack O'Quinn, Torben Hohn and others.
	jackd comes with ABSOLUTELY NO WARRANTY
	This is free software, and you are welcome to redistribute it
	under certain conditions; see the file COPYING for details
	
	
	JACK is running in realtime mode, but you are not allowed to use realtime scheduling.
	Please check your /etc/security/limits.conf for the following line
	and correct/add it if necessary:
	
	  @audio          -       rtprio          99
	
	After applying these changes, please re-login in order for them to take effect.
	
	You don't appear to have a sane system configuration. It is very likely that you
	encounter xruns. Please apply all the above mentioned changes and start jack again!
	jack_client_open() failed, status = 0x11
	Unable to connect to JACK server
so, because of https://ubuntuforums.org/showthread.php?t=1397461, i edited the recommended file
	sudo gedit /etc/security/limits.conf
and added the line: @audio - rtprio 99
tried again:
	./test
and got
	jackd 0.122.0
	Copyright 2001-2009 Paul Davis, Stephane Letz, Jack O'Quinn, Torben Hohn and others.
	jackd comes with ABSOLUTELY NO WARRANTY
	This is free software, and you are welcome to redistribute it
	under certain conditions; see the file COPYING for details
	
	
	JACK is running in realtime mode, but you are not allowed to use realtime scheduling.
	
	Your system has an audio group, but you are not a member of it.
	Please add yourself to the audio group by executing (as root):
	  usermod -a -G audio k-jax
	
	After applying these changes, please re-login in order for them to take effect.
	
	You don't appear to have a sane system configuration. It is very likely that you
	encounter xruns. Please apply all the above mentioned changes and start jack again!
	jack_client_open() failed, status = 0x11
	Unable to connect to JACK server
so of course i
	sudo usermod -a -G audio k-jax
logged out and logged back in
Installed JACK plugin for vlc using
	sudo apt-get install vlc-plugin-jack
After some sound advice, I decided to install qjackctl using
	sudo apt-get install qjackctl
And I will soon be implementing jack record . . .
