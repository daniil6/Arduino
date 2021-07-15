all:
	gcc -c  "../../rsa_basic.c"  -o build/up_up_rsa_basic.c.o -I. -I.
	gcc -c  "../../bigint.c"  -o build/up_up_bigint.c.o -I. -I.
	gcc -c  "../../rsaes_pkcs1v15.c"  -o build/up_up_rsaes_pkcs1v15.c.o -I. -I.
	gcc -c  "../../bigint_io.c"  -o build/up_up_bigint_io.c.o -I. -I.
	gcc -c  "../../random_dummy.c"  -o build/up_up_random_dummy.c.o -I. -I.
	ar rcu build/librsa-avr.a build/up_up_rsa_basic.c.o build/up_up_bigint.c.o build/up_up_rsaes_pkcs1v15.c.o build/up_up_bigint_io.c.o build/up_up_random_dummy.c.o   