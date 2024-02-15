#pragma once

#include <uthash.h>
#include "utils.h"

#define cache_entry(ptr, type, member) container_of(ptr, type, member)

struct cache;
struct cache_handle;

/// User-provided function to fetch a value for the cache, when it's not present.
/// Should return 0 if the value is fetched successfully, and a negative number if the
/// value cannot be fetched. Getter doesn't need to initialize fields of `struct
/// cache_handle`.
typedef int (*cache_getter_t)(struct cache *, const char *key, struct cache_handle **value);
typedef void (*cache_free_t)(struct cache *, struct cache_handle *value);

struct cache {
	cache_getter_t getter;
	struct cache_handle *entries;
};

struct cache_handle {
	char *key;
	UT_hash_handle hh;
};

/// Initialize a cache with `getter`
void cache_init(struct cache *cache, cache_getter_t getter);

/// Get a value from the cache. If the value doesn't present in the cache yet, the
/// getter will be called, and the returned value will be stored into the cache.
/// Returns 0 if the value is already present in the cache, 1 if the value is fetched
/// successfully, and a negative number if the value cannot be fetched.
int cache_get_or_fetch(struct cache *, const char *key, struct cache_handle **value);

/// Get a value from the cache. If the value doesn't present in the cache, NULL will be
/// returned.
struct cache_handle *cache_get(struct cache *, const char *key);

/// Invalidate a value in the cache.
void cache_invalidate(struct cache *, const char *key, cache_free_t free_fn);

/// Invalidate all values in the cache. After this call, `struct cache` holds no allocated
/// memory, and can be discarded.
void cache_invalidate_all(struct cache *, cache_free_t free_fn);
