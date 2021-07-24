#include <stdio.h>

#include <vec.h>

int
main()
{
	vec(int) v = vec_new(int);
	vec(int) a = vec_new(int);
	vec_push(v, 42);
	vec_push(v, 69);
	vec_push(a, 100);
	vec_push(a, 124);
	vec_cat(v, a);
	vec_cat(v, v);

	vec_foreach(int, it, v) {
		printf("%d\n", it);
	}
	
	vec_free(v);
}
