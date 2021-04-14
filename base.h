#ifndef __BASE_H__
#define __BASE_H__

#include <stdint.h>

#if defined(RPI0) || defined(RPI1)

#define PERIPHERAL_BASE       0x20000000UL

#elif defined(RPI2) || defined(RPI3)

#define PERIPHERAL_BASE       0x3F000000UL

#elif defined(RPI4)

#define PERIPHERAL_BASE       0xFE000000UL
#define GIC400_BASE           0xFF840000UL

#else

#error Unknown RPI Model!

#endif

#if defined(RPI0) || defined(RPI1) || defined (RPI2) || defined(RPI3)

#define SYSFREQ               250000000UL

#elif defined(RPI4)

#define SYSFREQ               200000000UL

#else

#error Unknown RPI Model!

#endif

typedef volatile uint32_t rpi_reg_rw_t;
typedef volatile const uint32_t rpi_reg_ro_t;
typedef volatile uint32_t rpi_reg_wo_t;
typedef volatile uint64_t rpi_wreg_rw_t;
typedef volatile const uint64_t rpi_wreg_ro_t;

#endif
