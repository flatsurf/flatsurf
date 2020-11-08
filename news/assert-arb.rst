**Changed:**

* add assertions that Arb approximations of finite numbers are always finite; there should not be any issues regarding this but we had some infinities show up in the sampling connections code and this is an extremely cheap check it seems.
