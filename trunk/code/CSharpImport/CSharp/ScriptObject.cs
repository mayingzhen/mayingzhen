using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

public class ScriptBase 
{
    //private IntPtr m_scriptObjPtr;
    private IntPtr m_gamePtr;

    //private ScriptObject m_scriptObj;

    public GameObject m_gameObj
	{
		get { return new GameObject(m_gamePtr, false);}
        //get { return new m_scriptObj.GetGameObj(); }
	}

    public SceneNode m_node
    {
        get { return m_gameObj.GetSceneNode(); }
    }


    //public Input  m_input
	//{
	//	get { return maEngine.GetInput(); }
	//}

    public void AddCollisionListener()
    {
        //EngineInternal.AddCollisionListener(0);
    }

    public void AddCollisionListener(GameObject gameObj)
    {
        HandleRef handle = GameObject.getCPtr(gameObj);
        IntPtr gamePtr = HandleRef.ToIntPtr(handle);
        //EngineInternal.AddCollisionListener(gamePtr);
    }

    public void RemoveCollisionListener()
    {
       // EngineInternal.RemoveCollisionListener(0);
    }

    public void RemoveCollisionListener(GameObject gameObj)
    {
        HandleRef handle = GameObject.getCPtr(gameObj);
        IntPtr gamePtr = HandleRef.ToIntPtr(handle);
       // EngineInternal.RemoveCollisionListener(gamePtr); 
    }

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

    public void SetScriptObjPtr(IntPtr cPtr)
    {
       // m_scriptObj = new ScriptObject(cPtr, false);
    }
}
