all: jbsh

jbsh: jbsh.c
	gcc -o jbsh jbsh.c -Wall -W -pedantic -std=gnu99

install: jbsh
	cp ./jbsh /usr/local/bin

clean:
	rm -f jbsh

