IF(NOT EXISTS "D:/denni/Documents/GameDev/LearningOpenGL/OpenGLTutorial/assimp-4.1.0/install_manifest.txt")
  MESSAGE(FATAL_ERROR "Cannot find install manifest: \"D:/denni/Documents/GameDev/LearningOpenGL/OpenGLTutorial/assimp-4.1.0/install_manifest.txt\"")
ENDIF(NOT EXISTS "D:/denni/Documents/GameDev/LearningOpenGL/OpenGLTutorial/assimp-4.1.0/install_manifest.txt")

FILE(READ "D:/denni/Documents/GameDev/LearningOpenGL/OpenGLTutorial/assimp-4.1.0/install_manifest.txt" files)
STRING(REGEX REPLACE "\n" ";" files "${files}")
FOREACH(file ${files})
  MESSAGE(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
  EXEC_PROGRAM(
    "C:/Users/denni/Downloads/cmake-3.10.2-win64-x64/cmake-3.10.2-win64-x64/bin/cmake.exe" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
    OUTPUT_VARIABLE rm_out
    RETURN_VALUE rm_retval
    )
  IF(NOT "${rm_retval}" STREQUAL 0)
    MESSAGE(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
  ENDIF(NOT "${rm_retval}" STREQUAL 0)
ENDFOREACH(file)
