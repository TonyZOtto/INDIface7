# file: {INDIface7 repo}/If7Engine/useOCV4.pri
##############################################
# For __DEBUG__ -----------------------------#
##############################################
#OCV4BASE = ../../../bin/DbgOpenCV-v4.10.0
OCV4BASE = /code/bin/DbgOpenCV-v4.10.0/
#                 ^--INDIface7
#              ^--repo
#           ^code
INCLUDEPATH *= $$OCV4BASE/include
    # typical #include's specify opencv2
    # change prefix on release
LIBS *= -L$$OCV4BASE/x64/mingw/lib # .a files
LIBS *= -lopencv_core4100d
LIBS *= -lopencv_highgui4100d
#LIBS *= -lopencv_imageproc4100d
LIBS *= -lopencv_objdetect4100d
OCV4BINPATH = $$(OCV4BASE)/x64/mingw/bin # libopencv_*d.dll
message(--------useOCV4.pri OCV4BASE=$$OCV4BASE OCV4BINPATH==$$OCV4BINPATH)
message(INCLUDEPATH==$$INCLUDEPATH DESTDIR==$$DESTDIR)
message(LIBS=$$LIBS)
