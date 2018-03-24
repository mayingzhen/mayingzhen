#pragma once

namespace ma
{
	struct AnimationNodeOutput
	{
		std::vector<Transform> boneTrans;
		std::vector<bool> boneMask;

		AnimationNodeOutput() 
		{
		}

		void Init(uint32_t nCount)
		{
			boneTrans.resize(nCount);
			boneMask.resize(nCount);

			std::fill(boneTrans.begin(), boneTrans.end(), Transform());
			std::fill(boneMask.begin(), boneMask.end(), false);
		}
	};

	class AnimationNodeOutputPool
	{
	private:
		std::vector<AnimationNodeOutput*> m_cache;
		int m_size;
		int m_index;

	public:
		AnimationNodeOutputPool() : m_size(0), m_index(-1) {}
		~AnimationNodeOutputPool()
		{
			for (auto &output : m_cache)
			{
				delete output;
			}
			m_cache.clear();
			m_size = 0;
			m_index = -1;
		}
		AnimationNodeOutput &get()
		{
			AnimationNodeOutput *output = NULL;
			if (m_size == 0)
			{
				output = new AnimationNodeOutput();
			}
			else
			{
				output = m_cache.back();
				m_cache.pop_back();
				--m_size;
			}
			return *output;
		}
		void returnToPool(AnimationNodeOutput & node)
		{
			m_cache.push_back(&node);
			++m_size;
		}

		static thread_local AnimationNodeOutputPool s_animation_node_output_pool;
	};

}