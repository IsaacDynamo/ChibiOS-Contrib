/****************************************************************************************************//**
 * @file     SN32F2xx.h
 *
 * @brief    Generic SN32F2xx header that includes a SN32 series specific header
 *
 * @note     Expects SN32_SERIES_* to be set from chibios-contrib/os/common/startup/ARMCMx/compilers/GCC/mk/startup_*.mk
 *
 *******************************************************************************************************/

#if   defined(SN32_SERIES_SN32F24x)
#include "SN32F240.h"
#elif defined(SN32_SERIES_SN32F24xB)
#include "SN32F240B.h"
#elif defined(SN32_SERIES_SN32F26x)
#include "SN32F260.h"
#else
#error "No known SN32_SERIES_* defined"
#endif