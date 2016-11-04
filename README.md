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

