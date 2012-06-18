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
		goto BINOP;

		default:
		return 0;
	}

BINOP:
	fprintf(file, "%s ", NAMES[code]);

	unsigned types = op.argtypes;
	switch(types & 3)
	{
		case M0_ARG_REGISTER:
		fprintf(file, "%%%u, ", (unsigned)op.args[0]);
		break;

		default:
		return 0;
	}

	for(int i = 1; i < 3; ++i)
	{
		types >>= 2;
		switch(types & 3)
		{
			case M0_ARG_IMMEDIATE:
			{
				unsigned id = op.args[i];
				const m0_value value = constants[id].value;

				switch(constants[id].type)
				{
					case M0_CONST_INT:
					fprintf(file, "%lli", (long long)value.as_int);
					goto NEXT;

					case M0_CONST_FLOAT:
					fprintf(file, "%f", (double)value.as_float);
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
				const m0_value value = constants[id].value;

				switch(constants[id].type)
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
	enum { UNKNOWN, INT, FLOAT };

	static const void *const OPS[] = {
		[M0_OP_HALT] = m0_core_yield,
		[M0_OP_ADD] = m0_core_add_ia,
		[M0_OP_FADD] = m0_core_add_fa,
	};

	unsigned code = op.code;
	unsigned types = op.argtypes;
	unsigned mode = UNKNOWN;
	size_t count = 0;

	switch(code)
	{
		case M0_OP_HALT:
		buffer->as_cptr = OPS[code];
		return 1;

		case M0_OP_ADD:
		mode = INT;
		goto BINOP;

		case M0_OP_FADD:
		mode = FLOAT;
		goto BINOP;

		default:
		return 0;
	}

BINOP:
	switch(types >> 2 & 3)
	{
		case M0_ARG_IMMEDIATE:
		switch(mode)
		{
			case INT:
			{
				unsigned id = op.args[1];
				if(constants[id].type != M0_CONST_INT)
					return 0;

				buffer[count++].as_cptr = m0_core_set_ia;
				buffer[count++].as_uint = constants[id].value.as_uint;
				break;
			}

			default:
			return 0;
		}
		break;

		default:
		return 0;
	}

	switch(types >> 4 & 3)
	{
		case M0_ARG_IMMEDIATE:
		switch(mode)
		{
			case INT:
			{
				unsigned id = op.args[2];
				if(constants[id].type != M0_CONST_INT)
					return 0;

				buffer[count++].as_cptr = m0_core_set_ib;
				buffer[count++].as_uint = constants[id].value.as_uint;
				break;
			}

			default:
			return 0;
		}
		break;

		default:
		return 0;
	}

	buffer[count++].as_cptr = OPS[code];

	switch(types & 3)
	{
		case M0_ARG_REGISTER:
		switch(mode)
		{
			case INT:
			buffer[count++].as_cptr = m0_core_put_ia;
			buffer[count++].as_uword = op.args[0];
			break;

			default:
			return 0;
		}
		break;

		default:
		return 0;
	}

	return count;
}
