import os
import sys
import subprocess
import shutil
from pathlib import Path
from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext
from distutils.core import setup, Command

class CMakeExtension(Extension):
	def __init__(self, name, sourcedir=""):
		super().__init__(name, sources=[])
		self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):

	def run(self):
		build_dir = Path(self.build_temp).resolve()
		build_dir.mkdir(parents=True, exist_ok=True)
		cfg = 'Release'

		cmake_args = [
			f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={build_dir}",
			f"-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY={build_dir}",
			f"-DCMAKE_BUILD_TYPE={cfg}",
			f"-DLIB_NAME={LIB_NAME}",
			f"-DSO_NAME={MODULE_NAME}",
			f"-DC7X_MMA_TIDL_PATH={C7X_MMA_TIDL_PATH}",
			"-DBUILD_PYTHON_BINDINGS=ON",
		]

		if 'CMAKE_TOOLCHAIN_FILE' in os.environ:
			del os.environ['CMAKE_TOOLCHAIN_FILE']

		if BUILD_TYPE == "aarch64":
			toolchain_path = os.path.join(os.path.abspath(os.path.dirname(__file__)),"aarch64_toolchain.cmake")
			cmake_args.append(f"-DCMAKE_TOOLCHAIN_FILE={toolchain_path}")

		# Run CMake
		subprocess.check_call(["cmake", self.extensions[0].sourcedir] + cmake_args, cwd=build_dir)
		subprocess.check_call(["cmake", "--build", ".", "--config", cfg], cwd=build_dir)

		# Copy artifacts into package
		pkg_dir = Path(self.build_lib) / PACKAGE_NAME

		if os.path.isdir(pkg_dir / "lib"):
			shutil.rmtree(pkg_dir / "lib")
		if os.path.isdir(pkg_dir / "include"):
			shutil.rmtree(pkg_dir / "include")

		(pkg_dir / "lib").mkdir(parents=True, exist_ok=True)
		(pkg_dir / "include").mkdir(parents=True, exist_ok=True)
		
		for sofile in pkg_dir.rglob(f"{MODULE_NAME}*.so"):
			os.remove(sofile)

		# Create __init__.py
		with open(f"{pkg_dir}/__init__.py", "w+") as f:
			f.write(f"from .{MODULE_NAME} import *")

		# Copy static library
		for libfile in build_dir.rglob(f"lib{LIB_NAME}.a"):
			shutil.copy(libfile, pkg_dir / "lib" / libfile.name)

		# Copy shared library
		for sofile in build_dir.rglob(f"{MODULE_NAME}*.so"):
			shutil.copy(sofile, pkg_dir / sofile.name)

		# Copy headers
		for hfile in Path("include").rglob("*.h*"):
			dst = pkg_dir / "include" / hfile.relative_to("include")
			dst.parent.mkdir(parents=True, exist_ok=True)
			shutil.copy(hfile, dst)

		for f in pkg_dir.rglob("*"):
			print(f)

BUILD_TYPE = os.environ.get("BUILD_TYPE")
if BUILD_TYPE:
	BUILD_TYPE = BUILD_TYPE.strip().lower()
	if BUILD_TYPE not in ("x86_64", "aarch64"):
		print (f"Invalid BUILD_TYPE {BUILD_TYPE}. Allowed values are (x86_64 or aarch64)")
		sys.exit(-1)
else:
	BUILD_TYPE = "x86_64"

C7X_MMA_TIDL_PATH = os.environ.get("C7X_MMA_TIDL_PATH")
if C7X_MMA_TIDL_PATH:
	C7X_MMA_TIDL_PATH = C7X_MMA_TIDL_PATH.strip()
	if not os.path.isdir(C7X_MMA_TIDL_PATH):
		print(f"C7X_MMA_TIDL_PATH ({C7X_MMA_TIDL_PATH}) does not exist")
		sys.exit(-1)
else:
	print("Please set C7X_MMA_TIDL_PATH environment variable as it is needed to build libraries")
	sys.exit(-1)

print(f"\nC7X_MMA_TIDL_PATH = {C7X_MMA_TIDL_PATH}")
print(f"BUILD_TYPE = {BUILD_TYPE}\n")

PACKAGE_NAME = "tidlruntime"
MODULE_NAME = "tidlruntime_core"
LIB_NAME = "tidlruntime"

setup(
	name=PACKAGE_NAME,
	version="0.1.0",
	author="Abhay Chirania",
	description="C++ static/shared lib and pybind11 bindings for tidlruntime",
	ext_modules=[CMakeExtension(f"{PACKAGE_NAME}.{MODULE_NAME}")],
	cmdclass={"build_ext": CMakeBuild},
	zip_safe=False,
)