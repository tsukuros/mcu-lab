# shared/

Code used by **more than one** project. Wired in through `lib_extra_dirs`
in the consuming project's `platformio.ini`.

Empty on purpose. Shared code with a single consumer isn't shared — it's an
extra layer of indirection. Things move here on the second use, not the first.
