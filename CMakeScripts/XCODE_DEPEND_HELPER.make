# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.3d-cube.Debug:
/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/openGl-3D-cube/Debug/3d-cube.app/Contents/MacOS/3d-cube:\
	/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.15.sdk/System/Library/Frameworks/GLUT.framework/GLUT\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libfreetyped.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libharfbuzz.a\
	/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.15.sdk/System/Library/Frameworks/Cocoa.framework/Cocoa\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libfreetyped.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libz.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libbz2d.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libpng16d.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libz.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libbz2d.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libpng16d.a
	/bin/rm -f /Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/openGl-3D-cube/Debug/3d-cube.app/Contents/MacOS/3d-cube


PostBuild.unit-test.Debug:
/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/openGl-3D-cube/Debug/unit-test:\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libgtestd.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/manual-link/libgtest_maind.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libgmockd.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/manual-link/libgmock_maind.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libgmockd.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libgtestd.a
	/bin/rm -f /Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/openGl-3D-cube/Debug/unit-test


PostBuild.3d-cube.Release:
/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/openGl-3D-cube/Release/3d-cube.app/Contents/MacOS/3d-cube:\
	/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.15.sdk/System/Library/Frameworks/GLUT.framework/GLUT\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libfreetype.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libharfbuzz.a\
	/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.15.sdk/System/Library/Frameworks/Cocoa.framework/Cocoa\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libz.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libpng.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libz.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libpng.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libz.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libpng.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libz.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libpng.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libbz2.a
	/bin/rm -f /Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/openGl-3D-cube/Release/3d-cube.app/Contents/MacOS/3d-cube


PostBuild.unit-test.Release:
/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/openGl-3D-cube/Release/unit-test:\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libgtest.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/manual-link/libgtest_main.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libgmock.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/manual-link/libgmock_main.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libgmock.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libgtest.a
	/bin/rm -f /Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/openGl-3D-cube/Release/unit-test


PostBuild.3d-cube.MinSizeRel:
/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/openGl-3D-cube/MinSizeRel/3d-cube.app/Contents/MacOS/3d-cube:\
	/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.15.sdk/System/Library/Frameworks/GLUT.framework/GLUT\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libfreetype.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libharfbuzz.a\
	/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.15.sdk/System/Library/Frameworks/Cocoa.framework/Cocoa\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libz.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libpng.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libz.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libpng.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libz.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libpng.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libz.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libpng.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libbz2.a
	/bin/rm -f /Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/openGl-3D-cube/MinSizeRel/3d-cube.app/Contents/MacOS/3d-cube


PostBuild.unit-test.MinSizeRel:
/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/openGl-3D-cube/MinSizeRel/unit-test:\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libgtest.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/manual-link/libgtest_main.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libgmock.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/manual-link/libgmock_main.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libgmock.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libgtest.a
	/bin/rm -f /Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/openGl-3D-cube/MinSizeRel/unit-test


PostBuild.3d-cube.RelWithDebInfo:
/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/openGl-3D-cube/RelWithDebInfo/3d-cube.app/Contents/MacOS/3d-cube:\
	/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.15.sdk/System/Library/Frameworks/GLUT.framework/GLUT\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libfreetype.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libharfbuzz.a\
	/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.15.sdk/System/Library/Frameworks/Cocoa.framework/Cocoa\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libz.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libpng.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libz.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libpng.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libz.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libpng.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libz.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libpng.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libbz2.a
	/bin/rm -f /Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/openGl-3D-cube/RelWithDebInfo/3d-cube.app/Contents/MacOS/3d-cube


PostBuild.unit-test.RelWithDebInfo:
/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/openGl-3D-cube/RelWithDebInfo/unit-test:\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libgtest.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/manual-link/libgtest_main.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libgmock.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/manual-link/libgmock_main.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libgmock.a\
	/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libgtest.a
	/bin/rm -f /Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/openGl-3D-cube/RelWithDebInfo/unit-test




# For each target create a dummy ruleso the target does not have to exist
/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.15.sdk/System/Library/Frameworks/Cocoa.framework/Cocoa:
/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.15.sdk/System/Library/Frameworks/GLUT.framework/GLUT:
/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libbz2d.a:
/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libfreetyped.a:
/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libgmockd.a:
/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libgtestd.a:
/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libharfbuzz.a:
/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libpng.a:
/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libpng16d.a:
/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/libz.a:
/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/manual-link/libgmock_maind.a:
/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/debug/lib/manual-link/libgtest_maind.a:
/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libbz2.a:
/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libfreetype.a:
/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libgmock.a:
/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libgtest.a:
/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libharfbuzz.a:
/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libpng.a:
/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/libz.a:
/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/manual-link/libgmock_main.a:
/Users/mohammad.fakhreddin/Documents/c-plus-plus-projects/vcpkg/installed/x64-osx/lib/manual-link/libgtest_main.a:
