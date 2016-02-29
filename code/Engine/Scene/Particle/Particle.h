#pragma once

namespace ma
{
	class RenderComponent;

	enum BillboardType
	{
		BBT_None,
		BBT_AlignViewPlane,// 基于视平面
		BBT_AlignViewPoint,// 基于视点
		BBT_AlignViewPlane_YConstraint,
		BBT_AlignViewPoint_YConstraint,
		BBT_AlignDirection,// up is direction
		BBT_AlignPos,// up is direction
		BBT_ZAlignDirection,
		BBT_ZAlignPos,
		BBT_AlignBottomCenter,
	};

	//! Struct for holding particle data
	struct SParticle
	{
		//! Position of the particle
		Vector3 pos;
		Vector3 startPos;
        Vector3 prePos;

		//! Direction and speed of the particle
		Vector3 vector;

		//! Original direction and speed of the particle.
		/** The direction and speed the particle had when it was emitted. */
		Vector3 startVector;

		//! Start life time of the particle
		Real startTime;

		//! End life time of the particle
		Real endTime;

		//! Current color of the particle
		ColourValue color;

		//! Original color of the particle.
		/** That's the color of the particle it had when it was emitted. */
		ColourValue startColor;

		//! Scale of the particle.
		/** The current scale of the particle. */
		FloatSize size;

		//! Original scale of the particle.
		/** The scale of the particle when it was emitted. */
		FloatSize startSize;

		/// Current rotation value
		Radian rotation;
		Radian rotationStart;
		/// Speed of rotation in radians/sec
		Radian rotationSpeed;
		
        // 粒子自身的朝向
		Quaternion orientation;
        Vector3 preAlignDirection;

        // subUV使用
		Rectangle uv;
		Rectangle nextUV;
		uint32 nBlend;
        int nStartIndex;

		SParticle(){rotation = rotationStart = rotationSpeed = Radian(0.f); orientation = Quaternion::IDENTITY;
		uv = nextUV= Rectangle(0,0,1,1); nBlend = 0;nStartIndex = 0;}
	};

	typedef list<SParticle> LST_PARTICLE;
}