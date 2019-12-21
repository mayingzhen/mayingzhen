#pragma once

#include "Thread/Event.h"

namespace ma
{
	inline void AppMemoryBarrier()
	{
#ifdef __GNUC__
#if defined(__arm__)
		asm volatile ("dmb":::"memory");
#elif defined(__aarch64__)
		asm volatile ("dmb sy":::"memory");
#elif defined( __i386__ ) || defined( __x86_64__ )
		asm volatile ("mfence");
#endif
#else // __GNUC__
		// _mm_mfence();
#endif // __GNUC__
	}


	/**
	 * A ring buffer for use with two threads: a reading thread and a writing thread.
	 */
	class RingBuffer
	{
	public:

		/**
		 * Minimal initialization constructor.
		 * @param buffer_size - The size of the data buffer to allocate.
		 * @param in_alignment - m_alignment of each allocation unit (in uint8_ts)
		 */
		RingBuffer(uint32_t buffer_size, uint32_t in_alignment = 1);

		/**
		 * A reference to an allocated chunk of the ring buffer.
		 * Upon destruction of the context, the chunk is committed as written.
		 */
		class AllocationContext
		{
		public:

			/**
			 * Upon construction, AllocationContext allocates a chunk from the ring buffer.
			 * @param in_ring_buffer - The ring buffer to allocate from.
			 * @param in_allocation_size - The size of the allocation to make.
			 */
			AllocationContext(RingBuffer& in_ring_buffer, uint32_t in_allocation_size);

			/** Upon destruction, the allocation is committed, if Commit hasn't been called manually. */
			~AllocationContext();

			/** Commits the allocated chunk of memory to the ring buffer. */
			void Commit();

			// Accessors.
			void* GetAllocation() const { return m_allocation_start; }
			uint32_t GetAllocatedSize() const { return (uint32_t)(m_allocation_end - m_allocation_start); }

		private:
			RingBuffer& m_ring_buffer;
			uint8_t* m_curr_write_pointer;
			uint8_t* m_allocation_start;
			uint8_t* m_allocation_end;
		};

		/**
		 * Checks if there is data to be read from the ring buffer, and if so accesses the pointer to the data to be read.
		 * @param out_read_pointer - When returning TRUE, this will hold the pointer to the data to read.
		 * @param MaxReadSize - When returning TRUE, this will hold the number of uint8_ts available to read.
		 * @return TRUE if there is data to be read.
		 */
		bool BeginRead(void*& out_read_pointer, uint32_t& out_read_size);

		/**
		 * Frees the first read_size uint8_ts available for reading via BeginRead to the writing thread.
		 * @param read_size - The number of uint8_ts to free.
		 */
		void FinishRead(uint32_t read_size);

		/**
		 * Waits for data to be available for reading.
		 * @param wait_time Time in milliseconds to wait before returning.
		 */
		void WaitForRead(uint32_t wait_time = -1);

	private:
		// Buffer Size in uint8_ts
		//uint32_t m_buffer_size;

		// The data buffer.
		uint8_t* m_data;

		// The first uint8_t after end the of the data buffer.
		uint8_t* m_data_end;

		// The next uint8_t to be written to.
		uint8_t* volatile m_write_pointer;

		// TRUE if there is an AllocationContext outstanding for this ring buffer.
		bool m_is_writing;

		// The next uint8_t to be read from.
		uint8_t* volatile m_read_pointer;

		//Alignment of each allocation unit (in uint8_ts).
		uint32_t m_alignment;

		//The event used to signal the reader thread when the ring buffer has data to read.
		Event m_data_written_event;
	};
}