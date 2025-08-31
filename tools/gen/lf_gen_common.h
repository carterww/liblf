/* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Carter Williams
 */

#ifndef LF_GEN_COMMON_H
#define LF_GEN_COMMON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>

#include "lf_gen_string.h"

static const char *license = "/* SPDX-License-Identifier: MIT\n"
			     " * Copyright (c) 2025 Carter Williams\n"
			     " */\n\n";

static void output(const char *s)
{
	int res = fputs(s, stdout);
	lf_assert(res != EOF);
}

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
	size_t current_width;
	while (i < comment_len) {
		current_width = tab_len;
		for (size_t tab_n = 0; tab_n < tab_count; ++tab_n) {
			string_append_raw(&s, "\t", 1);
		}
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
		string_append_raw(&s, &comment[start], i - start);
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
	}
	string_append_raw(&h, " */\n\n", 0);

	output(h.buffer);
	string_destroy(&h);
}

#endif /* LF_GEN_COMMON_H */
