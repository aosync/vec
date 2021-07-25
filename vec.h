#ifndef VEC_H
#define VEC_H

#include <stdlib.h>
#include <string.h>

struct vec_allocator{
	void *(*allocate)(size_t);
	void *(*reallocate)(void *, size_t);
	void (*deallocate)(void *);
};

static struct vec_allocator vec_std_allocator = {
	.allocate = malloc,
	.reallocate = realloc,
	.deallocate = free
};

struct vec_header{
	size_t el_size;
	size_t len;
	size_t cap;
	struct vec_allocator allocator;
};

#define vec(T) T*

void *_vec_new(size_t, struct vec_allocator);

void *_vec(struct vec_header *);
struct vec_header *_vec_header(void *);

struct vec_header *_vec_ensure_size(struct vec_header *, int);

#define _concat(a, b) a ## b
#define concat(a, b) _concat(a, b)
#define macro_var(name) concat(name, __LINE__)

#define vec_new(T) _vec_new(sizeof(T), vec_std_allocator)
#define vec_new_with(T, a) _vec_new(sizeof(T), a)
#define vec_push(x, y) (x = _vec(_vec_ensure_size(_vec_header(x), 1)),  \
                        (x)[_vec_header(x)->len++] = (y))

#define vec_pop(x) (x = _vec(_vec_ensure_size(_vec_header(x), -1)),\
                    (x)[--_vec_header(x)->len])

#define vec_cat(a, b) (a = _vec(_vec_ensure_size(_vec_header(a), _vec_header(b)->len)), \
		memcpy(a + _vec_header(a)->len, b, _vec_header(b)->len * _vec_header(b)->el_size), \
                       _vec_header(a)->len += _vec_header(b)->len)

#define vec_len(x) (_vec_header(x)->len)

#define vec_empty(x) (_vec_header(x)->len = 0,	\
                      x = _vec(_vec_ensure_size(_vec_header(x), 0)))

#define vec_free(x) _vec_header(x)->allocator.deallocate(_vec_header(x))
#define vec_foreach(type, a, v) size_t macro_var(_mi_) = 0; for(type a = v[0]; macro_var(_mi_) < _vec_header(v)->len; a = v[++macro_var(_mi_)])
#define vec_foreach_ref(type, a, v) size_t macro_var(_mi_) = 0; for(type *a = v; macro_var(_mi_) < _vec_header(v)->len; (a++, macro_var(_mi_)++))

#endif
