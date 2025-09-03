SRCS :=

include cfg.mk

all: $(ALL_LIBS)

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
