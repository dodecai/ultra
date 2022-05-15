#pragma once

struct AllocationMetrics {
	size_t TotalAllocated = 0;
	size_t TotalFreed = 0;

	size_t CurrentUsage() { return TotalAllocated - TotalFreed; }
};

static AllocationMetrics iAllocationMetrics;

static void PrintMemoryUsage() {
	std::cout << "Memory Usage: " << iAllocationMetrics.CurrentUsage() << " bytes\n";
}


void *operator new(size_t size) {
	iAllocationMetrics.TotalAllocated += size;
	return malloc(size);
}

void operator delete(void *memory, size_t const size) noexcept {
	iAllocationMetrics.TotalFreed += size;
	free(memory);
}
