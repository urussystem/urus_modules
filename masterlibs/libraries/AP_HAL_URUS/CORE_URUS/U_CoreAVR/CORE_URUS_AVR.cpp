
#include <AP_HAL_URUS/AP_HAL_URUS.h>
#if (CONFIG_SHAL_CORE == SHAL_CORE_APM)

#include "../CORE_URUS.h"
#include "../CORE_URUS_NAMESPACE.h"
#include "CORE_URUS_AVR.h"

#include "CoreUrusTimers_Avr.h"
#include "CoreUrusScheduler_Avr.h"
#include "CoreUrusUARTDriver_Avr.h"
#include "CoreUrusGPIO_Avr.h"
#include "CoreUrusRCOutput_Avr.h"
#include "CoreUrusRCInput_Avr.h"
#include "CoreUrusAnalogIn_Avr.h"
#include "CoreUrusI2CDevice_Avr.h"
#include "CoreUrusUtil_Avr.h"
#include "CoreUrusStorage_Avr.h"
#include "CoreUrusSPIDevice_Avr.h"
#include "CoreUrusSemaphores_Avr.h"

#include "utility/ISRRegistry.h"
#include <avr/wdt.h>

#if defined(SHAL_CORE_APM16U)
ISRRegistry CORE_AVR::isrregistry;
static CLCoreUrusTimers_Avr coreTimers;
static CLCoreUrusScheduler_Avr coreScheduler;
static CLCoreUrusGPIO_Avr coreGPIO;
#else
CLCoreUrusUARTDriver_AvrISRs(0);
#if defined(SHAL_CORE_APM2)
CLCoreUrusUARTDriver_AvrISRs(1);
CLCoreUrusUARTDriver_AvrISRs(2);
#endif

CLCoreUrusUARTDriver_AvrInstance(coreUARTA_Driver, 0);
#if defined(SHAL_CORE_APM2)
CLCoreUrusUARTDriver_AvrInstance(coreUARTB_Driver, 1);
CLCoreUrusUARTDriver_AvrInstance(coreUARTC_Driver, 2);
#endif
ISRRegistry CORE_AVR::isrregistry;
static CLCoreUrusTimers_Avr coreTimers;
static CLCoreUrusScheduler_Avr coreScheduler;
static CLCoreUrusGPIO_Avr coreGPIO;
static CLCoreUrusRCOutput_Avr coreRCOutput;
static CLCoreUrusRCInput_Avr coreRCInput;
static CLCoreUrusAnalogIn_Avr coreAnalogIn;
static CLCoreUrusI2CDeviceManager_Avr coreI2C_mgr;
static CLCoreUrusUtil_Avr coreUtil;
static CLCoreUrusEEStorage_Avr coreStorage;
static CLCoreUrusSPIDeviceManager_Avr coreSPI_mgr;
#endif

CORE_AVR::CORE_AVR() :
    NSCORE_URUS::CLCORE_URUS(
        &coreTimers,
        &coreScheduler)
{}

void CORE_AVR::init_core() const
{
	wdt_reset();
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

    /* Enable the pullups on the RX pins of the 3 UARTs This is important when
     * the RX line is high-Z: capacitive coupling between input and output pins
     * can cause bytes written to show up as an input. Occasionally this causes
     * us to detect a phantom GPS by seeing our own outgoing config message.
     * PE0 : RX0 (uartA)
     * PD2 : RX1 (uartB)
     * PH0 : RX2 (uartC)
     */
    coreScheduler.init();
#if defined(SHAL_CORE_APM2) || defined(SHAL_CORE_APM328)
    coreUARTA_Driver.begin(115200, 32, 32);
#endif

#if defined(SHAL_CORE_APM2)
    PORTE |= _BV(0);
    PORTH |= _BV(0);
#endif
    PORTD |= _BV(2);
}

NSCORE_URUS::CLCoreUrusScheduler* NSCORE_URUS::get_scheduler()
{
    return &coreScheduler;
}

NSCORE_URUS::CLCoreUrusUARTDriver* NSCORE_URUS::get_uartA_Driver()
{
#if defined(SHAL_CORE_APM2) || defined(SHAL_CORE_APM328)
    return &coreUARTA_Driver;
#else
    return nullptr;
#endif
}

NSCORE_URUS::CLCoreUrusUARTDriver* NSCORE_URUS::get_uartB_Driver()
{
#if defined(SHAL_CORE_APM2)
    return &coreUARTB_Driver;
#else
    return nullptr;
#endif
}

NSCORE_URUS::CLCoreUrusUARTDriver* NSCORE_URUS::get_uartC_Driver()
{
#if defined(SHAL_CORE_APM2)
    return &coreUARTC_Driver;
#else
    return nullptr;
#endif
}

NSCORE_URUS::CLCoreUrusUARTDriver* NSCORE_URUS::get_uartD_Driver()
{
    return nullptr;
}

NSCORE_URUS::CLCoreUrusUARTDriver* NSCORE_URUS::get_uartE_Driver()
{
    return nullptr;
}

NSCORE_URUS::CLCoreUrusUARTDriver* NSCORE_URUS::get_uartF_Driver()
{
    return nullptr;
}

NSCORE_URUS::CLCoreUrusI2CDeviceManager* NSCORE_URUS::get_I2CDeviceManager()
{
#if defined(SHAL_CORE_APM2) || defined(SHAL_CORE_APM328)
    return &coreI2C_mgr;
#else
    return nullptr;
#endif
}

NSCORE_URUS::CLCoreUrusSPIDeviceManager* NSCORE_URUS::get_SPIDeviceManager()
{
#if defined(SHAL_CORE_APM2) || defined(SHAL_CORE_APM328)
    return &coreSPI_mgr;
#else
    return nullptr;
#endif
}

NSCORE_URUS::CLCoreUrusAnalogSource* NSCORE_URUS::get_AnalogSource()
{
    return nullptr;
}

NSCORE_URUS::CLCoreUrusAnalogIn* NSCORE_URUS::get_AnalogIn()
{
#if defined(SHAL_CORE_APM2) || defined(SHAL_CORE_APM328)
    return &coreAnalogIn;
#else
    return nullptr;
#endif
}

NSCORE_URUS::CLCoreUrusUtil* NSCORE_URUS::get_Util()
{
#if defined(SHAL_CORE_APM2) || defined(SHAL_CORE_APM328)
    return &coreUtil;
#else
    return nullptr;
#endif
}

NSCORE_URUS::CLCoreUrusEEStorage* NSCORE_URUS::get_Storage()
{
#if defined(SHAL_CORE_APM2) || defined(SHAL_CORE_APM328)
    return &coreStorage;
#else
    return nullptr;
#endif
}

NSCORE_URUS::CLCoreUrusGPIO* NSCORE_URUS::get_GPIO()
{
    return &coreGPIO;
}

NSCORE_URUS::CLCoreUrusRCInput* NSCORE_URUS::get_RCInput()
{
#if defined(SHAL_CORE_APM2) || defined(SHAL_CORE_APM328)
    return &coreRCInput;
#else
    return nullptr;
#endif
}

NSCORE_URUS::CLCoreUrusRCOutput* NSCORE_URUS::get_RCOutput()
{
#if defined(SHAL_CORE_APM2) || defined(SHAL_CORE_APM328)
    return &coreRCOutput;
#else
    return nullptr;
#endif
}

const NSCORE_URUS::CLCORE_URUS& NSCORE_URUS::get_CORE()
{
    static const CORE_AVR _urus_core;
    return _urus_core;
}

#endif // CONFIG_SHAL_CORE == SHAL_CORE_APM
