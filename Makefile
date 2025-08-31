SRCS :=

include cfg.mk

all: $(ALL_LIBS)

$(SHARED_LIB): LDFLAGS += -Wl,-soname,$(LDNAME_MAJOR) -shared
$(SHARED_LIB): $(OBJ_SUBDIRS) $(OBJS)
	@$(quiet_LD)
	$(Q)$(LD) $(LDFLAGS) -o $(SHARED_LIB) $(OBJS)

$(STATIC_LIB): $(OBJ_SUBDIRS) $(OBJS)
	@$(quiet_AR)
	$(Q)$(AR) rcs $(STATIC_LIB) $(OBJS)

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
