using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

public class EngineInternal
{
	[MethodImplAttribute(MethodImplOptions.InternalCall)]
    public static extern ScriptBase MonoGameObject_GetScript(int goM, string name);

}