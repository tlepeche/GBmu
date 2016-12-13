#!/bin/sh

QTLIB_DIR="/goinfre/Qt/5.7/clang_64/lib/"
QTPLG_DIR="/goinfre/Qt/5.7/clang_64/plugins"

qmake -config release && make -j4 && 
	(
	mkdir ./GBmu.app/Contents/Frameworks;
	cp -rf	"$QTLIB_DIR"QtDBus.framework \
			"$QTLIB_DIR"QtPrintSupport.framework\
			"$QTLIB_DIR"QtOpenGL.framework\
			"$QTLIB_DIR"QtWidgets.framework\
			"$QTLIB_DIR"QtCore.framework\
			"$QTLIB_DIR"QtGui.framework\
			./GBmu.app/Contents/Frameworks
	) && (
		for i in `find GBmu.app | grep Headers` ; do rm -rf $i ; done ;
		for i in `find GBmu.app | grep _debug` ; do rm -rf $i ; done
	) && (
		lol=`otool -L GBmu.app/Contents/MacOS/GBmu | grep SDL2 | cut -d \( -f 1 | tr -d " \t"` &&
		mkdir ./GBmu.app/Contents/Libraries ;
		cp $lol ./GBmu.app/Contents/Libraries &&
		install_name_tool ./GBmu.app/Contents/MacOS/GBmu -change $lol "@rpath/libSDL2-2.0.0.dylib"
	) ; (
		mkdir ./GBmu.app/Contents/Plugins/ ./GBmu.app/Contents/Plugins/platforms/ ;
		cp -rf "$QTPLG_DIR"/platforms/libqcocoa.dylib ./GBmu.app/Contents/Plugins/platforms/ && echo coucou &&
		cp -rf "$QTPLG_DIR"/imageformats \
	   			"$QTPLG_DIR"/printsupport \
				./GBmu.app/Contents/Plugins/ &&
		touch ./GBmu.app/Contents/Resources/qt.conf
	)
