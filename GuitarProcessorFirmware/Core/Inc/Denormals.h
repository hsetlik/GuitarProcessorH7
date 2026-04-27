#ifndef _denormals_
#define _denormals_

#define undenormalize(sample) if(((*(unsigned int*)&sample)&0x7f800000)==0) sample=0.0f

#endif//_denormals_