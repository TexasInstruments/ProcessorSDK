# edgeai-tiovx-kernels
Repository to host TI's OpenVx kernels used in EdgeAI SDK. This repo will contain some ARM NEON optimized OpenVx target kernels.

## Dependencies
This OpenVx kernels are validated only on TDA4VM/J721E/J7ES board using
EdgeAI image built using PSDK-LINUX and PSDK-RTOS

These modules are used by edgeai-tiovx-modules https://git.ti.com/cgit/edgeai/edgeai-tiovx-modules/

## Steps to clone and build on target
clone the repo under '/opt'
```
/opt# git clone git://git.ti.com/edgeai/edgeai-tiovx-kernels.git
```

### Compilation on the target
The library can be built directly on the target as follows.

```
/opt# cd /opt/edgeai-tiovx-kernels
/opt/edgeai-tiovx-kernels# mkdir build
/opt/edgeai-tiovx-kernels# cd build
/opt/edgeai-tiovx-kernels/build# cmake ..
/opt/edgeai-tiovx-kernels/build# make -j2
```

### Installation
The following command installs the library and header files under /usr dirctory. The headers
and library will be placed as follows

```
/opt/edgeai-tiovx-kernels/build# make install
```

The headers and library will be placed as follows

- **Headers**: /usr/**include**/edgeai-tiovx-kernels/
- **Library**: /usr/**lib**/

The desired install location can be specified as follows

```
/opt/edgeai-tiovx-kernels/build# cmake -DCMAKE_INSTALL_PREFIX=<path/to/install> ..
/opt/edgeai-tiovx-kernels/build# make -j2
/opt/edgeai-tiovx-kernels/build# make install
```

- **Headers**: path/to/install/**include**/edgeai-tiovx-kernels/
- **Library**: path/to/install/**lib**/

### Cross-Compilation for the target
The library can be cross-compiled on an x86_64 machine for the target. Here are the steps for cross-compilation.
Here 'work_area' is used as the root directory for illustration.

```
cd work_area
git clone git://git.ti.com/edgeai/edgeai-tiovx-kernels.git
cd edgeai-tiovx-kernels
# Update cmake/setup_cross_compile.sh to specify tool paths and settings
mkdir build
cd build
source ../cmake/setup_cross_compile.sh
export SOC=(j721e/j721s2/j784s4/j722s/j742s2/am62a/tda54)
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/cross_compile_aarch64.cmake ..
make -j2
make install DESTDIR=<path/to/targetfs>
```
The library and headers will be placed under the following directory structire. Here we use the default '/usr/' install prefix and we prepend the targetfs directory location

- **Headers**: path/to/targetfs/usr/**include**/edgeai-tiovx-kernels/
- **Library**: path/to/targetfs/usr/**lib**/
