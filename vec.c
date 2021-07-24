#include <vec.h>

void *
_vec_new(size_t el_size, struct vec_allocator allocator)
{
	struct vec_header *v = allocator.allocate(sizeof(struct vec_header) + el_size);
	*v = (struct vec_header) {
		.el_size = el_size,
		.len = 0,
		.cap = 1,
		.allocator = allocator
	};
	return _vec(v);
}

struct vec_header *
_vec_header(void *v)
{
	return v - sizeof(struct vec_header);
}

void *
_vec(struct vec_header *vh)
{
	return (void*)vh + sizeof(struct vec_header);
}

struct vec_header *
_vec_ensure_size(struct vec_header *v, int n)
{
	size_t nl = v->len + n;
	if(nl >= v->cap)
		v->cap *= 2;
	else if (nl < v->cap / 2)
		v->cap /= 2;
	else
		return v;
	struct vec_header *nv = _vec_ensure_size(v, n);
	if(nv != v)
		v = nv;
	else
		v = v->allocator.reallocate(v, sizeof(struct vec_header) + v->el_size * v->cap);
	return v;
}
