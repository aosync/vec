#include <stdio.h>

#include <vec.h>

struct test {
	char i;
	short x;
};

int
main()
{
	printf("%d %d\n", sizeof(struct test), _Alignof(struct test));
	vec(int) v = vec_new(int);
	vec(int) a = vec_new(int);
	vec_push(v, 42);
	vec_push(v, 69);
	vec_push(a, 100);
	vec_push(a, 124);
	vec_cat(v, a);
	vec_cat(v, v);

	for (int i = 0; i < vec_len(v); i++)
		printf("%d\n", v[i]);
	
	vec_free(v);
	vec_free(a);
}
