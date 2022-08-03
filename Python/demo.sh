# You should set corresponding width and height of each .YUV file

#  ########################################
#  ##       YUV2PNG-python3 runner       ##
#  ##                                    ##
#  ##     you shoud send 6 parameters    ##
#  ##      1. YUV root path (./YUVs)     ##
#  ##      2. IMG root path (./PNGs)     ##
#  ##            3. Bit depth            ##
#  ##              4. width              ##
#  ##              5. height             ##
#  ##        6. format (lower case)      ##
#  ########################################

# 1. To get 10bit .png file from 4:2:0 10bit YUV
python3 yuv2png.py ./YUV420_10 ./TIFF_10 10 3840 2176 tiff

# 2. To get 8bit .png file from 4:2:0 8bit YUV
# python3 yuv2png.py ./YUV420_8 ./PNG_8 8 1920 1080 png
