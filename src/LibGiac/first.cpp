#include "config.h"
#include "first.h"

int init_gmp_memory::refcount = 0;
init_gmp_memory init_gmp_memory_instance;


init_gmp_memory::init_gmp_memory() { }
init_gmp_memory::~init_gmp_memory() { }

#ifdef NSPIRE
#include <os.h>
#else
#include <new>
#include <cstdlib>
#include <stdexcept>
#endif

#ifdef GIAC_CHECK_NEW

#include <iostream>

size_t giac_allocated = 0;
void* operator new(std::size_t size) {
	std::cerr << giac_allocated << " + " << size << std::endl;
	giac_allocated += size;
	void * p =  std::malloc(size);
	if (!p) {
		std::bad_alloc ba;
		throw ba;
	}
	return p;
}

void* operator new[](std::size_t size) {
	std::cerr << giac_allocated << " + [] " << size << std::endl;
	giac_allocated += size;
	void * p =  std::malloc(size);
	if (!p) {
		std::bad_alloc ba;
		throw ba;
	}
	return p;
}

void operator delete[](void* obj) {
	free(obj);
}
#endif



