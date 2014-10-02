using System;


public class SharpTest : ScriptBase
{
    public float m_fRotSpeed = 15.0f;

    public void Start()
    {
    }

    public void Update()
    {
	    m_sceneNode.RotateAround(Vector3.ZERO,Vector3.UNIT_Z, m_fRotSpeed * maEngine.GetTimer().GetFrameDeltaTime());
    }
}


