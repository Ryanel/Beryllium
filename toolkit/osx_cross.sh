export PREFIX=/usr/local/cross
export TARGET=i586-elf
export MAKEFLAGS="-j 4"
export CC=/usr/gcc-4.7.0/bin/gcc-4.7.0
export CXX=/usr/gcc-4.7.0/bin/g++-4.7.0
export CPP=/usr/gcc-4.7.0/bin/cpp-4.7.0
export LD=/usr/gcc-4.7.0/bin/gcc-4.7.0
mkdir -p ~/src/build-{binutils,gcc}/
#curl ftp://ftp.gnu.org/gnu/binutils/binutils-2.22.tar.bz2 | tar -x -f - -C ~/src/
#curl ftp://ftp.gnu.org/gnu/gcc/gcc-4.6.2/gcc-core-4.6.2.tar.bz2 | tar -x -f - -C ~/src/
#curl ftp://ftp.gnu.org/gnu/gmp/gmp-5.0.2.tar.bz2 | tar -x -f - -C ~/src/gcc-4.6.2/
#curl ftp://ftp.gnu.org/gnu/mpfr/mpfr-3.1.0.tar.bz2 | tar -x -f - -C ~/src/gcc-4.6.2/
#curl http://www.multiprecision.org/mpc/download/mpc-0.9.tar.gz | tar -x -f - -C ~/src/gcc-4.6.2/
cd ~/src/build-binutils/
#../binutils-2.22/configure --prefix=$PREFIX --target=$TARGET --disable-nls
#make -j 4 all
#sudo make -j 4 install
#export PATH=$PATH:$PREFIX/bin
cd ~/src/build-gcc/
#mv ../gcc-4.6.2/gmp-5.0.2/ ../gcc-4.6.2/gmp/
#mv ../gcc-4.6.2/mpfr-3.1.0/ ../gcc-4.6.2/mpfr/
#mv ../gcc-4.6.2/mpc-0.9/ ../gcc-4.6.2/mpc/
#../gcc-4.6.2/configure --prefix=$PREFIX --target=$TARGET --disable-nls --without-headers --with-mpfr-include=$HOME/src/gcc-4.6.2/mpfr/src/ --with-mpfr-lib=$HOME/src/build-gcc/mpfr/src/.libs/
make -j 4 all-gcc
sudo make -j 4 install-gcc
