LIBLF_BUILD_TYPE := debug
SRCS :=

include ../config.mk
include ../settings.mk

HDEPS := lf_cc.h lf_op.h lf_op_sup.h lf_platform.h \
	 lf/gcc/lf_op_gcc.h lf/gcc/lf_op_x64.h \
	 lf/gcc/lf_op_gcc_sup.h lf/gcc/lf_op_x64_sup.h \
	 lf_bitstring.h lf_ring.h
HDEPS := $(patsubst %,../include/%,$(HDEPS))

EXAMPLES := example_basic example_dcasx example_bitstring example_ring
EXAMPLE_SRCS := $(patsubst %,%.c,$(EXAMPLES))

all: $(EXAMPLES)

%: %.c $(HDEPS)
	@$(quiet_CC)
	$(Q)$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

clean:
	$(Q)rm $(EXAMPLES)
	$(Q)rm *.d

.PHONY: all clean
