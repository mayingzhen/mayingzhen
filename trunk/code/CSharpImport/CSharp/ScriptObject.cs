using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

public class ScriptBase 
{
    private IntPtr m_scriptObjPtr;
    private IntPtr m_sceneNodePtr;

    public SceneNode m_sceneNode
	{
        get { return new SceneNode(m_sceneNodePtr, false); }
	}


    public Input  m_input
	{
        get { return maEngine.GetInput(); }
	}

    public void AddCollisionListener()
    {
        EngineInternal.AddCollisionListener(m_scriptObjPtr.ToInt32(),0);
    }

    public void AddCollisionListener(SceneNode gameObj)
    {
        HandleRef handle = SceneNode.getCPtr(gameObj);
        IntPtr gamePtr = HandleRef.ToIntPtr(handle);
        EngineInternal.AddCollisionListener(m_scriptObjPtr.ToInt32(), gamePtr.ToInt32());
    }

    public void RemoveCollisionListener()
    {
        EngineInternal.RemoveCollisionListener(m_scriptObjPtr.ToInt32(),0);
    }

    public void RemoveCollisionListener(SceneNode gameObj)
    {
        HandleRef handle = SceneNode.getCPtr(gameObj);
        IntPtr gamePtr = HandleRef.ToIntPtr(handle);
        EngineInternal.RemoveCollisionListener(m_scriptObjPtr.ToInt32(),gamePtr.ToInt32()); 
    }

    public ScriptBase GetScript(string name)
	{
        return EngineInternal.MonoGameObject_GetScript(m_sceneNodePtr.ToInt32(), name);
	}

    public ScriptBase GetScript(SceneNode gameObj, string name)
    {
        HandleRef handle = SceneNode.getCPtr(gameObj);
        IntPtr gamePtr = HandleRef.ToIntPtr(handle);
        return EngineInternal.MonoGameObject_GetScript(gamePtr.ToInt32(), name);
    }


    public void SetGameObjPtr(IntPtr cPtr)
    {
        maEngine.Log(".......SetGameObjPtr.....");
        m_sceneNodePtr = cPtr;
    }

    public void SetScriptObjPtr(IntPtr cPtr)
    {
        m_scriptObjPtr = cPtr;
    }
}
