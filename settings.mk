LIBLF_VERSION_MAJOR ?= 0
LIBLF_VERSION_MINOR ?= 1
LIBLF_VERSION_PATCH ?= 0
LIBLF_VERSION := $(LIBLF_VERSION_MAJOR).$(LIBLF_VERSION_MINOR).$(LIBLF_VERSION_PATCH)

CC ?= cc
LD := $(CC)
PKG_CONFIG ?= pkg-config

DESTDIR ?=
PREFIX ?= $(DESTDIR)/usr/local
LIBDIR ?= $(PREFIX)/lib
INCLUDEDIR ?= $(PREFIX)/include
PKGCONFIGDIR ?= $(LIBDIR)/pkgconfig

LDNAME = liblxev.so
LDNAME_MAJOR = $(LDNAME).$(LIBLF_VERSION_MAJOR)
LDNAME_VERSION = $(LDNAME).$(LIBLF_VERSION)

ROOT_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
SRC_DIR := $(ROOT_DIR)/src
BUILD_DIR := $(ROOT_DIR)/build
OBJ_DIR := $(BUILD_DIR)/objs
OBJ_SUBDIRS := $(OBJ_DIR)/src

STATIC_LIB := $(BUILD_DIR)/liblxev.a
SHARED_LIB := $(BUILD_DIR)/$(LDNAME)
ALL_LIBS := $(STATIC_LIB) $(SHARED_LIB)

CFLAGS += -std=gnu99 -fPIC -I$(ROOT_DIR)/include -I$(SRC_DIR)
CFLAGS += -MMD -MP

# Warning flags
CFLAGS += -Werror -Wall -Wextra -Wpedantic -Wno-unused -Wfloat-equal
CFLAGS += -Wdouble-promotion -Wformat=2 -Wformat-security -Wstack-protector
CFLAGS += -Walloca -Wvla -Wcast-qual -Wconversion -Wformat-signedness -Wshadow
CFLAGS += -Wstrict-overflow=4 -Wundef -Wstrict-prototypes -Wswitch-default
CFLAGS += -Wswitch-enum -Wnull-dereference -Wmissing-include-dirs -Wstrict-aliasing

# Security flags
CFLAGS += -fstack-protector-strong -fvisibility=hidden

LDFLAGS += -fPIC -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack -Wl,-z,separate-code

# Release CFLAGS
CFLAGS_RELEASE := -D_FORTIFY_SOURCE=2
CFLAGS_DEBUG := -g -fsanitize=undefined,address,pointer-compare,pointer-subtract \
		-fstack-clash-protection -fno-omit-frame-pointer

ifeq ($(LIBLF_BUILD_TYPE),release)
LIBLF_COMPILE_ASSERTS ?= 0
CFLAGS += -O2 $(CFLAGS_RELEASE)
CFLAGS += -DLIBLF_RELEASE
else ifeq ($(LIBLF_BUILD_TYPE),debug)
LIBLF_COMPILE_ASSERTS ?= 1
CFLAGS += -Og $(CFLAGS_DEBUG)
CFLAGS += -DLIBLF_DEBUG
else ifeq ($(LIBLF_BUILD_TYPE),reldebug)
LIBLF_COMPILE_ASSERTS ?= 1
CFLAGS += -O2 $(CFLAGS_DEBUG) $(CFLAGS_RELEASE)
CFLAGS += -DLIBLF_RELDEBUG
else
$(error Invalid LIBLF_BUILD_TYPE '$(LIBLF_BUILD_TYPE)'. Expected one of: release, debug, reldebug)
endif

CFLAGS += -DLIBLF_VERSION_MAJOR=$(LIBLF_VERSION_MAJOR)
CFLAGS += -DLIBLF_VERSION_MINOR=$(LIBLF_VERSION_MINOR)
CFLAGS += -DLIBLF_VERSION_PATCH=$(LIBLF_VERSION_PATCH)
CFLAGS += -DLIBLF_VERSION_STR=\"$(LIBLF_VERSION)\"
CFLAGS += -DLIBLF_COMPILE_ASSERTS=$(LIBLF_COMPILE_ASSERTS)

VERBOSE ?= 0
ifeq ($(VERBOSE),1)
quiet_CC =
quiet_LD =
quiet_AR =
Q =
else
quiet_CC = echo " CC    $(subst $(ROOT_DIR)/,,$@)"
quiet_LD = echo " LD    $(subst $(ROOT_DIR)/,,$@)"
quiet_AR = echo " AR    $(subst $(ROOT_DIR)/,,$@)"
Q = @
endif

OBJS := $(patsubst %.c,%.o,$(SRCS))
SRCS := $(patsubst %,$(SRC_DIR)/%,$(SRCS))
OBJS := $(patsubst %,$(OBJ_DIR)/%,$(OBJS))

# Add config flags
CFLAGS += -DLIBLF_CONFIG_CACHELINE=$(LIBLF_CONFIG_CACHELINE)
