cmake_minimum_required(VERSION 3.5)
project(AudioPathAnalyzer)

set(CMAKE_CXX_STANDARD 17)

find_package(ALSA)
find_package(Threads)

include_directories(
        signalreader
        singen
        analyzer
        kissfft/tools
        kissfft
        popl/include
        csvparser
        ${ALSA_INCLUDE_DIRS}
)

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
        analyzer/Measurement.cpp
        analyzer/Measurement.h
        analyzer/Smoothing.tcc
        analyzer/Experiment.h
        analyzer/Analyzer.cpp
        analyzer/Analyzer.h
)

target_link_libraries(
        AudioPathAnalyzer
        stdc++fs
        ${ALSA_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
)