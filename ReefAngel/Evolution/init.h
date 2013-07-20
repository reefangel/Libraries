// Evolution

RTC_INTERNAL.init();
RTC_INTERNAL.set_time(10, 29, 9);
RTC_INTERNAL.set_date(22, 10, 2012);
setSyncProvider(getnow);   // the function to get the time from the RTC
setSyncInterval(SECS_PER_HOUR*6);  // Changed to sync every 6 hours.
