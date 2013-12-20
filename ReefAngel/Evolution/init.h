// Evolution

RTC_INTERNAL.init();
setSyncProvider(getnow);   // the function to get the time from the RTC
setSyncInterval(SECS_PER_HOUR*6);  // Changed to sync every 6 hours.
