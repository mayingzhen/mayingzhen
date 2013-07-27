using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

public class ScriptBase 
{
    private IntPtr m_gamePtr;

    public GameObject m_gameObj
	{
		get { return new GameObject(m_gamePtr, false);}
	}

    public SceneNode m_node
    {
        get { return m_gameObj.GetSceneNode(); }
    }


    //public Input  m_input
	//{
	//	get { return maEngine.GetInput(); }
	//}

    public ScriptBase GetScript(string name)
	{
		return EngineInternal.MonoGameObject_GetScript(m_gamePtr.ToInt32(),name);
	}

    public ScriptBase GetScript(GameObject gameObj, string name)
    {
		HandleRef handle = GameObject.getCPtr(gameObj);
        IntPtr gamePtr = HandleRef.ToIntPtr(handle);
        return EngineInternal.MonoGameObject_GetScript(gamePtr.ToInt32(), name);
    }

    public void SetGameObject(IntPtr cPtr)
    {
		//Debug.WriteLine("ScriptObject.SetGameObject()");
		m_gamePtr = cPtr;
    }
}
