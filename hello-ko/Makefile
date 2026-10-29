# SPDX-License-Identifier: GPL-2.0
# Usage:
#   make
#   make clean
#   make install
#   make KDIR=<path>
#
# Cross-compile example:
#   make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- KDIR=/path/to/kernel
#

obj-m := hello-ko.o

KDIR ?= /lib/modules/$(shell uname -r)/build

MODDIR := $(CURDIR)

KBUILD_OPTS := M=$(MODDIR)

all:
	$(MAKE) -C $(KDIR) $(KBUILD_OPTS) modules

modules_install install:
	$(MAKE) -C $(KDIR) $(KBUILD_OPTS) modules_install

clean:
	$(MAKE) -C $(KDIR) $(KBUILD_OPTS) clean

.PHONY: all modules_install install clean
