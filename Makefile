CONTIKI_PROJECT = hello

all: $(CONTIKI_PROJECT)

CONTIKI = ${CNG_PATH}
PROJECTDIRS += $(CURDIR)/src
PROJECT_SOURCEFILES += hello.c

include $(CONTIKI)/Makefile.include

