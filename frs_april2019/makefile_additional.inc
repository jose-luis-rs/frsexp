# -*- Makefile -*-

USE_LMD_INPUT=1

# If you use an external unpacker (and then copy e.g. the
# external_data.{hh,cc} template files to this directory,
# you need to also get them into the list of sources =>
# enable external_hacky.o below.

OBJS += frs_april2019_user.o 

LAND_COMMON_DIR = ./../land_common

# The file containing user structure elements that are ntuple-dumpable,
# bia psdc-parsing


SRC_DIRS += $(LAND_COMMON_DIR)

CXXFLAGS += -DCONTROL_INCLUDE -I$(LAND_COMMON_DIR)
