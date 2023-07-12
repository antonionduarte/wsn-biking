ifeq ($(NODE_SELECT), end) 
	CONTIKI_PROJECT = end-node
	PROJECT_SOURCEFILES += end-node.c
endif

ifeq ($(NODE_SELECT), relay)
	CONTIKI_PROJECT = relay-node
	PROJECT_SOURCEFILES += relay-node.c
endif

ifeq ($(NODE_SELECT), transfer)
	CONTIKI_PROJECT = transfer-experiments
	PROJECT_SOURCEFILES = transfer-experiments.c
endif

PROJECT_SOURCEFILES += project-conf.c 

all: $(CONTIKI_PROJECT)

MODULES += $(CONTIKI)/os/services/shell
CONTIKI = ${CNG_PATH}
PROJECTDIRS += $(CURDIR)/src

include $(CONTIKI)/Makefile.include

