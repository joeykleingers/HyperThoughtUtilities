
set (${PLUGIN_NAME}_FilterParameters_SRCS "")

set (${PLUGIN_NAME}_FilterParameters_HDRS "")

set(_FilterParameters
  HTConnectionFilterParameter
  HTFilePathFilterParameter
  HTUploadPathFilterParameter
  HTMetaDataFilterParameter
)

foreach(f ${_FilterParameters} )
  set (${PLUGIN_NAME}_FilterParameters_SRCS
      ${${PLUGIN_NAME}_FilterParameters_SRCS}
      ${${PLUGIN_NAME}_SOURCE_DIR}/FilterParameters/${f}.cpp
  )

  set (${PLUGIN_NAME}_FilterParameters_HDRS
      ${${PLUGIN_NAME}_FilterParameters_HDRS}
      ${${PLUGIN_NAME}_SOURCE_DIR}/FilterParameters/${f}.h
  )
endforeach()


cmp_IDE_SOURCE_PROPERTIES( "FilterParameters" "${${PLUGIN_NAME}_FilterParameters_HDRS}" "${${PLUGIN_NAME}_FilterParameters_SRCS}" "${PROJECT_INSTALL_HEADERS}")
