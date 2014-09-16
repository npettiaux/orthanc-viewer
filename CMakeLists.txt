cmake_minimum_required(VERSION 2.8)

PROJECT(OrthancViewer)


##
# Orthanc client config
##

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  set(ORTHANC_CLIENT_FILENAME "libOrthancClient.so")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
  set(ORTHANC_CLIENT_FILENAME "libOrthancClient.dll")
endif()

add_definitions(-DORTHANC_CLIENT_FILENAME="${ORTHANC_CLIENT_FILENAME}")


##
# Program config
##

file(COPY
  ${CMAKE_SOURCE_DIR}/Resources/OrthancViewer.config
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
  )


##
# Qt config
##

find_package(Qt4 REQUIRED QtCore QtGui QtMain)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})


##
# VTK config
##

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})


##
# Compiler-related options
##

if ("${CMAKE_COMPILER_IS_GNUCXX}")
  set(CMAKE_CXX_FLAGS "-Wall -Wno-deprecated")
endif()


include_directories(Code)

QT4_WRAP_UI(UI
  )

QT4_WRAP_CPP(CPP 
  Code/View/Qt/customwidget/CheckBox.h
  Code/View/Qt/customwidget/ComboBox.h
  Code/View/Qt/customwidget/Dial.h
  Code/View/Qt/customwidget/Dialog.h
  Code/View/Qt/customwidget/DoubleSpinBox.h
  Code/View/Qt/customwidget/GroupBox.h
  Code/View/Qt/customwidget/Label.h
  Code/View/Qt/customwidget/LineEdit.h
  Code/View/Qt/customwidget/MainWindow.h
  Code/View/Qt/customwidget/Menu.h
  Code/View/Qt/customwidget/MenuBar.h
  Code/View/Qt/customwidget/ProgressBar.h
  Code/View/Qt/customwidget/PushButton.h
  Code/View/Qt/customwidget/RadioButton.h
  Code/View/Qt/customwidget/Slider.h
  Code/View/Qt/customwidget/SpinBox.h
  Code/View/Qt/customwidget/SplashScreen.h
  Code/View/Qt/customwidget/Splitter.h
  Code/View/Qt/customwidget/StandardItem.h
  Code/View/Qt/customwidget/StatusBar.h
  Code/View/Qt/customwidget/TabWidget.h
  Code/View/Qt/customwidget/ToolBar.h
  Code/View/Qt/customwidget/ToolButton.h
  Code/View/Qt/customwidget/TreeView.h
  Code/View/Qt/customwidget/VTKWidget.h
  Code/View/Qt/customwidget/Widget.h

  Code/Model/Colormap.h
  Code/Model/ProgramConfiguration.h
  Code/Model/Range.h
  Code/Model/SeriesData.h
  Code/Model/Vector3D.h
  Code/Model/ViewConfiguration.h

  Code/View/Qt/ColorbarWidget.h
  Code/View/Qt/ColormapWidget.h
  Code/View/Qt/DoubleSlider.h
  Code/View/Qt/HounsfieldColormapDialog.h
  Code/View/Qt/HounsfieldWidget.h
  Code/View/Qt/OkCancelDialog.h
  Code/View/Qt/TranslationRotationDialog.h
  Code/View/Qt/ViewConfigurationDialog.h

  Code/View/VTK/MergedSeriesSliceViewer.h
  Code/View/VTK/MergedSeriesViewer.h
  Code/View/VTK/MergedSeriesVolumeViewer.h
  Code/View/VTK/SeriesSliceViewer.h
  Code/View/VTK/SeriesViewer.h
  Code/View/VTK/SeriesVolumeViewer.h
  Code/View/VTK/Viewer.h

  Code/Controller/DisplayInterface.h
  Code/Controller/FusionDialog.h
  Code/Controller/LoadSeriesThread.h
  Code/Controller/MergedSeriesInterface.h
  Code/Controller/OrthancConnectionDialog.h
  Code/Controller/OrthancDialog.h
  Code/Controller/SeriesInterface.h
  Code/Controller/SliceSubInterface.h
  Code/Controller/SubInterface.h
  Code/Controller/ViewerWindow.h
  Code/Controller/VolumeSubInterface.h

  Code/main.h
  )

