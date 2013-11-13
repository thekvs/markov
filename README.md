## Simple Markov chain text generator.

### Build

This is a typical CMake project, so the build procedure is quite straitforward.
```
$ mkdir /tmp/markov-buildroot
$ cd /tmp/markov-buildroot
$ cmake /path/to/sources -DCMAKE_BUILD_TYPE=Release
$ make
$ make test
```

### Usage

There are three components provided:

1. Utility to compile and store on the disk Markov chain model.
1. Utility to generate random texts using compiled model.
1. Utility to dump compiled model.

If you want to download files from the internet to build a model you need to have installed ```curl``` utility.

#### Examples

Generate Markov chain order 2 using files stored in the internet and file on the disk:

```
$ ./mc-build --urls=http://sigterm.ru/WP1.txt,http://sigterm.ru/WP2.txt,http://sigterm.ru/WP3.txt  --files /tmp/AK.txt -d 2 -m /tmp/WPAK.model
```

Generate 'sentence' of length 10 (8 + 2) using as a seed sequence phrase "увидел что" and phrases from the file /tmp/seeds.
Number of words in every seed phrase has to coinside with model's order (-d parameter for mc-build utility).

```
$ ./mc-generate -m /tmp/WPAK.model -S /tmp/seeds -s "увидел что" -c 8
```

Example output (in Russian):
```
$ ./mc-generate -m /tmp/WPAK.model -S /tmp/seeds -s "увидел что" -c 8
увидел что весловский нагнувшись и спотыкаясь и задыхаясь от волнения 
и что способность видеть свои недостатки но мы пересаливаем мы 
солдат был счастлив когда не думала об этом думает она 
князь василий нахмурился сморщил рот на сторону этот солдат с
```
