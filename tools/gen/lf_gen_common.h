/* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Carter Williams
 */

#ifndef LF_GEN_COMMON_H
#define LF_GEN_COMMON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>

#include "lf_gen_functions.h"
#include "lf_gen_string.h"
#include "lf_gen_types.h"

#define ASM "__asm__ __volatile__"
#define ASM_LEN (sizeof(ASM) - 1)

static const char *license = "/* SPDX-License-Identifier: MIT\n"
			     " * Copyright (c) 2025 Carter Williams\n"
			     " */\n\n";

static void output(const char *s)
{
	int res = fputs(s, stdout);
	lf_assert(res != EOF);
}

#define APPEND_TABS(s_ptr, tab_count)                             \
	do {                                                      \
		for (size_t xyab = 0; xyab < tab_count; ++xyab) { \
			string_append_raw(s_ptr, "\t", 1);        \
		}                                                 \
	} while (0)

/* I know this function is crappy. I just wanted something quick and dirty */
static string lf_gen_format_comment(const char *comment, size_t tab_count,
				    size_t tab_width, bool inline_comment,
				    bool in_middle)
{
	size_t line_length_max = 80;
	string s;
	size_t comment_len = strlen(comment);
	size_t tab_len = tab_count * tab_width;
	string_init(&s, comment_len + 64);

	if (tab_len + 3 >= line_length_max - 40) {
		line_length_max += tab_len + 3;
	}

	size_t last_sp = 0;
	size_t i = 0;
	size_t start = 0;
	size_t current_width = tab_len;
	while (i < comment_len) {
		current_width = tab_len;
		APPEND_TABS(&s, tab_count);
		if (inline_comment) {
			string_append_raw(&s, "// ", 3);
		} else {
			if (i == 0 && !in_middle) {
				string_append_raw(&s, "/* ", 3);
			} else {
				string_append_raw(&s, " * ", 3);
			}
		}
		current_width += 3;
		for (; i < comment_len; ++i) {
			switch (comment[i]) {
			case '\t':
				current_width += tab_width;
				continue;
			case ' ':
				current_width += 1;
				last_sp = i;
				break;
			case '\n':
				goto flush_line;
			case '\r':
				continue;
			default:
				current_width += 1;
				continue;
			}
			if (current_width > line_length_max) {
				if (last_sp != 0) {
					i = last_sp;
				}
				goto flush_line;
			}
		}
		break;
flush_line:
		if (i != start) {
			string_append_raw(&s, &comment[start], i - start);
		}
		string_append_raw(&s, "\n", 1);
		i += 1;
		start = i;
		last_sp = 0;
	}

	if (start != i) {
		if (current_width > line_length_max && last_sp != 0) {
			string_append_raw(&s, &comment[start], last_sp - start);
			string_append_raw(&s, "\n", 1);
			start = last_sp + 1;
			if (inline_comment) {
				string_append_raw(&s, "// ", 3);
			} else {
				string_append_raw(&s, " * ", 3);
			}
		}
		string_append_raw(&s, &comment[start], i - start);
		string_append_raw(&s, "\n", 1);
	}

	if (!in_middle) {
		string_append_raw(&s, " */\n", 4);
	}

	return s;
}

static void lf_gen_module_comment(const char *comment)
{
	string s = lf_gen_format_comment(comment, 0, 8, false, false);
	output(s.buffer);
	output("\n");
	string_destroy(&s);
}

static void lf_gen_header_guard(const char *name, bool start)
{
	if (start) {
		output("#ifndef ");
		output(name);
		output("\n#define ");
		output(name);
		output("\n\n");
	} else {
		output("#endif /* ");
		output(name);
		output(" */\n");
	}
}

static void lf_gen_header_output(const char *generator_name,
				 const char *extra_comment)
{
	string h;
	struct tm tm;
	time_t t = time(NULL);
	localtime_r(&t, &tm);
	char timestamp[64];
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &tm);

	string_init(&h, 1024);
	string_append_raw(&h, license, 0);
	string_append_raw(
		&h,
		"/* This file was automatically generated. DO NOT MODIFY IT DIRECTLY.\n",
		0);
	string_append_raw(&h, " *\n", 0);
	string_append_raw(&h, " * Date:      ", 0);
	string_append_raw(&h, timestamp, 0);
	string_append_raw(&h, "\n", 0);
	string_append_raw(&h, " * Generator: ", 0);
	string_append_raw(&h, generator_name, 0);
	string_append_raw(&h, "\n", 0);
	string_append_raw(&h, " * Version:   v", 0);
	string_append_raw(&h, LIBLF_VERSION_STR, 0);
	string_append_raw(&h, "\n", 0);
	string_append_raw(&h, " *\n", 0);
	string_append_raw(
		&h,
		" * Any changes made to this file will be overwritten when the generator runs\n",
		0);
	string_append_raw(&h, " * again.\n", 0);
	if (extra_comment != NULL) {
		string_append_raw(&h, " *\n", 0);
		string comment =
			lf_gen_format_comment(extra_comment, 0, 8, false, true);
		string_append(&h, &comment);
		string_destroy(&comment);
	}
	string_append_raw(&h, " */\n\n", 0);

	output(h.buffer);
	string_destroy(&h);
}

