using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

public class EngineInternal
{
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
    public static extern ScriptBase MonoGameObject_GetScript(int gameObjPtr, string name);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    public static extern void AddCollisionListener(int scriptObjPtr, int gameObjPtr);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    public static extern void RemoveCollisionListener(int scriptObjPtr, int gameObjPtr);
}