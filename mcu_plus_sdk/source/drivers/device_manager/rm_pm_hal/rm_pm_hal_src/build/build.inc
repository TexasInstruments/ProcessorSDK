#
# Build include file for various includes used for build
#
# Copyright (C) 2017 Texas Instruments Incorporated - http://www.ti.com/
# ALL RIGHTS RESERVED
#

# Initial variables 

yn-files := obj extra prepare_headers target
yn-paths := subdirs
yn-vars := $(yn-files) $(yn-paths)

build_files := $(foreach v,$(yn-files),$v-y $v-n)
build_paths := obj src

objtree ?= .
srctree ?= .

objroot ?= $(objtree)/..
srcroot ?= $(srctree)/..

obj := $(objtree)
src := $(srctree)

just_clean := 0
fresh_clean := 0
ifneq ($(filter clean distclean, $(MAKECMDGOALS)),)
        ifeq ($(filter-out clean distclean, $(MAKECMDGOALS)),)
                just_clean := 1
		config_e := $(wildcard $(objtree)/.config)
		ifeq ($(config_e),)
			fresh_clean := 1
		endif
        endif
endif

# Helpers
dot=$(dir $1).$(notdir $1)
add_dot=$(join $(dir $1),$(addprefix .,$(notdir $1)))
var_subst=$(subst /,_,$(subst :,_,$1))

# Namespace variables (push)
_push=$(eval $2$1 := $($1))
_push_v := $(build_files) $(build_paths) _curr
_push_vars=$(foreach v,$(_push_v),$(call _push,$(v),$(call var_subst,$(_MODULE))))
_push_all=$(if $(_MODULE),$(call _push_vars)$(eval _curr := $(_MODULE)))

# Reset local variables
_clear=$(eval $1 :=)
_reset_v := $(foreach v,$(yn-files),$v- $v-y $v-n)
_reset_vars=$(foreach v,$(_reset_v),$(call _clear,$v))

# Set src and obj based on _MODULE
_next_path=$(if $(1:.=),$1/$(notdir $(_MODULE:%/=%)),$(notdir $(_MODULE:%/=%)))
_set_src=$(if $(_MODULE),$(eval src := $(call _next_path,$(src))))
_set_obj=$(if $(_MODULE),$(eval obj := $(call _next_path,$(obj))))

# obj-n := $(obj-n) $(obj-)
_set_var_n=$(eval $(1)-n := $($(1)-n) $($(1)-))
_set_vars_n=$(foreach v,$(yn-vars),$(call _set_var_n,$v))

# Return variable name if the variable is set,
# return nothing otherwise.
# Used to filter variable lists
_null_if_unset=$(if $($1),$1)

# Change the prefix from CFLAGS_ to CFLAGS_ModuleName
_get_mod_cflags=$(subst CFLAGS_,CFLAGS_$(_MODULE),$1)
_prepend_cflags=$(eval $(call _get_mod_cflags,$1):=$($1))
# Get CFLAGS variables for all obj files
_all_cflag_vars=$(addprefix CFLAGS_,$1)
# Filter the CFLAGS variables list for set variables
_valid_cflag_vars=$(strip $(foreach v,$(call _all_cflag_vars,$1),$(call _null_if_unset,$v)))
# Update prefix for each valid CFLAGS_ variable
_prepend_cflags_obj_y=$(foreach v,$(call _valid_cflag_vars,$(obj-y)),$(call _prepend_cflags,$v))

# Clear all the local CFLAGS_ variables
_clear_cflags_obj_y=$(foreach v,$(call _all_cflag_vars,$(obj-y)),$(call _clear,$v))

_top_actions=\
	$(call _push_all) \
	$(call _reset_vars) \
	$(call _set_obj) \
	$(call _set_src)

# Prepend paths, if a path ends in '/', prepend $(src), otherwise $(obj).
# skip paths that start with '/', './', and drive letters
_path=$(eval $1 := \
	$(patsubst %,$(src)/%,$(filter-out %:/% ./% /%,$(filter %/,$($1)))) \
	$(patsubst %,$(obj)/%,$(filter-out %:/% ./% /%,$(filter-out %/,$($1)))) \
	$(filter %:/% ./% /%,$($1)))

_prepend_paths=$(foreach f,$(build_files),$(call _path,$f))

# Add each <name>/ in yn-files to yn-paths
_set_path=$(eval $(1)-$(2) := $(filter %/,$(foreach f,$(yn-files:%=%-$2),$($f))))
_set_paths=$(foreach d,$(yn-paths),$(call _set_path,$d,y)$(call _set_path,$d,n))

# Remove each <name>/ from yn-files
_rm_path=$(eval $1 := $(filter-out %/,$($1)))
_rm_paths=$(foreach f,$(yn-files),$(call _rm_path,$(f)-y)$(call _rm_path,$(f)-n))

# "recurse" into -yn directories
_build=$(eval include $(1)build.mk)
_recurse_inc=$(call _top_actions)$(call _build,$1)$(call _bottom_actions)
_recurse_vars=$(foreach d,$1,$(eval _MODULE:=$d)$(call _recurse_inc,$d))
_recurse=$(foreach d,$(yn-paths:%=%-y),$(call _recurse_vars,$($d)))

# Create directory if objtree != srctree

# Remove namespace from variables and shift into current (pop)
_pop_file=$(eval $1 := $($2$1) $($1))$(eval $2$1 :=)
_pop_path=$(eval $1 := $($2$1))$(eval $2$1 :=)
_pop_files=$(foreach v,$(build_files),$(call _pop_file,$(v),$(call var_subst,$(_curr))))
_pop_paths=$(foreach v,$(build_paths) _curr,$(call _pop_path,$(v),$(call var_subst,$(_curr))))
_pop_all=$(if $(_curr),$(call _pop_files)$(call _pop_paths))

_bottom_actions= \
	$(call _prepend_cflags_obj_y) \
	$(call _clear_cflags_obj_y) \
	$(call _set_vars_n) \
	$(call _prepend_paths) \
	$(call _set_paths) \
	$(call _rm_paths) \
	$(call _recurse) \
	$(call _pop_all) \
	$(eval _MODULE :=)
