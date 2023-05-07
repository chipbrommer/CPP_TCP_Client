# CMakeList.txt : CMake project for CPP_TCP_Client, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (CPP_TCP_Client "main.cpp"  "Source/tcp_client.cpp" "Source/tcp_client.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CPP_TCP_Client PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
