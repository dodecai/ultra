module;

export module Ultra.Debug.Memory;

///
/// @brief: This module serves something like a memory allocation/deallocation watcher.
///         It will notify the application, if possible memory leaks are detected.
/// @note:  Currently this is only the barebone, there is still some work to be done.
/// 

import <memory>;

import Ultra.Core;
import Ultra.Logger;

// Properties
struct AllocationMetrics {
    std::size_t TotalAllocated = 0;
    std::size_t TotalFreed = 0;

    std::size_t CurrentUsage() { return TotalAllocated - TotalFreed; }
} static sAllocationMetrics;

// Overrides
export void *operator new(size_t size) {
    sAllocationMetrics.TotalAllocated += size;
    return malloc(size);
}

export void operator delete(void *memory, size_t const size) noexcept {
    sAllocationMetrics.TotalFreed += size;
    free(memory);
}

// Verification
export void VerifyMemoryUsage() {
    Ultra::logger
        << "Current Memory Usage: " << sAllocationMetrics.CurrentUsage() << " bytes\n"
        << " - Total Allocated:   " << sAllocationMetrics.TotalAllocated << " bytes\n"
        << " - Total Deallocated: " << sAllocationMetrics.TotalFreed     << " bytes\n";
}
