#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/BinaryFileReader.o \
	${OBJECTDIR}/GlobalConfiger.o \
	${OBJECTDIR}/MultiScence.o \
	${OBJECTDIR}/ThreadManager.o \
	${OBJECTDIR}/XDRScenceGroup.o \
	${OBJECTDIR}/XDRUserGroup.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/tools/Markup.o \
	${OBJECTDIR}/tools/dir_info.o \
	${OBJECTDIR}/tools/log.o \
	${OBJECTDIR}/tools/macro.o \
	${OBJECTDIR}/tools/sem_unix.o \
	${OBJECTDIR}/tools/toolkit.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lpthread

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/tmcmultiscence

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/tmcmultiscence: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/tmcmultiscence ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/BinaryFileReader.o: BinaryFileReader.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BinaryFileReader.o BinaryFileReader.cpp

${OBJECTDIR}/GlobalConfiger.o: GlobalConfiger.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/GlobalConfiger.o GlobalConfiger.cpp

${OBJECTDIR}/MultiScence.o: MultiScence.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/MultiScence.o MultiScence.cpp

${OBJECTDIR}/ThreadManager.o: ThreadManager.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ThreadManager.o ThreadManager.cpp

${OBJECTDIR}/XDRScenceGroup.o: XDRScenceGroup.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/XDRScenceGroup.o XDRScenceGroup.cpp

${OBJECTDIR}/XDRUserGroup.o: XDRUserGroup.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/XDRUserGroup.o XDRUserGroup.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/tools/Markup.o: tools/Markup.cpp 
	${MKDIR} -p ${OBJECTDIR}/tools
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tools/Markup.o tools/Markup.cpp

${OBJECTDIR}/tools/dir_info.o: tools/dir_info.c 
	${MKDIR} -p ${OBJECTDIR}/tools
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tools/dir_info.o tools/dir_info.c

${OBJECTDIR}/tools/log.o: tools/log.cpp 
	${MKDIR} -p ${OBJECTDIR}/tools
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tools/log.o tools/log.cpp

${OBJECTDIR}/tools/macro.o: tools/macro.cpp 
	${MKDIR} -p ${OBJECTDIR}/tools
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tools/macro.o tools/macro.cpp

${OBJECTDIR}/tools/sem_unix.o: tools/sem_unix.c 
	${MKDIR} -p ${OBJECTDIR}/tools
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tools/sem_unix.o tools/sem_unix.c

${OBJECTDIR}/tools/toolkit.o: tools/toolkit.c 
	${MKDIR} -p ${OBJECTDIR}/tools
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tools/toolkit.o tools/toolkit.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/tmcmultiscence

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
