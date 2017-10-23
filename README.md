# Xcas

Xcas is a fork of [Xcas/Giac](https://www-fourier.ujf-grenoble.fr/~parisse/giac.html).

The original xcas/giac is currently being converted to use U++ GUI instead of FLTK, but the core will remain in the original state.

```
libgiac 1.2.1
Copyright (C) 2001,2014 B. Parisse, Institut Fourier, 38402 St Martin d'Heres
License GNU GPLv3
>>> simplify(2*x+5*x)
7*x
>>> usimplify(1_kg*1_m/(1_s^2))
1.0_N
>>> for(x:=0;x<2;x++){if(x==0){print('hello');}else{print('world');}}
hello
world
0
```

