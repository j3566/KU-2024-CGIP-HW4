﻿# CMakeList.txt: KU-2024-CGIP-HW4에 대한 CMake 프로젝트, 여기에 소스를 포함하고
# 프로젝트 특정 논리를 정의합니다.
#
cmake_minimum_required (VERSION 3.8)

# 지원되는 경우 MSVC 컴파일러에 대해 핫 다시 로드 사용하도록 설정합니다.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("KU-2024-CGIP-HW4")

link_directories("glut37")

# 이 프로젝트의 실행 파일에 소스를 추가합니다.
add_executable (KU-2024-CGIP-HW4 "KU-2024-CGIP-HW4.cpp" )

add_custom_command(TARGET KU-2024-CGIP-HW4 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/glut37/glut32.dll"
    $<TARGET_FILE_DIR:KU-2024-CGIP-HW4>)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET KU-2024-CGIP-HW4 PROPERTY CXX_STANDARD 20)
endif()

# TODO: 필요한 경우 테스트를 추가하고 대상을 설치합니다.
