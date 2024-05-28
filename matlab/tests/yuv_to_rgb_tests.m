addpath(genpath("matlab/"))

[yuv_rows, yuv_cols, yuv_img] = LoadYUVImage('../../data/1713236782670888742yuv.bin');
assert(~isempty(yuv_img), 'Error loading file 1713236782670888742yuv.bin');
assert(yuv_cols == 2560);
assert(yuv_rows == 720);

png_img = LoadPNGImage('../../data/1713236782670888742rgb.png');

rgb = YUVToRGB(yuv_img, yuv_rows, yuv_cols);

diff = png_img - rgb;
imshow(diff);