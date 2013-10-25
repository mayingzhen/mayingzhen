%module maEngine

#pragma SWIG nowarn=325,516,503,401,402,312,319,205

//The default visibility is internal but it needs to be public for access from a different assembly.
//The macros are for the proxy and typewrapper classes and can respectively be used to to make the
//method and constructor public
SWIG_CSBODY_PROXY(public, public, SWIGTYPE)
SWIG_CSBODY_TYPEWRAPPER(public, public, public, SWIGTYPE)



%include Common.i
%include Engine.i



