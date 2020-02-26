set(HyperThoughtRequestsDir ${${PLUGIN_NAME}_SOURCE_DIR}/HyperThoughtRequests)

set(${PLUGIN_NAME}_Requests_HDRS
    ${HyperThoughtRequestsDir}/HTAbstractRequest.h
    ${HyperThoughtRequestsDir}/HTAbstractUploadRequest.h
    ${HyperThoughtRequestsDir}/HTDownloadRequest.h
    ${HyperThoughtRequestsDir}/HTFileInfoRequest.h
    ${HyperThoughtRequestsDir}/HTFileUploadRequest.h
    ${HyperThoughtRequestsDir}/HTUpdateMetaDataRequest.h
)

set(${PLUGIN_NAME}_Requests_SRCS
    ${HyperThoughtRequestsDir}/HTAbstractRequest.cpp
    ${HyperThoughtRequestsDir}/HTAbstractUploadRequest.cpp
    ${HyperThoughtRequestsDir}/HTDownloadRequest.cpp
    ${HyperThoughtRequestsDir}/HTFileInfoRequest.cpp
    ${HyperThoughtRequestsDir}/HTFileUploadRequest.cpp
    ${HyperThoughtRequestsDir}/HTUpdateMetaDataRequest.cpp
)

source_group("HyperThoughtRequests" FILES ${${PLUGIN_NAME}_Requests_HDRS} ${${PLUGIN_NAME}_Requests_SRCS})