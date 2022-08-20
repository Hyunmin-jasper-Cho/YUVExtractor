import sys
import os


if __name__ == '__main__':

    RGB16 = 'rgb16'
    RGB24 = 'rgb24'

    if len(sys.argv) != 6:
        sys.exit(1)

    yuv_path = sys.argv[1]
    img_path = sys.argv[2]
    bit_depth = int(sys.argv[3])
    wid = int(sys.argv[4])
    hei = int(sys.argv[5])

    pix_fmt = 'yuv420p10le' if bit_depth == 10 else 'yuv420p'

    yuv_list = os.listdir(yuv_path)

    f = open((img_path + '/conv.sh'), 'w')
    for yuv_name in yuv_list:
        f.write(f'ffmpeg -s {wid}x{hei} -pix_fmt {pix_fmt} -i {yuv_name} -pix_fmt {RGB24} ./images/{yuv_name}_%3d.png\n')

    f.close()
