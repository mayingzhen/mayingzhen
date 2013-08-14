using System;

namespace CSharpTest
{
    public class MyClass : ScriptBase
	{
        public float m_fRotSpeed = 5.0f;

        public void Start()
        {
        }

        public void Update()
        {
			m_node.RotateYAxisLS(m_fRotSpeed);
        }

	}
}

