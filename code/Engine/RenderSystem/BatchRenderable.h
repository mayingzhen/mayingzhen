#pragma once

namespace ma
{
	/// Data for one geometry instance.
	struct InstanceData
	{
		/// Construct undefined.
		InstanceData()
		{
		}

		/// Construct with transform, instancing data and distance.
		InstanceData(const Matrix3x4* worldTransform, const void* instancingData, float distance) :
			worldTransform_(*worldTransform),
			instancingData_(instancingData),
			distance_(distance)
		{
		}

		/// World transform.
		//const Matrix3x4* worldTransform_;
		Matrix3x4 worldTransform_;
		/// Instancing data buffer.
		const void* instancingData_;
		/// Distance from camera.
		float distance_;
	};

	class BatchGroup : Renderable
	{
		/// Construct with defaults.
		BatchGroup() {}

		/// Construct from a batch.
		BatchGroup(const Renderable& batch) :
			//Batch(batch),
			startIndex_(M_MAX_UNSIGNED)
		{
		}

		/// Destruct.
		~BatchGroup()
		{
		}

		/// Add world transform(s) from a batch.
		void AddTransforms(const Renderable& batch);

		/// Pre-set the instance data. Buffer must be big enough to hold all data.
		void SetInstancingData(void* lockedData, unsigned stride, unsigned& freeIndex);

		/// Prepare and draw.
		void Render(Technique* pTech);

		/// Instance data.
		std::vector<InstanceData> instances_;
		/// Instance stream start index, or M_MAX_UNSIGNED if transforms not pre-set.
		unsigned startIndex_ = -1;
	};

	class BatchRenderable
	{
	public:
		BatchRenderable();

		virtual void	AddRenderObj(Renderable* pRenderObj);

		virtual void	PrepareRender();

		virtual void	Render();

		virtual void	Clear();

	protected:
		typedef std::vector<Renderable*> VEC_RENDERABLE;
		VEC_RENDERABLE	m_arrRenderList;

		std::vector< VEC_RENDERABLE > m_arrMerge;

		struct InstanceData
		{
			Matrix3x4 m_world;
		};
		std::vector<InstanceData> m_vecIntanceData;

		RefPtr<VertexBuffer> m_pInstanceBuffer;
		
	};
}

