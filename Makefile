ifeq ($(NODE_SELECT), end) 
	CONTIKI_PROJECT = main-process-end
	PROJECT_SOURCEFILES += main-process-end.c
endif

ifeq ($(NODE_SELECT), intermediate)
	CONTIKI_PROJECT = main-process-intermediate
	PROJECT_SOURCEFILES += main-process-intermediate.c
endif

all: $(CONTIKI_PROJECT)

MODULES += $(CONTIKI)/os/services/shell
CONTIKI = ${CNG_PATH}
PROJECTDIRS += $(CURDIR)/src

include $(CONTIKI)/Makefile.include

