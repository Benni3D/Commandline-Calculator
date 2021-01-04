#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "evaluater.h"

static value_t builtin_sin(const value_t* args, size_t num) {
	if (num < 1) return make_value(VALUE_INVALID);
	const value_t val = value_cast(args[0], VALUE_FLOAT);
	return val.type == VALUE_INVALID ? val : make_float_value(sin(val.fVal));
}
static value_t builtin_cos(const value_t* args, size_t num) {
	if (num < 1) return make_value(VALUE_INVALID);
	const value_t val = value_cast(args[0], VALUE_FLOAT);
	return val.type == VALUE_INVALID ? val : make_float_value(cos(val.fVal));
}
static value_t builtin_tan(const value_t* args, size_t num) {
	if (num < 1) return make_value(VALUE_INVALID);
	const value_t val = value_cast(args[0], VALUE_FLOAT);
	return val.type == VALUE_INVALID ? val : make_float_value(tan(val.fVal));
}

static value_t builtin_log(const value_t* args, size_t num) {
	if (num < 1) return make_value(VALUE_INVALID);
	const value_t val = value_cast(args[0], VALUE_FLOAT);
	return val.type == VALUE_INVALID ? val : make_float_value(log(val.fVal));
}
static value_t builtin_log10(const value_t* args, size_t num) {
	if (num < 1) return make_value(VALUE_INVALID);
	const value_t val = value_cast(args[0], VALUE_FLOAT);
	return val.type == VALUE_INVALID ? val : make_float_value(log10(val.fVal));
}
static value_t builtin_log2(const value_t* args, size_t num) {
	if (num < 1) return make_value(VALUE_INVALID);
	const value_t val = value_cast(args[0], VALUE_FLOAT);
	return val.type == VALUE_INVALID ? val : make_float_value(log2(val.fVal));
}

static value_t builtin_exit(const value_t* args, size_t num) {
	int ec = 0;
	if (num >= 1) {
		const value_t val = value_cast(args[0], VALUE_INTEGER);
		if (val.type == VALUE_INVALID) return val;
		else ec = val.iVal;
	}
	exit(ec);
	return make_value(VALUE_EMPTY);
}
static value_t builtin_typestr(const value_t* args, size_t num) {
	if (num < 0) return make_value(VALUE_INVALID);
	switch (args->type) {
	case VALUE_INVALID: return make_string_value("invalid");
	case VALUE_STRING:  return make_string_value("string");
	case VALUE_FLOAT:   return make_string_value("float");
	case VALUE_INTEGER: return make_string_value("integer");
	case VALUE_EMPTY:   return make_string_value("empty");
	default:            return make_string_value("error");
	}
}

#define ch1(t, a, b) (((a.type) == (t)) || ((b.type) == (t)))
static value_t builtin_equals(const value_t* args, size_t num) {
	if (num != 2) return make_value(VALUE_INVALID);
	if (ch1(VALUE_INVALID, args[0], args[1]))
		return make_integer_value(0);
	else if (ch1(VALUE_EMPTY, args[0], args[1]))
		return make_integer_value(args[0].type == args[1].type);
	else if (ch1(VALUE_STRING, args[0], args[1]))
		return make_integer_value(args[0].type == args[1].type ? strcmp(args[0].str, args[1].str) == 0 : 0);
	else if (ch1(VALUE_FLOAT, args[0], args[1])) {
		if (ch1(VALUE_INTEGER, args[0], args[1]))
			return make_integer_value(value_cast(args[0], VALUE_FLOAT).fVal == value_cast(args[1], VALUE_FLOAT).fVal);
		else if (args[0].type == args[1].type)
			return make_integer_value(args[0].fVal == args[1].fVal);
		else return make_value(VALUE_INVALID);
	}
	else if (args[0].type == args[1].type && args[0].type == VALUE_INTEGER)
		return make_integer_value(args[0].iVal == args[1].iVal);
	else return make_value(VALUE_INVALID);
}
static value_t builtin_not(const value_t* args, size_t num) {
	if (num != 1) return make_value(VALUE_INVALID);
	return make_integer_value(args[0].type == VALUE_INTEGER ? !args[0].iVal : 0);
}
#define ch2(t, a, b) ((a).type == (b).type && (a).type == (t))
static value_t builtin_and(const value_t* args, size_t num) {
	if (num != 2) return make_value(VALUE_INVALID);
	else return make_integer_value(ch2(VALUE_INTEGER, args[0], args[1]) ? (args[0].iVal && args[1].iVal) : 0);
}
static value_t builtin_or(const value_t* args, size_t num) {
	if (num != 2) return make_value(VALUE_INVALID);
	else return make_integer_value(ch2(VALUE_INTEGER, args[0], args[1]) ? (args[0].iVal || args[1].iVal) : 0);
}
static value_t builtin_print(const value_t* args, size_t num) {
	for (size_t i = 0; i < num; ++i) {
		print_value(args[i], stdout);
	}
	putchar('\n');
	return make_value(VALUE_EMPTY);
}


#define add_builtin(name) evaluation_context_add_func(ctx, #name, builtin_##name)
void evaluation_context_add_builtins(evaluation_context_t* ctx) {
	add_builtin(sin);
	add_builtin(cos);
	add_builtin(tan);
	add_builtin(log);
	add_builtin(log10);
	add_builtin(log2);
	add_builtin(exit);
	add_builtin(typestr);
	add_builtin(equals);
	add_builtin(and);
	add_builtin(or);
	add_builtin(print);
}
