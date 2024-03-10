PACKAGE	:= numlock
VERSION	:= 0.2
AUTHOR	:= R.Jaksa 2024 GPLv3
SUBVERSION :=a

export SIGN="$(PACKAGE)-$(VERSION)$(SUBVERSION) $(AUTHOR)"

all: numlock README.md

numlock: numlock.c Makefile
	gcc -std=gnu11 -Os -Wall -DSIGN="$$SIGN" -o $@ $< -lX11 -lXtst
	strip $@

README.md: numlock
	$< -h | man2md > $@

install: numlock
	install $< /usr/local/bin

clean:
	rm -f numlock

mrproper: clean
	rm -f README.md

-include ~/.github/Makefile.git
