PROJECT_DIR = /usr/mcl
PROJECT_INC = $(PROJECT_DIR)/include
PROJECT_LIB = $(PROJECT_DIR)/lib
PROJECT_BIN = $(PROJECT_DIR)/bin
CURRENT_DIR = $(PWD)

TARGET_BEGIN      = 
TARGET_LIB_STATIC = 
TARGET_LIB_SHARED = 
TARGET_BIN        = mcl
TARGET_END        = 

mcl_ver   = 1.0.0
mcl_src   = mcl_array_hash.c addr_v4_stat.c
mcl_lib   = pthread

mcl_cflag = -Wall -Wno-strict-aliasing -m64 -finline-functions -pipe -DPROJECT_PATH="\"$(PROJECT_DIR)\"" -DAPP_VERSION="\"$(mcl_ver)\""
mcl_lflag = -Wall -m64 -finline-functions -pipe
ifeq ($(DEBUG), yes)
  mcl_cflag := -g -O0 $(mcl_cflag)
else 
  mcl_cflag := -g -O2 $(mcl_cflag)
endif

include common.mk

.PHONY: install

install:
	@echo "Installing Library ... "
	-$(CP) bin/$(TARGET_BIN)-$(mcl_ver) $(PROJECT_BIN)
	@echo

etags:
	(find . -name "*.[ch]" && find . -name "*.cpp") |xargs etags

