#################################################
# 目标类型：
#   TARGET_LIB_STATIC：静态库
#   TARGET_LIB_SHARED：动态库
#   TARGET_BIN：可执行文件
#   TARGET_BEGIN：开头执行的目标
#   TARGET_END：结尾执行的目标
# 目标属性命名格式：目标名_属性名
# 目标属性：
#   src：源文件列表
#   lib：需要链接的库列表
#   cflag：编译选项
#   lflag：链接选项
# 编译说明：
#   编译出的目标文件在源文件目录下的OBJ_DIR中
#   编译出的库文件在LIB_DIR中
#   编译出的可执行文件在BIN_DIR中
#   C编译器默认为gcc，C++编译器默认为g++
#################################################
######################	history ######################
######################	v 1.0.* ######################
#######################################################

CPPC    = g++
CC      = gcc
LINK    = g++
AR      = ar cru
RANLIB  = ranlib
RM      = rm -f
MKDIR   = mkdir -p
RMDIR   = rmdir --ignore-fail-on-non-empty
LN      = ln -sf
CP      = cp --remove-destination --force

PUBLIC_INC  =/tlg/include/
PUBLIC_LIB  =/tlg/lib

OBJ_DIR = obj
BIN_DIR = bin
LIB_DIR = lib

.PHONY: all
all: $(TARGET_BEGIN) $(TARGET_LIB_STATIC) $(TARGET_LIB_SHARED) $(TARGET_BIN) $(TARGET_END)


SRC_ADD_OBJ_DIR    = $(addsuffix $(notdir $(1)),$(addsuffix $(OBJ_DIR)/,$(dir $(1))))
SRC_ADD_TARGET     = $(addsuffix $(notdir $(2)),$(addsuffix $(1)_,$(dir $(2))))
SRC_GET_OBJ        = $(call SRC_ADD_OBJ_DIR,$(call SRC_ADD_TARGET,$(1),$(2)))
SRC_GET_OBJ_STATIC = $(addsuffix o,$(call SRC_GET_OBJ,$(1),$(2)))
SRC_GET_OBJ_SHARED = $(addsuffix lo,$(call SRC_GET_OBJ,$(1),$(2)))
SRC_GET_DEP        = $(addsuffix .dep,$(call SRC_GET_OBJ,$(1),$(2)))
SRC_GET_COMP       = $(if $(filter %.c,$(1)),$(CC),$(CPPC))

define MK_OBJ_DIR
@if [ ! -d $(dir $@) ]; then \
    $(MKDIR) $(dir $@); \
fi
endef

define SRC_STATIC_COMP
$$(call SRC_GET_OBJ_STATIC,$(1),$(2)): $(2)
#	-@echo "Compiling file $$< for $(1)"
	$$(MK_OBJ_DIR)
	$$(call SRC_GET_COMP,$(2)) $$($(1)_cflag) -MMD -MF "$$(call SRC_GET_DEP,$(1),$(2))" -c $$< -o $$@
	@echo
endef


define SRC_SHARED_COMP
$$(call SRC_GET_OBJ_SHARED,$(1),$(2)): $(2)
#	-@echo "Compiling file $$< for $(1)"
	$$(MK_OBJ_DIR)
	$$(call SRC_GET_COMP,$(2)) $$($(1)_cflag) -fPIC -MMD -MF "$$(call SRC_GET_DEP,$(1),$(2))" -c $$< -o $$@
	@echo
endef

define BUILD_LIB_STATIC
$(1)_ver   ?= 1.0.0
$(1)_target = $$(LIB_DIR)/lib$(1)-$$($(1)_ver).a
$(1)_obj    = $$(foreach f,$$($(1)_src),$$(call SRC_GET_OBJ_STATIC,$(1),$$(f)))
$(1)_dep    = $$(foreach f,$$($(1)_src),$$(call SRC_GET_DEP,$(1),$$(f)))
$(1)_cflag ?= $$(CFLAG)
$$(foreach f,$$($(1)_src),$$(eval $$(call SRC_STATIC_COMP,$(1),$$(f))))
$$($(1)_target): $$($(1)_obj)
	-@echo "Link static library $$@"
	$$(MK_OBJ_DIR)
	$$(RM) $$@
	$$(AR) $$@ $$^
	$$(RANLIB) $$@
	$$(LN) ../$$@ $$(LIB_DIR)/lib$(1).a
	@echo
