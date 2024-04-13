Bare-bones Qt application, intended for trying to get Drawpile onto iOS.

Just a bare window with a label and some menu options to test if things work at all. Works on Qt5 and Qt6.

On Linux, you configure the build via:

```sh
cmake -DCMAKE_BUILD_TYPE=Debug -DUSE_QT=6 -B build
```

And then build via:

```sh
cmake --build build
```

Then run `build/barebones`. Other platforms are analogous. If you want to use Qt5, use `-DUSE_QT=5` when configuring.
