/* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Carter Williams
 */

#ifndef LF_GEN_FUNCTIONS_H
#define LF_GEN_FUNCTIONS_H

#include <stddef.h>

#include "lf_gen_string.h"
#include "lf_gen_types.h"

static const char *func_prefix = "LF_ATTR_ALWAYS_INLINE\nstatic ";

#define funcs_faops_for_each(func_enum_var)     \
	for (func_enum_var = LF_GEN_FUNC_FAINC; \
	     func_enum_var <= LF_GEN_FUNC_FAXOR; ++func_enum_var)

#define funcs_ops_for_each(func_enum_var)     \
	for (func_enum_var = LF_GEN_FUNC_INC; \
	     func_enum_var <= LF_GEN_FUNC_XOR; ++func_enum_var)

enum lf_gen_func_category {
	/* Don't reorder these */
	LF_GEN_FUNC_FENCE = 0,
	LF_GEN_FUNC_LOAD,
	LF_GEN_FUNC_STORE,
	LF_GEN_FUNC_SWAP,
	LF_GEN_FUNC_CAS,
	LF_GEN_FUNC_CASX,
	LF_GEN_FUNC_INC,
	LF_GEN_FUNC_DEC,
	LF_GEN_FUNC_ADD,
	LF_GEN_FUNC_SUB,
	LF_GEN_FUNC_AND,
	LF_GEN_FUNC_OR,
	LF_GEN_FUNC_XOR,
	LF_GEN_FUNC_FAINC,
	LF_GEN_FUNC_FADEC,
	LF_GEN_FUNC_FAADD,
	LF_GEN_FUNC_FASUB,
	LF_GEN_FUNC_FAAND,
	LF_GEN_FUNC_FAOR,
	LF_GEN_FUNC_FAXOR,
	LF_GEN_FUNC_BTS,
	LF_GEN_FUNC_BTC,
	LF_GEN_FUNC_BTR,
	LF_GEN_FUNC_COUNT,
};

static const char *lf_gen_func_category_namespaces[] = {
	[LF_GEN_FUNC_FENCE] = "lf_fence_",
	[LF_GEN_FUNC_LOAD] = "lf_op_load_",
	[LF_GEN_FUNC_STORE] = "lf_op_store_",
	[LF_GEN_FUNC_SWAP] = "lf_op_swap_",
	[LF_GEN_FUNC_CAS] = "lf_op_cas_",
	[LF_GEN_FUNC_CASX] = "lf_op_casx_",
	[LF_GEN_FUNC_INC] = "lf_op_inc_",
	[LF_GEN_FUNC_DEC] = "lf_op_dec_",
	[LF_GEN_FUNC_ADD] = "lf_op_add_",
	[LF_GEN_FUNC_SUB] = "lf_op_sub_",
	[LF_GEN_FUNC_AND] = "lf_op_and_",
	[LF_GEN_FUNC_OR] = "lf_op_or_",
	[LF_GEN_FUNC_XOR] = "lf_op_xor_",
	[LF_GEN_FUNC_FAINC] = "lf_op_fainc_",
	[LF_GEN_FUNC_FADEC] = "lf_op_fadec_",
	[LF_GEN_FUNC_FAADD] = "lf_op_faadd_",
	[LF_GEN_FUNC_FASUB] = "lf_op_fasub_",
	[LF_GEN_FUNC_FAAND] = "lf_op_faand_",
	[LF_GEN_FUNC_FAOR] = "lf_op_faor_",
	[LF_GEN_FUNC_FAXOR] = "lf_op_faxor_",
	[LF_GEN_FUNC_BTS] = "lf_op_bts_",
	[LF_GEN_FUNC_BTC] = "lf_op_btc_",
	[LF_GEN_FUNC_BTR] = "lf_op_btr_",
};

#define fence_all_for_each(fence_enum_var)          \
	for (fence_enum_var = LF_GEN_FUNC_FENCE_CC; \
	     fence_enum_var < LF_GEN_FUNC_FENCE_COUNT; ++fence_enum_var)

#define fence_force_for_each(fence_enum_var)        \
	for (fence_enum_var = LF_GEN_FUNC_FENCE_CC; \
	     fence_enum_var <= LF_GEN_FUNC_FENCE_LOAD_FORCE; ++fence_enum_var)

#define fence_other_for_each(fence_enum_var)          \
	for (fence_enum_var = LF_GEN_FUNC_FENCE_FULL; \
	     fence_enum_var < LF_GEN_FUNC_FENCE_COUNT; ++fence_enum_var)

enum lf_gen_func_fence {
	/* Don't reorder these first 4 */
	LF_GEN_FUNC_FENCE_CC = 0,
	LF_GEN_FUNC_FENCE_FULL_FORCE,
	LF_GEN_FUNC_FENCE_STORE_FORCE,
	LF_GEN_FUNC_FENCE_LOAD_FORCE,