.PHONY: $(1)
$(1): $$($(1)_target)	
-include $$($(1)_dep)    
all_objs += $$($(1)_target)
all_objs += $$($(1)_obj) 
all_objs += $$($(1)_dep)
endef

#$$(LN) ../$$@ $$(LIB_DIR)/lib$(1).so
define BUILD_LIB_SHARED
$(1)_ver   ?= 1.0.0
$(1)_target = $$(LIB_DIR)/lib$(1)-$$($(1)_ver).so
$(1)_obj    = $$(foreach f,$$($(1)_src),$$(call SRC_GET_OBJ_SHARED,$(1),$$(f)))
$(1)_dep    = $$(foreach f,$$($(1)_src),$$(call SRC_GET_DEP,$(1),$$(f)))
$(1)_cflag ?= $$(CFLAG)
$(1)_lflag ?= $$(LFLAG)
$$(foreach f,$$($(1)_src),$$(eval $$(call SRC_SHARED_COMP,$(1),$$(f))))
$$($(1)_target): $$($(1)_obj)
	-@echo "Link shared library $$@"
	$$(MK_OBJ_DIR)
	$$(LINK) $$+ -shared $$($(1)_lflag) $$(addprefix -l,$$($(1)_lib)) -o $$@
	@echo
.PHONY: $(1)
$(1): $$($(1)_target)
-include $$($(1)_dep)    
all_objs += $$($(1)_target)
all_objs += $$($(1)_obj) 
all_objs += $$($(1)_dep)
endef

#$$(LN) ../$$@ $$(BIN_DIR)/$(1)
define BUILD_BIN
$(1)_ver   ?= 1.0.0
$(1)_target = $$(BIN_DIR)/$(1)-$$($(1)_ver)
$(1)_obj    = $$(foreach f,$$($(1)_src),$$(call SRC_GET_OBJ_STATIC,$(1),$$(f)))
$(1)_dep    = $$(foreach f,$$($(1)_src),$$(call SRC_GET_DEP,$(1),$$(f)))
$(1)_cflag ?= $$(CFLAG)
$(1)_lflag ?= $$(LFLAG)
$$(foreach f,$$($(1)_src),$$(eval $$(call SRC_STATIC_COMP,$(1),$$(f))))
$$($(1)_target): $$($(1)_obj)
	-@echo "Link binary $$@"
	$$(MK_OBJ_DIR)
	$$(LINK) $$+ $$($(1)_lflag) $$(addprefix -l,$$($(1)_lib)) -o $$@
	@echo
.PHONY: $(1)
$(1): $$($(1)_target)	
-include $$($(1)_dep)    
all_objs += $$($(1)_target)
all_objs += $$($(1)_obj) 
all_objs += $$($(1)_dep)
endef

$(foreach f,$(TARGET_LIB_STATIC),$(eval $(call BUILD_LIB_STATIC,$(f))))
$(foreach f,$(TARGET_LIB_SHARED),$(eval $(call BUILD_LIB_SHARED,$(f))))
$(foreach f,$(TARGET_BIN),$(eval $(call BUILD_BIN,$(f))))

.PHONY: clean		
clean:
	@$(RM) $(all_objs)
	@if [ -d $(OBJ_DIR) ]; then \
		$(RMDIR) $(OBJ_DIR); \
	fi
	@if [ -d $(BIN_DIR) ]; then \
		$(RMDIR) $(BIN_DIR); \
	fi
	@if [ -d $(LIB_DIR) ]; then \
		$(RMDIR) $(LIB_DIR); \
	fi

.PHONY: debug	
debug:
	@echo $(all_objs)
