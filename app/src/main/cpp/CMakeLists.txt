
cmake_minimum_required(VERSION 3.22.1)

project("openglnative")

set(jnilibs "${CMAKE_SOURCE_DIR}/../jniLibs")

include_directories(
        util
        render
        sample
        glm
        inc
        inc/assimp
        inc/freetype_2_9_1
        model
        opencv_3_0_0
        egl
        looper
)

find_library(
        log-lib
        log)

find_library(
        android-lib
        android)

file(GLOB src_files
        ${CMAKE_SOURCE_DIR}/egl/*.cpp
        ${CMAKE_SOURCE_DIR}/looper/*.cpp
        ${CMAKE_SOURCE_DIR}/util/*.cpp
        ${CMAKE_SOURCE_DIR}/render/*.cpp
        ${CMAKE_SOURCE_DIR}/sample/*.cpp
        ${CMAKE_SOURCE_DIR}/*.cpp)

#[[set(src_files
        JNIImpl.cpp
        util/GLUtils.cpp
        render/EGLRender.cpp
        render/MineGlRenderContext.cpp
        sample/TriangleSample.cpp
        sample/TextureMapSample.cpp
        sample/NV21TextureMapSample.cpp
        sample/VaoSample.cpp
        sample/FBOSample.cpp
        sample/FBOLegLengthSample.cpp
        sample/CoordSystemSample.cpp
        sample/BasicLightingSample.cpp
        sample/TransformFeedbackSample.cpp
        sample/MultiLightsSample.cpp
        sample/DepthTestingSample.cpp
        sample/InstancingSample.cpp
        sample/Instancing3DSample.cpp
        sample/StencilTestingSample.cpp
        sample/BlendingSample.cpp
        sample/ParticlesSample.cpp
        sample/SkyBoxSample.cpp
        sample/Model3DSample.cpp
        sample/PBOSample.cpp
        sample/BeatingHeartSample.cpp
        sample/TimeTunnelSample.cpp
        sample/CloudSample.cpp
        sample/BezierCurveSample.cpp
        sample/FaceSlenderSample.cpp
        sample/BigEyesSample.cpp
        sample/BigHeadSample.cpp
        sample/RotaryHeadSample.cpp
        sample/VisualizeAudioSample.cpp
        sample/ScratchCardSample.cpp
        sample/AvatarSample.cpp
        sample/ShockWaveSample.cpp
        sample/MRTSample.cpp
        sample/FBOBlitSample.cpp
        sample/TextureBufferSample.cpp
        sample/UniformBufferSample.cpp
        sample/RGB2YUYVSample.cpp
        sample/SharedEGLContextSample.cpp
        sample/TextRenderSample.cpp
        looper/Looper.cpp
        looper/GLRenderLooper.cpp
        sample/PortraitStayColorExample.cpp
        sample/GLTransitionExample.cpp
        sample/GLTransitionExample_2.cpp
        sample/GLTransitionExample_3.cpp
        sample/GLTransitionExample_4.cpp
        sample/ConveyorBeltExample.cpp)]]

add_library(
        ${CMAKE_PROJECT_NAME}
        SHARED
        ${src_files})

add_library(assimp SHARED IMPORTED)
set_target_properties(assimp PROPERTIES IMPORTED_LOCATION ${jnilibs}/${ANDROID_ABI}/libassimp.so)

add_library(opencv_java3 SHARED IMPORTED)
set_target_properties(opencv_java3 PROPERTIES IMPORTED_LOCATION ${jnilibs}/${ANDROID_ABI}/libopencv_java3.so)

add_library(freetype STATIC IMPORTED )
set_target_properties(freetype PROPERTIES IMPORTED_LOCATION ${jnilibs}/${ANDROID_ABI}/libfreetype.a)

set(third-party-libs
        assimp
        freetype
        opencv_java3)

target_link_libraries(${CMAKE_PROJECT_NAME}
        # List libraries link to the target library
        ${log-lib}
        ${android-lib}
        ${third-party-libs}
        GLESv3
        EGL)