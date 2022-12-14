#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/LabFinal.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/LabFinal.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=LabFinal.c Fan_Support.c I2C_Soft.c I2C_Support.c Interrupt.c Main_Screen.c Setup_Alarm_Time.c Setup_Heater_Temp.c Setup_Time.c ST7735_TFT.c utils.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/LabFinal.p1 ${OBJECTDIR}/Fan_Support.p1 ${OBJECTDIR}/I2C_Soft.p1 ${OBJECTDIR}/I2C_Support.p1 ${OBJECTDIR}/Interrupt.p1 ${OBJECTDIR}/Main_Screen.p1 ${OBJECTDIR}/Setup_Alarm_Time.p1 ${OBJECTDIR}/Setup_Heater_Temp.p1 ${OBJECTDIR}/Setup_Time.p1 ${OBJECTDIR}/ST7735_TFT.p1 ${OBJECTDIR}/utils.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/LabFinal.p1.d ${OBJECTDIR}/Fan_Support.p1.d ${OBJECTDIR}/I2C_Soft.p1.d ${OBJECTDIR}/I2C_Support.p1.d ${OBJECTDIR}/Interrupt.p1.d ${OBJECTDIR}/Main_Screen.p1.d ${OBJECTDIR}/Setup_Alarm_Time.p1.d ${OBJECTDIR}/Setup_Heater_Temp.p1.d ${OBJECTDIR}/Setup_Time.p1.d ${OBJECTDIR}/ST7735_TFT.p1.d ${OBJECTDIR}/utils.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/LabFinal.p1 ${OBJECTDIR}/Fan_Support.p1 ${OBJECTDIR}/I2C_Soft.p1 ${OBJECTDIR}/I2C_Support.p1 ${OBJECTDIR}/Interrupt.p1 ${OBJECTDIR}/Main_Screen.p1 ${OBJECTDIR}/Setup_Alarm_Time.p1 ${OBJECTDIR}/Setup_Heater_Temp.p1 ${OBJECTDIR}/Setup_Time.p1 ${OBJECTDIR}/ST7735_TFT.p1 ${OBJECTDIR}/utils.p1

