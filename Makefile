ifeq ("$(shell which pkg-config  2> /dev/null)","")
$(error 'pkg-config' NOT FOUND)
endif

ifeq ("$(shell pkg-config --libs libnotify  2> /dev/null)","")
$(error 'libnotify' NOT FOUND)
endif


APP := chkpasswd
CC = gcc
CLIBS := $(shell pkg-config --libs libnotify) -lpthread -lpam
CFLAGS := -Wall -std=c99 -D_GNU_SOURCE $(shell pkg-config --cflags libnotify)

OBJ := $(subst .c,.o,$(wildcard *.c))

release: CFLAGS+= -O3
release: CLIBS+= -Wl,-s
release: $(APP)

debug: CFLAGS+= -O0 -ggdb -DDEBUG -Wall -Wextra
debug: $(APP)

$(APP): $(OBJ)
	$(CC) $(OBJ) $(CLIBS) -o $(APP)


.o:
	$(CC) -c $<

.PHONY: release

install:
	install -m 0755 $(APP) /usr/bin/ 
	install -m 0644 $(APP).desktop /etc/xdg/autostart

uninstall:
	rm -v /usr/bin/$(APP)
	rm -v  /etc/xdg/autostart/$(APP).desktop


clean:
	rm  *.o $(APP)
