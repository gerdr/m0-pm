#ifndef M0_H_
#define M0_H_

#include <stdint.h>
#include <stdio.h>

#define m0_op_argtypes_(A1, A2, A3) \
	(M0_ARG_ ## A1 | M0_ARG_ ## A2 << 2 | M0_ARG_ ## A3 << 4)

#define m0_op_(NAME, A1, A2, A3, ...) \
	((m0_op){ M0_OP_ ## NAME, m0_op_argtypes_(A1, A2, A3), { __VA_ARGS__ } })

#define m0_op_rii(NAME, ...) \
	m0_op_(NAME, REGISTER, IMMEDIATE, IMMEDIATE, __VA_ARGS__)

#define m0_op_rrr(NAME, ...) \
	m0_op_(NAME, REGISTER, REGISTER, REGISTER, __VA_ARGS__)

enum
{
	M0_OP_HALT,
	M0_OP_ADD,
	M0_OP_FADD
};

enum
{
	M0_ARG_IMMEDIATE,
	M0_ARG_REGISTER,
	M0_ARG_SYMBOLIC,
	M0_ARG_MEMORY
};

enum
{
	M0_CONST_INT,
	M0_CONST_STRING
};

typedef int64_t m0_int;
typedef uint64_t m0_uint;
typedef ptrdiff_t m0_word;
typedef size_t m0_uword;
typedef double m0_float;

typedef union m0_value_ m0_value;
typedef struct m0_op_ m0_op;
typedef struct m0_constant_ m0_constant;
typedef struct m0_vm_ m0_vm;

union m0_value_
{
	m0_int as_int;
	m0_uint as_uint;
	m0_word as_word;
	m0_uword as_uword;
	m0_float as_float;
	void *as_ptr;
	const void *as_cptr;
};

struct m0_op_
{
	uint8_t code;
	uint8_t argtypes;
	uint16_t args[3];
};

struct m0_constant_
{
	unsigned type;
	m0_value value;
};

struct m0_vm_
{
	m0_value *rp;
	const m0_value *ip;
};

extern const char m0_core_yield[];
extern const char m0_core_set_ia[];
extern const char m0_core_set_ib[];
extern const char m0_core_put_ia[];
extern const char m0_core_add_ia[];
extern const char m0_core_add_fa[];

extern void m0_core(m0_vm *restrict vm);

extern _Bool m0_disassemble(
	m0_op op, const m0_constant *constants, FILE *file);

extern size_t m0_compile(
	m0_op op, const m0_constant *constants, m0_value *buffer);

#endif