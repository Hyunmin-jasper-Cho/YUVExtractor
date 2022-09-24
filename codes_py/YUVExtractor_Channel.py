import sys
import os
import numpy as np
from PIL import Image


def metadata_yuv(yuv_src, img_src, wid, hei):
    y_size = wid * hei
    uv_size = (wid / 2) * (hei / 2)

    y_end = int(y_size)
    u_end = int(y_end + (uv_size))
    v_end = int(u_end + (uv_size))

    yuv_size = os.path.getsize(yuv_src)

    return [yuv_size, y_end, u_end, v_end, img_src]


# https://forum.opencv.org/t/remove-6-bit-from-16-bit-depth-pixels/7938/5

def yuv_preprocess(yuv_src, lst, wid, hei, bit, save_as):
    size = lst[0]
    y_end = lst[1]  # upper y size, wid * hei
    u_end = lst[2]  # upper u size
    v_end = lst[3]  # upper v size

    img_src = lst[4]  # img root for save imgs(png/tiff)

    # Offset with number of total frame

    # YUV 4:2:0
    offset = wid * hei * 3 // 2
    N = size // offset if bit == 8 else size // offset // 2
    print(f'Number of frames to read: {N}')

    # file pointer which indicates current target YUV path
    fp = open(yuv_src, 'rb')

    # read full sequence
    frame = None
    if bit == 8:
        frame = np.frombuffer(fp.read(offset * N), dtype=np.uint8)
    else:
        frame = np.frombuffer(fp.read(offset * N * 2), dtype=np.uint16)
        frame = frame << 6

    print(f'All frames reading complete\nWrite each y, u and v component as .{save_as} format')

    yuv_name = yuv_src.split('/')[2].split('.')[0]

    save_to = img_src
    # save_to = os.path.join(img_src, yuv_name)
    print(f'Image save path: {save_to}\n')

    # iterate each frame and save it into img file
    for f in range(N):

        # fs: frame start, init = 0
        fs = offset * f

        Y = frame[fs: fs + y_end].reshape((hei, wid))
        U = frame[fs + y_end: fs + u_end].reshape((hei // 2, wid // 2))
        V = frame[fs + u_end: fs + v_end].reshape((hei // 2, wid // 2))

        y_img, u_img, v_img = make_image(Y, U, V, bit)

        if f == 0 or f % 10 == 0:
            y_img.save(f'{save_to}/Y/{yuv_name}_Y_{f}.{save_as}')
            u_img.save(f'{save_to}/U/{yuv_name}_U_{f}.{save_as}')
            v_img.save(f'{save_to}/V/{yuv_name}_V_{f}.{save_as}')


def make_image(y_np, u_np, v_np, bit: int):
    if bit == 8:
        y = Image.fromarray(y_np)
        u = Image.fromarray(u_np)
        v = Image.fromarray(v_np)

        return y, u, v
    else:
        y = Image.fromarray(y_np, 'I;16')
        u = Image.fromarray(u_np, 'I;16')
        v = Image.fromarray(v_np, 'I;16')

        return y, u, v


def parameter_help():
    print('########################################')
    print('##       YUV2PNG-python3 runner       ##')
    print('##                                    ##')
    print('##     you shoud send 6 parameters    ##')
    print('##      1. YUV root path (./YUVs)     ##')
    print('##      2. IMG root path (./PNGs)     ##')
    print('##            3. Bit depth            ##')
    print('##              4. width              ##')
    print('##              5. height             ##')
    print('##        6. format (lower case)      ##')
    print('##     7. ref directory (optional)    ##')
    print('########################################\n\n')
    print('Ref directory is used when wid and hei are unknown. ')
    print('Example command of 10bit: \n$ python3 yuv2png.py ./YUV420_10 ./PNG_10 8 3840 2160 png')
    print('Example command of  8bit: \n$ python3 yuv2png.py ./YUV420_8 ./TIFF_8 8 1920 1080 tiff')

    sys.exit()

# 1: YUV root path \
# 2: IMG root path \
# 3: bit depth \
# 4: width \
# 5: height \
# 6: format (yuv or tiff) \
# 7: reference folder path \


def get_meta(meta_img_path):
    meta_path = meta_img_path
    meta_path = os.path.join(meta_path, (filename.split('.')[0]) + '.png')
    print(f'Find the original width and height value from {meta_path} ...\n')

    meta_png = Image.open(meta_path)
    wid = meta_png.width
    hei = meta_png.height
    print(f'Find success! wid: {wid}, hei: {hei}')
    return [wid, hei]


if __name__ == '__main__':

    '''
    File structure
    
    |--YUV420_8
        |-- Sequence1.yuv
        |-- Sequence2.yuv
        |-- ...
    
    |--YUV420_10
        |-- SequenceX.yuv
        |-- ...
    
    |--yuv2png.py
    |--demo.sh
    
    '''

    if len(sys.argv) != 8:
        parameter_help()

    yuv_path = sys.argv[1]
    img_root = sys.argv[2]
    bit_depth = int(sys.argv[3])
    save_format = sys.argv[6]

    # mkdir img(png / tiff)
    if not os.path.exists(img_root): os.makedirs(img_root)

    wid = int(sys.argv[4])
    hei = int(sys.argv[5])
    find_meta = True if wid == 0 or hei == 0 else False

    for filename in os.listdir(yuv_path):
        if filename[0] == '.':
            continue

        # mkdir img
        if not os.path.exists(os.path.join(img_root, 'Y')): \
                os.makedirs(os.path.join(img_root, 'Y'))
        if not os.path.exists(os.path.join(img_root, 'U')): \
                os.makedirs(os.path.join(img_root, 'U'))
        if not os.path.exists(os.path.join(img_root, 'V')): \
                os.makedirs(os.path.join(img_root, 'V'))

        size_lst = get_meta(sys.argv[7]) if find_meta is True else [wid, hei]
        wid, hei = size_lst[0], size_lst[1]
        print(wid, hei)

        # ignore hidden(trash) file (for mac os only)
        if filename[0] != '.':
            print(f'Start to process {filename}\n')
            target_yuv_path = os.path.join(yuv_path, filename)

            info = metadata_yuv(target_yuv_path, img_root, wid, hei)
            yuv_preprocess(target_yuv_path, info, wid, hei, bit_depth, save_format)