static void lf_gen_all_same_impl(const char *impl,
				 string (*func)(enum lf_gen_type, const char *))
{
	enum lf_gen_type type;
	types_for_each(type) {
		string s = func(type, impl);
		output(s.buffer);
		string_destroy(&s);
	}
}

static void lf_gen_integral_same_impl(
	enum lf_gen_func_category cat, const char *impl,
	string (*func)(enum lf_gen_type, enum lf_gen_func_category cat,
		       const char *))
{
	enum lf_gen_type type;
	types_integral_for_each(type) {
		string s = func(type, cat, impl);
		output(s.buffer);
		string_destroy(&s);
	}
}

static void lf_gen_all_impl_from_func(string (*generate_impl)(enum lf_gen_type),
				      string (*define)(enum lf_gen_type,
						       const char *))
{
	enum lf_gen_type type;
	types_for_each(type) {
		string impl = generate_impl(type);
		string s = define(type, impl.buffer);
		output(s.buffer);
		string_destroy(&s);
		string_destroy(&impl);
	}
}

static void lf_gen_integral_impl_from_func(
	enum lf_gen_func_category cat,
	string (*generate_impl)(enum lf_gen_type, enum lf_gen_func_category),
	string (*define)(enum lf_gen_type, enum lf_gen_func_category,
			 const char *))
{
	enum lf_gen_type type;
	types_integral_for_each(type) {
		string impl = generate_impl(type, cat);
		string s = define(type, cat, impl.buffer);
		output(s.buffer);
		string_destroy(&s);
		string_destroy(&impl);
	}
}

static void lf_gen_declare_var_str_type(string *s, const char *type_name,
					const char *name, size_t tab_count)
{
	APPEND_TABS(s, tab_count);
	size_t type_name_len = strlen(type_name);
	string_append_raw(s, type_name, 0);
	if (type_name[type_name_len - 1] != '*') {
		string_append_raw(s, " ", 1);
	}
	string_append_raw(s, name, 0);
	string_append_raw(s, ";\n", 2);
}

static void lf_gen_declare_var(string *s, enum lf_gen_type type,
			       const char *name, size_t tab_count)
{
	APPEND_TABS(s, tab_count);
	const char *type_name = lf_gen_type_names[type];
	size_t type_name_len = strlen(type_name);
	string_append_raw(s, type_name, 0);
	if (type_name[type_name_len - 1] != '*') {
		string_append_raw(s, " ", 1);
	}
	string_append_raw(s, name, 0);
	string_append_raw(s, ";\n", 2);
}

static void lf_gen_declare_and_set_var(string *s, enum lf_gen_type type,
				       const char *name, const char *value,
				       size_t tab_count)
{
	APPEND_TABS(s, tab_count);
	const char *type_name = lf_gen_type_names[type];
	size_t type_name_len = strlen(type_name);
	string_append_raw(s, type_name, 0);
	if (type_name[type_name_len - 1] != '*') {
		string_append_raw(s, " ", 1);
	}
	string_append_raw(s, name, 0);
	string_append_raw(s, " = ", 3);
	string_append_raw(s, value, 0);
	string_append_raw(s, ";\n", 2);
}

static void lf_gen_set_var(string *s, const char *name, const char *value,
			   size_t tab_count)
{
	APPEND_TABS(s, tab_count);
	string_append_raw(s, name, 0);
	string_append_raw(s, " = ", 3);
	string_append_raw(s, value, 0);
	string_append_raw(s, ";\n", 2);
}

static void lf_gen_return(string *s, const char *name, size_t tab_count)
{
	APPEND_TABS(s, tab_count);
	string_append_raw(s, "return ", 7);
	string_append_raw(s, name, 0);
	string_append_raw(s, ";", 1);
}

static void lf_gen_output_op_sup_macro(enum lf_gen_type type,
				       enum lf_gen_func_category func,
				       bool supported)
{
	string s;
	string_init(&s, 64);
	const char *namespace = lf_gen_func_category_namespaces[func];
	const char *type_alias = lf_gen_type_alias[type];
	string_append_raw(&s, "#define ", 8);
	for (size_t i = 0; i < strlen(namespace); ++i) {
		char c = char_to_upper(namespace[i]);
		string_append_char(&s, c);
	}
	for (size_t i = 0; i < strlen(type_alias); ++i) {
		char c = char_to_upper(type_alias[i]);
		string_append_char(&s, c);
	}
	if (supported) {
		string_append_raw(&s, "_SUP 1\n", 7);
	} else {
		string_append_raw(&s, "_SUP 0\n", 7);
	}
	output(s.buffer);
	string_destroy(&s);
}

#endif /* LF_GEN_COMMON_H */
