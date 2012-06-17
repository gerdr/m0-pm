#include "m0.h"

_Bool m0_disassemble(m0_op op, const m0_constant *constants, FILE *file)
{
	static const char *const NAMES[] = {
		[M0_OP_HALT] = "halt",
		[M0_OP_ADD] = "add",
		[M0_OP_FADD] = "fadd",
	};

	unsigned code = op.code;

	switch(code)
	{
		case M0_OP_HALT:
		fprintf(file, "%s\n", NAMES[code]);
		return 1;

		case M0_OP_ADD:
		case M0_OP_FADD:
		fprintf(file, "%s ", NAMES[code]);
		break;

		default:
		return 0;
	}

	unsigned types = op.argtypes;
	for(int i = 0; i < 3; ++i, types >>= 2)
	{
		switch(types & 3)
		{
			case M0_ARG_IMMEDIATE:
			{
				unsigned id = op.args[i];
				unsigned type = constants[id].type;
				const m0_value value = constants[id].value;

				switch(type)
				{
					case M0_CONST_INT:
					fprintf(file, "%lli", (long long)value.as_int);
					goto NEXT;

					default:
					return 0;
				}
			}

			case M0_ARG_REGISTER:
			fprintf(file, "%%%u", (unsigned)op.args[i]);
			goto NEXT;

			case M0_ARG_SYMBOLIC:
			{
				unsigned id = op.args[i];
				unsigned type = constants[id].type;
				const m0_value value = constants[id].value;

				switch(type)
				{
					case M0_CONST_STRING:
					fprintf(file, "@%s", (const char *)value.as_cptr);
					goto NEXT;

					default:
					return 0;
				}
			}

			default:
			return 0;
		}

	NEXT:
		fputs(i < 2 ? ", " : "\n", file);
	}

	return 1;
}

size_t m0_compile(m0_op op, const m0_constant *constants, m0_value *buffer)
{
	static const void *const OPS[] = {
		[M0_OP_HALT] = m0_core_yield,
		[M0_OP_ADD] = m0_core_add_ia,
		[M0_OP_FADD] = m0_core_add_fa,
	};

	unsigned code = op.code;
	size_t count = 0;

	switch(code)
	{
		case M0_OP_HALT:
		buffer->as_cptr = OPS[code];
		return 1;

		case M0_OP_ADD:
		case M0_OP_FADD:
		// TODO
		return count;

		default:
		return 0;
	}
}