# Source Files
SOURCEFILES=LabFinal.c Fan_Support.c I2C_Soft.c I2C_Support.c Interrupt.c Main_Screen.c Setup_Alarm_Time.c Setup_Heater_Temp.c Setup_Time.c ST7735_TFT.c utils.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/LabFinal.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F4620
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/LabFinal.p1: LabFinal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/LabFinal.p1.d 
	@${RM} ${OBJECTDIR}/LabFinal.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/LabFinal.p1 LabFinal.c 
	@-${MV} ${OBJECTDIR}/LabFinal.d ${OBJECTDIR}/LabFinal.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/LabFinal.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Fan_Support.p1: Fan_Support.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Fan_Support.p1.d 
	@${RM} ${OBJECTDIR}/Fan_Support.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/Fan_Support.p1 Fan_Support.c 
	@-${MV} ${OBJECTDIR}/Fan_Support.d ${OBJECTDIR}/Fan_Support.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Fan_Support.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/I2C_Soft.p1: I2C_Soft.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/I2C_Soft.p1.d 
	@${RM} ${OBJECTDIR}/I2C_Soft.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/I2C_Soft.p1 I2C_Soft.c 
	@-${MV} ${OBJECTDIR}/I2C_Soft.d ${OBJECTDIR}/I2C_Soft.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/I2C_Soft.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/I2C_Support.p1: I2C_Support.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/I2C_Support.p1.d 
	@${RM} ${OBJECTDIR}/I2C_Support.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/I2C_Support.p1 I2C_Support.c 
	@-${MV} ${OBJECTDIR}/I2C_Support.d ${OBJECTDIR}/I2C_Support.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/I2C_Support.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Interrupt.p1: Interrupt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Interrupt.p1.d 
	@${RM} ${OBJECTDIR}/Interrupt.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/Interrupt.p1 Interrupt.c 
	@-${MV} ${OBJECTDIR}/Interrupt.d ${OBJECTDIR}/Interrupt.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Interrupt.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Main_Screen.p1: Main_Screen.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Main_Screen.p1.d 
	@${RM} ${OBJECTDIR}/Main_Screen.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/Main_Screen.p1 Main_Screen.c 
	@-${MV} ${OBJECTDIR}/Main_Screen.d ${OBJECTDIR}/Main_Screen.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Main_Screen.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Setup_Alarm_Time.p1: Setup_Alarm_Time.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Setup_Alarm_Time.p1.d 
	@${RM} ${OBJECTDIR}/Setup_Alarm_Time.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/Setup_Alarm_Time.p1 Setup_Alarm_Time.c 
	@-${MV} ${OBJECTDIR}/Setup_Alarm_Time.d ${OBJECTDIR}/Setup_Alarm_Time.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Setup_Alarm_Time.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Setup_Heater_Temp.p1: Setup_Heater_Temp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Setup_Heater_Temp.p1.d 
	@${RM} ${OBJECTDIR}/Setup_Heater_Temp.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/Setup_Heater_Temp.p1 Setup_Heater_Temp.c 
	@-${MV} ${OBJECTDIR}/Setup_Heater_Temp.d ${OBJECTDIR}/Setup_Heater_Temp.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Setup_Heater_Temp.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Setup_Time.p1: Setup_Time.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Setup_Time.p1.d 
	@${RM} ${OBJECTDIR}/Setup_Time.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/Setup_Time.p1 Setup_Time.c 
	@-${MV} ${OBJECTDIR}/Setup_Time.d ${OBJECTDIR}/Setup_Time.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Setup_Time.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/ST7735_TFT.p1: ST7735_TFT.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ST7735_TFT.p1.d 
	@${RM} ${OBJECTDIR}/ST7735_TFT.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/ST7735_TFT.p1 ST7735_TFT.c 
	@-${MV} ${OBJECTDIR}/ST7735_TFT.d ${OBJECTDIR}/ST7735_TFT.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/ST7735_TFT.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/utils.p1: utils.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/utils.p1.d 
	@${RM} ${OBJECTDIR}/utils.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/utils.p1 utils.c 
	@-${MV} ${OBJECTDIR}/utils.d ${OBJECTDIR}/utils.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/utils.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/LabFinal.p1: LabFinal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/LabFinal.p1.d 
	@${RM} ${OBJECTDIR}/LabFinal.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/LabFinal.p1 LabFinal.c 
	@-${MV} ${OBJECTDIR}/LabFinal.d ${OBJECTDIR}/LabFinal.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/LabFinal.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Fan_Support.p1: Fan_Support.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Fan_Support.p1.d 
	@${RM} ${OBJECTDIR}/Fan_Support.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/Fan_Support.p1 Fan_Support.c 
	@-${MV} ${OBJECTDIR}/Fan_Support.d ${OBJECTDIR}/Fan_Support.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Fan_Support.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/I2C_Soft.p1: I2C_Soft.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/I2C_Soft.p1.d 
	@${RM} ${OBJECTDIR}/I2C_Soft.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/I2C_Soft.p1 I2C_Soft.c 
	@-${MV} ${OBJECTDIR}/I2C_Soft.d ${OBJECTDIR}/I2C_Soft.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/I2C_Soft.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/I2C_Support.p1: I2C_Support.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/I2C_Support.p1.d 
	@${RM} ${OBJECTDIR}/I2C_Support.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/I2C_Support.p1 I2C_Support.c 
	@-${MV} ${OBJECTDIR}/I2C_Support.d ${OBJECTDIR}/I2C_Support.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/I2C_Support.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Interrupt.p1: Interrupt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Interrupt.p1.d 
	@${RM} ${OBJECTDIR}/Interrupt.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/Interrupt.p1 Interrupt.c 
	@-${MV} ${OBJECTDIR}/Interrupt.d ${OBJECTDIR}/Interrupt.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Interrupt.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Main_Screen.p1: Main_Screen.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Main_Screen.p1.d 
	@${RM} ${OBJECTDIR}/Main_Screen.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/Main_Screen.p1 Main_Screen.c 
	@-${MV} ${OBJECTDIR}/Main_Screen.d ${OBJECTDIR}/Main_Screen.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Main_Screen.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Setup_Alarm_Time.p1: Setup_Alarm_Time.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Setup_Alarm_Time.p1.d 
	@${RM} ${OBJECTDIR}/Setup_Alarm_Time.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/Setup_Alarm_Time.p1 Setup_Alarm_Time.c 
	@-${MV} ${OBJECTDIR}/Setup_Alarm_Time.d ${OBJECTDIR}/Setup_Alarm_Time.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Setup_Alarm_Time.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Setup_Heater_Temp.p1: Setup_Heater_Temp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Setup_Heater_Temp.p1.d 
	@${RM} ${OBJECTDIR}/Setup_Heater_Temp.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/Setup_Heater_Temp.p1 Setup_Heater_Temp.c 
	@-${MV} ${OBJECTDIR}/Setup_Heater_Temp.d ${OBJECTDIR}/Setup_Heater_Temp.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Setup_Heater_Temp.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/Setup_Time.p1: Setup_Time.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Setup_Time.p1.d 
	@${RM} ${OBJECTDIR}/Setup_Time.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/Setup_Time.p1 Setup_Time.c 
	@-${MV} ${OBJECTDIR}/Setup_Time.d ${OBJECTDIR}/Setup_Time.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/Setup_Time.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/ST7735_TFT.p1: ST7735_TFT.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ST7735_TFT.p1.d 
	@${RM} ${OBJECTDIR}/ST7735_TFT.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/ST7735_TFT.p1 ST7735_TFT.c 
	@-${MV} ${OBJECTDIR}/ST7735_TFT.d ${OBJECTDIR}/ST7735_TFT.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/ST7735_TFT.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/utils.p1: utils.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/utils.p1.d 
	@${RM} ${OBJECTDIR}/utils.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/utils.p1 utils.c 
	@-${MV} ${OBJECTDIR}/utils.d ${OBJECTDIR}/utils.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/utils.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/LabFinal.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) --chip=$(MP_PROCESSOR_OPTION) -G -mdist/${CND_CONF}/${IMAGE_TYPE}/LabFinal.X.${IMAGE_TYPE}.map  -D__DEBUG=1  --debugger=none  -DXPRJ_default=$(CND_CONF)  "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     --rom=default,-fd30-ffff --ram=default,-ef4-eff,-f9c-f9c,-fd4-fd4,-fdb-fdf,-fe3-fe7,-feb-fef,-ffd-fff  $(COMPARISON_BUILD) --memorysummary dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -odist/${CND_CONF}/${IMAGE_TYPE}/LabFinal.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_default=$(CND_CONF) 
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/LabFinal.X.${IMAGE_TYPE}.hex 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/LabFinal.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) --chip=$(MP_PROCESSOR_OPTION) -G -mdist/${CND_CONF}/${IMAGE_TYPE}/LabFinal.X.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     $(COMPARISON_BUILD) --memorysummary dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -odist/${CND_CONF}/${IMAGE_TYPE}/LabFinal.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_default=$(CND_CONF) 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
