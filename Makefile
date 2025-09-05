SRCS :=

include config.mk
include settings.mk

CFLAGS += -DLIBLF_CONFIG_CACHELINE=$(LIBLF_CONFIG_CACHELINE)

all:
	@echo "Making examples in ./examples"
	$(Q)$(MAKE) -C examples

scratch: $(OBJ_SUBDIRS) $(OBJ_DIR)/scratch.o $(OBJS)
	@$(quiet_CC)
	$(Q)$(CC) $(CFLAGS) $(OBJ_DIR)/scratch.o $(OBJS) -o $(BUILD_DIR)/scratch

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(quiet_CC)
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_SUBDIRS):
	$(Q)mkdir -p $@

-include $(OBJS:.o=.d)

clean:
	$(Q)rm -rf $(BUILD_DIR)

.PHONY: all scratch clean
