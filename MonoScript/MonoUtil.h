#ifndef __MonoUtil_h__
#define __MonoUtil_h__

namespace ma
{
	namespace MonoUtil
	{
// 		void Init();
// 
// 		const char* MapTypeMonoToCRT(const char* pszMonoType);
// 
// 		template<class T>
// 		bool GetField(T* pRet,MonoObject* pObj,Name fieldName)
// 		{
//			MonoClass*		pCls = NULL != pObj ? mono_object_get_class(pObj) : NULL;
//			MonoClassField* pField = NULL != pCls ? mono_class_get_field_from_name(pCls,fieldName.GetName()) : NULL;
//			if (NULL != pField)
// 			{
// 				mono_field_get_value(pObj,pField,pRet);
// 				return true;
// 			}
//
//			return false;
// 		}
// 
// 		template<class T>
// 		bool SetField(MonoObject* pObj,Name fieldName,T* pVal)
// 		{
// 			MonoClass*		pCls = NULL != pObj ? mono_object_get_class(pObj) : NULL;
//			MonoClassField* pField = NULL != pCls ? mono_class_get_field_from_name(pCls,fieldName.GetName()) : NULL;
//			if (NULL != pField)
// 			{
// 				mono_field_set_value(pObj,pField,pVal);
// 				return true;
// 			}
//
//			return false;
// 		}
// 
// 		Name GetClassName(MonoObject* pObj)
// 		{
// 			MonoClass* pCls = NULL != pObj ? mono_object_get_class(pObj) : NULL;
// 			const char* pszClsName = NULL != pCls ? mono_class_get_name(pCls) : NULL;
// 			return pszClsName != NULL ? pszClsName : "";			
// 
// 		}

		Vector3 MonoVector3ValueToValue(MonoClassField* pField,MonoObject* pObject)
		{
			Vector3 value(0.0f, 0.0f, 0.0f);
		
			MonoScriptDevice* pMonoDScriptevive = (MonoScriptDevice*)GetScriptDevice();
			if (pMonoDScriptevive == NULL)
				return value;

			MonoDomain* pDomain = pMonoDScriptevive->GetAppDomain();
		
			if (!pField || !pDomain ||!pObject)
				return value;

			MonoObject* pMonoObject = mono_field_get_value_object(pDomain,pField,pObject);
			assert(pMonoObject != NULL);
			if (pMonoObject == NULL)
				return value;

			MonoClass* pMonoClass = mono_object_get_class(pMonoObject);
			assert(pMonoClass != NULL);
			if (pMonoClass == NULL)
				return value;

			MonoProperty *pProperty = NULL;
			void* pIter = NULL;

			while ( ( pProperty = mono_class_get_properties(pMonoClass, &pIter) ) ) 
			{	
				const char* pPropertyName = mono_property_get_name(pProperty);
				MonoObject* pResObject = mono_property_get_value(pProperty,pMonoObject,NULL,NULL);
				float fRes = *( (float*)mono_object_unbox(pResObject) );
				if ( strcmp(pPropertyName,"x") == 0 )
				{
					value.x = fRes;
				}
				else if ( strcmp(pPropertyName,"y") == 0 )
				{
					value.y = fRes;
				}
				else if ( strcmp(pPropertyName,"z") == 0 )
				{
					value.z = fRes;
				}
			}

			return value;
		}

		bool VECTOR3ValueToMonoValue(const Vector3& value, MonoClassField* pField,MonoObject* pObject)
		{
			MonoScriptDevice* pMonoDScriptevive = (MonoScriptDevice*)GetScriptDevice();
			if (pMonoDScriptevive == NULL)
				return false;

			MonoDomain* pDomain = pMonoDScriptevive->GetAppDomain();

			if (!pField || !pDomain ||!pObject)
				return false;

			MonoObject* pMonoObject = mono_field_get_value_object(pDomain,pField,pObject);
			MonoClass* pMonoClass = mono_object_get_class(pMonoObject);
			if (!pMonoObject || !pMonoClass)
				return false;

			MonoProperty *pProperty = NULL;
			void* pIter = NULL;

			while ( ( pProperty = mono_class_get_properties(pMonoClass, &pIter) ) ) 
			{	
				float fValue = 0;
				const char* pPropertyName = mono_property_get_name(pProperty);
				if ( strcmp(pPropertyName,"x") == 0 )
				{
					fValue = value.x;
				}
				else if ( strcmp(pPropertyName,"y") == 0 )
				{
					fValue = value.y;
				}
				else if ( strcmp(pPropertyName,"z") == 0 )
				{
					fValue = value.z;
				}
				void *pparams [1] = {&fValue};
				mono_property_set_value(pProperty,pMonoObject,pparams,NULL);
			}

			return true;
		}
	}
}


#endif //__MonoUtil_h__
