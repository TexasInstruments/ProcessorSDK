# edgeai-apps-utils
Repository to host TI's utilities used in EdgeAI SDK. This repo contains ARM NEON optimized 
functions for color format conversion and pre processing for inference.

These modules are used by edgeai-tiovx-kernels (in case of devices supporting openVX middleware)
and direclty used by edgeai-gst-plugins (in case of devices not supporting openVX middleware)

## Steps to clone and build on target
clone the repo under '/opt'
```
/opt# git clone git://git.ti.com/edgeai/edgeai-apps-utils.git
```

### Compilation on the target
The library can be built directly on the target as follows.

```
/opt# cd /opt/edgeai-apps-utils
/opt/edgeai-apps-utils# mkdir build
/opt/edgeai-apps-utils# cd build
/opt/edgeai-apps-utils/build# cmake ..
/opt/edgeai-apps-utils/build# make -j2
```

### Installation
The following command installs the library and header files under /usr dirctory. The headers
and library will be placed as follows

```
/opt/edgeai-apps-utils/build# make install
```

The headers and library will be placed as follows

- **Headers**: /usr/**include**/edgeai-apps-utils/
- **Library**: /usr/**lib**/

The desired install location can be specified as follows

```
/opt/edgeai-apps-utils/build# cmake -DCMAKE_INSTALL_PREFIX=<path/to/install> ..
/opt/edgeai-apps-utils/build# make -j2
/opt/edgeai-apps-utils/build# make install
```

- **Headers**: path/to/install/**include**/edgeai-apps-utils/
- **Library**: path/to/install/**lib**/

### Cross-Compilation for the target
The library can be cross-compiled on an x86_64 machine for the target. Here are the steps for cross-compilation.
Here 'work_area' is used as the root directory for illustration.

```
cd work_area
git clone git://git.ti.com/edgeai/edgeai-apps-utils.git
cd edgeai-apps-utils
# Update cmake/setup_cross_compile.sh to specify tool paths and settings
mkdir build
cd build
source ../cmake/setup_cross_compile.sh
export SOC=(j721e/j721s2/j784s4/j722s/j742s2/am62a/am62x/am62p)
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/cross_compile_aarch64.cmake ..
make -j2
make install DESTDIR=<path_to_targetfs>
```
The library and headers will be placed under the following directory structure. Here we use the default '/usr/' install prefix and we prepend the targetfs directory location

- **Headers**: path_to_targetfs/usr/**include**/edgeai-apps-utils/
- **Library**: path_to_targetfs/usr/**lib**/
