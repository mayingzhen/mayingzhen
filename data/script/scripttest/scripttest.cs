using System;


public class SharpTest : ScriptBase
{
    public float m_fRotSpeed = 15.0f;

    public void Start()
    {
    }

    public void Update()
    {
	    m_node.RotateYAxisLS(m_fRotSpeed * maEngine.GetTimer().GetFrameDeltaTime());
    }
}