	LF_GEN_FUNC_FENCE_FULL,
	LF_GEN_FUNC_FENCE_STORE,
	LF_GEN_FUNC_FENCE_LOAD,
	LF_GEN_FUNC_FENCE_STORE_LOAD,
	LF_GEN_FUNC_FENCE_LOAD_STORE,
	LF_GEN_FUNC_FENCE_ATOMIC,
	LF_GEN_FUNC_FENCE_ATOMIC_LOAD,
	LF_GEN_FUNC_FENCE_LOAD_ATOMIC,
	LF_GEN_FUNC_FENCE_ATOMIC_STORE,
	LF_GEN_FUNC_FENCE_STORE_ATOMIC,
	LF_GEN_FUNC_FENCE_COUNT,
};

static const char *lf_gen_func_fence_names[] = {
	[LF_GEN_FUNC_FENCE_CC] = "cc",
	[LF_GEN_FUNC_FENCE_FULL_FORCE] = "full_force",
	[LF_GEN_FUNC_FENCE_STORE_FORCE] = "store_force",
	[LF_GEN_FUNC_FENCE_LOAD_FORCE] = "load_force",
	[LF_GEN_FUNC_FENCE_FULL] = "full",
	[LF_GEN_FUNC_FENCE_STORE] = "store",
	[LF_GEN_FUNC_FENCE_LOAD] = "load",
	[LF_GEN_FUNC_FENCE_STORE_LOAD] = "store_load",
	[LF_GEN_FUNC_FENCE_LOAD_STORE] = "load_store",
	[LF_GEN_FUNC_FENCE_ATOMIC] = "atomic",
	[LF_GEN_FUNC_FENCE_ATOMIC_LOAD] = "atomic_load",
	[LF_GEN_FUNC_FENCE_LOAD_ATOMIC] = "load_atomic",
	[LF_GEN_FUNC_FENCE_ATOMIC_STORE] = "atomic_store",
	[LF_GEN_FUNC_FENCE_STORE_ATOMIC] = "store_atomic"
};

static string lf_gen_func_define(const char *return_type, const char *namespace,
				 const char *name, const char *param_types[],
				 const char *param_names[], size_t nparams,
				 const char *body)
{
	string s;
	size_t size, prlen, rlen, nslen, nlen, plen, blen;

	prlen = strlen(func_prefix);
	rlen = strlen(return_type);
	nslen = strlen(namespace);
	nlen = strlen(name);
	plen = 0;
	for (size_t i = 0; i < nparams; ++i) {
		plen += strlen(param_types[i]);
		plen += strlen(param_names[i]);
		/* Space between type and name (not always there but add anyway) */
		plen += 1;
	}
	/* Comma and space between params */
	if (nparams > 0) {
		plen += 2 * (nparams - 1);
	} else {
		plen += 4;
	}
	blen = strlen(body);

	lf_assert(plen > 0);
	lf_assert(rlen > 0);
	lf_assert(nslen > 0);
	lf_assert(nlen > 0);
	lf_assert(blen > 0);
	size = prlen + rlen + nslen + nlen + plen + blen + 16;
	string_init(&s, size);
	string_append_raw(&s, func_prefix, prlen);
	string_append_raw(&s, return_type, rlen);
	if (return_type[rlen - 1] != '*') {
		string_append_raw(&s, " ", 1);
	}
	string_append_raw(&s, namespace, nslen);
	string_append_raw(&s, name, nlen);
	string_append_raw(&s, "(", 1);

	if (nparams == 0) {
		string_append_raw(&s, "void", 4);
	} else {
		size_t i;
		for (i = 0; i < nparams - 1; ++i) {
			size_t atlen = strlen(param_types[i]);
			lf_assert(atlen > 0);
			string_append_raw(&s, param_types[i], atlen);
			if (param_types[i][atlen - 1] != '*') {
				string_append_raw(&s, " ", 1);
			}
			string_append_raw(&s, param_names[i], 0);
			string_append_raw(&s, ", ", 2);
		}
		size_t atlen = strlen(param_types[i]);
		lf_assert(atlen > 0);
		string_append_raw(&s, param_types[i], atlen);
		if (param_types[i][atlen - 1] != '*') {
			string_append_raw(&s, " ", 1);
		}
		string_append_raw(&s, param_names[i], 0);
	}

	string_append_raw(&s, ")\n{\n", 4);
	string_append_raw(&s, body, blen);
	string_append_raw(&s, "\n}\n\n", 4);
	return s;
}

#define TYPE_NAME_VARS(type)                                     \
	const char *type_name = lf_gen_type_names[type];         \
	const char *type_name_ptr = lf_gen_type_name_ptrs[type]; \
	const char *type_alias = lf_gen_type_alias[type]

#define NAMESPACE_VAR(cat) \
	const char *namespace = lf_gen_func_category_namespaces[cat]

static string lf_gen_func_fence_define(enum lf_gen_func_fence func,
				       const char *impl)
{
	NAMESPACE_VAR(LF_GEN_FUNC_FENCE);
	return lf_gen_func_define("void", namespace,
				  lf_gen_func_fence_names[func], NULL, NULL, 0,
				  impl);
}

