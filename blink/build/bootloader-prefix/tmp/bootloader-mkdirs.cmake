# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/AnnmonRajiJames/esp/v5.2.1/esp-idf/components/bootloader/subproject"
  "C:/Lectrix_company/work/Git_Projects/Python_C_Link_Tkinter/blink/build/bootloader"
  "C:/Lectrix_company/work/Git_Projects/Python_C_Link_Tkinter/blink/build/bootloader-prefix"
  "C:/Lectrix_company/work/Git_Projects/Python_C_Link_Tkinter/blink/build/bootloader-prefix/tmp"
  "C:/Lectrix_company/work/Git_Projects/Python_C_Link_Tkinter/blink/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Lectrix_company/work/Git_Projects/Python_C_Link_Tkinter/blink/build/bootloader-prefix/src"
  "C:/Lectrix_company/work/Git_Projects/Python_C_Link_Tkinter/blink/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Lectrix_company/work/Git_Projects/Python_C_Link_Tkinter/blink/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Lectrix_company/work/Git_Projects/Python_C_Link_Tkinter/blink/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
