CONTIKI_PROJECT = send-message

all: $(CONTIKI_PROJECT)

MODULES += $(CONTIKI)/os/services/shell
CONTIKI = ${CNG_PATH}
PROJECTDIRS += $(CURDIR)/src
PROJECT_SOURCEFILES += send-message.c

include $(CONTIKI)/Makefile.include

