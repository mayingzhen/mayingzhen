#ifndef __TwoJointIKSolver_H__
#define __TwoJointIKSolver_H__

namespace ma
{
	class ANIMATION_API TwoJointIKSolver : public PoseModifier
	{
	public:
		void Init(BoneIndex uBoneAInd,BoneIndex uBoneBInd,BoneIndex uBoneCInd,
			const D3DVECTOR& vHingAxisLs,const Skeleton* pResfSkel);

		void UpdatePose(SkeletonPose* pNodePose);

		void Update(const float timeDeltaInSeconds);

		/**
		 * Solve the two bone IK problem.
		 * This function works with positions only, and does not solve the rotations of nodes.
		 * Actually this function has nothing to do with nodes. It is however the core of the algorithm.
		 * Please keep in mind that all parameters should be in the same space, for example world space.
		 * The output of this method is a vector that contains the position of the middle joint.
		 * If you draw a line between the specified posA and this output middle joint pos and a line starting at
		 * the output middle pos in direction of the goal, with a length of (posC - posB).Length() you are drawing
		 * the solution of this solver.
		 * @param posA The position of the first joint (for example the upper arm / shoulder).
		 * @param posB The position of the middle joint (for example the elbow / lower arm).
		 * @param posC The position of the end effector (for example the hand / wrist).
		 * @param goal The goal the solver should try to reach.
		 * @param bendDir The desired middle joint bend direction (as unit vector). So the direction the knee or elbow should point to.
		 * @param outMidPos A pointer to the vector to which the function will write the new middle joint position.
		 * @result Returns true when a solution has been found, or false when the goal is not reachable.
		 */
		static bool Solve2LinkIK(const Vector3& posA, const Vector3& posB, const Vector3& posC, const Vector3& goal, const Vector3& bendDir, Vector3* outMidPos);

	private:

		/**
		 * Calculates the matrix that rotates the object from solve space back into the original space.
		 * The inverse (which equals the transpose) of this matrix is used to convert the specified parameters into solve space.
		 * @param goal The goal we try to reach.
		 * @param bendDir The desired middle joint bend direction (as unit vector). So the direction the knee or elbow should point to.
		 * @param outForward The output matrix which will contain the matrix that rotates from solve space into the space of the goal.
		 */
		static void CalculateMatrix(const Vector3& goal, const Vector3& bendDir, Matrix4x4* outForward);
	
	private:
		BoneIndex m_uBoneAID;
		BoneIndex m_uBoneBID;
		BoneIndex m_uBoneCID;
		Vector3 m_vHingAxisLS;
		Vector3 m_vGoalOS;

		// 
		NodeTransform m_pivotALS;
		NodeTransform m_pivotBLS;
		NodeTransform m_pivotCLS;
	};
}


#endif