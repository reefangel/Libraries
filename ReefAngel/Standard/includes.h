// Standard

#include <RA_NokiaLCD.h>
#include <RA_Joystick.h>
#include <avr/pgmspace.h>
// Watchdog Timers
#if defined WDT || defined WDT_FORCE
#include <avr/wdt.h>
#endif  // defined WDT || defined WDT_FORCE

// WDT overrides WDT_FORCE
#if defined WDT
	#define wdtenabled pgm_read_word(0x7ffe)==0x400
#elif defined WDT_FORCE
	#define wdtenabled 1
#endif  // WDT

static unsigned long ButtonDebounce;

#ifdef CUSTOM_MAIN
void DrawCustomMain();
void DrawCustomGraph();
#endif  // CUSTOM_MAIN

#ifdef CUSTOM_MENU
void MenuEntry1();
#if CUSTOM_MENU_ENTRIES >= 2
void MenuEntry2();
#endif  // CUSTOM_MENU_ENTRIES >= 2
#if CUSTOM_MENU_ENTRIES >= 3
void MenuEntry3();
#endif  // CUSTOM_MENU_ENTRIES >= 3
#if CUSTOM_MENU_ENTRIES >= 4
void MenuEntry4();
#endif  // CUSTOM_MENU_ENTRIES >= 4
#if CUSTOM_MENU_ENTRIES >= 5
void MenuEntry5();
#endif  // CUSTOM_MENU_ENTRIES >= 5
#if CUSTOM_MENU_ENTRIES >= 6
void MenuEntry6();
#endif  // CUSTOM_MENU_ENTRIES >= 6
#if CUSTOM_MENU_ENTRIES >= 7
void MenuEntry7();
#endif  // CUSTOM_MENU_ENTRIES >= 7
#if CUSTOM_MENU_ENTRIES >= 8
void MenuEntry8();
#endif  // CUSTOM_MENU_ENTRIES >= 8
#if CUSTOM_MENU_ENTRIES >= 9
void MenuEntry9();
#endif  // CUSTOM_MENU_ENTRIES >= 9
#endif  // CUSTOM_MENU
