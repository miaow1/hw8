# hw8

Homework 8

This homework involves [bootstrapping](https://github.com/keithalewis/papers/blob/master/bootstrap.pdf) 
piecewise constant forward curves.

Write functions implementing forward rate agreements in the file `fms_instrument_fra.h`
and interest rate swaps in the file `fms_instrument_swap.h`. Use `fms_instrument_cd.h` as a template.
Add tests for these in `fms_instrument.t.cpp`.

Write Excel add-ins for these in `xll_instrument.cpp` including documentation.

Create documentation by starting the `xll_bootstrap.xll` add-in and running (Alt-F8) the macro `MAKE.SHFB`.
Move `xll_bootstrap.chm` to the `hw8` folder and add that to git.

Add calls for the add-ins you wrote to the `hw8.xlsx` spreadsheet in the `hw8` folder.

Implement `PWFLAT.FORWARD.SPOT` and `PWFLAT.FORWARD.DISCOUNT` in `xll_pwflat.cpp` based on `PWFLAT.FORWARD.VALUE`
