CMAKE_MINIMUM_REQUIRED(VERSION 2.6) 

PROJECT( BaseCamp )

SET( BC_SRCS
	main.cpp
	TrajectoryForecaster.cpp
	TrajectoryForecaster.h
	EarthTrajectory.cpp
	EarthTrajectory.h
	MapViewer.cpp
	MapViewer.h
        MainWindow.cpp
        MainWindow.h
        SondeViewer.cpp
        SondeViewer.h
	BasicDataTypes.h
        SimpleSerial.h
        Ground.cpp
        Ground.h
)

  
SET( BC_MOC_HDRS
    MainWindow.h
    SondeViewer.h
)
  
# SET( BC_UIS Dialog1.ui )
# SET( BC_RCS sample.qrc )

ADD_DEFINITIONS( -Wall )
  
SET( QT_USE_QT3SUPPORT TRUE )
SET( QT_USE_QTXML TRUE )

find_package(Qt4 REQUIRED QtCore QtGui QtOpenGL QtWebKit)
find_package(Boost COMPONENTS system filesystem date_time REQUIRED)
link_directories(${Boost_LIBRARY_DIRS})
  
INCLUDE( ${QT_USE_FILE} )
  
# QT4_ADD_RESOURCES( BC_RC_SRCS ${SAMPLE_RCS} )
QT4_WRAP_UI( BC_UI_HDRS ${BC_UIS} )
QT4_WRAP_CPP( BC_MOC_SRCS ${BC_MOC_HDRS} )
INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} )
  
ADD_EXECUTABLE( BaseCamp ${BC_SRCS} ${BC_MOC_SRCS} ${BC_RC_SRCS} ${BC_UI_HDRS} )

TARGET_LINK_LIBRARIES( BaseCamp ${QT_LIBRARIES} ${Boost_LIBRARIES} netcdf_c++)


