# -*- Makefile -*-

USE_LMD_INPUT=1

OBJS += wasa_user.o

LAND_COMMON_DIR = ./../land_common
SRC_DIRS += $(LAND_COMMON_DIR)

UCESB_CPP_FLAGS += -I.
