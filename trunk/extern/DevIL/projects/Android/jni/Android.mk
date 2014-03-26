#规定工程目录是主代码目录
LOCAL_PATH:= $(APP_PROJECT_PATH)

include $(CLEAR_VARS)

LOCAL_MODULE    := DevIL
LOCAL_SRC_FILES := 	src-IL/src/il_alloc.c	\
					src-IL/src/il_bits.c	\
					src-IL/src/il_bmp.c	\
					src-IL/src/il_convbuff.c	\
					src-IL/src/il_convert.c	\
					src-IL/src/il_cut.c		\
					src-IL/src/il_dcx.c		\
					src-IL/src/il_dds-save.c	\
					src-IL/src/il_dds.c	\
					src-IL/src/il_devil.c	\
					src-IL/src/il_doom.c	\
					src-IL/src/il_endian.c	\
					src-IL/src/il_error.c	\
					src-IL/src/il_fastconv.c	\
					src-IL/src/il_files.c	\
					src-IL/src/il_gif.c	\
					src-IL/src/il_header.c	\
					src-IL/src/il_icon.c	\
					src-IL/src/il_internal.c	\
					src-IL/src/il_io.c	\
					src-IL/src/il_lif.c	\
					src-IL/src/il_main.c	\
					src-IL/src/il_manip.c	\
					src-IL/src/il_mdl.c	\
					src-IL/src/il_neuquant.c	\
					src-IL/src/il_pcd.c	\
					src-IL/src/il_pal.c	\
					src-IL/src/il_pcx.c	\
					src-IL/src/il_pic.c	\
					src-IL/src/il_pix.c	\
					src-IL/src/il_png.c	\
					src-IL/src/il_pnm.c	\
					src-IL/src/il_psd.c	\
					src-IL/src/il_psp.c	\
					src-IL/src/il_pxr.c	\
					src-IL/src/il_quantizer.c	\
					src-IL/src/il_raw.c	\
					src-IL/src/il_register.c	\
					src-IL/src/il_rawdata.c	\
					src-IL/src/il_rle.c	\
					src-IL/src/il_sgi.c	\
					src-IL/src/il_stack.c	\
					src-IL/src/il_states.c	\
					src-IL/src/il_targa.c	\
					src-IL/src/il_tiff.c	\
					src-IL/src/il_utility.c	\
					src-IL/src/il_wal.c	\
					src-IL/src/il_xpm.c	\
					src-IL/src/il_jpeg.c	\
					src-IL/src/il_pvr.c	\
					src-IL/src/il_ktx.c	\
					src-IL/src/il_atc.c	\
					src-IL/src/il_mjp.c
					

LOCAL_C_INCLUDES:= 	include	\
					src-IL/include	\
					extlibs/LibPNG	\
					extlibs/Libjpeg	\
					extlibs/zlib

#LOCAL_CFLAGS :=	-fvisibility=hidden

include $(BUILD_STATIC_LIBRARY)
