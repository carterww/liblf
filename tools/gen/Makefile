LIBLF_BUILD_TYPE := reldebug
LIBLF_COMPILE_ASSERTS := 1

include ../../cfg.mk

COMMON_DEPS := lf_gen_common.h lf_gen_functions.h lf_gen_string.h lf_gen_types.h

all: lf_gen_gcc lf_gen_x64

lf_gen_gcc: lf_gen_gcc.c $(COMMON_DEPS)
	$(Q)$(CC) $(CFLAGS) $(LDFLAGS) lf_gen_gcc.c -o $@

lf_gen_x64: lf_gen_x64.c $(COMMON_DEPS)
	$(Q)$(CC) $(CFLAGS) $(LDFLAGS) lf_gen_x64.c -o $@

clean:
	$(Q)rm lf_gen_gcc lf_gen_gcc.d
	$(Q)rm lf_gen_x64 lf_gen_x64.d

-include lf_gen_gcc.d

.PHONY: all clean
