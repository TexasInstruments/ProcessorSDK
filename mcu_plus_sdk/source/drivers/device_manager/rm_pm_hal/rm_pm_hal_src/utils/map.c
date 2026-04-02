/*
 * CC0 (Public domain) - see licenses/CC0 file for details
 * This code is based on ccan/strset.c.
 */

#include <map.h>
#include <types/short_types.h>
#include <str.h>
#include <ilog.h>
#include <stddef.h>

/* Closest member to this in a non-empty map. */
static struct map *closest(struct map *n, const u8 *bytes, size_t len)
{
	struct map *n_val_p = n;

	/* Anything with NULL value is a node. */
	while (!n_val_p->v) {
		u8 direction = 0;

		if (n_val_p->u.n->byte_num < len) {
			u8 c = bytes[n_val_p->u.n->byte_num];
			direction = (c >> n_val_p->u.n->bit_num) & 1U;
		}
		n_val_p = &n_val_p->u.n->child[direction];
	}
	return n_val_p;
}

struct map_node *strmap_get(const struct map *map_ptr, const char *member)
{
	struct map *n;
	struct map_node *strmap_get_ret = NULL;

	/* Not empty map? */
	if (map_ptr != NULL) {
		n = closest((struct map *) map_ptr, (const u8 *) member,
			    strlen(member));
		if (streq(member, (const char *) n->u.s)) {
			strmap_get_ret = n->v;
		}
	} else {
		strmap_get_ret = NULL;
	}
	return strmap_get_ret;
}

struct map_node *u32map_get(const struct map *map_ptr, u32 member)
{
	struct map *n;
	struct map_node *u32map_get_ret = NULL;

	/* Not empty map? */
	if (map_ptr != NULL) {
		n = closest((struct map *) map_ptr, (const u8 *) &member,
			    sizeof(u32));
		if (member == (u32) n->u.s) {
			u32map_get_ret = n->v;
		}
	} else {
		u32map_get_ret = NULL;
	}
	return u32map_get_ret;
}

const u8 *str_get_bytes(struct map *map_ptr)
{
	return (const u8 *) map_ptr->u.s;
}

const u8 *u32_get_bytes(struct map *map_ptr)
{
	return (const u8 *) &map_ptr->u.s;
}

struct map *map_add(struct map *map_ptr, const void *member, const u8 *bytes,
		    size_t len, struct map_node *newn,
		    const u8 *(*get_bytes)(struct map *m))
{
	struct map *n;
	size_t byte_num;
	u8 bit_num, new_dir;
	const u8 *s;
	struct map *mapp_ptr_p = map_ptr;

	/* Empty map? */
	if (mapp_ptr_p == NULL) {
		mapp_ptr_p = &newn->child[0];
		mapp_ptr_p->u.s = member;
		mapp_ptr_p->v = newn;
	} else {
		/* Find closest existing member. */
		n = closest(mapp_ptr_p, bytes, len);

		/* Find where they differ. */
		s = get_bytes(n);
		for (byte_num = 0; s[byte_num] == bytes[byte_num]; byte_num++) {
			if (byte_num == len) {
				/* All identical! */
				mapp_ptr_p = NULL;
				break;
			}
		}
		if (mapp_ptr_p != NULL) {
			/* Find which bit differs (if we had ilog8, we'd use it) */
			bit_num = s[byte_num] ^ bytes[byte_num];
			bit_num = (u8) (ilog32_nz(bit_num) - 1U);

			/* Which direction do we go at this bit? */
			new_dir = ((bytes[byte_num]) >> bit_num) & 1U;

			newn->byte_num = byte_num;
			newn->bit_num = bit_num;
			newn->child[new_dir].v = newn;
			newn->child[new_dir].u.s = member;

			/* Find where to insert: not closest, but first which differs! */
			n = mapp_ptr_p;
			while (!n->v) {
				u8 direction = 0;
				sbool flag_break = SFALSE;

				if (n->u.n->byte_num > byte_num) {
					flag_break = STRUE;
				}
				if (flag_break != STRUE) {
					/* Subtle: bit numbers are "backwards" for comparison */
					if ((n->u.n->byte_num == byte_num) && (n->u.n->bit_num <
									       bit_num)) {
						flag_break = STRUE;
					}
				}
				if (flag_break == STRUE) {
					break;
				}

				if (n->u.n->byte_num < len) {
					u8 c = bytes[n->u.n->byte_num];
					direction = (c >> n->u.n->bit_num) & 1U;
				}
				n = &n->u.n->child[direction];
			}

			newn->child[(new_dir == 0U) ? 1 : 0] = *n;
			n->u.n = newn;
			n->v = NULL;
		}
	}
	return mapp_ptr_p;
}

static sbool str_iterate(struct map n,
			 sbool (*handle)(const char *, struct map_node *, void *),
			 const void *data)
{
	sbool ret;

	if (n.v != NULL) {
		ret = handle((const char *) n.u.s, n.v, (void *) data);
	} else {
		ret = str_iterate(n.u.n->child[0], handle, data)
		      && str_iterate(n.u.n->child[1], handle, data);
	}
	return ret;
}

void strmap_iterate_(const struct map *map_ptr,
		     sbool (*handle)(const char *const_ptr, struct map_node *node_ptr, void *v_ptr),
		     const void *data)
{
	/* Empty map? */
	if (!map_ptr) {
		/* Do nothing - return */
	} else {
		str_iterate(*map_ptr, handle, data);
	}
	return;
}

static sbool u32_iterate(struct map n,
			 sbool (*handle)(u32, struct map_node *node_ptr, void *v_ptr),
			 const void *data)
{
	sbool ret;

	if (n.v != NULL) {
		ret = handle((u32) n.u.s, n.v, (void *) data);
	} else {
		ret = u32_iterate(n.u.n->child[0], handle, data)
		      && u32_iterate(n.u.n->child[1], handle, data);
	}
	return ret;
}

void u32map_iterate_(const struct map *map_ptr,
		     sbool (*handle)(u32 hdata, struct map_node *node_ptr, void *v_ptr),
		     const void *data)
{
	/* Empty map? */
	if (!map_ptr) {
		/* Do nothing - return */
	} else {
		u32_iterate(*map_ptr, handle, data);
	}
	return;
}

const struct map *strmap_prefix(const struct map *map_ptr, const char *prefix)
{
	const struct map *n, *top;
	size_t len = strlen(prefix);
	const u8 *bytes = (const u8 *) prefix;
	const struct map *strmap_prefix_ret;

	/* Empty map -> return empty map. */
	if (!map_ptr) {
		strmap_prefix_ret = map_ptr;
	} else {
		n = map_ptr;
		top = n;

		/* We walk to find the top, but keep going to check prefix matches. */
		while (!n->v) {
			u8 c = 0, direction;

			if (n->u.n->byte_num < len) {
				c = bytes[n->u.n->byte_num];
			}

			direction = (c >> n->u.n->bit_num) & 1U;
			n = &n->u.n->child[direction];
			if (c != 0U) {
				top = n;
			}
		}

		if (!strstarts((const char *) n->u.s, prefix)) {
			strmap_prefix_ret = NULL;
		} else {
			strmap_prefix_ret = top;
		}
	}
	return strmap_prefix_ret;
}
