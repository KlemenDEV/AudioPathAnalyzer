cmake_minimum_required(VERSION 3.12)
project(AudioPathAnalyzer)

set(CMAKE_CXX_STANDARD 14)

find_package(ALSA)
find_package(Threads)

include_directories(${ALSA_INCLUDE_DIRS} kissfft/tools kissfft signalreader singen)

add_executable(
        AudioPathAnalyzer
        conf.h
        main.cpp
        singen/SineWaveGenerator.cpp
        singen/SineWaveGenerator.h
        kissfft/kiss_fft.c
        kissfft/kiss_fft.h
        kissfft/tools/kiss_fftr.h
        kissfft/tools/kiss_fftr.c
        signalreader/InputSignalReader.cpp
        signalreader/InputSignalReader.h
        analyzer/DataAcquisition.cpp
        analyzer/DataAcquisition.h
        analyzer/Measurement.cpp analyzer/Measurement.h analyzer/Smoothing.tcc)

target_link_libraries(AudioPathAnalyzer ${ALSA_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})