i = -123;
d = 123.456;
s = "hello";

Print(s, " world");
DumbFn(arg1, arg2) {}


test0 = !false;
test1 = (4 * 3 / 2 % 5) == 1;
test2 = (1 + 2 + 3 - 5) == 1;
test3 = (1 << 3 >> 2 >> 1) == 1;
test4 = (123 != 456 && 3 < 5 && 5 > 3) == 1;
test5 = (3 & 1 | 1 << 5) == 1;
test6 = true ? 1 : false ? 0 : 1;
test7 = 1 <= 2;
test7 += 123 + 456;
test7 -= 123 + 456;
test7 = 123 + 456;
{{{{
	test8 = 1;
}}}}
test9;
switch (3) {
 	case 1: test9 = 10; break;
	case 3: test9 = 1; break;
	default: test9 = 5; break;
}
if (true) {} else {}
if (true) {} else if (true) {} else if (true) {} else {}
while (true) {}
for (i=0; i < 10; i++) {continue; break; return 0;}

value = SomeFn(2 + 3, ((5*2)) ) * 54;
abc = 123'kg + 456'kg;

SomeFn(arg0, arg1) {
	arg0 = (1'(m/s**2) + 2'(m/s**2)) * (3 - 4) / 5**2 == 123;
	return 0;
}

value = SomeFn(2 + 3, ((5*2)) ) * 54;

