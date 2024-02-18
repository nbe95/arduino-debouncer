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

Take a quick look at the following code to see how everything works.

### Example code

```cpp
#include "./src/lib/timer/timer.h"

// Create a timer with or without a specific duration in ms...
Timer my_timer(1000ul * 60 * 60 * 24 * 3);  // =3 days
Timer another_timer;

void setup() {
    // ...or provide a duration when starting the timer...
    my_timer.start(1000);
    // ...or set/change it anytime you want
    my_timer.setDuration(2000);

    // Check if my_timer is set and running
    if (my_timer) { // short for my_timer.isSet()
        Serial.println("my_timer is set, i.e. has got a duration.");
    }
    if (my_timer.isRunning()) {
        Serial.println("... and has already been started!");
    }
}

void loop() {
    // Easy check for repetitive tasks
    if (my_timer.checkAndRestart()) {
        task_each_2s();
    }

    // Just check once without resetting another_timer
    if (another_timer.check()) {
        Serial.print("Congratulations, 7s have elapsed in total! ");
        Serial.println("This message should be printed only once.");
    }
}

void task_each_2s() {
    Serial.println("Yay, 2 seconds have passed again.");

    // Check if another_timer is set
    if (!another_timer) {   // short for another_timer.isSet()
        another_timer.setDuration(5000);
        Serial.println("Setting another_timer to 5s.");

        // Fetch elapsed time
        another_timer.start();
        delay(10);
        Serial.print("Right now, about 10ms should have elapsed: ");
        Serial.println(another_timer.getElapsedTime());
    }
}
```
