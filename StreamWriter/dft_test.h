#ifndef __DFT_H_
#define __DFT_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {  // only need to export C interface if
              // used by C++ source code
#endif

void __declspec(dllimport) dft2(int32_t* x, int size);

#ifdef __cplusplus
}
#endif
#endif