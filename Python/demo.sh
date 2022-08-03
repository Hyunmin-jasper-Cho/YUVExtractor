# You should set corresponding width and height of each .YUV file

# 1. To get 10bit .png file from 4:2:0 10bit YUV
python3 yuv2png.py ./YUV420_10 ./TIFF_10 10 3840 2176 tiff

# 2. To get 8bit .png file from 4:2:0 8bit YUV
# python3 yuv2png.py ./YUV420_8 ./PNG_8 8 1920 1080 png
