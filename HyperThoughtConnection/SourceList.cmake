set(HyperThoughtConnectionDir ${${PLUGIN_NAME}_SOURCE_DIR}/HyperThoughtConnection)

set(${PLUGIN_NAME}_HyperThought_HDRS
    ${HyperThoughtConnectionDir}/HTConnection.h
    ${HyperThoughtConnectionDir}/HTFileCache.h
    ${HyperThoughtConnectionDir}/HTFileInfo.h
    ${HyperThoughtConnectionDir}/HTFileInfoModel.h
    ${HyperThoughtConnectionDir}/HTFileInfoTree.h
    ${HyperThoughtConnectionDir}/HTFilePath.h
    ${HyperThoughtConnectionDir}/HTMetaData.h
)

set(${PLUGIN_NAME}_HyperThought_SRCS
    ${HyperThoughtConnectionDir}/HTConnection.cpp
    ${HyperThoughtConnectionDir}/HTFileCache.cpp
    ${HyperThoughtConnectionDir}/HTFileInfo.cpp
    ${HyperThoughtConnectionDir}/HTFileInfoModel.cpp
    ${HyperThoughtConnectionDir}/HTFileInfoTree.cpp
    ${HyperThoughtConnectionDir}/HTFilePath.cpp
    ${HyperThoughtConnectionDir}/HTMetaData.cpp
)

source_group("HyperThoughtConnection" FILES ${${PLUGIN_NAME}_HyperThought_HDRS} ${${PLUGIN_NAME}_HyperThought_SRCS})