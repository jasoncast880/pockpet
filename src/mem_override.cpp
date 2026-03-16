#include <cstddef>
#include <new>

#include "FreeRTOS.h"
#include "task.h"

	// Global overload for 'new'
	void* operator new(size_t size) {
			void* p = pvPortMalloc(size);
			if (p == nullptr) {
					// Optional: Throw std::bad_alloc() or handle error
			}
			return p;
	}

	// Global overload for 'delete'
	void operator delete(void* p) noexcept {
			vPortFree(p);
	}

	void* operator new[](size_t size) {
			return pvPortMalloc(size);
	}

	void operator delete[](void* p) noexcept {
			vPortFree(p);
	}

extern "C" {
}
