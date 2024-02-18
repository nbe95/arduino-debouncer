# Simple Debouncer for Arduino etc.

Whenever an embedded project relies on hardware input, signal states of buttons,
switches etc. are not stable and
[debouncing](https://en.wikipedia.org/wiki/Switch#Contact_bounce) is a
must-have.

This is a basic OOP implementation for easy debouncing in software without any
need for additional hardware. [Scroll down](#example-code) for a meaningful
example.

## Features

### Debouncer for arbitrary types

All Debouncer objects are independent of each other. Debounce anything you like!
A value of any data type will be considered 'debounced' if it did not change
during the threshold time.

- `Debouncer<T>` (constructor): Create a Debouncer object. If a threshold is
  provided as argument, it will be immediately applied as threshold time for a
  signal considered as stable.
- `set/getThreshold`: Sets or gets the desired threshold time used to mark a
  value as stable/debounced.
- `isReady`: Returns true if a stable value has been recorded at least once
  since start-up or reset.
- `getDebounced`: Returns the latest value considered as stable.
- `getRaw`: Returns the latest value read, which is not processed any further.
- `hasChanged`: Returns true *once* if tht debounced state has changed since the
  last call of this function. (Tip: Store the result if needed more than once on
  a particular signal!)
- `debounce`: The debounce routine which does all the work. Must be called with
  the actual input value once in each `loop()` cycle!
- `reset`: Resets all internal states and sets the signal as stale.

### Specialized debouncer for buttons or boolean sensors

A DebouncedSwitch object is derived from Debouncer\<bool\> and especially useful
for simple binary sensors like a hardware button.

- `DebouncedSwitch` (constructor): Create a DebouncedSwitch object which an
  optional threshold as described above. Also, a specific pin to read is
  directly assigned and whether this pin is hooked with a pull-up resistor or
  not.
- `debounce`: The debounce routine as above. Note that by the given pin, the
  digital values is read automatically and does not need any argument. However,
  this function must be called once in each `loop()` cycle, too.
- `isOpen`: Convenience function to quickly check if the attached switch is
  open.
- `isClosed`: Convenience function to quickly check if the attached switch is
  closed.

A Debouncer object that is casted to a `bool` will evaluate to `true` if a
stable/debounced value has been recorded at least once, otherwise `false`. This
can be useful e.g. for a quick check in a single if-clause.

## Quick start

Add the timer header file to your source code, e.g. by cloning this repository
as a [git submodule](https://git-scm.com/book/en/v2/Git-Tools-Submodules) under
`src/lib/` of your main sketch. Use the default git workflow to pull/upgrade
etc.

```sh
git clone https://github.com/nbe95/arduino-debouncer.git ./src/lib/debouncer/

```

> :information_source: In the Arduino universe, it is required that the code
resides under the main `src` directory of your sketch!

Then, simply include the `timer.h` in your sketch and you're ready to go.

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
