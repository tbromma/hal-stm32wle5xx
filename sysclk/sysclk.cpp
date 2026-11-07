/******************************************************************************************************************************************
                                                               START OF FILE
*******************************************************************************************************************************************
                                                @
                                                @
                                                @@@ @@@@@@@@@@
                @@@@@@@@@@@@@  @@@        @@@   @ @@          @    @@@@@@@@@@@    @@@   @@@        @@@   @@@@@       @@@@@
                     @@@       @@@        @@@   @@@            @@  @@@      @@@   @@@   @@@        @@@   @@@@@@     @@@@@@
                     @@@       @@@        @@@  @  @             @  @@@      @@@   @@@   @@@        @@@   @@@ @@     @@ @@@
                     @@@       @@@@@@@@@@@@@@  @  @             @  @@@@@@@@@@@    @@@   @@@        @@@   @@@ @@@   @@@ @@@
                     @@@       @@@        @@@  @                @  @@@@@@@@@      @@@   @@@        @@@   @@@  @@@ @@@  @@@
                     @@@       @@@        @@@  @@              @@  @@@    @@@     @@@   @@@        @@@   @@@   @@@@@   @@@
                     @@@       @@@        @@@    @            @    @@@     @@@    @@@    @@@@@@@@@@@@    @@@    @@@    @@@
                     @@@       @@@        @@@     @@@@@@@@@@@@     @@@      @@@   @@@      @@@@@@@@      @@@     @     @@@

                     S      P      A      C      E        T      E      C      H      N      O      L      O      G      Y

*******************************************************************************************************************************************
                                                                 FILE INFO
******************************************************************************************************************************************/

// CREATED BY:       T. Bromma
// CREATION DATE:    April 2023
// DESCRIPTION:      Low level layer for clock system setup & control

/******************************************************************************************************************************************
                                                                  INCLUDES
******************************************************************************************************************************************/

#include "sysclk.h"
#include "regRcc.h"
#include "regPwr.h"
#include "regUsart.h"
#include "perFlash.h"
#include "halTime.h"