static string lf_gen_func_load_define(enum lf_gen_type type, const char *impl)
{
	static const char *param_name[] = { "p" };
	TYPE_NAME_VARS(type);
	NAMESPACE_VAR(LF_GEN_FUNC_LOAD);
	return lf_gen_func_define(type_name, namespace, type_alias,
				  &lf_gen_type_name_ptrs[type], param_name, 1,
				  impl);
}

static string lf_gen_func_store_define(enum lf_gen_type type, const char *impl)
{
	static const char *param_names[2] = { "p", "val" };
	const char *param_types[2] = { NULL, NULL };
	TYPE_NAME_VARS(type);
	NAMESPACE_VAR(LF_GEN_FUNC_STORE);
	param_types[0] = lf_gen_type_name_ptrs[type];
	param_types[1] = lf_gen_type_names[type];

	return lf_gen_func_define("void", namespace, type_alias, param_types,
				  param_names, 2, impl);
}

static string lf_gen_func_swap_define(enum lf_gen_type type, const char *impl)
{
	static const char *param_names[2] = { "p", "val" };
	const char *param_types[2] = { NULL, NULL };
	TYPE_NAME_VARS(type);
	NAMESPACE_VAR(LF_GEN_FUNC_SWAP);
	param_types[0] = type_name_ptr;
	param_types[1] = type_name;

	return lf_gen_func_define(type_name, namespace, type_alias, param_types,
				  param_names, 2, impl);
}

static string lf_gen_func_cas_define_helper(enum lf_gen_type type,
					    const char *impl, bool isx)
{
	static const char *param_names[3] = { "p", "val_old", "val_new" };
	const char *param_types[3] = { NULL, NULL, NULL };
	TYPE_NAME_VARS(type);
	param_types[0] = type_name_ptr;
	param_types[1] = type_name;
	param_types[2] = type_name;

	const char *return_value = isx ? type_name : "bool";
	const char *namespace =
		isx ? lf_gen_func_category_namespaces[LF_GEN_FUNC_CASX] :
		      lf_gen_func_category_namespaces[LF_GEN_FUNC_CAS];

	return lf_gen_func_define(return_value, namespace, type_alias,
				  param_types, param_names, 3, impl);
}

static string lf_gen_func_cas_define(enum lf_gen_type type, const char *impl)
{
	return lf_gen_func_cas_define_helper(type, impl, false);
}

static string lf_gen_func_casx_define(enum lf_gen_type type, const char *impl)
{
	return lf_gen_func_cas_define_helper(type, impl, true);
}

static string lf_gen_func_fainc_fadec_define(enum lf_gen_type type,
					     enum lf_gen_func_category cat,
					     const char *impl)
{
	static const char *param_names[1] = { "p" };
	const char *param_types[1] = { NULL };
	TYPE_NAME_VARS(type);
	NAMESPACE_VAR(cat);
	param_types[0] = type_name_ptr;
	return lf_gen_func_define(type_name, namespace, type_alias, param_types,
				  param_names, 1, impl);
}

static string lf_gen_func_faop_define(enum lf_gen_type type,
				      enum lf_gen_func_category cat,
				      const char *impl)
{
	static const char *param_names[2] = { "p", "val" };
	const char *param_types[2] = { NULL, NULL };
	TYPE_NAME_VARS(type);
	NAMESPACE_VAR(cat);
	param_types[0] = type_name_ptr;
	param_types[1] = type_name;
	return lf_gen_func_define(type_name, namespace, type_alias, param_types,
				  param_names, 2, impl);
}

static string lf_gen_func_inc_dec_define(enum lf_gen_type type,
					 enum lf_gen_func_category cat,
					 const char *impl)
{
	static const char *param_names[1] = { "p" };
	const char *param_types[1] = { NULL };
	TYPE_NAME_VARS(type);
	NAMESPACE_VAR(cat);
	param_types[0] = type_name_ptr;

	return lf_gen_func_define("void", namespace, type_alias, param_types,
				  param_names, 1, impl);
}

static string lf_gen_func_op_define(enum lf_gen_type type,
				    enum lf_gen_func_category cat,
				    const char *impl)
{
	static const char *param_names[2] = { "p", "val" };
	const char *param_types[2] = { NULL, NULL };
	TYPE_NAME_VARS(type);
	NAMESPACE_VAR(cat);
	param_types[0] = type_name_ptr;
	param_types[1] = type_name;
	return lf_gen_func_define("void", namespace, type_alias, param_types,
				  param_names, 2, impl);
}

static string lf_gen_func_bitop_define(enum lf_gen_type type,
				       enum lf_gen_func_category cat,
				       const char *impl)
{
	static const char *param_names[2] = { "p", "index" };
	const char *param_types[2] = { NULL, "unsigned int" };
	TYPE_NAME_VARS(type);
	NAMESPACE_VAR(cat);
	param_types[0] = type_name_ptr;
	return lf_gen_func_define("bool", namespace, type_alias, param_types,
				  param_names, 2, impl);
}

#endif /* LF_GEN_FUNCTIONS_H */
