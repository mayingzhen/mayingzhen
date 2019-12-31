#include "RingBuffer.h"
#include <algorithm>

namespace ma
{


	#define Align(in_size, alignment) ( ((in_size)+(alignment)-1) & ~((alignment)-1) )

	RingBuffer::RingBuffer(uint32_t buffer_size, uint32_t in_alignment)
		: /*m_buffer_size(buffer_size), */m_is_writing(false)
	{
		m_data = new uint8_t[/*appRoundUpToPowerOfTwo*/(buffer_size)];
		m_data_end = m_data + buffer_size;
		m_alignment = /*appRoundUpToPowerOfTwo*/(in_alignment);
		m_read_pointer = m_write_pointer = m_data;
	}

	RingBuffer::AllocationContext::AllocationContext(RingBuffer& in_ring_buffer, uint32_t in_allocation_size)
		: m_ring_buffer(in_ring_buffer)
	{
		// Only allow a single AllocationContext at a time for the ring buffer.
		//ASSERT(IsInGameThread);
		ASSERT(!m_ring_buffer.m_is_writing);

		m_ring_buffer.m_is_writing = true;
		m_curr_write_pointer = m_ring_buffer.m_write_pointer;

		// Check that the allocation will fit in the buffer.
		const uint32_t aligned_allocation_size = Align(in_allocation_size, m_ring_buffer.m_alignment);
		//const uint32_t buffer_size = m_ring_buffer.m_buffer_size;//(uint32)( m_ring_buffer.m_data_end - m_ring_buffer.m_data );
		//ASSERT( aligned_allocation_size < buffer_size );


		// Use the memory referenced by m_write_pointer for the allocation, wrapped around to the beginning of the buffer
		// if it was at the end.
		//m_allocation_start = m_curr_write_pointer != m_ring_buffer.m_data_end ? m_curr_write_pointer : m_ring_buffer.m_data;
		if (m_curr_write_pointer != m_ring_buffer.m_data_end)
		{
			m_allocation_start = m_curr_write_pointer;
		}
		else
		{
			m_allocation_start = m_ring_buffer.m_data;
		}

		// If there isn't enough space left in the buffer to allocate the full size, allocate all the remaining bytes in the buffer.
		m_allocation_end = std::min<uint8_t*>(m_ring_buffer.m_data_end, m_allocation_start + aligned_allocation_size);

		// Wait until the reading thread has finished reading the area of the buffer we want to allocate.
		while (true)
		{
			// Make a snapshot of a recent value of m_read_pointer.
			uint8_t* current_read_pointer = m_ring_buffer.m_read_pointer;

			// If the m_read_pointer and m_write_pointer are the same, the buffer is empty and there's no risk of overwriting unread data.
			if (current_read_pointer == m_curr_write_pointer)
			{
				break;
			}
			else
			{
				// If the allocation doesn't contain the read pointer, the allocation won't overwrite unread data.
				// Note that it needs to also prevent advancing m_write_pointer to match the current m_read_pointer, since that would signal that the
				// buffer is empty instead of the expected full.
				const bool allocation_contains_read_pointer =
					m_allocation_start <= current_read_pointer &&
					current_read_pointer <= m_allocation_end;
				if (!allocation_contains_read_pointer)
				{
					break;
				}
			}
		}
	}

	RingBuffer::AllocationContext::~AllocationContext()
	{
		Commit();
	}

	void RingBuffer::AllocationContext::Commit()
	{
		if (m_allocation_start)
		{
			// Use a memory barrier to ensure that data written to the ring buffer is visible to the reading thread before the m_write_pointer
			// update.
			AppMemoryBarrier();

			// Advance the write pointer to the next unallocated byte.
			m_ring_buffer.m_write_pointer = m_allocation_end;

			// Reset the bIsWriting flag to allow other AllocationContexts to be created for the ring buffer.
			m_ring_buffer.m_is_writing = false;

			// Use a memory barrier to ensure that the modification of m_read_pointer is done by reading thread
			AppMemoryBarrier();
			if (m_curr_write_pointer == m_ring_buffer.m_read_pointer)
			{
				m_ring_buffer.m_data_written_event.Signal();
			}

			// Clear the allocation pointer, to signal that it has been committed.
			m_allocation_start = NULL;
		}
	}

	bool RingBuffer::BeginRead(void*& out_read_pointer, uint32_t& out_read_size)
	{
		// Make a snapshot of a recent value of m_write_pointer, and use a memory barrier to ensure that reads from the data buffer
		// will see writes no older than this snapshot of the m_write_pointer.
		uint8_t* current_write_pointer = m_write_pointer;
		AppMemoryBarrier();

		// Determine whether the write pointer or the buffer end should delimit this contiguous read.
		uint8_t* read_end_pointer;
		if (current_write_pointer >= m_read_pointer)
		{
			read_end_pointer = current_write_pointer;
		}
		else
		{
			// If the read pointer has reached the end of readable data in the buffer, reset it to the beginning of the buffer.
			if (m_read_pointer == m_data_end)
			{
				m_read_pointer = m_data;
				read_end_pointer = current_write_pointer;
			}
			else
			{
				read_end_pointer = m_data_end;
			}
		}

		// Determine whether there's data to read, and how much.
		if (m_read_pointer < read_end_pointer)
		{
			out_read_pointer = m_read_pointer;
			out_read_size = (uint32_t)(read_end_pointer - m_read_pointer);
			return true;
		}
		else
		{
			return false;
		}
	}

	void RingBuffer::FinishRead(uint32_t read_size)
	{
		m_read_pointer += Align(read_size, m_alignment);
	}

	void RingBuffer::WaitForRead(uint32_t wait_time)
	{
		// If the buffer is empty, wait for the data-written event to be triggered.
		if (m_read_pointer == m_write_pointer)
		{
			m_data_written_event.Wait(wait_time);
		}
	}
}
