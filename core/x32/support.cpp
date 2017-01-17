#include "support.h"
#include "kernel.h"

#define STD_SIZE	0
#define SHORT_SIZE	1
#define LONG_SIZE	2
static v7_err js_msg(struct v7 *rt, v7_val_t *res) {

	size_t l;

	//varargs
	//unsigned long numArgs = v7_argc(rt);
	v7_val_t valFormat = v7_arg(rt, 0);
	char * format = (char *)v7_get_string(rt, &valFormat, &l);

	int len = strlen(format);

	int arg = 1;
	size_t x;
	char formatArgs[9];
	v7_val_t val;

	for (int i = 0; i<len; i++) {
		if (format[i] == '%') {
			i++;

			switch (format[i]) {
			case '%': { //Modulus
				cputc('%');
				break;
			}
			case 'c': { //Char
				val = v7_arg(rt, arg);
				cputc(v7_get_string(rt, &val, &x)[0]);
				arg++;
				break;
			}
			case 's': { //String
				val = v7_arg(rt, arg);
				message("%s", v7_get_string(rt, &val, &x));
				arg++;
				break;
			}
			case 'i': //Integer
			case 'd': //Integer
			case 'u': //Unsigned Integer
			case 'x': //Hexadecimal
			case 'l': //Modifier - long
			case 'n': //Modifier - short
			case 'h': //Modifier - short
			case '+': //Modifier - +
			case '-': //Modifier - -
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '0': {
				//Clear format string
				int j = 0;

				for (j = 0; j < 9; j++)
					formatArgs[j] = 0;

				j = 1;
				formatArgs[0] = '%';

				//Build up format string upto Non-Modifier
				if ((format[i] != 'i') && (format[i] != 'd') && (format[i] != 'u') && (format[i] != 'x')) {
					while (((format[i + j] != 'i') || (format[i + j] != 'd') || (format[i + j] != 'u') || (format[i + j] != 'x')) && (j < 8)) {
						formatArgs[j] = format[i + j];
						j++;
					}
					formatArgs[j] = format[i + j];
				}
				else {
					formatArgs[j] = format[i];
					j++;
				}

				val = v7_arg(rt, arg);
				message(formatArgs, v7_get_int(rt, val));

				i += j;

				arg++;
				break;
			}
			default:
				//Malformed
				message("Malformed Format Args\n");
			}

		}
		else {
			cputc(format[i]);
		}
	}

	*res = v7_mk_number(rt, (double)l);

	return V7_OK;
}

Support::Result Support::Init() {

	//Initialise support functions such as in out etc
	enum v7_err rcode = V7_OK;
	rt = v7_create();

	v7_val_t result;

	v7_set_method(rt, v7_get_global(rt), "message", &js_msg);

	//Send (in javascript) to console "Hello World!" [test]
	rcode = v7_exec(rt, "message(\"Initialising JavaScript Runtime Components\n\");", &result);

    //Just for testing, print a message in printf format, and do some math
    //It works fine, so we'll just comment it out
    //rcode = v7_exec(rt, "message(\"%s%i\n\", \"Hello\", 2+2);", &result);

	if (rcode != V7_OK) {
        message("Unable to initialise JavaScript Runtime");
		return FAIL;
	}

	return OK;
    //v7_destroy(v7); -- TODO:: AT POWERDOWN
}

Support::Result Support::Exec(char * jsCode, uint64_t * result) {

    //Execute the UTF8/ASCII code within built in engine
    //Check for success
    enum v7_err rcode = V7_OK;

    rcode = v7_exec(rt, jsCode, result);

    if (rcode != V7_OK) {
        //Log error
        message("Support::JSExec: %s\n", v7_get_parser_error(rt));
        return FAIL;
    }

    return OK;
}


