#include "m0.h"

#define say(...) do if(fprintf(file, __VA_ARGS__) < 0) return 0; while(0)

static const char *const NAMES[] = {
	[M0_OP_HALT] = "halt",
	[M0_OP_ADD] = "add",
	[M0_OP_MUL] = "mul",
	[M0_OP_FADD] = "fadd",
};

_Bool m0_disassemble(m0_op op, const m0_constant *constants, FILE *file)
{
	unsigned code = op.code;
	unsigned arg_count;

	switch(code)
	{
		case M0_OP_HALT:
		arg_count = 0;
		break;

		case M0_OP_ADD:
		case M0_OP_MUL:
		case M0_OP_FADD:
		arg_count = 3;
		break;

		default:
		return 0;
	}

	say("%s ", NAMES[code]);

	for(unsigned i = 0; i < arg_count; ++i)
	{
		if(i > 0) fputs(", ", file);

		switch(m0_argtype(op, i))
		{
			case M0_ARG_IMMEDIATE:
			goto IMMEDIATE_ARG;

			case M0_ARG_REGISTER:
			goto REGISTER_ARG;

			case M0_ARG_SYMBOLIC:
			goto SYMBOLIC_ARG;

			case M0_ARG_MEMORY:
			goto MEMORY_ARG;

			default:
			return 0;
		}

	IMMEDIATE_ARG:
		{
			unsigned id = op.args[i];
			const m0_value value = constants[id].value;

			switch(constants[id].type)
			{
				case M0_CONST_INT:
				say("%lli", (long long)value.as_int);
				continue;

				case M0_CONST_FLOAT:
				say("%f", (double)value.as_float);
				continue;

				default:
				return 0;
			}
		}

	REGISTER_ARG:
		{
			say("%%%u", (unsigned)op.args[i]);
			continue;
		}

	SYMBOLIC_ARG:
		{
			unsigned id = op.args[i];
			if(!constants[id].type == M0_CONST_SYMBOL)
				return 0;

			const m0_symbol *symbol = constants[id].value.as_cptr;
			say("@%s", symbol->name);
			continue;
		}

	MEMORY_ARG:
		{
			static const char *const TYPES[] = {
				[M0_MEM_I32] = "i32",
				[M0_MEM_U32] = "u32"
			};

			unsigned id = op.args[i];
			if(!constants[id].type == M0_CONST_ADDRESS)
				return 0;

			const m0_address *address = constants[id].value.as_cptr;
			_Bool is_symbolic = address->base_symbol;
			_Bool is_displaced = address->displacement;
			_Bool has_offset = address->offset_multiplier;

			switch(address->type)
			{
				case M0_MEM_I32:
				case M0_MEM_U32:
				say("%s(", TYPES[address->type]);
				break;

				default:
				return 0;
			}

			if(is_symbolic)
				say("@%s", address->base_symbol->name);
			else
				say("%%%u", (unsigned)address->base_register);

			if(is_displaced || has_offset)
				say(":");

			if(has_offset)
			{
				say("%%%u", (unsigned)address->offset_register);
				if(address->offset_multiplier != 1)
					say("*%li", (long)address->offset_multiplier);
			}

			if(is_displaced)
			{
				say("%s%li", has_offset ?
					(address->displacement < 0 ? "-" : "+") : "",
					(long)address->displacement);
			}

			say(")");

			continue;
		}
	}

	fputs("\n", file);

	return 1;
}