/******************************************************************************************************************************************
                                                          PRIVATE TYPES & VARIABLES
******************************************************************************************************************************************/
namespace hal::clk {

enum hclkClk_t {
    HCLK_CLK_1,
    HCLK_CLK_3,
};

enum pclkClk_t {
    PCLK_CLK_1,
    PCLK_CLK_2,
    PCLK_CLK_3,
};

static constexpr uint16 LSE_CLK_FREQ_Hz = 32768;
static constexpr uint32 HSE32_CLK_FREQ_Hz = 32000000;
static constexpr uint32 HSI16_CLK_FREQ_Hz = 16000000;
static constexpr uint16 LSI_EN_TIMEOUT_ms = 200;
static constexpr uint16 LSE_EN_TIMEOUT_ms = 500;
static constexpr uint8 CLK_EN_TIMEOUT_ms = 100;
static constexpr uint8 PLL_DIVM_MIN = 1;
static constexpr uint8 PLL_DIVM_MAX = 8;
static constexpr uint8 PLL_DIVP_MIN = 3;
static constexpr uint8 PLL_DIVP_MAX = 33;
static constexpr uint8 PLL_DIVQ_MIN = 2;
static constexpr uint8 PLL_DIVQ_MAX = 8;
static constexpr uint8 PLL_DIVR_MIN = 3;
static constexpr uint8 PLL_DIVR_MAX = 9;
static constexpr uint8 PLL_MULN_MIN = 6;
static constexpr uint8 PLL_MULN_MAX = 127;
static constexpr uint32 PLL_VCOIN_MIN_Hz = 2660000;         // 2.66MHz
static constexpr uint32 PLL_VCOIN_MAX_Hz = 16000000;        // 16MHz
static constexpr uint32 PLL_VCOOUT_MIN_Hz = 96000000;       // 96MHz
static constexpr uint32 PLL_VCOOUT_MAX_Hz = 344000000;      // 344MHz

// MSI clock frequency depending on MSIRANGE bit field setting in RCC_CSR register
static constexpr uint32 MSI_CLK_FREQ_Hz[] = { 100000, 200000, 400000, 800000, 1000000, 2000000, 4000000,
                                              8000000, 16000000, 24000000, 32000000, 48000000 };

static constexpr uint32 HPRE_PRES_VAL[] = {1, 3, 5, 1, 1, 6, 10, 32, 2, 4, 8, 16, 64, 128, 256, 512};
static constexpr uint32 PPRE_PRES_VAL[] = {1, 1, 1, 1, 2, 4, 8, 16};
static constexpr uint32 SHDHPRE_PRES_VAL[] = {1, 3, 5, 1, 1, 6, 10, 32, 2, 4, 8, 16, 64, 128, 256, 512};

/******************************************************************************************************************************************
                                                        PRIVATE FUNCTION DECLARATIONS
******************************************************************************************************************************************/

uint16 LseClkGetFreqHz();
uint16 LsiClkGetFreqHz();
uint32 Hsi16ClkGetFreqHz();
uint32 Hse32ClkGetFreqHz();
uint32 MsiClkGetFreqHz();
uint32 PllClkGetFreqHz(pllClk_t pllClk);
uint32 SysClkGetFreqHz();
uint32 HclkClkGetFreqHz(hclkClk_t hclkClk);
uint32 PclkClkGetFreqHz(pclkClk_t pclkClk);
uint32 SystickGetFreqHz();

/******************************************************************************************************************************************
                                                         PUBLIC FUNCTION DEFINITIONS
******************************************************************************************************************************************/

Exec SysClkSetSrc(sysClkSrc_t clkSrc)
{
    uint32 clkSrcFreqHz = 0;

    if (clkSrc == SYSCLK_SRC_MSI)           clkSrcFreqHz = MsiClkGetFreqHz();
    else if (clkSrc == SYSCLK_SRC_HSI16)    clkSrcFreqHz = Hsi16ClkGetFreqHz();
    else if (clkSrc == SYSCLK_SRC_HSE32)    clkSrcFreqHz = Hse32ClkGetFreqHz();
    else if (clkSrc == SYSCLK_SRC_PLLRCLK)  clkSrcFreqHz = PllClkGetFreqHz(PLL_CLK_R);
    else                                    REPORT_FAT("CLOCK SOURCE");

    CHECK_EXEC(hal::flash::ChangeWaitStates(clkSrcFreqHz));
    REG_RCC->CFGR.bit.SW = clkSrc & 0b11;
    return Exec::ERR;
}

uint32 SysClkGetFreqHz(sysClk_t sysClk)
{
    switch(sysClk) {
        case SYSCLK_LSI:        return LsiClkGetFreqHz();
        case SYSCLK_LSE:        return LseClkGetFreqHz();
        case SYSCLK_HSI16:      return Hsi16ClkGetFreqHz();
        case SYSCLK_HSE32:      return Hse32ClkGetFreqHz();
        case SYSCLK_MSI:        return MsiClkGetFreqHz();
        case SYSCLK_PLLP:       return PllClkGetFreqHz(PLL_CLK_P);
        case SYSCLK_PLLQ:       return PllClkGetFreqHz(PLL_CLK_Q);
        case SYSCLK_PLLR:       return PllClkGetFreqHz(PLL_CLK_R);
        case SYSCLK_HCLK1:      return HclkClkGetFreqHz(HCLK_CLK_1);
        case SYSCLK_HCLK3:      return HclkClkGetFreqHz(HCLK_CLK_3);
        case SYSCLK_PCLK1:      return PclkClkGetFreqHz(PCLK_CLK_1);
        case SYSCLK_PCLK2:      return PclkClkGetFreqHz(PCLK_CLK_2);
        case SYSCLK_PCLK3:      return PclkClkGetFreqHz(PCLK_CLK_3);
        case SYSCLK_SYSTICK:    return SystickGetFreqHz();
        default:                return 0;
    }
    return 0;
}

Exec LsiClkEnable()
{
    if (REG_RCC->CSR.bit.LSION == true) {
        RET_WAR("LSI CLK ALREADY ENABLED");
    }

    //-----------------------------------------------------------------------------------
    // Enable the LSI clock

    REG_RCC->CSR.bit.LSION = true;

    //-----------------------------------------------------------------------------------
    // Wait until LSI clock is ready

    uint16 delayMs = 0;

    while (REG_RCC->CSR.bit.LSIRDY == false) {
        DelayMs(1);
        delayMs++;
        if (delayMs == LSI_EN_TIMEOUT_ms) {
            RET_ERR("LSI CLOCK ENABLE TIMEOUT");
        }
    }

    //-----------------------------------------------------------------------------------

    return Exec::OK;
}

Exec LsiClkDisable()
{
    if (REG_RCC->CSR.bit.LSION == false) {
        RET_WAR("LSI CLK ALREADY DISABLED");
    }

    //-----------------------------------------------------------------------------------
    // Enable the LSI clock

    REG_RCC->CSR.bit.LSION = false;

    //-----------------------------------------------------------------------------------
    // Wait until LSI clock disabled

    uint16 delayMs = 0;

    while (REG_RCC->CSR.bit.LSIRDY == false) {
        DelayMs(1);
        delayMs++;
        if (delayMs == LSI_EN_TIMEOUT_ms) {
            RET_ERR("LSI CLOCK DISABLE TIMEOUT");
        }
    }

    //-----------------------------------------------------------------------------------

    return Exec::OK;
}

State LsiClkGetState()
{
    return (REG_RCC->CSR.bit.LSIRDY == true) ? State::ON : State::OFF;
}

Exec LseClkEnable()
{
    if (REG_RCC->BDCR.bit.LSEON == true) {
        RET_WAR("LSE CLK ALREADY ENABLED");
    }

    //-----------------------------------------------------------------------------------
    // Enable the LSE clock

    REG_PWR->CR1.bit.DBP = 1;
    REG_RCC->BDCR.bit.LSEON = true;         // Enable LSE
    REG_RCC->BDCR.bit.LSESYSEN = true;      // Enable LSE as system clock

    //-----------------------------------------------------------------------------------
    // Wait until LSE clock is ready

    uint16 delayMs = 0;

    while ((REG_RCC->BDCR.bit.LSERDY == false)  && (REG_RCC->BDCR.bit.LSESYSRDY == false)) {
        DelayMs(1);
        delayMs++;
        if (delayMs == LSE_EN_TIMEOUT_ms) {
            REG_PWR->CR1.bit.DBP = 0;
            RET_ERR("LSE CLOCK ENABLE TIMEOUT");
        }
    }

    //-----------------------------------------------------------------------------------

    REG_PWR->CR1.bit.DBP = 0;
    return Exec::OK;
}

Exec LseClkSysEnable()
{
    if (REG_RCC->BDCR.bit.LSESYSRDY == true) {
        RET_WAR("LSE SYS CLK ALREADY ENABLED");
    }

    //-----------------------------------------------------------------------------------
    // Check if LSE clock running

    ASSERT(LseClkGetState() == State::ON, "LSE DISABLED, FIRST ENABLE LSE CLOCK");

    //-----------------------------------------------------------------------------------
    // Enable LSE as system clock (can be used by USARTx, LPUARTx, LPTIMx, TIMx, RNG)

    REG_RCC->BDCR.bit.LSESYSEN = true;      // Enable LSE as system clock

    //-----------------------------------------------------------------------------------
    // Wait until LSESYS clock is ready

    uint16 delayMs = 0;

    while (REG_RCC->BDCR.bit.LSESYSRDY == false) {
        DelayMs(1);
        if (delayMs++ == LSE_EN_TIMEOUT_ms) {
            RET_ERR("LSESYS CLOCK ENABLE TIMEOUT");
        }
    }

    //-----------------------------------------------------------------------------------

    return Exec::OK;
}

Exec LseClkDisable()
{
    if (REG_RCC->BDCR.bit.LSEON == false) {
        RET_WAR("LSE CLK ALREADY DISABLED");
    }

    //-----------------------------------------------------------------------------------
    // Enable the LSE clock

    REG_PWR->CR1.bit.DBP = 1;
    REG_RCC->BDCR.bit.LSEON = false;

    //-----------------------------------------------------------------------------------
    // Wait until LSE clock disabled

    uint16 delayMs = 0;

    while (REG_RCC->BDCR.bit.LSERDY == false) {
        DelayMs(1);
        if (delayMs++ == LSE_EN_TIMEOUT_ms) {
            REG_PWR->CR1.bit.DBP = 0;
            RET_ERR("LSE CLOCK DISABLE TIMEOUT");
        }
    }

    //-----------------------------------------------------------------------------------

    REG_PWR->CR1.bit.DBP = 0;
    return Exec::OK;
}

State LseClkGetState()
{
    return (REG_RCC->BDCR.bit.LSERDY == true) ? State::ON : State::OFF;
}

Exec Hse32ClkEnable()
{
    if (REG_RCC->CR.bit.HSEON == true) {
        RET_WAR("HSE CLK ALREADY ENABLED");
    }

    //-----------------------------------------------------------------------------------
    // Enable the HSE clock

    REG_RCC->CR.bit.HSEBYPPWR = true;
    REG_RCC->CR.bit.HSEON = true;

    //-----------------------------------------------------------------------------------
    // Wait until HSE clock is ready

    uint16 delayMs = 0;

    while (REG_RCC->CR.bit.HSERDY == false) {
        DelayMs(1);
        if (delayMs++ == CLK_EN_TIMEOUT_ms) {
            RET_ERR("HSE32 CLOCK ENABLE TIMEOUT");
        }
    }

    //-----------------------------------------------------------------------------------

    return Exec::OK;
}

Exec Hse32ClkDisable()
{
    if (REG_RCC->CR.bit.HSEON == false) {
        RET_WAR("HSE32 CLK ALREADY DISABLED");
    }

    //-----------------------------------------------------------------------------------
    // Enable the HSE clock

    REG_RCC->CR.bit.HSEON = false;

    //-----------------------------------------------------------------------------------
    // Wait until HSE clock disabled

    uint16 delayMs = 0;

    while (REG_RCC->CR.bit.HSERDY == false) {
        DelayMs(1);
        if (delayMs++ == CLK_EN_TIMEOUT_ms) {
            RET_ERR("HSE32 CLOCK DISABLE TIMEOUT");
        }
    }

    //-----------------------------------------------------------------------------------

    return Exec::OK;
}

void Hse32ClkSetPresc(hse32Presc_t presc)
{
    REG_RCC->CR.bit.HSEPRE = (presc == HSE32_PRESC1) ? 0 : 1;
}

State Hse32ClkGetState()
{
    return (REG_RCC->CR.bit.HSERDY == true) ? State::ON : State::OFF;
}

Exec MsiClkEnable()
{
    if (REG_RCC->CR.bit.MSION == true) {
        RET_WAR("MSI CLK ALREADY ENABLED");
    }

    //-----------------------------------------------------------------------------------
    // Enable the MSI clock

    REG_RCC->CR.bit.MSION = true;

    //-----------------------------------------------------------------------------------
    // Wait until MSI clock is ready

    uint16 delayMs = 0;

    while (REG_RCC->CR.bit.MSIRDY == false) {
        DelayMs(1);
        if (delayMs++ == CLK_EN_TIMEOUT_ms) {
            RET_ERR("MSI CLOCK ENABLE TIMEOUT");
        }
    }

    //-----------------------------------------------------------------------------------

    return Exec::OK;
}

Exec MsiClkDisable()
{
    if (REG_RCC->CR.bit.MSION == false) {
        RET_WAR("MSI CLK ALREADY DISABLED");
    }

    //-----------------------------------------------------------------------------------
    // Enable the MSI clock

    REG_RCC->CR.bit.MSION = false;

    //-----------------------------------------------------------------------------------
    // Wait until MSI clock is ready

    uint16 delayMs = 0;

    while (REG_RCC->CR.bit.MSIRDY == false) {
        DelayMs(1);
        if (delayMs++ == CLK_EN_TIMEOUT_ms) {
            RET_ERR("MSI CLOCK DISABLE TIMEOUT");
        }
    }

    //-----------------------------------------------------------------------------------

    return Exec::OK;
}

State MsiClkGetState()
{
    return (REG_RCC->CR.bit.MSIRDY == true) ? State::ON : State::OFF;
}

Exec Hsi16ClkEnable()
{
    if (REG_RCC->CR.bit.HSION == true) {
        RET_WAR("HSI16 CLK ALREADY ENABLED");
    }

    //-----------------------------------------------------------------------------------
    // Enable the HSI clock

    REG_RCC->CR.bit.HSION = true;

    //-----------------------------------------------------------------------------------
    // Wait until HSI clock is ready

    uint16 delayMs = 0;

    while (REG_RCC->CR.bit.HSIRDY == false) {
        DelayMs(1);
        if (delayMs++ == CLK_EN_TIMEOUT_ms) {
            RET_ERR("HSI16 CLOCK ENABLE TIMEOUT");
        }
    }

    //-----------------------------------------------------------------------------------

    return Exec::OK;
}

Exec  Hsi16ClkDisable()
{
    if (REG_RCC->CR.bit.HSION == false) {
        RET_WAR("HSI16 CLK ALREADY DISABLED");
    }

    //-----------------------------------------------------------------------------------
    // Enable the HSI clock

    REG_RCC->CR.bit.HSION = false;

    //-----------------------------------------------------------------------------------
    // Wait until HSI clock is ready

    uint16 delayMs = 0;

    while (REG_RCC->CR.bit.HSIRDY == false) {
        DelayMs(1);
        if (delayMs++ == CLK_EN_TIMEOUT_ms) {
            RET_ERR("HSI16 CLOCK DISABLE TIMEOUT");
        }
    }

    //-----------------------------------------------------------------------------------

    return Exec::OK;
}

State Hsi16ClkGetState()
{
    return (REG_RCC->CR.bit.HSIRDY == true) ? State::ON : State::OFF;
}

Exec PllClkEnable(pllCfg_t& config)
{
    //-----------------------------------------------------------------------------------
    // Check config

    ASSERT((config.divM >= PLL_DIVM_MIN) && (config.divM <= PLL_DIVM_MAX),
           "PLL 'M' DIV FACTOR, MIN = %d MAX = %d", PLL_DIVM_MIN, PLL_DIVM_MAX);

    ASSERT((config.mulN >= PLL_MULN_MIN) && (config.divM <= PLL_MULN_MAX),
           "PLL 'M' MUL FACTOR, MIN = %d MAX = %d", PLL_MULN_MIN, PLL_MULN_MAX);

    ASSERT_COND(config.divP, (config.divP >= PLL_DIVP_MIN) && (config.divP <= PLL_DIVP_MAX),
                "PLL 'P' DIV FACTOR, MIN = %d MAX = %d", PLL_DIVP_MIN, PLL_DIVP_MAX);

    ASSERT_COND(config.divQ, (config.divQ >= PLL_DIVQ_MIN) && (config.divQ <= PLL_DIVQ_MAX),
                "PLL 'Q' DIV FACTOR, MIN = %d, MAX = %d", PLL_DIVQ_MIN, PLL_DIVQ_MAX);

    ASSERT_COND(config.divR, (config.divR >= PLL_DIVR_MIN) && (config.divR <= PLL_DIVR_MAX),
                "PLL 'R' DIV FACTOR, MIN = %d, MAX = %d", PLL_DIVR_MIN, PLL_DIVR_MAX);

    ASSERT(config.divP || config.divQ || config.divR, "NO PLL OUTPUT (P, Q, R) ENABLED");

    //-----------------------------------------------------------------------------------

    uint32 clkSrcFreqHz;
    uint32 vcoInFreqHz;
    uint32 vcoOutFreqHz;

    //-----------------------------------------------------------------------------------
    // Set PLL clock source

    if (config.clkSrc == PLL_CLKSRC_MSI) {
        ASSERT(MsiClkGetState() == State::ON, "MSI CLOCK DISABLED");
        REG_RCC->PLLCFGR.bit.PLLSRC = 0b01;
        clkSrcFreqHz = SysClkGetFreqHz(SYSCLK_MSI);
    }
    else if (config.clkSrc == PLL_CLKSRC_HSI16) {
        ASSERT(Hsi16ClkGetState() == State::ON, "HSI16 CLOCK DISABLED");
        REG_RCC->PLLCFGR.bit.PLLSRC = 0b10;
        clkSrcFreqHz = SysClkGetFreqHz(SYSCLK_HSI16);
    }
    else {
        ASSERT(Hse32ClkGetState() == State::ON, "HSE32 CLOCK DISABLED");
        REG_RCC->PLLCFGR.bit.PLLSRC = 0b11;
        Hse32ClkSetPresc((config.clkSrc == PLL_CLKSRC_HSE32PRES1) ? HSE32_PRESC1 : HSE32_PRESC2);
        clkSrcFreqHz = SysClkGetFreqHz(SYSCLK_HSE32) / (REG_RCC->CR.bit.HSEPRE + 1);
    }

    //-----------------------------------------------------------------------------------
    // Check VCO input and output frequency

    vcoInFreqHz = clkSrcFreqHz / config.divM;
    vcoOutFreqHz = vcoInFreqHz * config.mulN;

    ASSERT(vcoInFreqHz >= PLL_VCOIN_MIN_Hz, "PLL VCO INPUT FREQ. TO LOW (MIN 2.66MHz)");
    ASSERT(vcoInFreqHz <= PLL_VCOIN_MAX_Hz, "PLL VCO INPUT FREQ. TO HIHG (MAX 16MHz)");
    ASSERT(vcoOutFreqHz >= PLL_VCOOUT_MIN_Hz, "PLL VCO OUTPUT FREQ. TO LOW (MIN 96MHz)");
    ASSERT(vcoOutFreqHz <= PLL_VCOOUT_MAX_Hz, "PLL VCO OUPUT FREQ. TO HIHG (MAX 344MHz)");

    //-----------------------------------------------------------------------------------
    // Set division factor M and multiplication factor N

    REG_RCC->PLLCFGR.bit.PLLM = (config.divM - 1) & 0b111;
    REG_RCC->PLLCFGR.bit.PLLN = config.mulN & 0b1111111;

    //-----------------------------------------------------------------------------------
    // Set PLL division factors

    REG_RCC->PLLCFGR.bit.PLLP = (config.divP - 1) & 0b11111;
    REG_RCC->PLLCFGR.bit.PLLQ = (config.divQ - 1) & 0b111;
    REG_RCC->PLLCFGR.bit.PLLR = (config.divR - 1) & 0b111;

    //-----------------------------------------------------------------------------------
    // Enable outputs

    REG_RCC->CR.bit.PLLON = true;
    uint16 delayMs = 0;
    while (REG_RCC->CR.bit.PLLRDY == false) {
        DelayMs(1);
        delayMs++;
        if (delayMs == CLK_EN_TIMEOUT_ms) {
            REG_PWR->CR1.bit.DBP = 0;
            RET_ERR("PLL CLOCK DISABLE TIMEOUT");
        }
    }

    if (config.divP) REG_RCC->PLLCFGR.bit.PLLPEN = true;
    if (config.divQ) REG_RCC->PLLCFGR.bit.PLLQEN = true;
    if (config.divR) REG_RCC->PLLCFGR.bit.PLLREN = true;

    //-----------------------------------------------------------------------------------

    return Exec::OK;
}

State PllClkGetState(pllClk_t clk)
{
    if (clk == PLL_CLK_P)       return REG_RCC->PLLCFGR.bit.PLLPEN ? State::ON : State::OFF;
    else if (clk == PLL_CLK_Q)  return REG_RCC->PLLCFGR.bit.PLLQEN ? State::ON : State::OFF;
    else                        return REG_RCC->PLLCFGR.bit.PLLREN ? State::ON : State::OFF;
}

void RtcClkSrcSelect(rtcClkSrc_t clkSrc)
{
    REG_PWR->CR1.bit.DBP = 1;
    REG_RCC->BDCR.bit.RTCSEL = clkSrc & 0b11;
    REG_PWR->CR1.bit.DBP = 0;
}

void  UsartClkSrcSelect(usartNum_t usartNum, usartClkSrc_t clkSrc)
{
    if (usartNum == USART_1)        REG_RCC->CCIPR.bit.USART1SEL = clkSrc & 0b11;
    else if (usartNum == USART_2)   REG_RCC->CCIPR.bit.USART2SEL = clkSrc & 0b11;
    else                            REG_RCC->CCIPR.bit.LPUARTSEL = clkSrc & 0b11;
}

uint32 UsartClkGetFreqHz(usartNum_t usartNum)
{
    usartClkSrc_t usartClkSrc;

    if (usartNum == USART_1)        usartClkSrc = (usartClkSrc_t)REG_RCC->CCIPR.bit.USART1SEL;
    else if (usartNum == USART_2)   usartClkSrc = (usartClkSrc_t)REG_RCC->CCIPR.bit.USART2SEL;
    else                            usartClkSrc = (usartClkSrc_t)REG_RCC->CCIPR.bit.LPUARTSEL;

    if (usartClkSrc == USART_CLKSRC_PCLK) {
        if (usartNum == USART_1)    return SysClkGetFreqHz(SYSCLK_PCLK2);
        else                        return SysClkGetFreqHz(SYSCLK_PCLK1);
    }
    else if (usartClkSrc == USART_CLKSRC_SYSCLK)    return SysClkGetFreqHz(SYSCLK_HCLK1);
    else if (usartClkSrc == USART_CLKSRC_HSI16)     return SysClkGetFreqHz();
    else                                            return SysClkGetFreqHz(SYSCLK_LSE);
}

void I2cClkSrcSelect(i2cNum_t i2cNum, i2cClkSrc_t clkSrc)
{
    if (i2cNum == I2C_1)        REG_RCC->CCIPR.bit.I2C1SEL = clkSrc & 0b11;
    else if (i2cNum == I2C_2)   REG_RCC->CCIPR.bit.I2C2SEL = clkSrc & 0b11;
    else                        REG_RCC->CCIPR.bit.I2C3SEL = clkSrc & 0b11;
}

uint32 I2cClkGetFreqHz(i2cNum_t i2cNum)
{
    i2cClkSrc_t i2cClkSrc;

    if (i2cNum == I2C_1)        i2cClkSrc = (i2cClkSrc_t)REG_RCC->CCIPR.bit.I2C1SEL;
    else if (i2cNum == I2C_2)   i2cClkSrc = (i2cClkSrc_t)REG_RCC->CCIPR.bit.I2C2SEL;
    else                        i2cClkSrc = (i2cClkSrc_t)REG_RCC->CCIPR.bit.I2C3SEL;

    if (i2cClkSrc == I2C_CLKSRC_PCLK)           return SysClkGetFreqHz(SYSCLK_PCLK1);
    else if (i2cClkSrc == I2C_CLKSRC_SYSCLK)    return SysClkGetFreqHz();
    else if (i2cClkSrc == I2C_CLKSRC_HSI16)     return SysClkGetFreqHz(SYSCLK_HSI16);
    else                                        return 0;
}

void AdcClkSrcSelect(adcClkSrc_t clkSrc)
{
    REG_RCC->CCIPR.bit.ADCSEL = clkSrc & 0b11;
}

uint32  AdcClkGetFreqHz()
{
    adcClkSrc_t clkSrc = (adcClkSrc_t)REG_RCC->CCIPR.bit.ADCSEL;
    if (clkSrc == ADC_CLKSRC_HSI16)         return SysClkGetFreqHz(SYSCLK_HSI16);
    else if (clkSrc == ADC_CLKSRC_PLLP)     return SysClkGetFreqHz(SYSCLK_PLLP);
    else if (clkSrc == ADC_CLKSRC_SYSCLK)   return SysClkGetFreqHz();
    else                                    return 0;
}

Exec RngClkSrcSelect(rngClkSrc_t clkSrc)
{
    //-----------------------------------------------------------------------------------
    // Check selected clock source status

    State clkSrcState;
    if (clkSrc == RNG_CLKSRC_PLLQ)      clkSrcState = PllClkGetState(PLL_CLK_Q);
    else if (clkSrc == RNG_CLKSRC_LSI)  clkSrcState = LsiClkGetState();
    else if (clkSrc == RNG_CLKSRC_LSE)  clkSrcState = LseClkGetState();
    else if (clkSrc == RNG_CLKSRC_MSI)  clkSrcState = MsiClkGetState();
    else                                clkSrcState = State::OFF;
    ASSERT(clkSrcState == State::ON, "SELECTED CLOCK SOURCE FOR RNG IS DISABLED");

    //-----------------------------------------------------------------------------------

    REG_RCC->CCIPR.bit.RNGSEL = clkSrc & 0b11;
    return Exec::OK;
}

uint32 RngClkGetFreqHz()
{
    rngClkSrc_t rngClkSrc = (rngClkSrc_t)REG_RCC->CCIPR.bit.RNGSEL;
    if (rngClkSrc == RNG_CLKSRC_PLLQ)       return SysClkGetFreqHz(SYSCLK_PLLQ);
    else if (rngClkSrc == RNG_CLKSRC_LSI)   return SysClkGetFreqHz(SYSCLK_LSI);
    else if (rngClkSrc == RNG_CLKSRC_LSE)   return SysClkGetFreqHz(SYSCLK_LSE);
    else                                    return SysClkGetFreqHz(SYSCLK_MSI);
}

void PerClkEnable(perId_t perId)
{
    if      (perId >= PER_DMA1 && perId <= PER_CRC)         REG_RCC->AHB1ENR.val |= 1 << RCC_AHB1ENR_bitpos[perId];
    else if (perId >= PER_GPIOA && perId <= PER_GPIOH)      REG_RCC->AHB2ENR.val |= 1 << RCC_AHB2ENR_bitpos[perId - PER_GPIOA];
    else if (perId >= PER_PKA && perId <= PER_FLASH)        REG_RCC->AHB3ENR.val |= 1 << RCC_AHB3ENR_bitpos[perId - PER_PKA];
    else if (perId >= PER_TIM2 && perId <= PER_LPTIM1)      REG_RCC->APB1ENR1.val |= 1 << RCC_APB1ENR1_bitpos[perId - PER_TIM2];
    else if (perId >= PER_LPUART1 && perId <= PER_LPTIM3)   REG_RCC->APB1ENR2.val |= 1 << RCC_APB1ENR2_bitpos[perId - PER_LPUART1];
    else if (perId >= PER_ADC && perId <= PER_TIM17)        REG_RCC->APB2ENR.val |= 1 << RCC_APB2ENR_bitpos[perId - PER_ADC];
    else if (perId == PER_SPI3_RF)                          REG_RCC->APB3ENR.val |= 1 << RCC_APB3ENR_bitpos[perId - PER_SPI3_RF];
    else                                                    REPORT_FAT("PERIPHERAL ID");
}

void PerClkDisable(perId_t perId)
{
    if      (perId >= PER_DMA1 && perId <= PER_CRC)         REG_RCC->AHB1ENR.val &= ~(1 << RCC_AHB1ENR_bitpos[perId - PER_DMA1]);
    else if (perId >= PER_GPIOA && perId <= PER_GPIOH)      REG_RCC->AHB2ENR.val &= ~(1 << RCC_AHB2ENR_bitpos[perId - PER_GPIOA]);
    else if (perId >= PER_PKA && perId <= PER_FLASH)        REG_RCC->AHB3ENR.val &= ~(1 << RCC_AHB3ENR_bitpos[perId - PER_PKA]);
    else if (perId >= PER_TIM2 && perId <= PER_LPTIM1)      REG_RCC->APB1ENR1.val &= ~(1 << RCC_APB1ENR1_bitpos[perId - PER_TIM2]);
    else if (perId >= PER_LPUART1 && perId <= PER_LPTIM3)   REG_RCC->APB1ENR2.val &= ~(1 << RCC_APB1ENR2_bitpos[perId - PER_LPUART1]);
    else if (perId >= PER_ADC && perId <= PER_TIM17)        REG_RCC->APB2ENR.val &= ~(1 << RCC_APB2ENR_bitpos[perId - PER_ADC]);
    else if (perId == PER_SPI3_RF)                          REG_RCC->APB3ENR.val &= ~(1 << RCC_APB3ENR_bitpos[perId - PER_SPI3_RF]);
    else                                                    REPORT_FAT("PERIPHERAL ID");
}

State PerClkState(perId_t perId)
{
    bool clkEnable = false;

    if      (perId >= PER_DMA1 && perId <= PER_CRC)         clkEnable = 0b1 & (REG_RCC->AHB1ENR.val >> RCC_AHB1ENR_bitpos[perId]);
    else if (perId >= PER_GPIOA && perId <= PER_GPIOH)      clkEnable = 0b1 & (REG_RCC->AHB2ENR.val >> RCC_AHB2ENR_bitpos[perId - PER_GPIOA]);
    else if (perId >= PER_PKA && perId <= PER_FLASH)        clkEnable = 0b1 & (REG_RCC->AHB3ENR.val >> RCC_AHB3ENR_bitpos[perId - PER_PKA]);
    else if (perId >= PER_TIM2 && perId <= PER_LPTIM1)      clkEnable = 0b1 & (REG_RCC->APB1ENR1.val >> RCC_APB1ENR1_bitpos[perId - PER_TIM2]);
    else if (perId >= PER_LPUART1 && perId <= PER_LPTIM3)   clkEnable = 0b1 & (REG_RCC->APB1ENR2.val >> RCC_APB1ENR2_bitpos[perId - PER_LPUART1]);
    else if (perId >= PER_ADC && perId <= PER_TIM17)        clkEnable = 0b1 & (REG_RCC->APB2ENR.val >> RCC_APB2ENR_bitpos[perId - PER_ADC]);
    else if (perId == PER_SPI3_RF)                          clkEnable = 0b1 & (REG_RCC->APB3ENR.val >> RCC_APB3ENR_bitpos[perId - PER_SPI3_RF]);
    else                                                    REPORT_FAT("PERIPHERAL ID");

    return clkEnable ? State::ON : State::OFF;
}

uint32 PerClkGetFreqHz(hal::perId_t perId)
{
    uint32 perFreqHz = {};
    if (PerClkState(perId) == State::OFF)                   perFreqHz = 0;
    else if (perId >= PER_DMA1 && perId <= PER_GPIOH)       perFreqHz = SysClkGetFreqHz(SYSCLK_HCLK1);
    else if (perId >= PER_PKA && perId <= PER_FLASH)        perFreqHz = SysClkGetFreqHz(SYSCLK_HCLK3);
    else if (perId >= PER_TIM2 && perId <= PER_LPTIM3)      perFreqHz = SysClkGetFreqHz(SYSCLK_PCLK1);
    else if (perId >= PER_ADC && perId <= PER_TIM17)        perFreqHz = SysClkGetFreqHz(SYSCLK_PCLK2);
    else if (perId == PER_SPI3_RF)                          perFreqHz = SysClkGetFreqHz(SYSCLK_PCLK3);
    else                                                    REPORT_FAT("PERIPHERAL ID");
    return perFreqHz;
}

/******************************************************************************************************************************************
                                                        PRIVATE FUNCTION DEFINITIONS
******************************************************************************************************************************************/

uint16 LsiClkGetFreqHz()
{
    if (REG_RCC->CSR.bit.LSIRDY == false)   return 0;
    else if (REG_RCC->CSR.bit.LSIPRE == 0)  return LSE_CLK_FREQ_Hz;         // LSI clock prescaler is 1
    else                                    return LSE_CLK_FREQ_Hz / 128;   // LSI clock prescaler is 128
}

uint16 LseClkGetFreqHz()
{
    if (REG_RCC->BDCR.bit.LSERDY == false)  return 0;
    else                                    return LSE_CLK_FREQ_Hz;
}

uint32 Hsi16ClkGetFreqHz()
{
    if (REG_RCC->CR.bit.HSIRDY == false)    return 0;
    else                                    return HSI16_CLK_FREQ_Hz;
}

uint32 Hse32ClkGetFreqHz()
{
    if (REG_RCC->CR.bit.HSERDY == false)    return 0;
    else                                    return HSE32_CLK_FREQ_Hz;
}

uint32 MsiClkGetFreqHz()
{
    if (REG_RCC->CR.bit.MSIRDY == false)    return 0;
    else                                    return MSI_CLK_FREQ_Hz[REG_RCC->CR.bit.MSIRANGE];
}

uint32 PllClkGetFreqHz(pllClk_t pllClk)
{
    pllClkSrc_t clkSrc;
    uint64 clkSrcFreqHz;
    uint8 mDiv;
    uint8 nMul = REG_RCC->PLLCFGR.bit.PLLN;
    uint8 outDiv;

    //-----------------------------------------------------------------------------------
    // Check if PLL clock enabled

    if (((pllClk == PLL_CLK_P) && (REG_RCC->PLLCFGR.bit.PLLPEN == false))   ||
        ((pllClk == PLL_CLK_Q) && (REG_RCC->PLLCFGR.bit.PLLQEN == false))   ||
        ((pllClk == PLL_CLK_R) && (REG_RCC->PLLCFGR.bit.PLLREN == false)))  return 0;

    //-----------------------------------------------------------------------------------
    // Get PLL clock source frequency

    clkSrc = (pllClkSrc_t)REG_RCC->PLLCFGR.bit.PLLSRC;
    if (clkSrc == PLL_CLKSRC_MSI)           clkSrcFreqHz = MsiClkGetFreqHz();
    else if (clkSrc == PLL_CLKSRC_HSI16)    clkSrcFreqHz = Hsi16ClkGetFreqHz();
    else                                    clkSrcFreqHz = Hse32ClkGetFreqHz() / (REG_RCC->CR.bit.HSEPRE + 1);

    //-----------------------------------------------------------------------------------
    // Get PLL M division & N multiplication factor

    mDiv = REG_RCC->PLLCFGR.bit.PLLM + 1;
    nMul = REG_RCC->PLLCFGR.bit.PLLN;

    //-----------------------------------------------------------------------------------
    // Get PLL clock output division factor

    if (pllClk == PLL_CLK_P)        outDiv = REG_RCC->PLLCFGR.bit.PLLP + 1;
    else if (pllClk == PLL_CLK_Q)   outDiv = REG_RCC->PLLCFGR.bit.PLLQ + 1;
    else                            outDiv = REG_RCC->PLLCFGR.bit.PLLR + 1;

    return (uint32)(((clkSrcFreqHz / mDiv) * nMul) / outDiv);
}

uint32 SysClkGetFreqHz()
{
    sysClkSrc_t clkSrc = (sysClkSrc_t)REG_RCC->CFGR.bit.SW;
    if (clkSrc == SYSCLK_SRC_MSI)           return MsiClkGetFreqHz();
    else if (clkSrc == SYSCLK_SRC_HSI16)    return Hsi16ClkGetFreqHz();
    else if (clkSrc == SYSCLK_SRC_HSE32)    return Hse32ClkGetFreqHz() / (REG_RCC->CR.bit.HSEPRE + 1);
    else                                    return PllClkGetFreqHz(PLL_CLK_R);
}

uint32 HclkClkGetFreqHz(hclkClk_t hclkClk)
{
    uint32 sysClkFreqHz;

    //-----------------------------------------------------------------------------------
    // Get SYSCLK frequency

    sysClkFreqHz = SysClkGetFreqHz();

    //-----------------------------------------------------------------------------------
    //

    if (hclkClk == HCLK_CLK_1)  return (sysClkFreqHz / HPRE_PRES_VAL[REG_RCC->CFGR.bit.HPRE]);
    else                        return (sysClkFreqHz / HPRE_PRES_VAL[REG_RCC->EXTCFGR.bit.SHDHPRE]);
}

uint32 PclkClkGetFreqHz(pclkClk_t pclkClk)
{
    if (pclkClk == PCLK_CLK_1)      return (HclkClkGetFreqHz(HCLK_CLK_1) / PPRE_PRES_VAL[REG_RCC->CFGR.bit.PRE1]);
    else if (pclkClk == PCLK_CLK_2) return (HclkClkGetFreqHz(HCLK_CLK_1) / PPRE_PRES_VAL[REG_RCC->CFGR.bit.PRE2]);
    else                            return (HclkClkGetFreqHz(HCLK_CLK_3) / SHDHPRE_PRES_VAL[REG_RCC->EXTCFGR.bit.SHDHPRE]);
}

uint32 SystickGetFreqHz()
{
    return HclkClkGetFreqHz(HCLK_CLK_1);
}

/******************************************************************************************************************************************
                                                                END OF FILE
******************************************************************************************************************************************/
} // END OF NAMESPACE 'hal::clk'