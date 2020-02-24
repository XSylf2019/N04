#include "iwdg.h"
/******************************************************************************/
#define RELOAD_VALUE   254
/******************************************************************************/
void watchdog_init(void)
{
  /* IWDG configuration: IWDG is clocked by LSI = 38KHz */
  /* IWDG timeout equal to 214.7 ms (the timeout may varies due to LSI frequency dispersion) */
  /* IWDG timeout = (RELOAD_VALUE + 1) * Prescaler / LSI = (254 + 1) * 32 / 38 000 = 214.7 ms */
  /* Enable the IWDG */
  IWDG_Enable();
  /* Enable write access to IWDG_PR and IWDG_RLR registers */
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  /* Set IWDG timeout */
  IWDG_SetPrescaler(IWDG_Prescaler_256);
  IWDG_SetReload(RELOAD_VALUE);
  /* Refresh IWDG */
  IWDG_ReloadCounter();
}
/******************************************************************************/
void IWDG_Feed(void)//Î¹¹·
{
  IWDG_ReloadCounter();
}
/******************************************************************************/