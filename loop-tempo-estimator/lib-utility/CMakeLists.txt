#[[
A library defining various constants and utilities

No dependencies on wxWidgets.  Few symbols with linkage.  Many templates.

One very important utility is finally() as defined in The C++ Programming
Language, fourth edition, for describing ad-hoc RAII actions.

Historically, MemoryX.h began as a wrapper of C++11 <memory>, which by itself
did not yet provide std::make_unique.  That explains the name.

Audacity now uses C++17, and yet, need arose for other anticipations of future
standards.  It also provides other pervasively used utilities that don't
correspond to things in the standard.
]]

set( SOURCES
   IteratorX.cpp
   IteratorX.h
   MapToPositiveHalfIndex.h
   MathApprox.h
   MemoryX.cpp
   MemoryX.h
)
set( LIBRARIES
)
add_library(lib-utility "${SOURCES}")
target_link_libraries(lib-utility PUBLIC ${LIBRARIES})
target_include_directories(lib-utility PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(tests)