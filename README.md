# Simple Debouncer for Arduino etc.

Whenever an embedded project relies on hardware inputs, the signal states of
physical buttons etc. are not stable and
[debouncing](https://en.wikipedia.org/wiki/Switch#Contact_bounce) is a
must-have.

This is a basic OOP implementation for easy debouncing done in software, thus it
works without any need for additional hardware. It can handle both arbitrary
types (which can be compared for equality among two distinct instances) as well
as bare boolean signals, i.e. buttons and switches.

This means, a specific signal's state will be read in each `loop()` cycle and
stored internally. If it did not change during a specific threshold time, it
will be considered as stable/debounced and can be processed further with ease.
[Scroll down](#example-code) for a meaningful example.

## Features

### Debouncer for arbitrary types (`Debouncer<T>`)

All Debouncer objects created at build time or runtime are independent of each
other. Because any data type which offers an equality operator can be processed,
debounce anything you like! A value will be considered as debounced if it did
not change during the set threshold time.

- `Debouncer<T>` (constructor): Create a Debouncer object for use with a signal
  of a specific type `T`. If a threshold is provided as argument, it will be
  immediately applied.
- `set/getThreshold`: Explicitly sets or gets the desired threshold time after
  which a value will become stable/debounced.
- `isReady`: Returns true if a stable value has been recorded at least once
  since start-up or reset.
- `getDebounced`: *The most important function*: Returns the latest value
  considered as stable.
- `getRaw`: Returns the *bare* value which has been read latest by `debounce()`.
- `hasChanged`: Returns true **once** if the debounced state has changed since
  the last call of this function. (Tip: Store the result in a variable if needed
  more than once on a particular signal!)
- `debounce`: The debounce routine which does all the work. Must be called with
  the actual input value *once in each `loop()` cycle*.
- `reset`: Resets all internal states and sets the signal as stale.

### Specialized form for buttons or boolean sensors (`DebouncedSwitch`)

A DebouncedSwitch object is derived from `Debouncer<bool>` and especially useful
for simple binary sensors like hardware buttons.

- `DebouncedSwitch` (constructor): Create a DebouncedSwitch object which an
  optional threshold as described above. Also, *a specific pin to read* is
  directly assigned and whether this pin is hooked with a pull-up resistor
  (default) or not.
- `debounce`: The debounce routine as above. Note that by the given pin, its
  digital value will be read automatically - no argument is required here!
  However, this function must be called once in each `loop()` cycle, too.
- `isOpen`: Convenience function to quickly check if the attached switch is
  open.
- `isClosed`: Convenience function to quickly check if the attached switch is
  closed.

Any Debouncer object that is casted to a `bool` will evaluate to `true` if a
stable/debounced value has been recorded at least once, otherwise `false`. This
can be useful e.g. for a quick check in a single if-clause.

## Quick start

Add the debouncer header file to your source code, e.g. by cloning this
repository as a [git submodule](https://git-scm.com/book/en/v2/Git-Tools-Submodules)
under `src/lib/` of your main sketch. Use the default git workflow to
pull/upgrade etc.

```sh
git clone https://github.com/nbe95/arduino-debouncer.git ./src/lib/debouncer/

```

> :information_source: In the Arduino universe, it is required that the code
resides under the main `src` directory of your sketch!

Then, simply include the `debouncer.h` in your sketch and you're ready to go.

Take a quick look at the following code or run it to see how everything works.

### Example code

```cpp
#include "./src/lib/debouncer/debouncer.h"

// Create a debouncer for an integer with a specific threshold
Debouncer<int> my_debouncer(1000 * 3);  // =3 seconds
DebouncedSwitch my_switch(4, 200);      // switch on pin 4 with 200ms threshold

void setup() {
    Serial.print("Threshold of my_debouncer is ");
    Serial.println(my_debouncer.getThreshold());
}

void loop() {
    // General debouncer for arbitrary types
    my_debouncer.debounce(5);           // Simulate an incoming "5"
    delay(1000);                        // Wait a second...

    if (my_debouncer)  {                // Short for my_debouncer.isReady()
        Serial.println("my_debouncer is ready.")
    } else {
        Serial.println("my_debouncer is not yet ready.")
    }

    if (my_debouncer.hasChanged()) {
        Serial.print("Change incoming! The debounced signal is now: ");
        Serial.println(my_debouncer.getDebounced())
    }

    // Specialized debouncer for switches
    if (my_switch.hasChanged()) {
        Serial.print("Hooray, somebody pushed the switch! State is now: ");
        Serial.println(my_switch.isClosed() ? "pressed" : "not pressed");
    }
}
```