add_executable(OrthancViewer 
  Code/View/Qt/customwidget/CheckBox.cpp
  Code/View/Qt/customwidget/ComboBox.cpp
  Code/View/Qt/customwidget/Dial.cpp
  Code/View/Qt/customwidget/Dialog.cpp
  Code/View/Qt/customwidget/DoubleSpinBox.cpp
  Code/View/Qt/customwidget/GroupBox.cpp
  Code/View/Qt/customwidget/Label.cpp
  Code/View/Qt/customwidget/LineEdit.cpp
  Code/View/Qt/customwidget/MainWindow.cpp
  Code/View/Qt/customwidget/Menu.cpp
  Code/View/Qt/customwidget/MenuBar.cpp
  Code/View/Qt/customwidget/ProgressBar.cpp
  Code/View/Qt/customwidget/PushButton.cpp
  Code/View/Qt/customwidget/RadioButton.cpp
  Code/View/Qt/customwidget/Slider.cpp
  Code/View/Qt/customwidget/SpinBox.cpp
  Code/View/Qt/customwidget/SplashScreen.cpp
  Code/View/Qt/customwidget/Splitter.cpp
  Code/View/Qt/customwidget/StandardItem.cpp
  Code/View/Qt/customwidget/StatusBar.cpp
  Code/View/Qt/customwidget/TabWidget.cpp
  Code/View/Qt/customwidget/ToolBar.cpp
  Code/View/Qt/customwidget/ToolButton.cpp
  Code/View/Qt/customwidget/TreeView.cpp
  Code/View/Qt/customwidget/VTKWidget.cpp
  Code/View/Qt/customwidget/Widget.cpp

  Code/Model/Colormap.cpp
  Code/Model/ProgramConfiguration.cpp
  Code/Model/Range.cpp
  Code/Model/SeriesData.cpp
  Code/Model/Vector3D.cpp
  Code/Model/ViewConfiguration.cpp

  Code/View/Qt/ColorbarWidget.cpp
  Code/View/Qt/ColormapWidget.cpp
  Code/View/Qt/DoubleSlider.cpp
  Code/View/Qt/HounsfieldColormapDialog.cpp
  Code/View/Qt/HounsfieldWidget.cpp
  Code/View/Qt/OkCancelDialog.cpp
  Code/View/Qt/TranslationRotationDialog.cpp
  Code/View/Qt/ViewConfigurationDialog.cpp

  Code/View/VTK/MergedSeriesSliceViewer.cpp
  Code/View/VTK/MergedSeriesViewer.cpp
  Code/View/VTK/MergedSeriesVolumeViewer.cpp
  Code/View/VTK/SeriesSliceViewer.cpp
  Code/View/VTK/SeriesViewer.cpp
  Code/View/VTK/SeriesVolumeViewer.cpp
  Code/View/VTK/Viewer.cpp

  Code/Controller/DisplayInterface.cpp
  Code/Controller/FusionDialog.cpp
  Code/Controller/LoadSeriesThread.cpp
  Code/Controller/MergedSeriesInterface.cpp
  Code/Controller/OrthancConnectionDialog.cpp
  Code/Controller/OrthancDialog.cpp
  Code/Controller/SeriesInterface.cpp
  Code/Controller/SliceSubInterface.cpp
  Code/Controller/SubInterface.cpp
  Code/Controller/ViewerWindow.cpp
  Code/Controller/VolumeSubInterface.cpp

  Code/main.cpp

  ${UI}
  ${CPP}
  )

target_link_libraries(OrthancViewer ${QT_LIBRARIES} QVTK)

if(VTK_LIBRARIES)
  target_link_libraries(OrthancViewer ${VTK_LIBRARIES})
else()
  target_link_libraries(OrthancViewer vtkHybrid vtkVolumeRendering)
endif()

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Resources/OrthancViewer.config 
  ${CMAKE_CURRENT_BINARY_DIR}/OrthancViewer.config @ONLY
  )



##
# Adds a target to generate API documentation with Doxygen
##

find_package(Doxygen)

if(DOXYGEN_FOUND)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Resources/OrthancViewer.doxygen 
    ${CMAKE_CURRENT_BINARY_DIR}/OrthancViewer.doxygen @ONLY
    )

  add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/OrthancViewer.doxygen
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif()
