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

	for(unsigned i = 0; i < 3; ++i)
	{
		switch(m0_argtype(op, i))
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
				if(!constants[id].type == M0_CONST_STRING)
					return 0;

				fprintf(file, "@%s", constants[id].value.as_cptr);
				goto NEXT;
			}

			case M0_ARG_MEMORY:
			{
				static const char *const TYPES[] = {
					[M0_MEM_I32] = "i32"
				};

				unsigned id = op.args[i];
				if(!constants[id].type == M0_CONST_ADDRESS)
					return 0;

				const m0_address *address = constants[id].value.as_cptr;
				_Bool is_symbolic = address->base_symbol;
				_Bool is_displaced = address->displacement;
				_Bool has_offset = address->offset_multiplier;

				fprintf(file, "%s(", TYPES[address->type]);

				if(is_symbolic)
					fprintf(file, "@%s", address->base_symbol);
				else
					fprintf(file, "%%%u", (unsigned)address->base_register);

				if(is_displaced || has_offset)
					fprintf(file, ":");

				if(has_offset)
				{
					fprintf(file, "%%%u", (unsigned)address->offset_register);
					if(address->offset_multiplier != 1)
						fprintf(file, "*%li", (long)address->offset_multiplier);
				}

				if(is_displaced)
				{
					fprintf(file, "%s%li", has_offset ?
						(address->displacement < 0 ? "-" : "+") : "",
						(long)address->displacement);
				}

				fprintf(file, ")");

				goto NEXT;
			}

			default:
			return 0;
		}

	NEXT:
		fputs(i < 2 ? ", " : "\n", file);
	}

	return 1;
}
