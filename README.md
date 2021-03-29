# Fuzzing libxml2 with AFL on Ubuntu

A quick summary how I installed AFL to fuzz libxml2 using an instrumented build and some glue code.

This is how it looked like to run the fuzzer on 16 cores in parallel:

![Fuzzing on 16 cores in parallel](https://github.com/brahmstaedt/libxml2-fuzzing/raw/main/screenshot.png)


## Prepare Ubuntu 20.04 system with necessary packages
```bash
apt update
apt full-upgrade
apt install build-essential autoconf libtool automake pkg-config -y
```

## Install latest AFL
```bash
wget https://lcamtuf.coredump.cx/afl/releases/afl-latest.tgz
tar xvzf afl-latest.tgz
rm afl-latest.tgz
cd afl-2.52b/
make
make install
cd ..
```

## Install libxml2 v2.9.10
```bash
wget https://gitlab.gnome.org/GNOME/libxml2/-/archive/v2.9.10/libxml2-v2.9.10.tar.gz
tar xvzf libxml2-v2.9.10.tar.gz
rm libxml2-v2.9.10.tar.gz
cd libxml2-v2.9.10/
mkdir output
./autogen.sh --without-python
CC=afl-gcc ./configure --prefix=$(pwd)/output
make clean all
make install
cd ..
```

## Build glue code that will accept XML in stdin and call libxml2 function
```bash
afl-g++ -I libxml2-v2.9.10/output/include/libxml2/ -L libxml2-v2.9.10/output/lib/ parse_xml_stdin.c -lm -lxml2 -o parse_xml_stdin
```

## Run AFL on glue code, passing XML via stdin and using the AFL XML dictionary
```bash
mkdir -p testcases
mkdir -p findings
echo "<xml>te<st</xml>" > testcases/test.xml
afl-fuzz -i testcases/ -o findings/ -x afl-2.52b/dictionaries/xml.dict ./parse_xml_stdin
```
