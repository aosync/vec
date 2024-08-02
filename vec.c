#include "vec.h"

void *
_vec_new(size_t el_size, size_t el_align, struct vec_allocator *alc)
{
	size_t off = (sizeof(struct vec_header) + el_align - 1) & ~(el_align - 1);
	
	struct vec_header *v = alc->allocate(off + el_size);
	*v = (struct vec_header) {
		.alc = alc,
		.len = 0,
		.cap = 1,
	};

	return vec_data(v, el_align);
}

struct vec_header *
_vec_header(void *v, size_t el_align)
{
	size_t off = (sizeof(struct vec_header) + el_align - 1) & ~(el_align - 1);

	return v - off;
}

void *
_vec_data(struct vec_header *vh, size_t el_align)
{
	size_t off = (sizeof(struct vec_header) + el_align - 1) & ~(el_align - 1);

	return (void*)vh + off;
}

struct vec_header *
_vec_ensure_size(struct vec_header *v, int n, size_t el_size, size_t el_align)
{
	size_t nl = v->len + n;
	if(nl >= v->cap)
		v->cap *= 2;
	else if (nl < v->cap / 2)
		v->cap /= 2;
	else
		return v;

	struct vec_header *nv = _vec_ensure_size(v, n, el_size, el_align);
	if(nv != v)
		v = nv;
	else {
		size_t off = (sizeof(struct vec_header) + el_align - 1) & ~(el_align - 1);

		v = v->alc->reallocate(v, off + el_size * v->cap);
	}

	return v;
}
