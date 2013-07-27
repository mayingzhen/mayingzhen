using System;

namespace CSharpTest
{
    public class MyClass : ScriptBase
	{
        public float m_float = 5.0f;
        public int m_Int = 5;

		public MyClass ()
		{
			int i = 5;
		}

        public void Start()
        {
            m_float = 4.0f;
        }

        public void Update()
        {
            m_Int = 6;
        }

	}
}

