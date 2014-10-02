#ifndef __Macro_h__
#define __Macro_h__

#define SAFE_SET(r,l)		{ if(l)	{(l)->AddRef();} SAFE_RELEASE(r); (r) = (l); }
#define SAFE_INC_REF(p)			{ if(p){ (p)->Ref();} }
#define SAFE_DEC_REF(p)			{ if(p){ (p)->Unref();} (p) = NULL;}
#define SAFE_SET_REF(pl,pr)		{ SAFE_INC_REF(pr); SAFE_DEC_REF(pl); (pl) = (pr);}

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif


// Math rand
#define MATH_RANDOM_MINUS1_1()      ((2.0f*((float)rand()/RAND_MAX))-1.0f)      // Returns a random float between -1 and 1.
#define MATH_RANDOM_0_1()           ((float)rand()/RAND_MAX)                    // Returns a random float between 0 and 1.



#endif
