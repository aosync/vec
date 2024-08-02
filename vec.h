#ifndef VEC_H
#define VEC_H

#include <stdlib.h>
#include <string.h>

#if __STDC_VERSION__ >= 202311L
#define vec_alignof alignof
#else
#define vec_alignof _Alignof
#endif

struct vec_allocator {
	void *(*allocate)(size_t);
	void *(*reallocate)(void *, size_t);
	void (*deallocate)(void *);
};

static struct vec_allocator vec_std_allocator = {
	.allocate = malloc,
	.reallocate = realloc,
	.deallocate = free
};

struct vec_header {
	struct vec_allocator *alc;
	size_t len;
	size_t cap;
};

#define vec(T) T*

void *_vec_new(size_t el_size, size_t el_align, struct vec_allocator *alc);

void *_vec_data(struct vec_header *vh, size_t el_align);
struct vec_header *_vec_header(void *v, size_t el_align);
struct vec_header *_vec_ensure_size(struct vec_header *vh, int n, size_t el_size, size_t el_align);

#define vec_data _vec_data
#define vec_header(x) _vec_header((x), vec_alignof((x)[0]))
#define vec_ensure_size(x, n) _vec_data(_vec_ensure_size(vec_header(x), (n), sizeof((x)[0]), vec_alignof((x)[0])), vec_alignof((x)[0]))

#define vec_new_with(T, a) _vec_new(sizeof(T), vec_alignof(T), a)
#define vec_new(T) vec_new_with(T, &vec_std_allocator)
#define vec_push(x, y) (x = vec_ensure_size(x, 1), \
                        (x)[vec_header(x)->len++] = (y))

#define vec_pop(x) (x = vec_ensure_size(x, -1), \
                    (x)[--vec_header(x)->len])

#define vec_cat(a, b) (a = vec_ensure_size(a, vec_header(b)->len),                                 \
                       memcpy((a) + vec_header(a)->len, (b), vec_header(b)->len * sizeof((b)[0])), \
                       vec_header(a)->len += vec_header(b)->len)

#define vec_len(x) (vec_header(x)->len)

#define vec_empty(x) (vec_header(x)->len = 0, \
                      x = vec_ensure_size(x, 0))

#define vec_free(x) (vec_header(x)->alc->deallocate(vec_header(x)))

#endif
