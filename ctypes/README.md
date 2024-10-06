# Python-C連携

## 共有ライブラリの生成

$ gcc -o libtest.so --shared test.c

## Cプログラムのコンパイル＆リンク

$ gcc -o test -DUSE_JPEGLIB test.c -ljpeg
or
$ gcc -o test -DUSE_LIBTIFF test.c -ltiff
or 
$ gcc -o test test.c

## Cプログラムの実行

$ export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
$ ./test

## Pythonプログラムの実行

$ python3 test.py
