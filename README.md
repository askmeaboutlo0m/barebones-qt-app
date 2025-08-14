Bare-bones Qt application, intended as a minimal construct for the sake of porting applications and reporting bugs.

Contains a main window, menu bar with some nested menus, a combo box, labels and some dock widgets. Works on Qt5 and Qt6.

On Linux, you configure the build via:

```sh
cmake -DCMAKE_BUILD_TYPE=Debug -DUSE_QT=6 -B build
```

And then build via:

```sh
cmake --build build
```

Then run `build/barebones`. Other platforms are analogous. If you want to use Qt5, use `-DUSE_QT=5` when configuring.
